#include "nfe.h"

#include <QDir>
#include <QHash>

namespace ExemploNFe {

const QList<GrupoIni> &camposConfiguracao()
{
    static const QList<GrupoIni> grupos = {
        {"Documento fiscal", {
            {"geral/modelo", "Modelo", "55", {{"55", "55 — NF-e"}, {"65", "65 — NFC-e"}}},
            {"geral/versao", "Versão do leiaute", "4.00", {{"4.00", "4.00"}}},
            {"geral/versaoQrCode", "Versão do QR Code", "2.00", {{"2.00", "2"}, {"3.00", "3"}}},
            {"geral/tipoEmissao", "Tipo de emissão", "1", {
                {"1", "Normal"}, {"2", "FS-IA"}, {"4", "EPEC"}, {"5", "FS-DA"},
                {"6", "SVC-AN"}, {"7", "SVC-RS"}, {"8", "SVC-SP"}, {"9", "Offline"}}},
            {"geral/sincrono", "Envio síncrono", true},
            {"geral/idCSC", "Identificador do CSC", ""},
            {"geral/CSC", "CSC", "", {}, true},
        }},
        {"Arquivos", {
            {"geral/schemas", "Pasta dos schemas XSD", ""},
            {"geral/salvar", "Pasta de saída", ""},
            {"arquivos/eventos", "Pasta de eventos", ""},
            {"arquivos/consultas", "Pasta de consultas", ""},
            {"arquivos/salvar", "Salvar XMLs", true},
            {"arquivos/salvarLogs", "Salvar logs", true},
            {"arquivos/porModelo", "Organizar por modelo", true},
            {"arquivos/porCNPJ", "Organizar por CNPJ", true},
            {"arquivos/porAnoMesDia", "Organizar por data (AAAAMMDD)", false},
            {"arquivos/porAnoMes", "Organizar por ano e mês (AAAAMM)", true},
            {"arquivos/porAno", "Organizar por ano", false},
            {"arquivos/porMes", "Organizar por mês", false},
            {"arquivos/porDia", "Organizar por dia", false},
        }},
        {"Web services", {
            {"geral/ambiente", "Ambiente", "homologacao",
             {{"homologacao", "Homologação"}, {"producao", "Produção"}}},
            {"geral/uf", "UF autorizadora", "RO", {
                {"AC", "AC"}, {"AL", "AL"}, {"AP", "AP"}, {"AM", "AM"}, {"BA", "BA"},
                {"CE", "CE"}, {"DF", "DF"}, {"ES", "ES"}, {"GO", "GO"}, {"MA", "MA"},
                {"MT", "MT"}, {"MS", "MS"}, {"MG", "MG"}, {"PA", "PA"}, {"PB", "PB"},
                {"PR", "PR"}, {"PE", "PE"}, {"PI", "PI"}, {"RJ", "RJ"}, {"RN", "RN"},
                {"RS", "RS"}, {"RO", "RO"}, {"RR", "RR"}, {"SC", "SC"}, {"SP", "SP"},
                {"SE", "SE"}, {"TO", "TO"}}},
            {"geral/http", "Cliente HTTP", "HttpQt", {{"HttpQt", "Qt"}, {"HttpOpenSSL", "OpenSSL"}}},
            {"webservices/protocoloSSL", "Protocolo TLS", "tls12+", {
                {"tls12+", "TLS 1.2 ou superior"}, {"tls12", "TLS 1.2"},
                {"tls13+", "TLS 1.3 ou superior"}, {"tls13", "TLS 1.3"},
                {"seguro", "Protocolos seguros do Qt"}}},
            {"webservices/timeoutConexao", "Timeout de conexão (ms)", 0},
            {"geral/timeout", "Espera entre consultas assíncronas (ms)", 8000},
            {"webservices/tentativas", "Tentativas de consulta", 3},
            {"webservices/compactar", "Compactar envio", true},
            {"webservices/compactarAcimaDe", "Limite para compactação (KB)", 300},
            {"webservices/verificarSslSocket", "Verificar SSL", true},
        }},
        {"Proxy", {
            {"proxy/host", "Servidor", ""},
            {"proxy/porta", "Porta", 0},
            {"proxy/usuario", "Usuário", ""},
            {"proxy/senha", "Senha", "", {}, true},
        }},
        {"Certificado", {
            {"certificado/crypto", "Biblioteca criptográfica", "OpenSSL",
             {{"OpenSSL", "OpenSSL"}, {"nenhuma", "Nenhuma"}}},
            {"certificado/arquivo", "Arquivo PFX", ""},
            {"certificado/senha", "Senha do PFX", "", {}, true},
            {"certificado/cadeia", "Arquivos da cadeia (separados por vírgula)", ""},
        }},
    };
    return grupos;
}

const GrupoIni &camposEmitente()
{
    static const GrupoIni grupo = {"Dados do emitente", {
        {"emitente/CNPJ", "CNPJ", ""},
        {"emitente/CPF", "CPF", ""},
        {"emitente/xNome", "Razão social ou nome", ""},
        {"emitente/xFant", "Nome fantasia", ""},
        {"emitente/IE", "Inscrição estadual", ""},
        {"emitente/IEST", "Inscrição estadual do substituto tributário", ""},
        {"emitente/IM", "Inscrição municipal", ""},
        {"emitente/CNAE", "CNAE", ""},
        {"emitente/CRT", "Regime tributário", "3", {
            {"1", "Simples Nacional"}, {"2", "Simples Nacional — excesso de sublimite"},
            {"3", "Regime normal"}, {"4", "MEI"}}},
        {"emitente/ISUFEmit", "Inscrição SUFRAMA", ""},
        {"emitente/xLgr", "Logradouro", ""},
        {"emitente/nro", "Número", 0},
        {"emitente/xCpl", "Complemento", ""},
        {"emitente/xBairro", "Bairro", ""},
        {"emitente/cMun", "Código IBGE do município", 0},
        {"emitente/xMun", "Município", ""},
        {"emitente/UF", "UF", ""},
        {"emitente/CEP", "CEP", 0},
        {"emitente/cPais", "Código do país", 1058},
        {"emitente/xPais", "País", "BRASIL"},
        {"emitente/fone", "Telefone", ""},
    }};
    return grupo;
}

void configurar(CppNFe &cppnfe, const QSettings &ini)
{
    ConfigNFe &cfg = *cppnfe.configuracoes;
    auto *arquivos = cfg.arquivos.get();
    auto *ws = cfg.webservices.get();

    const auto ler = [&ini](const QString &chave) {
        for (const auto &grupo : camposConfiguracao())
            for (const auto &campo : grupo.campos)
                if (campo.chave == chave)
                    return ini.value(chave, campo.padrao);
        return ini.value(chave);
    };

    cfg.set_ModeloDF(ConvNF::strToModeloDF(ler("geral/modelo").toString()));
    cfg.set_VersaoNF(ConvNF::strToVersaoNF(ler("geral/versao").toString()));
    cfg.set_VersaoQrCodeNF(ConvNF::strToVersaoQrCodeNF(ler("geral/versaoQrCode").toString()));
    cfg.set_TipoEmissao(ConvNF::strToTpEmis(ler("geral/tipoEmissao").toString()));
    cfg.set_indicadorSincrono(ler("geral/sincrono").toBool() ? IndSinc::Sim : IndSinc::Nao);
    cfg.set_IdCSC(ler("geral/idCSC").toString());
    cfg.set_CSC(ler("geral/CSC").toString());

    arquivos->set_caminhoSchema(ler("geral/schemas").toString());
    const QString pasta = ler("geral/salvar").toString();
    arquivos->set_caminhoSalvar(pasta.isEmpty() ? QCoreApplication::applicationDirPath() : pasta);
    arquivos->set_caminhoSalvarEvento(ler("arquivos/eventos").toString());
    arquivos->set_caminhoSalvarConsulta(ler("arquivos/consultas").toString());
    arquivos->set_salvar(ler("arquivos/salvar").toBool());
    arquivos->set_salvarLogs(ler("arquivos/salvarLogs").toBool());
    arquivos->set_salvarPorModelo(ler("arquivos/porModelo").toBool());
    arquivos->set_salvarPorCNPJ(ler("arquivos/porCNPJ").toBool());
    arquivos->set_salvarPorAnoMesDia(ler("arquivos/porAnoMesDia").toBool());
    arquivos->set_salvarPorAnoMes(ler("arquivos/porAnoMes").toBool());
    arquivos->set_salvarPorAno(ler("arquivos/porAno").toBool());
    arquivos->set_salvarPorMes(ler("arquivos/porMes").toBool());
    arquivos->set_salvarPorDia(ler("arquivos/porDia").toBool());

    const bool producao = ler("geral/ambiente").toString().compare("producao", Qt::CaseInsensitive) == 0;
    ws->set_tpAmb(producao ? TpAmb::Producao : TpAmb::Homologacao);
    ws->set_uf(ConvNF::strToUF(ler("geral/uf").toString()));
    ws->set_httpType(ler("geral/http").toString() == "HttpOpenSSL" ? HttpType::HttpOpenSSL
                                                                 : HttpType::HttpQt);
    static const QHash<QString, QSsl::SslProtocol> protocolos = {
        {"tls12+", QSsl::TlsV1_2OrLater}, {"tls12", QSsl::TlsV1_2},
        {"tls13+", QSsl::TlsV1_3OrLater}, {"tls13", QSsl::TlsV1_3},
        {"seguro", QSsl::SecureProtocols},
    };
    ws->set_protocoloSSL(protocolos.value(ler("webservices/protocoloSSL").toString(), QSsl::TlsV1_2OrLater));
    ws->set_timeoutConexao(ler("webservices/timeoutConexao").toInt());
    ws->set_timeoutAssincrono(ler("geral/timeout").toInt());
    ws->set_tentativas(ler("webservices/tentativas").toInt());
    ws->set_compactar(ler("webservices/compactar").toBool());
    ws->set_compactarAcimaDe(ler("webservices/compactarAcimaDe").toInt());
    ws->set_verificarSslSocket(ler("webservices/verificarSslSocket").toBool());
    ws->set_proxyHostName(ler("proxy/host").toString());
    ws->set_proxyPort(ler("proxy/porta").toInt());
    ws->set_proxyUser(ler("proxy/usuario").toString());
    ws->set_proxyPassword(ler("proxy/senha").toString());
}

bool carregarCertificado(CppNFe &cppnfe, const QSettings &ini, QTextStream &out)
{
    auto *certificado = cppnfe.configuracoes->certificado.get();
    QString senha = ini.value("certificado/senha").toString();
    if (senha.isEmpty())
        senha = qEnvironmentVariable("CPPBR_CERT_SENHA");
    const QString pfx = ini.value("certificado/arquivo").toString();
    if (ini.value("certificado/crypto", "OpenSSL").toString() != "OpenSSL"
        || pfx.isEmpty() || senha.isEmpty())
    {
        out << "Configure o certificado PFX, a senha e o OpenSSL para assinar e usar a SEFAZ.\n";
        return false;
    }

    certificado->set_cryptoLib(CryptoType::OpenSSL);
    certificado->set_caminhoCertificado(pfx);
    certificado->set_senhaCertificado(senha);
    const QString cadeia = ini.value("certificado/cadeia").toStringList().join(',');
    for (const QString &ac : cadeia.split(',', Qt::SkipEmptyParts))
    {
        if (ac.trimmed().isEmpty())
            continue;
        certificado->addCaminhoCertificadoAC(ac.trimmed());
    }

    if (!certificado->carregarCertificado())
    {
        out << "Erro ao carregar o certificado: " << certificado->get_error() << "\n";
        return false;
    }
    out << "Certificado: " << certificado->get_razaoSocial()
        << " (" << certificado->get_cnpj() << ")\n";
    return true;
}

namespace {

void preencherIde(Ide &ide, const ConfigNFe &cfg, const QSettings &ini)
{
    ide.set_cUF(ConvNF::ufToInt(cfg.webservices->get_uf()));
    ide.set_cNF(CppUtility::random(8));
    ide.set_natOp(DadosNota::naturezaOperacao);
    ide.set_mod(cfg.get_ModeloDF());
    ide.set_serie(ini.value("nota/serie", DadosNota::serie).toInt());
    ide.set_nNF(ini.value("nota/numero", 1).toInt());
    ide.set_dhEmi(QDateTime::currentDateTime());
    ide.set_tpNF(DadosNota::tipoOperacao);
    ide.set_idDest(DadosNota::localOperacao);
    ide.set_cMunFG(ini.value("emitente/cMun").toInt());
    ide.set_tpImp(cfg.get_ModeloDF() == ModeloDF::NFCe ? TpImp::NFCe : TpImp::Retrato);
    ide.set_tpEmis(cfg.get_TipoEmissao());
    ide.set_tpAmb(cfg.webservices->get_tpAmb());
    ide.set_finNFe(DadosNota::finalidade);
    ide.set_indFinal(DadosNota::consumidorFinal);
    ide.set_indPres(DadosNota::presenca);
    ide.set_indIntermed(DadosNota::intermediador);
    ide.set_procEmi(DadosNota::processoEmissao);
    ide.set_verProc(DadosNota::versaoAplicativo);
    if (cfg.get_TipoEmissao() != TpEmis::Normal)
    {
        ide.set_dhCont(ide.get_dhEmi());
        ide.set_xJust(DadosNota::justificativaContingencia);
    }
}

void preencherEmitente(Emit &emitente, const QSettings &ini)
{
    emitente.set_CNPJ(ini.value("emitente/CNPJ").toString());
    emitente.set_CPF(ini.value("emitente/CPF").toString());
    emitente.set_xNome(ini.value("emitente/xNome").toString());
    emitente.set_xFant(ini.value("emitente/xFant").toString());
    emitente.set_IE(ini.value("emitente/IE").toString());
    emitente.set_IEST(ini.value("emitente/IEST").toString());
    emitente.set_IM(ini.value("emitente/IM").toString());
    emitente.set_CNAE(ini.value("emitente/CNAE").toString());
    emitente.set_CRT(ConvNF::strToCrt(ini.value("emitente/CRT", "3").toString()));
    emitente.set_ISUFEmit(ini.value("emitente/ISUFEmit").toString());

    auto *ender = emitente.enderEmit.get();
    ender->set_xLgr(ini.value("emitente/xLgr").toString());
    ender->set_nro(ini.value("emitente/nro").toInt());
    ender->set_xCpl(ini.value("emitente/xCpl").toString());
    ender->set_xBairro(ini.value("emitente/xBairro").toString());
    ender->set_cMun(ini.value("emitente/cMun").toInt());
    ender->set_xMun(ini.value("emitente/xMun").toString());
    ender->set_UF(ini.value("emitente/UF").toString());
    ender->set_CEP(ini.value("emitente/CEP").toInt());
    ender->set_cPais(ini.value("emitente/cPais", 1058).toInt());
    ender->set_xPais(ini.value("emitente/xPais", "BRASIL").toString());
    ender->set_fone(ini.value("emitente/fone").toString());
}

void preencherDestinatario(Dest &dest, const QSettings &ini)
{
    dest.set_CPF(DadosNota::destinatarioCpf);
    dest.set_xNome(DadosNota::destinatarioNome);
    dest.set_indIEDest(DadosNota::destinatarioIndicadorIE);

    auto *ender = dest.enderDest.get();
    ender->set_xLgr(DadosNota::destinatarioLogradouro);
    ender->set_nro(DadosNota::destinatarioNumero);
    ender->set_xBairro(DadosNota::destinatarioBairro);
    if constexpr (DadosNota::destinatarioCodMunicipio == 0)
    {
        ender->set_cMun(ini.value("emitente/cMun").toInt());
        ender->set_xMun(ini.value("emitente/xMun").toString());
        ender->set_UF(ini.value("emitente/UF").toString());
        ender->set_CEP(ini.value("emitente/CEP").toInt());
    }
    else
    {
        ender->set_cMun(DadosNota::destinatarioCodMunicipio);
        ender->set_xMun(DadosNota::destinatarioMunicipio);
        ender->set_UF(DadosNota::destinatarioUF);
        ender->set_CEP(DadosNota::destinatarioCEP);
    }
    ender->set_cPais(1058);
    ender->set_xPais("BRASIL");
}

// Preenche o rascunho (obj) do container det e confirma com add().
void adicionarItem(Container<Det> &itens, const DadosNota::Item &item)
{
    Det *det = itens.obj;
    const int nItem = itens.count() + 1;
    const double valor = item.quantidade * item.valorUnitario;

    auto *prod = det->prod.get();
    prod->set_nItem(nItem);
    prod->set_cProd(item.codigo);
    prod->set_cEAN(DadosNota::gtin);
    prod->set_xProd(item.descricao);
    prod->set_NCM(item.ncm);
    prod->set_CFOP(DadosNota::cfop);
    prod->set_uCom(DadosNota::unidade);
    prod->set_qCom(item.quantidade);
    prod->set_vUnCom(item.valorUnitario);
    prod->set_vProd(valor);
    prod->set_cEANTrib(DadosNota::gtin);
    prod->set_uTrib(DadosNota::unidade);
    prod->set_qTrib(item.quantidade);
    prod->set_vUnTrib(item.valorUnitario);
    prod->set_indTot(IndTot::CompoeTotalNF);

    auto &imposto = *det->imposto;
    auto *icms = imposto.ICMS.get();
    icms->set_orig(DadosNota::origem);
    icms->set_CST(DadosNota::cstICMS);
    icms->set_modBC(DadosNota::modalidadeICMS);
    icms->set_vBC(valor);
    icms->set_pICMS(DadosNota::aliquotaICMS);
    icms->set_vICMS(qRound(valor * DadosNota::aliquotaICMS) / 100.0);
    imposto.PIS->set_CST(DadosNota::cstPIS);
    imposto.COFINS->set_CST(DadosNota::cstCOFINS);

    // IBS e CBS (Reforma Tributária, NT 2025.002): tributação integral, CST 000.
    // Alíquotas de teste de 2026: IBS UF 0,1%, IBS municipal 0% e CBS 0,9%.
    // A base exclui o ICMS (e o PIS/COFINS, aqui zerados).
    auto &ibscbs = *imposto.IBSCBS;
    ibscbs.set_CST(DadosNota::cstIBSCBS);
    ibscbs.set_cClassTrib(DadosNota::classeIBSCBS);

    auto *g = ibscbs.gIBSCBS.get();
    const double vBCIBSCBS = valor - icms->get_vICMS();
    const double vIBSUF = qRound(vBCIBSCBS * DadosNota::aliquotaIBSUF) / 100.0;
    const double vIBSMun = qRound(vBCIBSCBS * DadosNota::aliquotaIBSMun) / 100.0;
    const double vCBS = qRound(vBCIBSCBS * DadosNota::aliquotaCBS) / 100.0;
    g->set_vBC(vBCIBSCBS);
    g->gIBSUF->set_pIBSUF(DadosNota::aliquotaIBSUF);
    g->gIBSUF->set_vIBSUF(vIBSUF);
    g->gIBSMun->set_pIBSMun(DadosNota::aliquotaIBSMun);
    g->gIBSMun->set_vIBSMun(vIBSMun);
    g->set_vIBS(vIBSUF + vIBSMun);
    g->gCBS->set_pCBS(DadosNota::aliquotaCBS);
    g->gCBS->set_vCBS(vCBS);

    itens.add();
}

void preencherTotais(InfNFe &inf)
{
    double vProd = 0, vBC = 0, vICMS = 0;
    double vBCIBSCBS = 0, vIBSUF = 0, vIBSMun = 0, vCBS = 0;
    for (const Det *det : *inf.det)
    {
        const auto &imposto = *det->imposto;
        vProd += det->prod->get_vProd();
        vBC += imposto.ICMS->get_vBC();
        vICMS += imposto.ICMS->get_vICMS();

        const auto *g = imposto.IBSCBS->gIBSCBS.get();
        vBCIBSCBS += g->get_vBC();
        vIBSUF += g->gIBSUF->get_vIBSUF();
        vIBSMun += g->gIBSMun->get_vIBSMun();
        vCBS += g->gCBS->get_vCBS();
    }

    auto *tot = inf.total->ICMSTot.get();
    tot->set_vBC(vBC);
    tot->set_vICMS(vICMS);
    tot->set_vProd(vProd);
    tot->set_vNF(vProd);

    // Totais do IBS e da CBS (grupo W03). Em 2026 eles não compõem o vNF.
    auto &rtc = *inf.total->IBSCBSTot;
    rtc.set_vBCIBSCBS(vBCIBSCBS);
    rtc.gIBS->gIBSUF->set_vIBSUF(vIBSUF);
    rtc.gIBS->gIBSMun->set_vIBSMun(vIBSMun);
    rtc.gIBS->set_vIBS(vIBSUF + vIBSMun);
    rtc.gCBS->set_vCBS(vCBS);

    inf.transp->set_modFrete(DadosNota::frete);

    auto &pagamentos = *inf.pag->detPag;
    pagamentos.obj->set_tPag(DadosNota::pagamento);
    pagamentos.obj->set_vPag(vProd);
    pagamentos.add();
}

void preencherNota(CppNFe &cppnfe, const QSettings &ini)
{
    auto &notas = *cppnfe.notafiscal->NFe;  // Container<Nfe>
    notas.clear();

    InfNFe *inf = notas.obj->infNFe.get();
    inf->set_versao(ConvNF::versaoNFToStr(cppnfe.configuracoes->get_VersaoNF()));
    preencherIde(*inf->ide, *cppnfe.configuracoes, ini);
    preencherEmitente(*inf->emite, ini);
    preencherDestinatario(*inf->dest, ini);

    auto &itens = *inf->det;
    for (const auto &item : DadosNota::itens)
        adicionarItem(itens, item);

    preencherTotais(*inf);
    inf->infAdic->set_infCpl(DadosNota::informacoesComplementares);

    notas.add();  // confirma a nota no container
}

}

bool gerar(CppNFe &cppnfe, const QSettings &ini, QTextStream &out)
{
    auto &nf = *cppnfe.notafiscal;
    preencherNota(cppnfe, ini);

    if (!nf.gerar())
    {
        out << "Erro ao gerar: " << nf.NFe->value(0)->get_error() << "\n";
        return false;
    }
    out << "Chave: " << nf.NFe->value(0)->get_chNFe() << "\n";
    return true;
}

bool assinar(CppNFe &cppnfe, QTextStream &out)
{
    auto &nf = *cppnfe.notafiscal;
    const Nfe *nota = nf.NFe->value(0);
    if (!nf.assinar())
    {
        out << "Erro ao assinar: " << nota->get_error() << "\n";
        return false;
    }
    if (!nf.validar())
    {
        out << "Erro de validação (XSD): " << nota->get_error() << "\n";
        return false;
    }
    out << "NF-e assinada e validada pelo XSD.\n";
    if (cppnfe.configuracoes->arquivos->get_salvar())
        out << "Pasta de saída: "
            << QDir::toNativeSeparators(cppnfe.configuracoes->arquivos->get_caminhoNF()) << "\n";
    return true;
}

bool enviar(CppNFe &cppnfe, int lote, QTextStream &out)
{
    auto &nf = *cppnfe.notafiscal;
    nf.retorno->clear();
    if (!nf.enviar(lote))
    {
        out << "Erro no envio: " << nf.get_error() << "\n";
        return false;
    }

    const auto &ret = *nf.retorno;
    out << "Lote " << lote << ": cStat " << ret.get_cStat() << " - " << ret.get_xMotivo() << "\n";
    for (const ProtNFe *prot : *ret.protNFe)
    {
        out << "  Chave " << prot->get_chNFe() << ": cStat " << prot->get_cStat()
            << " - " << prot->get_xMotivo() << "\n";
        if (!prot->get_nProt().isEmpty())
            out << "  Protocolo: " << prot->get_nProt() << "\n";
    }
    return true;
}

void status(CppNFe &cppnfe, QTextStream &out)
{
    auto &servico = *cppnfe.eventos->statusServico;
    servico.clear();
    if (!servico.status())
    {
        out << "Falha na consulta do status do serviço.\n";
        return;
    }

    const auto *ret = servico.retorno.get();
    out << "cStat: " << ret->get_cStat() << " - " << ret->get_xMotivo() << "\n"
        << "Ambiente: " << ConvNF::tpAmbToStr(ret->get_tpAmb()) << "\n"
        << "Recebimento: " << CppUtility::dateTimeToStr(ret->get_dhRecbto(), DtH::DateTimeBr) << "\n"
        << "Tempo médio: " << ret->get_tMed() << " s\n";
}

void consultar(CppNFe &cppnfe, const QString &chave, QTextStream &out)
{
    auto &consulta = *cppnfe.eventos->consSitNFe;
    consulta.clear();
    if (!consulta.consultar(chave))
    {
        out << "Falha na consulta da NF-e.\n";
        return;
    }

    const auto *ret = consulta.retorno.get();
    out << "cStat: " << ret->get_cStat() << " - " << ret->get_xMotivo() << "\n";
    if (!ret->protNFe->get_nProt().isEmpty())
        out << "Protocolo: " << ret->protNFe->get_nProt() << "\n";
    out << "Eventos vinculados: " << ret->procEventoNFe->count() << "\n";
}

void cancelar(CppNFe &cppnfe, const QSettings &ini, const QString &chave, const QString &protocolo,
              const QString &justificativa, QTextStream &out)
{
    auto &cancelamento = *cppnfe.eventos->cancNFe;
    cancelamento.clear();
    cancelamento.set_CNPJ(ini.value("emitente/CNPJ").toString());
    cancelamento.set_chNFe(chave);
    cancelamento.set_nProt(protocolo);
    cancelamento.set_xJust(justificativa);

    if (!cancelamento.cancelar())
    {
        out << "Erro no cancelamento: " << cancelamento.get_error() << "\n";
        return;
    }

    const auto *ret = cancelamento.retorno.get();
    out << "Lote: cStat " << ret->get_cStatLote() << " - " << ret->get_xMotivoLote() << "\n"
        << "Evento: cStat " << ret->get_cStat() << " - " << ret->get_xMotivo() << "\n";
    if (ret->get_cStat() == 135 || ret->get_cStat() == 155)
        out << "Cancelamento homologado. Protocolo: " << ret->get_nProt() << "\n";
}

void inutilizar(CppNFe &cppnfe, const QSettings &ini, int serie, int inicio, int fim,
                const QString &justificativa, QTextStream &out)
{
    auto &inutilizacao = *cppnfe.eventos->inutNFe;
    inutilizacao.clear();

    auto *inf = inutilizacao.infInut.get();
    inf->set_ano(QDate::currentDate().year() % 100);
    inf->set_CNPJ(ini.value("emitente/CNPJ").toString());
    inf->set_serie(serie);
    inf->set_nNFIni(inicio);
    inf->set_nNFFin(fim);
    inf->set_xJust(justificativa);

    if (!inf->inutilizar())
    {
        out << "Erro na inutilização: " << inf->get_error() << "\n";
        return;
    }

    const auto *ret = inutilizacao.retInutNFe->infInut.get();
    out << "cStat: " << ret->get_cStat() << " - " << ret->get_xMotivo() << "\n";
    if (ret->get_cStat() == 102)
        out << "Inutilização homologada. Protocolo: " << ret->get_nProt() << "\n";
}

QString nomeServico(const WebServicesNF &ws)
{
    switch (ws)
    {
    case WebServicesNF::NFeAutorizacao: return "Enviando a NF-e";
    case WebServicesNF::NFeRetAutorizacao: return "Consultando o recibo do lote";
    case WebServicesNF::NFeConsultaProtocolo: return "Consultando a NF-e";
    case WebServicesNF::NFeInutilizacao: return "Inutilizando a numeração";
    case WebServicesNF::NFeRecepcaoEvento: return "Enviando o evento";
    case WebServicesNF::NFeStatusServico: return "Consultando o status do serviço";
    case WebServicesNF::NFeConsultaCadastro: return "Consultando o cadastro";
    default: return "Comunicando com a SEFAZ";
    }
}

}
