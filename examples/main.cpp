/*
 * consoleNFe — exemplo de uso da libCppNFe em modo console.
 *
 * Uso:
 *   consoleNFe [--config arquivo.ini] <comando> [argumentos]
 *
 * Comandos:
 *   status                                        status do serviço da SEFAZ
 *   gerar                                         gera, assina e valida a NF-e (sem transmitir)
 *   enviar [lote]                                 gera, assina, valida e transmite a NF-e
 *   consultar <chave>                             consulta a situação de uma NF-e
 *   cancelar <chave> <protocolo> <justificativa>  cancela uma NF-e autorizada
 *   inutilizar <serie> <inicio> <fim> <justif.>   inutiliza uma faixa de numeração
 *
 * Sem comando, abre um menu interativo. A configuração vem de consoleNFe.ini
 * (veja consoleNFe.ini.exemplo) e a senha do certificado pode vir da variável
 * de ambiente CPPBR_CERT_SENHA.
 *
 * O exemplo mostra o acesso aos objetos da biblioteca por referências e ponteiros
 * locais, em vez de repetir o caminho inteiro a cada chamada. Por exemplo, em vez de
 *
 *   cppnfe.notafiscal->NFe->obj->infNFe->ide->set_nNF(1);
 *
 * usa-se
 *
 *   auto &notas = *cppnfe.notafiscal->NFe;   // Container<Nfe>
 *   InfNFe *inf = notas.obj->infNFe.get();   // nota em preenchimento
 *   Ide *ide = inf->ide.get();
 *   ide->set_nNF(1);
 *
 * Os objetos continuam pertencendo à CppNFe (std::unique_ptr); as referências e
 * os ponteiros locais apenas encurtam o acesso e não devem ser liberados.
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QTextStream>
#include <CppNFe/CppNFe>

namespace {

QTextStream out(stdout);
QTextStream in(stdin);

// ---------------------------------------------------------------- configuração

bool configurar(CppNFe &cppnfe, const QSettings &ini)
{
    ConfigNFe &cfg = *cppnfe.configuracoes;
    auto *arquivos = cfg.arquivos.get();
    auto *certificado = cfg.certificado.get();
    auto *ws = cfg.webservices.get();

    cfg.set_ModeloDF(ConvNF::strToModeloDF(ini.value("geral/modelo", "55").toString()));
    cfg.set_VersaoNF(VersaoNF::v400);
    cfg.set_VersaoQrCodeNF(VersaoQrCodeNF::v200);
    cfg.set_TipoEmissao(TpEmis::Normal);
    cfg.set_indicadorSincrono(IndSinc::Sim);
    cfg.set_IdCSC(ini.value("geral/idCSC").toString());
    cfg.set_CSC(ini.value("geral/CSC").toString());

    arquivos->set_caminhoSchema(ini.value("geral/schemas").toString());
    arquivos->set_caminhoSalvar(ini.value("geral/salvar").toString());
    arquivos->set_salvar(true);
    arquivos->set_salvarLogs(true);
    arquivos->set_salvarPorModelo(true);
    arquivos->set_salvarPorCNPJ(true);
    arquivos->set_salvarPorAnoMes(true);

    const bool producao = ini.value("geral/ambiente").toString().compare("producao", Qt::CaseInsensitive) == 0;
    ws->set_tpAmb(producao ? TpAmb::Producao : TpAmb::Homologacao);
    ws->set_uf(ConvNF::strToUF(ini.value("geral/uf", "RO").toString()));
    ws->set_httpType(ini.value("geral/http").toString() == "HttpOpenSSL" ? HttpType::HttpOpenSSL
                                                                          : HttpType::HttpQt);
    ws->set_protocoloSSL(QSsl::TlsV1_2OrLater);
    ws->set_timeoutAssincrono(ini.value("geral/timeout", 8000).toInt());
    ws->set_tentativas(3);
    ws->set_compactar(true);
    ws->set_compactarAcimaDe(300);
    ws->set_verificarSslSocket(true);

    QString senha = ini.value("certificado/senha").toString();
    if (senha.isEmpty())
        senha = qEnvironmentVariable("CPPBR_CERT_SENHA");
    const QString pfx = ini.value("certificado/arquivo").toString();
    if (pfx.isEmpty() || senha.isEmpty())
    {
        out << "Aviso: certificado ou senha não informados; a NF-e é gerada sem assinatura"
               " e os serviços da SEFAZ ficam indisponíveis.\n";
        return false;
    }

    certificado->set_cryptoLib(CryptoType::OpenSSL);
    certificado->set_caminhoCertificado(pfx);
    certificado->set_senhaCertificado(senha);
    for (const QString &ac : ini.value("certificado/cadeia").toStringList())
        certificado->addCaminhoCertificadoAC(ac.trimmed());

    if (!certificado->carregarCertificado())
    {
        out << "Erro ao carregar o certificado: " << certificado->get_error() << "\n";
        return false;
    }
    out << "Certificado: " << certificado->get_razaoSocial()
        << " (" << certificado->get_cnpj() << ")\n";
    return true;
}

// ------------------------------------------------------------- preenchimento

void preencherIde(Ide &ide, const ConfigNFe &cfg, const QSettings &ini)
{
    ide.set_cUF(ConvNF::ufToInt(cfg.webservices->get_uf()));
    ide.set_cNF(CppUtility::random(8));
    ide.set_natOp("VENDA DE MERCADORIA");
    ide.set_mod(cfg.get_ModeloDF());
    ide.set_serie(ini.value("nota/serie", 1).toInt());
    ide.set_nNF(ini.value("nota/numero", 1).toInt());
    ide.set_dhEmi(QDateTime::currentDateTime());
    ide.set_tpNF(TpNF::Saida);
    ide.set_idDest(IdDest::Interna);
    ide.set_cMunFG(ini.value("emitente/cMun").toInt());
    ide.set_tpImp(cfg.get_ModeloDF() == ModeloDF::NFCe ? TpImp::NFCe : TpImp::Retrato);
    ide.set_tpEmis(cfg.get_TipoEmissao());
    ide.set_tpAmb(cfg.webservices->get_tpAmb());
    ide.set_finNFe(FinNFe::Normal);
    ide.set_indFinal(IndFinal::ConsumidorFinal);
    ide.set_indPres(IndPres::Presencial);
    ide.set_indIntermed(IndIntermed::SemIntermediador);
    ide.set_procEmi(ProcEmi::AplicativoContribuinte);
    ide.set_verProc("consoleNFe 1.0");
}

void preencherEmitente(Emit &emitente, const QSettings &ini)
{
    emitente.set_CNPJ(ini.value("emitente/CNPJ").toString());
    emitente.set_xNome(ini.value("emitente/xNome").toString());
    emitente.set_xFant(ini.value("emitente/xFant").toString());
    emitente.set_IE(ini.value("emitente/IE").toString());
    emitente.set_CRT(Crt::RegimeNormal);

    auto *ender = emitente.enderEmit.get();
    ender->set_xLgr(ini.value("emitente/xLgr").toString());
    ender->set_nro(ini.value("emitente/nro").toInt());
    ender->set_xBairro(ini.value("emitente/xBairro").toString());
    ender->set_cMun(ini.value("emitente/cMun").toInt());
    ender->set_xMun(ini.value("emitente/xMun").toString());
    ender->set_UF(ini.value("emitente/UF").toString());
    ender->set_CEP(ini.value("emitente/CEP").toInt());
    ender->set_cPais(1058);
    ender->set_xPais("BRASIL");
    ender->set_fone(ini.value("emitente/fone").toString());
}

void preencherDestinatario(Dest &dest, const QSettings &ini)
{
    dest.set_CPF("12345678909");
    dest.set_xNome("NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
    dest.set_indIEDest(IndIEDest::NaoContribuinte);

    auto *ender = dest.enderDest.get();
    ender->set_xLgr("RUA B");
    ender->set_nro(200);
    ender->set_xBairro("CENTRO");
    ender->set_cMun(ini.value("emitente/cMun").toInt());
    ender->set_xMun(ini.value("emitente/xMun").toString());
    ender->set_UF(ini.value("emitente/UF").toString());
    ender->set_CEP(ini.value("emitente/CEP").toInt());
    ender->set_cPais(1058);
    ender->set_xPais("BRASIL");
}

struct Item
{
    QString codigo;
    QString descricao;
    QString ncm;
    double quantidade;
    double valorUnitario;
};

// Preenche o rascunho (obj) do container det e confirma com add().
void adicionarItem(Container<Det> &itens, const Item &item)
{
    Det *det = itens.obj;
    const int nItem = itens.count() + 1;
    const double valor = item.quantidade * item.valorUnitario;

    auto *prod = det->prod.get();
    prod->set_nItem(nItem);
    prod->set_cProd(item.codigo);
    prod->set_cEAN("SEM GTIN");
    prod->set_xProd(item.descricao);
    prod->set_NCM(item.ncm);
    prod->set_CFOP("5102");
    prod->set_uCom("UN");
    prod->set_qCom(item.quantidade);
    prod->set_vUnCom(item.valorUnitario);
    prod->set_vProd(valor);
    prod->set_cEANTrib("SEM GTIN");
    prod->set_uTrib("UN");
    prod->set_qTrib(item.quantidade);
    prod->set_vUnTrib(item.valorUnitario);
    prod->set_indTot(IndTot::CompoeTotalNF);

    auto &imposto = *det->imposto;
    auto *icms = imposto.ICMS.get();
    icms->set_orig(Orig::Nacional);
    icms->set_CST(CstICMS::ICMS00);
    icms->set_modBC(ModBC::ValorOperacao);
    icms->set_vBC(valor);
    icms->set_pICMS(18);
    icms->set_vICMS(qRound(valor * 18) / 100.0);
    imposto.PIS->set_CST(CstPIS::PIS49);
    imposto.COFINS->set_CST(CstCOFINS::COFINS49);

    itens.add();
}

void preencherTotais(InfNFe &inf)
{
    double vProd = 0, vBC = 0, vICMS = 0;
    for (const Det *det : *inf.det)
    {
        vProd += det->prod->get_vProd();
        vBC += det->imposto->ICMS->get_vBC();
        vICMS += det->imposto->ICMS->get_vICMS();
    }

    auto *tot = inf.total->ICMSTot.get();
    tot->set_vBC(vBC);
    tot->set_vICMS(vICMS);
    tot->set_vProd(vProd);
    tot->set_vNF(vProd);

    inf.transp->set_modFrete(ModFrete::SemFrete);

    auto &pagamentos = *inf.pag->detPag;
    pagamentos.obj->set_tPag(TPag::Dinheiro);
    pagamentos.obj->set_vPag(vProd);
    pagamentos.add();
}

void preencherNota(CppNFe &cppnfe, const QSettings &ini)
{
    auto &notas = *cppnfe.notafiscal->NFe;  // Container<Nfe>
    notas.clear();

    InfNFe *inf = notas.obj->infNFe.get();
    inf->set_versao("4.00");
    preencherIde(*inf->ide, *cppnfe.configuracoes, ini);
    preencherEmitente(*inf->emite, ini);
    preencherDestinatario(*inf->dest, ini);

    auto &itens = *inf->det;
    adicionarItem(itens, {"001", "PRODUTO DE TESTE 1", "22021000", 2, 5.50});
    adicionarItem(itens, {"002", "PRODUTO DE TESTE 2", "19053100", 1, 12.90});

    preencherTotais(*inf);
    inf->infAdic->set_infCpl("Nota gerada pelo exemplo consoleNFe da CppBrasil.");

    notas.add();  // confirma a nota no container
}

// ------------------------------------------------------------------ comandos

bool gerar(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk)
{
    auto &nf = *cppnfe.notafiscal;
    preencherNota(cppnfe, ini);

    if (!nf.gerar())
    {
        out << "Erro ao gerar: " << nf.NFe->value(0)->get_error() << "\n";
        return false;
    }
    const Nfe *nota = nf.NFe->value(0);
    out << "Chave: " << nota->get_chNFe() << "\n";

    if (!certificadoOk)
    {
        out << "XML gerado (sem assinatura):\n" << nota->get_XMLOriginal() << "\n";
        return false;
    }
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
    out << "NF-e assinada e validada. XML salvo em: "
        << QDir::toNativeSeparators(cppnfe.configuracoes->arquivos->get_caminhoNF()) << "\n";
    return true;
}

void enviar(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk, int lote)
{
    if (!gerar(cppnfe, ini, certificadoOk))
        return;

    auto &nf = *cppnfe.notafiscal;
    if (!nf.enviar(lote))
    {
        out << "Erro no envio: " << nf.get_error() << "\n";
        return;
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
}

void status(CppNFe &cppnfe)
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

void consultar(CppNFe &cppnfe, const QString &chave)
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
              const QString &justificativa)
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
                const QString &justificativa)
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

// --------------------------------------------------------------------- menu

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

QString perguntar(const QString &texto)
{
    out << texto << ": " << Qt::flush;
    return in.readLine().trimmed();
}

int executar(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk, QStringList args)
{
    const QString comando = args.isEmpty() ? QString() : args.takeFirst().toLower();

    if (comando == "status")
        status(cppnfe);
    else if (comando == "gerar")
        return gerar(cppnfe, ini, certificadoOk) ? 0 : 1;
    else if (comando == "enviar")
        enviar(cppnfe, ini, certificadoOk, args.value(0, "1").toInt());
    else if (comando == "consultar" && args.size() == 1)
        consultar(cppnfe, args[0]);
    else if (comando == "cancelar" && args.size() >= 3)
        cancelar(cppnfe, ini, args[0], args[1], args.mid(2).join(' '));
    else if (comando == "inutilizar" && args.size() >= 4)
        inutilizar(cppnfe, ini, args[0].toInt(), args[1].toInt(), args[2].toInt(), args.mid(3).join(' '));
    else
    {
        out << "Comando inválido. Use --help para ver os comandos.\n";
        return 2;
    }
    return 0;
}

void menu(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk)
{
    for (;;)
    {
        out << "\n=== consoleNFe ===\n"
               "1 - Status do serviço\n"
               "2 - Gerar, assinar e validar NF-e\n"
               "3 - Gerar e enviar NF-e\n"
               "4 - Consultar NF-e\n"
               "5 - Cancelar NF-e\n"
               "6 - Inutilizar numeração\n"
               "0 - Sair\n";
        const QString opcao = perguntar("Opção");
        if (opcao == "1")
            status(cppnfe);
        else if (opcao == "2")
            gerar(cppnfe, ini, certificadoOk);
        else if (opcao == "3")
            enviar(cppnfe, ini, certificadoOk, perguntar("Número do lote").toInt());
        else if (opcao == "4")
            consultar(cppnfe, perguntar("Chave"));
        else if (opcao == "5")
            cancelar(cppnfe, ini, perguntar("Chave"), perguntar("Protocolo"), perguntar("Justificativa"));
        else if (opcao == "6")
            inutilizar(cppnfe, ini, perguntar("Série").toInt(), perguntar("Número inicial").toInt(),
                       perguntar("Número final").toInt(), perguntar("Justificativa"));
        else if (opcao == "0" || in.atEnd())
            return;
    }
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("consoleNFe");

    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Exemplo da libCppNFe.\n\nComandos:\n"
        "  status\n  gerar\n  enviar [lote]\n  consultar <chave>\n"
        "  cancelar <chave> <protocolo> <justificativa>\n"
        "  inutilizar <serie> <inicio> <fim> <justificativa>\n\n"
        "Sem comando, abre o menu interativo.");
    parser.addHelpOption();
    QCommandLineOption optConfig("config", "Arquivo de configuração.", "arquivo",
                                 QCoreApplication::applicationDirPath() + "/consoleNFe.ini");
    parser.addOption(optConfig);
    parser.addPositionalArgument("comando", "Comando a executar.");
    parser.process(app);

    const QString arquivoIni = parser.value(optConfig);
    if (!QFileInfo::exists(arquivoIni))
    {
        out << "Arquivo de configuração não encontrado: " << QDir::toNativeSeparators(arquivoIni) << "\n"
            << "Copie consoleNFe.ini.exemplo para consoleNFe.ini e ajuste os dados.\n";
        return 1;
    }
    const QSettings ini(arquivoIni, QSettings::IniFormat);

    CppNFe cppnfe;
    QObject::connect(&cppnfe, &CppNFe::errorOccurred,
                     [](const QString &erro) { out << "[erro] " << erro << Qt::endl; });
    QObject::connect(&cppnfe, &CppNFe::wsChange, [](const WebServicesNF &ws) {
        if (ws != WebServicesNF::None)
            out << "[sefaz] " << nomeServico(ws) << "..." << Qt::endl;
    });

    const bool certificadoOk = configurar(cppnfe, ini);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty())
    {
        menu(cppnfe, ini, certificadoOk);
        return 0;
    }
    return executar(cppnfe, ini, certificadoOk, args);
}
