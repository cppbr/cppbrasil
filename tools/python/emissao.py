"""NF-e 55 de teste com a CrossNFe em Python.

Venda interna em SP, regime normal, dois itens e pagamento à vista. A tributação é uma massa
didática, não uma regra fiscal para vendas reais. Nenhuma operação deste programa transmite
documentos à SEFAZ.

Uso: python emissao.py <config.properties> [--assinar-validar]
"""
from __future__ import annotations

import json
import os
import sys
from decimal import ROUND_HALF_UP, Decimal
from pathlib import Path

from crossnfe import CrossNFeError, Session
from crossnfe.models import ConfigNFe, InfNFe, Nfe, NotaFiscal


def ler_config(caminho: str) -> dict[str, str]:
    config: dict[str, str] = {}
    for linha in Path(caminho).read_text(encoding="utf-8").splitlines():
        texto = linha.strip()
        if not texto or texto.startswith("#"):
            continue
        chave, sep, valor = texto.partition("=")
        if not sep:
            raise ValueError("Configuração deve usar chave=valor")
        config[chave.strip()] = valor.strip()
    return config


def valor(config: dict[str, str], nome: str) -> str:
    texto = config.get(nome, "")
    if not texto:
        raise ValueError(f"Configuração ausente: {nome}")
    return texto


def ambiente(nome: str) -> str:
    texto = os.environ.get(nome)
    if texto is None:
        raise ValueError(f"Defina {nome}")
    return texto


def executar(fiscal: NotaFiscal, nota: Nfe, operacao: str) -> None:
    try:
        fiscal.execute(operacao)
    except CrossNFeError as erro:
        raise CrossNFeError(erro.status, f"{erro}\n{nota.error}") from erro


def configurar(cfg: ConfigNFe, assinar: bool) -> None:
    cfg.ModeloDF = 55
    cfg.VersaoNF = 0  # Enum interno VersaoNF.v400 = 0, não 400!
    cfg.TipoEmissao = 1
    cfg.indicadorSincrono = 1
    cfg.webservices.tpAmb = 2  # Homologação fixa neste exemplo.
    cfg.arquivos.salvar = False  # O exemplo salva explicitamente os XMLs.
    if not assinar:
        return
    schemas = ambiente("CROSSNFE_SCHEMAS")
    if not (Path(schemas) / "nfe_v4.00.xsd").is_file():
        raise ValueError("Schema nfe_v4.00.xsd não encontrado")
    cfg.arquivos.caminhoSchema = schemas
    certificado = {"arquivo": ambiente("CROSSNFE_PFX"), "senha": ambiente("CROSSNFE_PFX_PASSWORD")}
    cfg.certificado.execute("carregar", json.dumps(certificado))  # a senha não é impressa nem gravada


def moeda(valor: Decimal) -> Decimal:
    return valor.quantize(Decimal("0.01"), rounding=ROUND_HALF_UP)


def preencher(inf: InfNFe, c: dict[str, str]) -> None:
    inf.versao = "4.00"
    ide = inf.ide
    ide.cUF = 35
    ide.cNF = 12345678  # Fixo apenas para reproduzir o exemplo; gere outro em uma emissão real.
    ide.natOp = "VENDA DE MERCADORIA"
    ide.mod = 55
    ide.serie = int(valor(c, "serie"))
    ide.nNF = int(valor(c, "numero"))
    ide.dhEmi = valor(c, "dataEmissao")  # ISO 8601 com fuso explícito.
    ide.tpNF = 1
    ide.idDest = 1
    ide.cMunFG = 3509502
    ide.tpImp = 1
    ide.tpEmis = 1
    ide.tpAmb = 2
    ide.finNFe = 1
    ide.indFinal = 1
    ide.indPres = 1
    ide.indIntermed = 0
    ide.procEmi = 0
    ide.verProc = "CrossNFe exemplo 0.1"

    emitente = inf.emite
    emitente.CNPJ = valor(c, "emitente.cnpj")
    emitente.IE = valor(c, "emitente.ie")
    emitente.xNome = valor(c, "emitente.nome")
    emitente.xFant = "EMITENTE DE TESTE"
    emitente.CRT = 3
    endereco = emitente.enderEmit
    endereco.xLgr = "RUA DE TESTE"
    endereco.nro = 100
    endereco.xBairro = "CENTRO"
    endereco.cMun = 3509502
    endereco.xMun = "CAMPINAS"
    endereco.UF = "SP"
    endereco.CEP = 13010000
    endereco.cPais = 1058
    endereco.xPais = "BRASIL"
    endereco.fone = "1130000000"

    dest = inf.dest
    dest.CPF = "12345678909"
    dest.xNome = "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL"
    dest.indIEDest = 9
    entrega = dest.enderDest
    entrega.xLgr = "RUA DO DESTINATARIO"
    entrega.nro = 200
    entrega.xBairro = "CENTRO"
    entrega.cMun = 3509502
    entrega.xMun = "CAMPINAS"
    entrega.UF = "SP"
    entrega.CEP = 13010000
    entrega.cPais = 1058
    entrega.xPais = "BRASIL"

    adicionar_item(inf, "001", "PRODUTO DE TESTE A", Decimal(2), Decimal(50))
    adicionar_item(inf, "002", "PRODUTO DE TESTE B", Decimal(1), Decimal(25))
    preencher_totais(inf)
    inf.transp.modFrete = 9
    pagamento = inf.pag.detPag.add()
    pagamento.tPag = 1
    pagamento.vPag = inf.total.ICMSTot.vNF
    inf.infAdic.infCpl = "EXEMPLO DIDATICO SEM VALOR FISCAL. NAO TRANSMITIDO."


def adicionar_item(inf: InfNFe, codigo: str, descricao: str, quantidade: Decimal, unitario: Decimal) -> None:
    total = moeda(quantidade * unitario)
    icms = moeda(total * Decimal("0.18"))
    base_rtc = total - icms
    ibs = moeda(base_rtc * Decimal("0.001"))
    cbs = moeda(base_rtc * Decimal("0.009"))
    det = inf.det.add()  # Já pertence à nota; não existe a etapa obj/add do C++.
    prod = det.prod
    prod.nItem = len(inf.det)
    prod.cProd = codigo
    prod.cEAN = "SEM GTIN"
    prod.xProd = descricao
    prod.NCM = "61091000"
    prod.CFOP = "5102"
    prod.uCom = "UN"
    prod.qCom = float(quantidade)
    prod.vUnCom = float(unitario)
    prod.vProd = float(total)
    prod.cEANTrib = "SEM GTIN"
    prod.uTrib = "UN"
    prod.qTrib = float(quantidade)
    prod.vUnTrib = float(unitario)
    prod.indTot = 1
    imposto = det.imposto
    imposto.ICMS.orig = 0
    imposto.ICMS.CST = 0  # Enum interno CstICMS.ICMS00 = 0.
    imposto.ICMS.modBC = 3
    imposto.ICMS.vBC = float(total)
    imposto.ICMS.pICMS = 18
    imposto.ICMS.vICMS = float(icms)
    imposto.PIS.CST = 49
    imposto.COFINS.CST = 49
    # Cenário RTC didático reproduzido do exemplo C++ do núcleo, não parametrização fiscal.
    imposto.IBSCBS.CST = "000"
    imposto.IBSCBS.cClassTrib = "000001"
    rtc = imposto.IBSCBS.gIBSCBS
    rtc.vBC = float(base_rtc)
    rtc.gIBSUF.pIBSUF = 0.1
    rtc.gIBSUF.vIBSUF = float(ibs)
    rtc.gIBSMun.pIBSMun = 0
    rtc.gIBSMun.vIBSMun = 0
    rtc.vIBS = float(ibs)
    rtc.gCBS.pCBS = 0.9
    rtc.gCBS.vCBS = float(cbs)


def preencher_totais(inf: InfNFe) -> None:
    produtos = icms = base_rtc = ibs = cbs = Decimal(0)
    for item in inf.det:
        produtos += Decimal(str(item.prod.vProd))
        icms += Decimal(str(item.imposto.ICMS.vICMS))
        rtc = item.imposto.IBSCBS.gIBSCBS
        base_rtc += Decimal(str(rtc.vBC))
        ibs += Decimal(str(rtc.vIBS))
        cbs += Decimal(str(rtc.gCBS.vCBS))
    total = inf.total.ICMSTot
    total.vBC = float(produtos)
    total.vICMS = float(icms)
    total.vProd = float(produtos)
    total.vNF = float(produtos)
    tributos = inf.total.IBSCBSTot
    tributos.vBCIBSCBS = float(base_rtc)
    tributos.gIBS.gIBSUF.vIBSUF = float(ibs)
    tributos.gIBS.gIBSMun.vIBSMun = 0
    tributos.gIBS.vIBS = float(ibs)
    tributos.gCBS.vCBS = float(cbs)


def conferir_leitura(xml: str) -> None:
    with Session() as leitura:
        leitura.root.notafiscal.execute("importar", xml)
        inf = leitura.root.notafiscal.NFe[0].infNFe
        if len(inf.det) != 2 or inf.det[1].prod.cProd != "002" or inf.total.ICMSTot.vNF != 125:
            raise RuntimeError("Falha na conferência do XML relido")


def main(args: list[str]) -> int:
    try:
        if not 1 <= len(args) <= 2:
            raise ValueError("Uso: emissao.py <config.properties> [--assinar-validar]")
        assinar = len(args) == 2 and args[1] == "--assinar-validar"
        if len(args) == 2 and not assinar:
            raise ValueError("Opção desconhecida")
        config = ler_config(args[0])
        saida = Path(valor(config, "saida")).resolve()
        saida.mkdir(parents=True, exist_ok=True)
        with Session() as session:
            root = session.root
            configurar(root.configuracoes, assinar)
            nota = root.notafiscal.NFe.add()
            preencher(nota.infNFe, config)
            executar(root.notafiscal, nota, "gerar")
            chave = nota.chNFe
            (saida / f"{chave}-nfe-nao-assinado.xml").write_bytes(nota.XMLOriginal.encode("utf-8"))
            conferir_leitura(nota.XMLOriginal)
            if assinar:
                executar(root.notafiscal, nota, "assinar")
                (saida / f"{chave}-nfe-assinado.xml").write_bytes(nota.XMLAssinado.encode("utf-8"))
                executar(root.notafiscal, nota, "validar")
                print("XML assinado e validado pelo XSD.")
            else:
                print("XML gerado sem assinatura. Use --assinar-validar com certificado e schemas.")
            print(f"Chave: {chave}\nSaída: {saida}\nDocumento NÃO transmitido nem autorizado pela SEFAZ.")
        return 0
    except (CrossNFeError, OSError, ValueError, RuntimeError) as erro:
        print(erro, file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
