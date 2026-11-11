"""Modelos da CppNFe. Gerado por tools/generate.py a partir de schema.json. Não editar."""
from __future__ import annotations

from ._native import Model, _Child, _Collection, _bool, _float, _int, _str

class Adi(Model):
    __slots__ = ()
    nAdicao = _int("nAdicao")
    nSeqAdic = _int("nSeqAdic")
    cFabricante = _str("cFabricante")
    vDescDI = _float("vDescDI")
    nDraw = _str("nDraw")

class Agropecuario(Model):
    __slots__ = ()
    defensivo: _Collection[Defensivo] = _Collection("defensivo", "Defensivo")
    guiaTransito: _Child[GuiaTransito] = _Child("guiaTransito", "GuiaTransito")

class Arma(Model):
    __slots__ = ()
    tpArma = _int("tpArma")
    nSerie = _str("nSerie")
    nCano = _str("nCano")
    descr = _str("descr")

class ArquivosNF(Model):
    __slots__ = ()
    caminhoSalvar = _str("caminhoSalvar")
    caminhoSchema = _str("caminhoSchema")
    salvar = _bool("salvar")
    salvarLogs = _bool("salvarLogs")
    salvarPorAnoMesDia = _bool("salvarPorAnoMesDia")
    salvarPorAnoMes = _bool("salvarPorAnoMes")
    salvarPorAno = _bool("salvarPorAno")
    salvarPorMes = _bool("salvarPorMes")
    salvarPorDia = _bool("salvarPorDia")
    salvarPorCNPJ = _bool("salvarPorCNPJ")
    salvarPorModelo = _bool("salvarPorModelo")
    caminhoSalvarEvento = _str("caminhoSalvarEvento")
    caminhoSalvarConsulta = _str("caminhoSalvarConsulta")
    caminhoNF = _str("caminhoNF", write=False)
    caminhoLogs = _str("caminhoLogs", write=False)
    caminhoEvento = _str("caminhoEvento", write=False)
    caminhoConsulta = _str("caminhoConsulta", write=False)

class AutXML(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")

class AutXMLAtor(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")

class Avulsa(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    xOrgao = _str("xOrgao")
    matr = _str("matr")
    xAgente = _str("xAgente")
    fone = _str("fone")
    UF = _str("UF")
    nDAR = _str("nDAR")
    dEmi = _str("dEmi")
    vDAR = _float("vDAR")
    repEmi = _str("repEmi")
    dPag = _str("dPag")

class Cana(Model):
    __slots__ = ()
    forDia: _Collection[ForDia] = _Collection("forDia", "ForDia")
    deduc: _Collection[Deduc] = _Collection("deduc", "Deduc")
    safra = _str("safra")
    ref = _str("ref")
    qTotMes = _float("qTotMes")
    qTotAnt = _float("qTotAnt")
    qTotGer = _float("qTotGer")
    vFor = _float("vFor")
    vTotDed = _float("vTotDed")
    vLiqFor = _float("vLiqFor")

class CancNFe(Model):
    __slots__ = ()
    retorno: _Child[RetEventoCancNFe] = _Child("retorno", "RetEventoCancNFe")
    tpEvento = _int("tpEvento")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    chNFe = _str("chNFe")
    dhEvento = _str("dhEvento")
    nProt = _str("nProt")
    xJust = _str("xJust")
    chNFeRef = _str("chNFeRef")
    verAplic = _str("verAplic")
    versao = _str("versao")
    Id = _str("Id", write=False)
    XMLOriginal = _str("XMLOriginal", write=False)
    XMLAssinado = _str("XMLAssinado", write=False)
    error = _str("error", write=False)

class Card(Model):
    __slots__ = ()
    tpIntegra = _int("tpIntegra")
    CNPJ = _str("CNPJ")
    tBand = _int("tBand")
    cAut = _str("cAut")
    CNPJReceb = _str("CNPJReceb")
    idTermPag = _str("idTermPag")

class Certificado(Model):
    __slots__ = ()
    error = _str("error", write=False)
    numeroSerie = _str("numeroSerie", write=False)
    issuerName = _str("issuerName", write=False)
    subjectName = _str("subjectName", write=False)
    dataEmissao = _str("dataEmissao", write=False)
    dataVencimento = _str("dataVencimento", write=False)
    certificadora = _str("certificadora", write=False)
    razaoSocial = _str("razaoSocial", write=False)
    cnpj = _str("cnpj", write=False)

class Cide(Model):
    __slots__ = ()
    qBCProd = _float("qBCProd")
    vAliqProd = _float("vAliqProd")
    vCIDE = _float("vCIDE")

class Cobr(Model):
    __slots__ = ()
    fat: _Child[Fat] = _Child("fat", "Fat")
    dup: _Collection[Dup] = _Collection("dup", "Dup")

class Cofins(Model):
    __slots__ = ()
    CST = _int("CST")
    vBC = _float("vBC")
    pCOFINS = _float("pCOFINS")
    vCOFINS = _float("vCOFINS")
    qBCProd = _float("qBCProd")
    vAliqProd = _float("vAliqProd")

class CofinsST(Model):
    __slots__ = ()
    vBC = _float("vBC")
    pCOFINS = _float("pCOFINS")
    qBCProd = _float("qBCProd")
    vAliqProd = _float("vAliqProd")
    vCOFINS = _float("vCOFINS")
    indSomaCOFINSST = _int("indSomaCOFINSST")

class Comb(Model):
    __slots__ = ()
    CIDE: _Child[Cide] = _Child("CIDE", "Cide")
    encerrante: _Child[Encerrante] = _Child("encerrante", "Encerrante")
    origComb: _Collection[OrigComb] = _Collection("origComb", "OrigComb")
    cProdANP = _int("cProdANP")
    descANP = _str("descANP")
    pGLP = _float("pGLP")
    pGNn = _float("pGNn")
    pGNi = _float("pGNi")
    vPart = _float("vPart")
    CODIF = _str("CODIF")
    qTemp = _float("qTemp")
    UFCons = _str("UFCons")
    pBio = _float("pBio")

class Compra(Model):
    __slots__ = ()
    xNEmp = _str("xNEmp")
    xPed = _str("xPed")
    xCont = _str("xCont")

class ConfWebServices(Model):
    __slots__ = ()
    uf = _int("uf")
    tpAmb = _int("tpAmb")
    timeoutConexao = _int("timeoutConexao")
    timeoutAssincrono = _int("timeoutAssincrono")
    tentativas = _int("tentativas")
    proxyHostName = _str("proxyHostName")
    proxyPort = _int("proxyPort")
    proxyUser = _str("proxyUser")
    proxyPassword = _str("proxyPassword")
    httpType = _int("httpType")
    compactar = _bool("compactar")
    compactarAcimaDe = _int("compactarAcimaDe")
    verificarSslSocket = _bool("verificarSslSocket")

class ConfigNFe(Model):
    __slots__ = ()
    certificado: _Child[Certificado] = _Child("certificado", "Certificado")
    arquivos: _Child[ArquivosNF] = _Child("arquivos", "ArquivosNF")
    webservices: _Child[ConfWebServices] = _Child("webservices", "ConfWebServices")
    ModeloDF = _int("ModeloDF")
    VersaoNF = _int("VersaoNF")
    IdCSC = _str("IdCSC")
    CSC = _str("CSC")
    VersaoQrCodeNF = _int("VersaoQrCodeNF")
    indicadorSincrono = _int("indicadorSincrono")
    TipoEmissao = _int("TipoEmissao")

class ConsSitNFe(Model):
    __slots__ = ()
    retorno: _Child[RetConsSitNFe] = _Child("retorno", "RetConsSitNFe")
    versao = _str("versao")
    error = _str("error", write=False)

class CppNFe(Model):
    __slots__ = ()
    configuracoes: _Child[ConfigNFe] = _Child("configuracoes", "ConfigNFe")
    notafiscal: _Child[NotaFiscal] = _Child("notafiscal", "NotaFiscal")
    eventos: _Child[EventoNotaFiscal] = _Child("eventos", "EventoNotaFiscal")

class CreditoPresumidoEvento(Model):
    __slots__ = ()
    pCredPres = _float("pCredPres")
    vCredPres = _float("vCredPres")

class Deduc(Model):
    __slots__ = ()
    xDed = _str("xDed")
    vDed = _float("vDed")

class Defensivo(Model):
    __slots__ = ()
    nReceituario = _str("nReceituario")
    CPFRespTec = _str("CPFRespTec")

class Dest(Model):
    __slots__ = ()
    enderDest: _Child[EnderDest] = _Child("enderDest", "EnderDest")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    idEstrangeiro = _str("idEstrangeiro")
    xNome = _str("xNome")
    indIEDest = _int("indIEDest")
    IE = _str("IE")
    ISUF = _str("ISUF")
    IM = _str("IM")
    email = _str("email")

class DestEPEC(Model):
    __slots__ = ()
    UF = _int("UF")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    idEstrangeiro = _str("idEstrangeiro")
    IE = _str("IE")

class Det(Model):
    __slots__ = ()
    prod: _Child[Prod] = _Child("prod", "Prod")
    imposto: _Child[Imposto] = _Child("imposto", "Imposto")
    impostoDevol: _Child[ImpostoDevol] = _Child("impostoDevol", "ImpostoDevol")
    obsItem: _Child[ObsItem] = _Child("obsItem", "ObsItem")
    DFeReferenciado: _Child[DfeReferenciado] = _Child("DFeReferenciado", "DfeReferenciado")
    infAdProd = _str("infAdProd")
    vItem = _float("vItem")

class DetEvento(Model):
    __slots__ = ()
    dest: _Child[DestEPEC] = _Child("dest", "DestEPEC")
    itemPedido: _Collection[ItemPedProrrog] = _Collection("itemPedido", "ItemPedProrrog")
    autXML: _Child[AutXMLAtor] = _Child("autXML", "AutXMLAtor")
    detPag: _Collection[DetPag] = _Collection("detPag", "DetPag")
    respPedido: _Child[RespPedido] = _Child("respPedido", "RespPedido")
    respCancPedido: _Child[RespCancPedido] = _Child("respCancPedido", "RespCancPedido")
    rtc: _Child[DetEventoRTC] = _Child("rtc", "DetEventoRTC")
    descEvento = _str("descEvento")
    cOrgaoAutor = _int("cOrgaoAutor")
    tpAutor = _int("tpAutor")
    verAplic = _str("verAplic")
    nProt = _str("nProt")
    idPedidoCancelado = _str("idPedidoCancelado")
    idPedido = _str("idPedido")
    xJust = _str("xJust")
    chNFeRef = _str("chNFeRef")
    xCorrecao = _str("xCorrecao")
    xCondUso = _str("xCondUso")
    dhEmi = _str("dhEmi")
    tpNF = _int("tpNF")
    IE = _str("IE")
    vNF = _float("vNF")
    vICMS = _float("vICMS")
    vST = _float("vST")
    tpAutorizacao = _int("tpAutorizacao")
    dhEntrega = _str("dhEntrega")
    nDoc = _str("nDoc")
    xNome = _str("xNome")
    latGPS = _str("latGPS")
    longGPS = _str("longGPS")
    hashComprovante = _str("hashComprovante")
    dhHashComprovante = _str("dhHashComprovante")
    nProtEvento = _str("nProtEvento")
    dhTentativaEntrega = _str("dhTentativaEntrega")
    nTentativa = _int("nTentativa")
    tpMotivo = _int("tpMotivo")
    xJustMotivo = _str("xJustMotivo")
    hashTentativaEntrega = _str("hashTentativaEntrega")
    dhHashTentativaEntrega = _str("dhHashTentativaEntrega")

class DetEventoRTC(Model):
    __slots__ = ()
    itens: _Collection[ItemEventoRTC] = _Collection("itens", "ItemEventoRTC")
    gCredPresOper: _Collection[ItemCreditoPresumidoEvento] = _Collection("gCredPresOper", "ItemCreditoPresumidoEvento")
    gPgto: _Child[PagamentoVinculadoEvento] = _Child("gPgto", "PagamentoVinculadoEvento")
    indAceitacao = _int("indAceitacao")
    dPrevEntrega = _str("dPrevEntrega")
    tpEventoAut = _int("tpEventoAut")

class DetExport(Model):
    __slots__ = ()
    exportInd: _Child[ExportInd] = _Child("exportInd", "ExportInd")
    nDraw = _str("nDraw")

class DetPag(Model):
    __slots__ = ()
    card: _Child[Card] = _Child("card", "Card")
    indPag = _int("indPag")
    tPag = _int("tPag")
    vPag = _float("vPag")
    xPag = _str("xPag")
    dPag = _str("dPag")
    CNPJPag = _str("CNPJPag")
    UFPag = _int("UFPag")
    CNPJReceb = _str("CNPJReceb")
    UFReceb = _int("UFReceb")

class DfeReferenciado(Model):
    __slots__ = ()
    chaveAcesso = _str("chaveAcesso")
    nItem = _int("nItem")

class Di(Model):
    __slots__ = ()
    adi: _Collection[Adi] = _Collection("adi", "Adi")
    nDI = _str("nDI")
    dDI = _str("dDI")
    xLocDesemb = _str("xLocDesemb")
    UFDesemb = _str("UFDesemb")
    dDesemb = _str("dDesemb")
    tpViaTransp = _int("tpViaTransp")
    vAFRMM = _float("vAFRMM")
    tpIntermedio = _int("tpIntermedio")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    UFTerceiro = _str("UFTerceiro")
    cExportador = _str("cExportador")

class Dup(Model):
    __slots__ = ()
    nDup = _str("nDup")
    dVenc = _str("dVenc")
    vDup = _float("vDup")

class Emit(Model):
    __slots__ = ()
    enderEmit: _Child[EnderEmit] = _Child("enderEmit", "EnderEmit")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    xNome = _str("xNome")
    xFant = _str("xFant")
    IE = _str("IE")
    IEST = _str("IEST")
    IM = _str("IM")
    CNAE = _str("CNAE")
    CRT = _int("CRT")
    ISUFEmit = _str("ISUFEmit")

class Encerrante(Model):
    __slots__ = ()
    nBico = _int("nBico")
    nBomba = _int("nBomba")
    nTanque = _int("nTanque")
    vEncIni = _float("vEncIni")
    vEncFin = _float("vEncFin")
    pBio = _float("pBio")

class EnderDest(Model):
    __slots__ = ()
    xLgr = _str("xLgr")
    nro = _int("nro")
    xCpl = _str("xCpl")
    xBairro = _str("xBairro")
    cMun = _int("cMun")
    xMun = _str("xMun")
    UF = _str("UF")
    CEP = _int("CEP")
    cPais = _int("cPais")
    xPais = _str("xPais")
    fone = _str("fone")

class EnderEmit(Model):
    __slots__ = ()
    xLgr = _str("xLgr")
    nro = _int("nro")
    xCpl = _str("xCpl")
    xBairro = _str("xBairro")
    cMun = _int("cMun")
    xMun = _str("xMun")
    UF = _str("UF")
    CEP = _int("CEP")
    cPais = _int("cPais")
    xPais = _str("xPais")
    fone = _str("fone")

class Entrega(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    xNome = _str("xNome")
    xLgr = _str("xLgr")
    nro = _int("nro")
    nroTexto = _str("nroTexto")
    xCpl = _str("xCpl")
    xBairro = _str("xBairro")
    cMun = _int("cMun")
    xMun = _str("xMun")
    UF = _str("UF")
    CEP = _int("CEP")
    cPais = _int("cPais")
    xPais = _str("xPais")
    fone = _str("fone")
    email = _str("email")
    IE = _str("IE")

class EnvEvento(Model):
    __slots__ = ()
    evento: _Collection[TEvento] = _Collection("evento", "TEvento")
    retorno: _Child[RetEnvEvento] = _Child("retorno", "RetEnvEvento")
    versao = _str("versao")
    error = _str("error", write=False)

class EstoqueEventoRTC(Model):
    __slots__ = ()
    quantidade = _float("quantidade")
    unidade = _str("unidade")
    vIBSEstorno = _float("vIBSEstorno")
    vCBSEstorno = _float("vCBSEstorno")

class EventoNotaFiscal(Model):
    __slots__ = ()
    statusServico: _Child[StatusServico] = _Child("statusServico", "StatusServico")
    envEvento: _Child[EnvEvento] = _Child("envEvento", "EnvEvento")
    consSitNFe: _Child[ConsSitNFe] = _Child("consSitNFe", "ConsSitNFe")
    cancNFe: _Child[CancNFe] = _Child("cancNFe", "CancNFe")
    inutNFe: _Child[InutNFe] = _Child("inutNFe", "InutNFe")
    nfeDistribuicaoDFe: _Child[NFeDistribuicaoDFe] = _Child("nfeDistribuicaoDFe", "NFeDistribuicaoDFe")

class ExportInd(Model):
    __slots__ = ()
    nRE = _str("nRE")
    chNFe = _str("chNFe")
    qExport = _float("qExport")

class Exporta(Model):
    __slots__ = ()
    UFSaidaPais = _str("UFSaidaPais")
    xLocExporta = _str("xLocExporta")
    xLocDespacho = _str("xLocDespacho")

class Fat(Model):
    __slots__ = ()
    nFat = _str("nFat")
    vOrig = _float("vOrig")
    vDesc = _float("vDesc")
    vLiq = _float("vLiq")

class ForDia(Model):
    __slots__ = ()
    dia = _int("dia")
    qtde = _float("qtde")

class GALCZFMCBS(Model):
    __slots__ = ()
    tpALCZFMCBS = _int("tpALCZFMCBS")
    nProcSuframa = _str("nProcSuframa")
    pAliqEfetRegCBS = _float("pAliqEfetRegCBS")
    vTribRegCBS = _float("vTribRegCBS")

class GAjusteCompet(Model):
    __slots__ = ()
    competApur = _str("competApur")
    vIBS = _float("vIBS")
    vCBS = _float("vCBS")

class GCBS(Model):
    __slots__ = ()
    gDif: _Child[GDif] = _Child("gDif", "GDif")
    gDevTrib: _Child[GDevTrib] = _Child("gDevTrib", "GDevTrib")
    gRed: _Child[GRed] = _Child("gRed", "GRed")
    gALCZFMCBS: _Child[GALCZFMCBS] = _Child("gALCZFMCBS", "GALCZFMCBS")
    pCBS = _float("pCBS")
    vCBS = _float("vCBS")

class GCBSMonoAdRem(Model):
    __slots__ = ()
    gMonoPadrao: _Child[GCBSMonoPadraoAdRem] = _Child("gMonoPadrao", "GCBSMonoPadraoAdRem")
    gMonoReten: _Child[GCBSMonoRetenAdRem] = _Child("gMonoReten", "GCBSMonoRetenAdRem")
    gMonoRet: _Child[GCBSMonoRet] = _Child("gMonoRet", "GCBSMonoRet")
    gpBioDiferenca: _Child[GCBSpBioDiferenca] = _Child("gpBioDiferenca", "GCBSpBioDiferenca")

class GCBSMonoAdValorem(Model):
    __slots__ = ()
    gMonoPadrao: _Child[GCBSMonoPadraoAdValorem] = _Child("gMonoPadrao", "GCBSMonoPadraoAdValorem")
    gMonoReten: _Child[GCBSMonoRetenAdValorem] = _Child("gMonoReten", "GCBSMonoRetenAdValorem")
    gMonoRet: _Child[GCBSMonoRet] = _Child("gMonoRet", "GCBSMonoRet")
    gpBioDiferenca: _Child[GCBSpBioDiferenca] = _Child("gpBioDiferenca", "GCBSpBioDiferenca")

class GCBSMonoPadraoAdRem(Model):
    __slots__ = ()
    qBCMono = _float("qBCMono")
    adRemCBS = _float("adRemCBS")
    vCBSMono = _float("vCBSMono")

class GCBSMonoPadraoAdValorem(Model):
    __slots__ = ()
    vBCMono = _float("vBCMono")
    pAliqMonoCBS = _float("pAliqMonoCBS")
    vCBSMono = _float("vCBSMono")

class GCBSMonoRet(Model):
    __slots__ = ()
    vCBSMonoRet = _float("vCBSMonoRet")

class GCBSMonoRetenAdRem(Model):
    __slots__ = ()
    qBCMonoReten = _float("qBCMonoReten")
    adRemCBSReten = _float("adRemCBSReten")
    vCBSMonoReten = _float("vCBSMonoReten")

class GCBSMonoRetenAdValorem(Model):
    __slots__ = ()
    vBCMonoReten = _float("vBCMonoReten")
    pAliqMonoReten = _float("pAliqMonoReten")
    vCBSMonoReten = _float("vCBSMonoReten")

class GCBSTot(Model):
    __slots__ = ()
    vDif = _float("vDif")
    vDevTrib = _float("vDevTrib")
    vCBS = _float("vCBS")
    vCredPres = _float("vCredPres")
    vCredPresCondSus = _float("vCredPresCondSus")

class GCBSpBioDiferenca(Model):
    __slots__ = ()
    qBCBioComb = _float("qBCBioComb")
    vCBSDiferenca = _float("vCBSDiferenca")

class GCompraGov(Model):
    __slots__ = ()
    tpEnteGov = _int("tpEnteGov")
    pRedutor = _float("pRedutor")
    tpOperGov = _int("tpOperGov")

class GCred(Model):
    __slots__ = ()
    cCredPresumido = _str("cCredPresumido")
    pCredPresumido = _float("pCredPresumido")
    vCredPresumido = _float("vCredPresumido")

class GCredPres(Model):
    __slots__ = ()
    pCredPres = _float("pCredPres")
    vCredPres = _float("vCredPres")
    vCredPresCondSus = _float("vCredPresCondSus")

class GCredPresIBSZFM(Model):
    __slots__ = ()
    competApur = _str("competApur")
    tpCredPresIBSZFM = _int("tpCredPresIBSZFM")
    vCredPresIBSZFM = _float("vCredPresIBSZFM")

class GCredPresOper(Model):
    __slots__ = ()
    gIBSCredPres: _Child[GCredPres] = _Child("gIBSCredPres", "GCredPres")
    gCBSCredPres: _Child[GCredPres] = _Child("gCBSCredPres", "GCredPres")
    vBCCredPres = _float("vBCCredPres")
    cCredPres = _str("cCredPres")

class GDevTrib(Model):
    __slots__ = ()
    pDevTrib = _float("pDevTrib")
    vDevTrib = _float("vDevTrib")

class GDif(Model):
    __slots__ = ()
    pDif = _float("pDif")
    vDif = _float("vDif")

class GEstornoCred(Model):
    __slots__ = ()
    vIBSEstCred = _float("vIBSEstCred")
    vCBSEstCred = _float("vCBSEstCred")

class GEstornoCredTot(Model):
    __slots__ = ()
    vIBSEstCred = _float("vIBSEstCred")
    vCBSEstCred = _float("vCBSEstCred")

class GIBSCBS(Model):
    __slots__ = ()
    gIBSUF: _Child[GIBSUF] = _Child("gIBSUF", "GIBSUF")
    gIBSMun: _Child[GIBSMun] = _Child("gIBSMun", "GIBSMun")
    gCBS: _Child[GCBS] = _Child("gCBS", "GCBS")
    gTribRegular: _Child[GTribRegular] = _Child("gTribRegular", "GTribRegular")
    gTribCompraGov: _Child[GTribCompraGov] = _Child("gTribCompraGov", "GTribCompraGov")
    vBC = _float("vBC")
    vIBS = _float("vIBS")

class GIBSCBSMono(Model):
    __slots__ = ()
    gIBSMonoAdRem: _Child[GIBSMonoAdRem] = _Child("gIBSMonoAdRem", "GIBSMonoAdRem")
    gIBSMonoAdValorem: _Child[GIBSMonoAdValorem] = _Child("gIBSMonoAdValorem", "GIBSMonoAdValorem")
    gCBSMonoAdRem: _Child[GCBSMonoAdRem] = _Child("gCBSMonoAdRem", "GCBSMonoAdRem")
    gCBSMonoAdValorem: _Child[GCBSMonoAdValorem] = _Child("gCBSMonoAdValorem", "GCBSMonoAdValorem")
    vTotIBSMonoItem = _float("vTotIBSMonoItem")
    vTotCBSMonoItem = _float("vTotCBSMonoItem")

class GIBSMonoAdRem(Model):
    __slots__ = ()
    gMonoPadrao: _Child[GIBSMonoPadraoAdRem] = _Child("gMonoPadrao", "GIBSMonoPadraoAdRem")
    gMonoReten: _Child[GIBSMonoRetenAdRem] = _Child("gMonoReten", "GIBSMonoRetenAdRem")
    gMonoRet: _Child[GIBSMonoRet] = _Child("gMonoRet", "GIBSMonoRet")
    gpBioDiferenca: _Child[GIBSpBioDiferenca] = _Child("gpBioDiferenca", "GIBSpBioDiferenca")

class GIBSMonoAdValorem(Model):
    __slots__ = ()
    gMonoPadrao: _Child[GIBSMonoPadraoAdValorem] = _Child("gMonoPadrao", "GIBSMonoPadraoAdValorem")
    gMonoReten: _Child[GIBSMonoRetenAdValorem] = _Child("gMonoReten", "GIBSMonoRetenAdValorem")
    gMonoRet: _Child[GIBSMonoRet] = _Child("gMonoRet", "GIBSMonoRet")
    gpBioDiferenca: _Child[GIBSpBioDiferenca] = _Child("gpBioDiferenca", "GIBSpBioDiferenca")

class GIBSMonoPadraoAdRem(Model):
    __slots__ = ()
    qBCMono = _float("qBCMono")
    adRemIBS = _float("adRemIBS")
    vIBSMono = _float("vIBSMono")

class GIBSMonoPadraoAdValorem(Model):
    __slots__ = ()
    vBCMono = _float("vBCMono")
    pAliqMonoUF = _float("pAliqMonoUF")
    vIBSMonoUF = _float("vIBSMonoUF")
    pAliqMonoMun = _float("pAliqMonoMun")
    vIBSMonoMun = _float("vIBSMonoMun")
    vIBSMono = _float("vIBSMono")

class GIBSMonoRet(Model):
    __slots__ = ()
    vIBSMonoRet = _float("vIBSMonoRet")

class GIBSMonoRetenAdRem(Model):
    __slots__ = ()
    qBCMonoReten = _float("qBCMonoReten")
    adRemIBSReten = _float("adRemIBSReten")
    vIBSMonoReten = _float("vIBSMonoReten")

class GIBSMonoRetenAdValorem(Model):
    __slots__ = ()
    vBCMonoReten = _float("vBCMonoReten")
    pAliqMonoReten = _float("pAliqMonoReten")
    vIBSMonoReten = _float("vIBSMonoReten")

class GIBSMun(Model):
    __slots__ = ()
    gDif: _Child[GDif] = _Child("gDif", "GDif")
    gDevTrib: _Child[GDevTrib] = _Child("gDevTrib", "GDevTrib")
    gRed: _Child[GRed] = _Child("gRed", "GRed")
    pIBSMun = _float("pIBSMun")
    vIBSMun = _float("vIBSMun")

class GIBSMunTot(Model):
    __slots__ = ()
    vDif = _float("vDif")
    vDevTrib = _float("vDevTrib")
    vIBSMun = _float("vIBSMun")

class GIBSTot(Model):
    __slots__ = ()
    gIBSUF: _Child[GIBSUFTot] = _Child("gIBSUF", "GIBSUFTot")
    gIBSMun: _Child[GIBSMunTot] = _Child("gIBSMun", "GIBSMunTot")
    vIBS = _float("vIBS")
    vCredPres = _float("vCredPres")
    vCredPresCondSus = _float("vCredPresCondSus")

class GIBSUF(Model):
    __slots__ = ()
    gDif: _Child[GDif] = _Child("gDif", "GDif")
    gDevTrib: _Child[GDevTrib] = _Child("gDevTrib", "GDevTrib")
    gRed: _Child[GRed] = _Child("gRed", "GRed")
    pIBSUF = _float("pIBSUF")
    vIBSUF = _float("vIBSUF")

class GIBSUFTot(Model):
    __slots__ = ()
    vDif = _float("vDif")
    vDevTrib = _float("vDevTrib")
    vIBSUF = _float("vIBSUF")

class GIBSpBioDiferenca(Model):
    __slots__ = ()
    qBCBioComb = _float("qBCBioComb")
    vIBSDiferenca = _float("vIBSDiferenca")

class GMonoTot(Model):
    __slots__ = ()
    vIBSMono = _float("vIBSMono")
    vCBSMono = _float("vCBSMono")
    vIBSMonoReten = _float("vIBSMonoReten")
    vCBSMonoReten = _float("vCBSMonoReten")
    vIBSMonoRet = _float("vIBSMonoRet")
    vCBSMonoRet = _float("vCBSMonoRet")

class GPagAntecipado(Model):
    __slots__ = ()

class GRed(Model):
    __slots__ = ()
    pRedAliq = _float("pRedAliq")
    pAliqEfet = _float("pAliqEfet")

class GTransfCred(Model):
    __slots__ = ()
    vIBS = _float("vIBS")
    vCBS = _float("vCBS")

class GTribCompraGov(Model):
    __slots__ = ()
    pAliqIBSUF = _float("pAliqIBSUF")
    vTribIBSUF = _float("vTribIBSUF")
    pAliqIBSMun = _float("pAliqIBSMun")
    vTribIBSMun = _float("vTribIBSMun")
    pAliqCBS = _float("pAliqCBS")
    vTribCBS = _float("vTribCBS")

class GTribRegular(Model):
    __slots__ = ()
    CSTReg = _str("CSTReg")
    cClassTribReg = _str("cClassTribReg")
    pAliqEfetRegIBSUF = _float("pAliqEfetRegIBSUF")
    vTribRegIBSUF = _float("vTribRegIBSUF")
    pAliqEfetRegIBSMun = _float("pAliqEfetRegIBSMun")
    vTribRegIBSMun = _float("vTribRegIBSMun")
    pAliqEfetRegCBS = _float("pAliqEfetRegCBS")
    vTribRegCBS = _float("vTribRegCBS")

class GuiaTransito(Model):
    __slots__ = ()
    tpGuia = _int("tpGuia")
    UFGuia = _str("UFGuia")
    serieGuia = _str("serieGuia")
    nGuia = _str("nGuia")

class IbsCbs(Model):
    __slots__ = ()
    gIBSCBS: _Child[GIBSCBS] = _Child("gIBSCBS", "GIBSCBS")
    gIBSCBSMono: _Child[GIBSCBSMono] = _Child("gIBSCBSMono", "GIBSCBSMono")
    gTransfCred: _Child[GTransfCred] = _Child("gTransfCred", "GTransfCred")
    gAjusteCompet: _Child[GAjusteCompet] = _Child("gAjusteCompet", "GAjusteCompet")
    gEstornoCred: _Child[GEstornoCred] = _Child("gEstornoCred", "GEstornoCred")
    gCredPresOper: _Child[GCredPresOper] = _Child("gCredPresOper", "GCredPresOper")
    gCredPresIBSZFM: _Child[GCredPresIBSZFM] = _Child("gCredPresIBSZFM", "GCredPresIBSZFM")
    CST = _str("CST")
    cClassTrib = _str("cClassTrib")
    indDoacao = _int("indDoacao")

class IbsCbsTot(Model):
    __slots__ = ()
    gIBS: _Child[GIBSTot] = _Child("gIBS", "GIBSTot")
    gCBS: _Child[GCBSTot] = _Child("gCBS", "GCBSTot")
    gMono: _Child[GMonoTot] = _Child("gMono", "GMonoTot")
    gEstornoCred: _Child[GEstornoCredTot] = _Child("gEstornoCred", "GEstornoCredTot")
    vBCIBSCBS = _float("vBCIBSCBS")

class Icms(Model):
    __slots__ = ()
    orig = _int("orig")
    CST = _int("CST")
    CSOSN = _int("CSOSN")
    modBC = _int("modBC")
    vBC = _float("vBC")
    pICMS = _float("pICMS")
    vICMS = _float("vICMS")
    pFCP = _float("pFCP")
    vFCP = _float("vFCP")
    vBCFCP = _float("vBCFCP")
    modBCST = _int("modBCST")
    pMVAST = _float("pMVAST")
    pRedBCST = _float("pRedBCST")
    vBCST = _float("vBCST")
    pICMSST = _float("pICMSST")
    vICMSST = _float("vICMSST")
    vBCFCPST = _float("vBCFCPST")
    pFCPST = _float("pFCPST")
    vFCPST = _float("vFCPST")
    vICMSDeson = _float("vICMSDeson")
    motDesICMS = _int("motDesICMS")
    vICMSOp = _float("vICMSOp")
    pDif = _float("pDif")
    vICMSDif = _float("vICMSDif")
    vBCSTRet = _float("vBCSTRet")
    pST = _float("pST")
    vICMSSubstituto = _float("vICMSSubstituto")
    vICMSSTRet = _float("vICMSSTRet")
    vBCFCPSTRet = _float("vBCFCPSTRet")
    pFCPSTRet = _float("pFCPSTRet")
    vFCPSTRet = _float("vFCPSTRet")
    vBCSTDest = _float("vBCSTDest")
    vICMSSTDest = _float("vICMSSTDest")
    pRedBCEfet = _float("pRedBCEfet")
    vBCEfet = _float("vBCEfet")
    pICMSEfet = _float("pICMSEfet")
    vICMSEfet = _float("vICMSEfet")
    pBCOp = _float("pBCOp")
    UFST = _str("UFST")
    pRedBC = _float("pRedBC")
    cBenefRBC = _str("cBenefRBC")
    pCredSN = _float("pCredSN")
    vCredICMSSN = _float("vCredICMSSN")
    vICMSSTDeson = _float("vICMSSTDeson")
    motDesICMSST = _int("motDesICMSST")
    pFCPDif = _float("pFCPDif")
    vFCPDif = _float("vFCPDif")
    vFCPEfet = _float("vFCPEfet")
    qBCMono = _float("qBCMono")
    adRemICMS = _float("adRemICMS")
    vICMSMono = _float("vICMSMono")
    qBCMonoReten = _float("qBCMonoReten")
    adRemICMSReten = _float("adRemICMSReten")
    vICMSMonoReten = _float("vICMSMonoReten")
    pRedAdRem = _float("pRedAdRem")
    motRedAdRem = _int("motRedAdRem")
    vICMSMonoOp = _float("vICMSMonoOp")
    vICMSMonoDif = _float("vICMSMonoDif")
    qBCMonoRet = _float("qBCMonoRet")
    adRemICMSRet = _float("adRemICMSRet")
    vICMSMonoRet = _float("vICMSMonoRet")
    indDeduzDeson = _int("indDeduzDeson")

class IcmsTot(Model):
    __slots__ = ()
    vBC = _float("vBC")
    vICMS = _float("vICMS")
    vICMSDeson = _float("vICMSDeson")
    vFCPUFDest = _float("vFCPUFDest")
    vICMSUFDest = _float("vICMSUFDest")
    vICMSUFRemet = _float("vICMSUFRemet")
    vFCP = _float("vFCP")
    vBCST = _float("vBCST")
    vST = _float("vST")
    vFCPST = _float("vFCPST")
    vFCPSTRet = _float("vFCPSTRet")
    qBCMono = _float("qBCMono")
    vICMSMono = _float("vICMSMono")
    qBCMonoReten = _float("qBCMonoReten")
    vICMSMonoReten = _float("vICMSMonoReten")
    qBCMonoRet = _float("qBCMonoRet")
    vICMSMonoRet = _float("vICMSMonoRet")
    vProd = _float("vProd")
    vFrete = _float("vFrete")
    vSeg = _float("vSeg")
    vDesc = _float("vDesc")
    vII = _float("vII")
    vIPI = _float("vIPI")
    vIPIDevol = _float("vIPIDevol")
    vPIS = _float("vPIS")
    vCOFINS = _float("vCOFINS")
    vOutro = _float("vOutro")
    vNF = _float("vNF")
    vTotTrib = _float("vTotTrib")

class IcmsUFDest(Model):
    __slots__ = ()
    vBCUFDest = _float("vBCUFDest")
    vBCFCPUFDest = _float("vBCFCPUFDest")
    pFCPUFDest = _float("pFCPUFDest")
    pICMSUFDest = _float("pICMSUFDest")
    pICMSInter = _float("pICMSInter")
    pICMSInterPart = _float("pICMSInterPart")
    vFCPUFDest = _float("vFCPUFDest")
    vICMSUFDest = _float("vICMSUFDest")
    vICMSUFRemet = _float("vICMSUFRemet")

class Ide(Model):
    __slots__ = ()
    NFref: _Collection[NFRef] = _Collection("NFref", "NFRef")
    gCompraGov: _Child[GCompraGov] = _Child("gCompraGov", "GCompraGov")
    gPagAntecipado: _Child[GPagAntecipado] = _Child("gPagAntecipado", "GPagAntecipado")
    cUF = _int("cUF")
    cNF = _int("cNF")
    natOp = _str("natOp")
    mod = _int("mod")
    serie = _int("serie")
    nNF = _int("nNF")
    dhEmi = _str("dhEmi")
    dhSaiEnt = _str("dhSaiEnt")
    dPrevEntrega = _str("dPrevEntrega")
    tpNF = _int("tpNF")
    idDest = _int("idDest")
    cMunFG = _int("cMunFG")
    cMunFGIBS = _int("cMunFGIBS")
    tpImp = _int("tpImp")
    tpEmis = _int("tpEmis")
    cDV = _str("cDV")
    tpAmb = _int("tpAmb")
    finNFe = _int("finNFe")
    tpNFDebito = _int("tpNFDebito")
    tpNFCredito = _int("tpNFCredito")
    indFinal = _int("indFinal")
    indPres = _int("indPres")
    indIntermed = _int("indIntermed")
    cIndOp = _str("cIndOp")
    procEmi = _int("procEmi")
    verProc = _str("verProc")
    dhCont = _str("dhCont")
    xJust = _str("xJust")

class Ii(Model):
    __slots__ = ()
    vBC = _float("vBC")
    vDespAdu = _float("vDespAdu")
    vII = _float("vII")
    vIOF = _float("vIOF")

class Imposto(Model):
    __slots__ = ()
    ICMS: _Child[Icms] = _Child("ICMS", "Icms")
    ICMSUFDest: _Child[IcmsUFDest] = _Child("ICMSUFDest", "IcmsUFDest")
    IPI: _Child[Ipi] = _Child("IPI", "Ipi")
    II: _Child[Ii] = _Child("II", "Ii")
    PIS: _Child[Pis] = _Child("PIS", "Pis")
    PISST: _Child[PisST] = _Child("PISST", "PisST")
    COFINS: _Child[Cofins] = _Child("COFINS", "Cofins")
    COFINSST: _Child[CofinsST] = _Child("COFINSST", "CofinsST")
    ISSQN: _Child[IssQN] = _Child("ISSQN", "IssQN")
    IS: _Child[Is] = _Child("IS", "Is")
    IBSCBS: _Child[IbsCbs] = _Child("IBSCBS", "IbsCbs")
    vTotTrib = _float("vTotTrib")

class ImpostoDevol(Model):
    __slots__ = ()
    pDevol = _float("pDevol")
    vIPIDevol = _float("vIPIDevol")

class InfAdic(Model):
    __slots__ = ()
    obsCont: _Collection[ObsCont] = _Collection("obsCont", "ObsCont")
    obsFisco: _Collection[ObsFisco] = _Collection("obsFisco", "ObsFisco")
    procRef: _Collection[ProcRef] = _Collection("procRef", "ProcRef")
    infAdFisco = _str("infAdFisco")
    infCpl = _str("infCpl")

class InfEvento(Model):
    __slots__ = ()
    detEvento: _Child[DetEvento] = _Child("detEvento", "DetEvento")
    Id = _str("Id")
    cOrgao = _int("cOrgao")
    tpAmb = _int("tpAmb")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    chNFe = _str("chNFe")
    dhEvento = _str("dhEvento")
    tpEvento = _int("tpEvento")
    nSeqEvento = _int("nSeqEvento")
    verEvento = _str("verEvento")

class InfEventoRet(Model):
    __slots__ = ()
    Id = _str("Id")
    tpAmb = _int("tpAmb")
    verAplic = _str("verAplic")
    cOrgao = _int("cOrgao")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")
    chNFe = _str("chNFe")
    tpEvento = _int("tpEvento")
    xEvento = _str("xEvento")
    nSeqEvento = _int("nSeqEvento")
    cOrgaoAutor = _int("cOrgaoAutor")
    CNPJDest = _str("CNPJDest")
    CPFDest = _str("CPFDest")
    emailDest = _str("emailDest")
    dhRegEvento = _str("dhRegEvento")
    nProt = _str("nProt")
    chNFePend = _str("chNFePend")
    dhRecbto = _str("dhRecbto")
    cUF = _int("cUF")

class InfIntermed(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    idCadIntTran = _str("idCadIntTran")

class InfInut(Model):
    __slots__ = ()
    error = _str("error", write=False)
    XMLOriginal = _str("XMLOriginal", write=False)
    XMLAssinado = _str("XMLAssinado", write=False)
    XML = _str("XML", write=False)
    Id = _str("Id")
    tpAmb = _int("tpAmb")
    xServ = _str("xServ")
    cUF = _int("cUF")
    ano = _int("ano")
    CNPJ = _str("CNPJ")
    mod = _int("mod")
    serie = _int("serie")
    nNFIni = _int("nNFIni")
    nNFFin = _int("nNFFin")
    xJust = _str("xJust")
    verAplic = _str("verAplic")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")
    dhRecbto = _str("dhRecbto")
    nProt = _str("nProt")

class InfNFe(Model):
    __slots__ = ()
    ide: _Child[Ide] = _Child("ide", "Ide")
    emite: _Child[Emit] = _Child("emite", "Emit")
    avulsa: _Child[Avulsa] = _Child("avulsa", "Avulsa")
    dest: _Child[Dest] = _Child("dest", "Dest")
    retirada: _Child[Retirada] = _Child("retirada", "Retirada")
    entrega: _Child[Entrega] = _Child("entrega", "Entrega")
    autXML: _Collection[AutXML] = _Collection("autXML", "AutXML")
    det: _Collection[Det] = _Collection("det", "Det")
    total: _Child[Total] = _Child("total", "Total")
    transp: _Child[Transp] = _Child("transp", "Transp")
    cobr: _Child[Cobr] = _Child("cobr", "Cobr")
    pag: _Child[Pag] = _Child("pag", "Pag")
    infIntermed: _Child[InfIntermed] = _Child("infIntermed", "InfIntermed")
    infAdic: _Child[InfAdic] = _Child("infAdic", "InfAdic")
    exporta: _Child[Exporta] = _Child("exporta", "Exporta")
    compra: _Child[Compra] = _Child("compra", "Compra")
    cana: _Child[Cana] = _Child("cana", "Cana")
    infRespTec: _Child[InfRespTec] = _Child("infRespTec", "InfRespTec")
    agropecuario: _Child[Agropecuario] = _Child("agropecuario", "Agropecuario")
    versao = _str("versao")
    Id = _str("Id")

class InfNFeSupl(Model):
    __slots__ = ()
    qrCode = _str("qrCode")
    urlChave = _str("urlChave")

class InfRespTec(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    xContato = _str("xContato")
    email = _str("email")
    fone = _str("fone")
    idCSRT = _int("idCSRT")
    hashCSRT = _str("hashCSRT")

class InutNFe(Model):
    __slots__ = ()
    infInut: _Child[InfInut] = _Child("infInut", "InfInut")
    retInutNFe: _Child[RetInutNFe] = _Child("retInutNFe", "RetInutNFe")
    versao = _str("versao")

class Ipi(Model):
    __slots__ = ()
    CNPJProd = _str("CNPJProd")
    cSelo = _str("cSelo")
    qSelo = _int("qSelo")
    cEnq = _str("cEnq")
    CST = _int("CST")
    vBC = _float("vBC")
    pIPI = _float("pIPI")
    qUnid = _float("qUnid")
    vUnid = _float("vUnid")
    vIPI = _float("vIPI")

class Is(Model):
    __slots__ = ()
    CSTIS = _str("CSTIS")
    cClassTribIS = _str("cClassTribIS")
    vBCIS = _float("vBCIS")
    pIS = _float("pIS")
    adRemIS = _float("adRemIS")
    uTrib = _str("uTrib")
    qTrib = _float("qTrib")
    vIS = _float("vIS")

class IsTot(Model):
    __slots__ = ()
    vIS = _float("vIS")

class IssQN(Model):
    __slots__ = ()
    vBC = _float("vBC")
    vAliq = _float("vAliq")
    vISSQN = _float("vISSQN")
    cMunFG = _int("cMunFG")
    cListServ = _str("cListServ")
    vDeducao = _float("vDeducao")
    vOutro = _float("vOutro")
    vDescIncond = _float("vDescIncond")
    vDescCond = _float("vDescCond")
    vISSRet = _float("vISSRet")
    indISS = _int("indISS")
    cServico = _str("cServico")
    cMun = _int("cMun")
    cPais = _int("cPais")
    nProcesso = _str("nProcesso")
    indIncentivo = _int("indIncentivo")

class IssQNtot(Model):
    __slots__ = ()
    vServ = _float("vServ")
    vBC = _float("vBC")
    vISS = _float("vISS")
    vPIS = _float("vPIS")
    vCOFINS = _float("vCOFINS")
    dCompet = _str("dCompet")
    vDeducao = _float("vDeducao")
    vOutro = _float("vOutro")
    vDescIncond = _float("vDescIncond")
    vDescCond = _float("vDescCond")
    vISSRet = _float("vISSRet")
    cRegTrib = _int("cRegTrib")

class ItemCreditoPresumidoEvento(Model):
    __slots__ = ()
    gIBSCredPres: _Child[CreditoPresumidoEvento] = _Child("gIBSCredPres", "CreditoPresumidoEvento")
    gCBSCredPres: _Child[CreditoPresumidoEvento] = _Child("gCBSCredPres", "CreditoPresumidoEvento")
    nItem = _int("nItem")
    vBCCredPres = _float("vBCCredPres")
    cCredPres = _str("cCredPres")

class ItemEventoRTC(Model):
    __slots__ = ()
    gControleEstoque: _Child[EstoqueEventoRTC] = _Child("gControleEstoque", "EstoqueEventoRTC")
    nItem = _int("nItem")
    vIBS = _float("vIBS")
    vCBS = _float("vCBS")

class ItemPedProrrog(Model):
    __slots__ = ()
    numItem = _int("numItem")
    qtdeItem = _float("qtdeItem")

class ItemRespPedido(Model):
    __slots__ = ()
    numItem = _int("numItem")
    statPedido = _int("statPedido")
    justStatus = _int("justStatus")
    justStaOutra = _str("justStaOutra")

class Lacres(Model):
    __slots__ = ()
    nLacre = _str("nLacre")

class Med(Model):
    __slots__ = ()
    cProdANVISA = _str("cProdANVISA")
    xMotivoIsencao = _str("xMotivoIsencao")
    vPMC = _float("vPMC")

class NFRef(Model):
    __slots__ = ()
    refNF: _Child[RefNF] = _Child("refNF", "RefNF")
    refNFP: _Child[RefNFP] = _Child("refNFP", "RefNFP")
    refECF: _Child[RefECF] = _Child("refECF", "RefECF")
    refNFe = _str("refNFe")
    refNFeSig = _str("refNFeSig")
    refCTe = _str("refCTe")

class NFeDistribuicaoDFe(Model):
    __slots__ = ()
    error = _str("error", write=False)

class Nfe(Model):
    __slots__ = ()
    infNFe: _Child[InfNFe] = _Child("infNFe", "InfNFe")
    protNFe: _Child[ProtNFe] = _Child("protNFe", "ProtNFe")
    infNFeSupl: _Child[InfNFeSupl] = _Child("infNFeSupl", "InfNFeSupl")
    error = _str("error", write=False)
    chNFe = _str("chNFe")
    XMLOriginal = _str("XMLOriginal", write=False)
    XMLAssinado = _str("XMLAssinado", write=False)
    XMLAutorizado = _str("XMLAutorizado")

class NotaFiscal(Model):
    __slots__ = ()
    NFe: _Collection[Nfe] = _Collection("NFe", "Nfe")
    retorno: _Child[RetConsReciNFe] = _Child("retorno", "RetConsReciNFe")
    totalErros = _int("totalErros", write=False)
    error = _str("error", write=False)

class ObsCont(Model):
    __slots__ = ()
    xCampo = _str("xCampo")
    xTexto = _str("xTexto")

class ObsFisco(Model):
    __slots__ = ()
    xCampo = _str("xCampo")
    xTexto = _str("xTexto")

class ObsItem(Model):
    __slots__ = ()
    obsCont: _Child[ObsCont] = _Child("obsCont", "ObsCont")
    obsFisco: _Child[ObsFisco] = _Child("obsFisco", "ObsFisco")

class OrigComb(Model):
    __slots__ = ()
    indImport = _int("indImport")
    cUFOrig = _int("cUFOrig")
    pOrig = _float("pOrig")

class Pag(Model):
    __slots__ = ()
    detPag: _Collection[DetPag] = _Collection("detPag", "DetPag")
    vTroco = _float("vTroco")

class PagamentoVinculadoEvento(Model):
    __slots__ = ()
    idTransacao = _str("idTransacao")
    tpMeioPgto = _str("tpMeioPgto")
    CNPJReceb = _str("CNPJReceb")
    CNPJBasePSP = _str("CNPJBasePSP")

class Pis(Model):
    __slots__ = ()
    CST = _int("CST")
    vBC = _float("vBC")
    pPIS = _float("pPIS")
    vPIS = _float("vPIS")
    qBCProd = _float("qBCProd")
    vAliqProd = _float("vAliqProd")

class PisST(Model):
    __slots__ = ()
    vBC = _float("vBC")
    pPIS = _float("pPIS")
    qBCProd = _float("qBCProd")
    vAliqProd = _float("vAliqProd")
    vPIS = _float("vPIS")
    indSomaPISST = _int("indSomaPISST")

class ProcEventoNFe(Model):
    __slots__ = ()
    evento: _Child[TEvento] = _Child("evento", "TEvento")
    retEvento: _Child[RetEvento] = _Child("retEvento", "RetEvento")
    versao = _str("versao")

class ProcRef(Model):
    __slots__ = ()
    nProc = _str("nProc")
    indProc = _int("indProc")
    tpAto = _int("tpAto")

class Prod(Model):
    __slots__ = ()
    DI: _Collection[Di] = _Collection("DI", "Di")
    detExport: _Collection[DetExport] = _Collection("detExport", "DetExport")
    rastro: _Collection[Rastro] = _Collection("rastro", "Rastro")
    veicProd: _Child[VeicProd] = _Child("veicProd", "VeicProd")
    med: _Child[Med] = _Child("med", "Med")
    arma: _Collection[Arma] = _Collection("arma", "Arma")
    comb: _Child[Comb] = _Child("comb", "Comb")
    gCred: _Collection[GCred] = _Collection("gCred", "GCred")
    nItem = _int("nItem")
    cProd = _str("cProd")
    cEAN = _str("cEAN")
    cBarra = _str("cBarra")
    xProd = _str("xProd")
    NCM = _str("NCM")
    NVE = _str("NVE")
    EXTIPI = _str("EXTIPI")
    CFOP = _str("CFOP")
    uCom = _str("uCom")
    qCom = _float("qCom")
    vUnCom = _float("vUnCom")
    vProd = _float("vProd")
    cEANTrib = _str("cEANTrib")
    cBarraTrib = _str("cBarraTrib")
    uTrib = _str("uTrib")
    qTrib = _float("qTrib")
    vUnTrib = _float("vUnTrib")
    vFrete = _float("vFrete")
    vSeg = _float("vSeg")
    vDesc = _float("vDesc")
    vOutro = _float("vOutro")
    indTot = _int("indTot")
    CEST = _str("CEST")
    indEscala = _int("indEscala")
    CNPJFab = _str("CNPJFab")
    cBenef = _str("cBenef")
    tpCredPresIBSZFM = _int("tpCredPresIBSZFM")
    indBemMovelUsado = _int("indBemMovelUsado")
    xPed = _str("xPed")
    nItemPed = _int("nItemPed")
    nFCI = _str("nFCI")
    nRECOPI = _str("nRECOPI")

class ProtNFe(Model):
    __slots__ = ()

class Rastro(Model):
    __slots__ = ()
    nLote = _str("nLote")
    qLote = _float("qLote")
    dFab = _str("dFab")
    dVal = _str("dVal")
    cAgreg = _str("cAgreg")

class Reboque(Model):
    __slots__ = ()
    placa = _str("placa")
    UF = _str("UF")
    RNTC = _str("RNTC")

class RefECF(Model):
    __slots__ = ()
    mod = _str("mod")
    nECF = _str("nECF")
    nCOO = _str("nCOO")

class RefNF(Model):
    __slots__ = ()
    cUF = _int("cUF")
    AAMM = _str("AAMM")
    CNPJ = _str("CNPJ")
    mod = _str("mod")
    serie = _int("serie")
    nNF = _int("nNF")

class RefNFP(Model):
    __slots__ = ()
    cUF = _int("cUF")
    AAMM = _str("AAMM")
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    IE = _str("IE")
    mod = _str("mod")
    serie = _int("serie")
    nNF = _int("nNF")

class RespCancPedido(Model):
    __slots__ = ()
    statCancPedido = _int("statCancPedido")
    justStatus = _int("justStatus")
    justStaOutra = _str("justStaOutra")

class RespPedido(Model):
    __slots__ = ()
    itemPedido: _Collection[ItemRespPedido] = _Collection("itemPedido", "ItemRespPedido")
    statPrazo = _int("statPrazo")

class RetCancNFe(Model):
    __slots__ = ()
    infCanc: _Child[InfEventoRet] = _Child("infCanc", "InfEventoRet")
    versao = _str("versao")

class RetConsReciNFe(Model):
    __slots__ = ()
    protNFe: _Collection[ProtNFe] = _Collection("protNFe", "ProtNFe")
    versao = _str("versao")
    tpAmb = _int("tpAmb")
    verAplic = _str("verAplic")
    nRec = _str("nRec")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")
    cUF = _int("cUF")
    dhRecbto = _str("dhRecbto")
    cMsg = _int("cMsg")
    xMsg = _str("xMsg")

class RetConsSitNFe(Model):
    __slots__ = ()
    protNFe: _Child[ProtNFe] = _Child("protNFe", "ProtNFe")
    retCancNFe: _Child[RetCancNFe] = _Child("retCancNFe", "RetCancNFe")
    procEventoNFe: _Collection[ProcEventoNFe] = _Collection("procEventoNFe", "ProcEventoNFe")
    versao = _str("versao")
    tpAmb = _int("tpAmb")
    verAplic = _str("verAplic")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")
    cUF = _int("cUF")
    dhRecbto = _str("dhRecbto")
    chNFe = _str("chNFe")
    xml = _str("xml")

class RetConsStatServ(Model):
    __slots__ = ()
    versao = _str("versao")
    tpAmb = _int("tpAmb")
    verAplic = _str("verAplic")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")
    cUF = _int("cUF")
    dhRecbto = _str("dhRecbto")
    tMed = _int("tMed")
    dhRetorno = _str("dhRetorno")
    xObs = _str("xObs")

class RetEnvEvento(Model):
    __slots__ = ()
    retEvento: _Collection[RetEvento] = _Collection("retEvento", "RetEvento")
    versao = _str("versao")
    tpAmb = _int("tpAmb")
    idLote = _int("idLote")
    verAplic = _str("verAplic")
    cOrgao = _int("cOrgao")
    cStat = _int("cStat")
    xMotivo = _str("xMotivo")

class RetEvento(Model):
    __slots__ = ()
    infEvento: _Child[InfEventoRet] = _Child("infEvento", "InfEventoRet")
    versao = _str("versao")
    XML = _str("XML")

class RetEventoCancNFe(Model):
    __slots__ = ()
    cStatLote = _int("cStatLote", write=False)
    xMotivoLote = _str("xMotivoLote", write=False)
    idLote = _int("idLote", write=False)
    XMLLote = _str("XMLLote", write=False)
    versao = _str("versao", write=False)
    Id = _str("Id", write=False)
    tpAmb = _int("tpAmb", write=False)
    verAplic = _str("verAplic", write=False)
    cOrgao = _int("cOrgao", write=False)
    cStat = _int("cStat", write=False)
    xMotivo = _str("xMotivo", write=False)
    chNFe = _str("chNFe", write=False)
    tpEvento = _int("tpEvento", write=False)
    xEvento = _str("xEvento", write=False)
    nSeqEvento = _int("nSeqEvento", write=False)
    cOrgaoAutor = _int("cOrgaoAutor", write=False)
    CNPJDest = _str("CNPJDest", write=False)
    CPFDest = _str("CPFDest", write=False)
    emailDest = _str("emailDest", write=False)
    dhRegEvento = _str("dhRegEvento", write=False)
    nProt = _str("nProt", write=False)
    XML = _str("XML", write=False)

class RetInutNFe(Model):
    __slots__ = ()
    infInut: _Child[InfInut] = _Child("infInut", "InfInut")
    versao = _str("versao")
    XML = _str("XML", write=False)

class RetTransp(Model):
    __slots__ = ()
    vServ = _float("vServ")
    vBCRet = _float("vBCRet")
    pICMSRet = _float("pICMSRet")
    vICMSRet = _float("vICMSRet")
    CFOP = _str("CFOP")
    cMunFG = _int("cMunFG")

class RetTrib(Model):
    __slots__ = ()
    vRetPIS = _float("vRetPIS")
    vRetCOFINS = _float("vRetCOFINS")
    vRetCSLL = _float("vRetCSLL")
    vBCIRRF = _float("vBCIRRF")
    vIRRF = _float("vIRRF")
    vBCRetPrev = _float("vBCRetPrev")
    vRetPrev = _float("vRetPrev")

class Retirada(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    xNome = _str("xNome")
    xLgr = _str("xLgr")
    nro = _int("nro")
    nroTexto = _str("nroTexto")
    xCpl = _str("xCpl")
    xBairro = _str("xBairro")
    cMun = _int("cMun")
    xMun = _str("xMun")
    UF = _str("UF")
    CEP = _int("CEP")
    cPais = _int("cPais")
    xPais = _str("xPais")
    fone = _str("fone")
    email = _str("email")
    IE = _str("IE")

class StatusServico(Model):
    __slots__ = ()
    retorno: _Child[RetConsStatServ] = _Child("retorno", "RetConsStatServ")
    error = _str("error", write=False)

class TEvento(Model):
    __slots__ = ()
    infEvento: _Child[InfEvento] = _Child("infEvento", "InfEvento")
    versao = _str("versao")
    XMLOriginal = _str("XMLOriginal")
    XMLAssinado = _str("XMLAssinado")
    error = _str("error", write=False)

class Total(Model):
    __slots__ = ()
    ICMSTot: _Child[IcmsTot] = _Child("ICMSTot", "IcmsTot")
    ISSQNtot: _Child[IssQNtot] = _Child("ISSQNtot", "IssQNtot")
    retTrib: _Child[RetTrib] = _Child("retTrib", "RetTrib")
    ISTot: _Child[IsTot] = _Child("ISTot", "IsTot")
    IBSCBSTot: _Child[IbsCbsTot] = _Child("IBSCBSTot", "IbsCbsTot")
    vNFTot = _float("vNFTot")

class Transp(Model):
    __slots__ = ()
    transporta: _Child[Transporta] = _Child("transporta", "Transporta")
    retTransp: _Child[RetTransp] = _Child("retTransp", "RetTransp")
    veicTransp: _Child[VeicTransp] = _Child("veicTransp", "VeicTransp")
    reboque: _Collection[Reboque] = _Collection("reboque", "Reboque")
    vol: _Collection[Vol] = _Collection("vol", "Vol")
    modFrete = _int("modFrete")
    vagao = _str("vagao")
    balsa = _str("balsa")

class Transporta(Model):
    __slots__ = ()
    CNPJ = _str("CNPJ")
    CPF = _str("CPF")
    xNome = _str("xNome")
    IE = _str("IE")
    xEnder = _str("xEnder")
    xMun = _str("xMun")
    UF = _str("UF")

class VeicProd(Model):
    __slots__ = ()
    tpOp = _int("tpOp")
    chassi = _str("chassi")
    cCor = _str("cCor")
    xCor = _str("xCor")
    pot = _str("pot")
    cilin = _str("cilin")
    pesoL = _str("pesoL")
    pesoB = _str("pesoB")
    nSerie = _str("nSerie")
    tpComb = _str("tpComb")
    nMotor = _str("nMotor")
    CMT = _str("CMT")
    dist = _str("dist")
    anoMod = _int("anoMod")
    anoFab = _int("anoFab")
    tpPint = _str("tpPint")
    tpVeic = _str("tpVeic")
    espVeic = _str("espVeic")
    VIN = _str("VIN")
    condVeic = _int("condVeic")
    cMod = _str("cMod")
    cCorDENATRAN = _str("cCorDENATRAN")
    lota = _int("lota")
    tpRest = _int("tpRest")

class VeicTransp(Model):
    __slots__ = ()
    placa = _str("placa")
    UF = _str("UF")
    RNTC = _str("RNTC")

class Vol(Model):
    __slots__ = ()
    lacres: _Collection[Lacres] = _Collection("lacres", "Lacres")
    qVol = _float("qVol")
    esp = _str("esp")
    marca = _str("marca")
    nVol = _str("nVol")
    pesoL = _float("pesoL")
    pesoB = _float("pesoB")

__all__ = ['Adi', 'Agropecuario', 'Arma', 'ArquivosNF', 'AutXML', 'AutXMLAtor', 'Avulsa', 'Cana', 'CancNFe', 'Card', 'Certificado', 'Cide', 'Cobr', 'Cofins', 'CofinsST', 'Comb', 'Compra', 'ConfWebServices', 'ConfigNFe', 'ConsSitNFe', 'CppNFe', 'CreditoPresumidoEvento', 'Deduc', 'Defensivo', 'Dest', 'DestEPEC', 'Det', 'DetEvento', 'DetEventoRTC', 'DetExport', 'DetPag', 'DfeReferenciado', 'Di', 'Dup', 'Emit', 'Encerrante', 'EnderDest', 'EnderEmit', 'Entrega', 'EnvEvento', 'EstoqueEventoRTC', 'EventoNotaFiscal', 'ExportInd', 'Exporta', 'Fat', 'ForDia', 'GALCZFMCBS', 'GAjusteCompet', 'GCBS', 'GCBSMonoAdRem', 'GCBSMonoAdValorem', 'GCBSMonoPadraoAdRem', 'GCBSMonoPadraoAdValorem', 'GCBSMonoRet', 'GCBSMonoRetenAdRem', 'GCBSMonoRetenAdValorem', 'GCBSTot', 'GCBSpBioDiferenca', 'GCompraGov', 'GCred', 'GCredPres', 'GCredPresIBSZFM', 'GCredPresOper', 'GDevTrib', 'GDif', 'GEstornoCred', 'GEstornoCredTot', 'GIBSCBS', 'GIBSCBSMono', 'GIBSMonoAdRem', 'GIBSMonoAdValorem', 'GIBSMonoPadraoAdRem', 'GIBSMonoPadraoAdValorem', 'GIBSMonoRet', 'GIBSMonoRetenAdRem', 'GIBSMonoRetenAdValorem', 'GIBSMun', 'GIBSMunTot', 'GIBSTot', 'GIBSUF', 'GIBSUFTot', 'GIBSpBioDiferenca', 'GMonoTot', 'GPagAntecipado', 'GRed', 'GTransfCred', 'GTribCompraGov', 'GTribRegular', 'GuiaTransito', 'IbsCbs', 'IbsCbsTot', 'Icms', 'IcmsTot', 'IcmsUFDest', 'Ide', 'Ii', 'Imposto', 'ImpostoDevol', 'InfAdic', 'InfEvento', 'InfEventoRet', 'InfIntermed', 'InfInut', 'InfNFe', 'InfNFeSupl', 'InfRespTec', 'InutNFe', 'Ipi', 'Is', 'IsTot', 'IssQN', 'IssQNtot', 'ItemCreditoPresumidoEvento', 'ItemEventoRTC', 'ItemPedProrrog', 'ItemRespPedido', 'Lacres', 'Med', 'NFRef', 'NFeDistribuicaoDFe', 'Nfe', 'NotaFiscal', 'ObsCont', 'ObsFisco', 'ObsItem', 'OrigComb', 'Pag', 'PagamentoVinculadoEvento', 'Pis', 'PisST', 'ProcEventoNFe', 'ProcRef', 'Prod', 'ProtNFe', 'Rastro', 'Reboque', 'RefECF', 'RefNF', 'RefNFP', 'RespCancPedido', 'RespPedido', 'RetCancNFe', 'RetConsReciNFe', 'RetConsSitNFe', 'RetConsStatServ', 'RetEnvEvento', 'RetEvento', 'RetEventoCancNFe', 'RetInutNFe', 'RetTransp', 'RetTrib', 'Retirada', 'StatusServico', 'TEvento', 'Total', 'Transp', 'Transporta', 'VeicProd', 'VeicTransp', 'Vol']
