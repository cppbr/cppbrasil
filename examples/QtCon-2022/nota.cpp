#include "nota.h"

Nota::Nota(QObject *parent)
    : QObject{parent}, m_nfe{new CppNFe}
{
    connect(m_nfe, &CppNFe::wsChange, this, &Nota::onWSChange);
    connect(m_nfe, &CppNFe::errorOccurred, this, &Nota::errorOccurred);

}

Nota::~Nota()
{
    delete m_nfe;
}

void Nota::configurar(const ModeloDF &modeloDF)
{
    //configuração
    //geral
    m_nfe->configuracoes->set_ModeloDF(modeloDF); //modelo é passado na
    m_nfe->configuracoes->set_VersaoNF(VersaoNF::v400);
    m_nfe->configuracoes->set_VersaoQrCodeNF(VersaoQrCodeNF::v200);
    m_nfe->configuracoes->set_TipoEmissao(TpEmis::Normal);
    m_nfe->configuracoes->set_IdCSC("ID DO CSC");
    m_nfe->configuracoes->set_CSC("INSIRA O CSC");
    //conexão sincrona somente para NFCe e também se for uma por vez
    if (m_nfe->configuracoes->get_ModeloDF() == ModeloDF::NFCe)
        m_nfe->configuracoes->set_indicadorSincrono(IndSinc::Sim);
    else
        m_nfe->configuracoes->set_indicadorSincrono(IndSinc::Nao);

    //arquivos
#if defined(Q_OS_ANDROID)
    m_nfe->configuracoes->arquivos->set_caminhoSchema("/data/data/org.cppnfe.quicknfe/files/schemaNFe");
#elif defined(Q_OS_WIN)
    m_nfe->configuracoes->arquivos->set_caminhoSchema("C:/cppbrasil/schema/");
#else
    m_nfe->configuracoes->arquivos->set_caminhoSchema("/home/usuario/schemaNFe/");
#endif
    //salvar
    m_nfe->configuracoes->arquivos->set_salvar(true);
    m_nfe->configuracoes->arquivos->set_salvarLogs(true);
    m_nfe->configuracoes->arquivos->set_salvarPorModelo(true); //55 ou 65
    m_nfe->configuracoes->arquivos->set_salvarPorCNPJ(1);
    m_nfe->configuracoes->arquivos->set_salvarPorAnoMes(1); //yyyymm
    //m_nfe->configuracoes->arquivos->set_salvarPorAnoMesDia(1); //yyymmdd
    //m_nfe->configuracoes->arquivos->set_salvarPorAno(1); //yyyy
    //m_nfe->configuracoes->arquivos->set_salvarPorMes(1); //mm
    //m_nfe->configuracoes->arquivos->set_salvarPorDia(1); //dd

    //obs: se não informar o lugar para salvar, será salvo na pasta do executável.
#if defined(Q_OS_ANDROID)
    m_nfe->configuracoes->arquivos->set_caminhoSalvar("/data/data/org.cppnfe.quicknfe/files/xml");
#elif defined(Q_OS_WIN)
    m_nfe->configuracoes->arquivos->set_caminhoSalvar("C:/cppbrasil/xml");
#else
    m_nfe->configuracoes->arquivos->set_caminhoSalvar("/home/usuario/QtCon/exemplos/xml");
#endif

    m_nfe->configuracoes->certificado->set_cryptoLib(CryptoType::OpenSSL);
    //configuração certificado

#if defined(Q_OS_ANDROID)
    m_nfe->configuracoes->certificado->set_caminhoCertificado("/data/data/org.cppnfe.quicknfe/files/certificado.pfx");
    //certificados AC
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/data/data/org.cppnfe.quicknfe/files/icp-brasil/ICP-Brasil.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/data/data/org.cppnfe.quicknfe/files/icp-brasil/ICP-Brasilv2.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/data/data/org.cppnfe.quicknfe/files/icp-brasil/ICP-Brasilv5.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/data/data/org.cppnfe.quicknfe/files/icp-brasil/ICP-Brasilv10.crt");
#elif defined(Q_OS_WIN)
   m_nfe->configuracoes->certificado->set_caminhoCertificado("C:/cppbrasil/certificado/certificado.pfx");
   //certificados AC
   m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("C:/cppbrasil/certificado/icp-brasil/ICP-Brasil.crt");
   m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("C:/cppbrasil/certificado/icp-brasil/ICP-Brasilv2.crt");
   m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("C:/cppbrasil/certificado/icp-brasil/ICP-Brasilv5.crt");
   m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("C:/cppbrasil/certificado/icp-brasil/ICP-Brasilv10.crt");
#else
    m_nfe->configuracoes->certificado->set_caminhoCertificado("/home/usuario/certificado/certificado.pfx");
    //certificados AC
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/home/usuario/certificado/icp-brasil/ICP-Brasil.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/home/usuario/certificado/icp-brasil/ICP-Brasilv2.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/home/usuario/certificado/icp-brasil/ICP-Brasilv5.crt");
    m_nfe->configuracoes->certificado->addCaminhoCertificadoAC("/home/usuario/certificado/icp-brasil/ICP-Brasilv10.crt");
#endif
    //senha certificado
    m_nfe->configuracoes->certificado->set_senhaCertificado("1234");

    if (!m_nfe->configuracoes->certificado->carregarCertificado())
    {
        qInfo() << "Erro ao configurar certificado";
        QString _err =  m_nfe->configuracoes->certificado->get_error();
        emit errorOccurred(_err);
    }
    //webservices
    m_nfe->configuracoes->webservices->set_httpType(HttpType::HttpQt);
    m_nfe->configuracoes->webservices->set_tpAmb(TpAmb::Homologacao);
    m_nfe->configuracoes->webservices->set_protocoloSSL(ConvNF::strToQSslProtocol("TlsV1_2"));//QSsl::TlsV1_3
    m_nfe->configuracoes->webservices->set_timeoutAssincrono(5000);//milisegundos
    m_nfe->configuracoes->webservices->set_tentativas(3);
    m_nfe->configuracoes->webservices->set_uf(UF::RO);
    m_nfe->configuracoes->webservices->set_compactar(true);
    m_nfe->configuracoes->webservices->set_compactarAcimaDe(300);
    m_nfe->configuracoes->webservices->set_verificarSslSocket(true); //remove o tratamento de exception

}

void Nota::nfe(const int &numNota)
{
    infNFe();
    //atualizando para o modelo e numero indicado
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_nNF(numNota);

    m_nfe->notafiscal->NFe->add();
}

void Nota::infNFe()
{
    m_nfe->notafiscal->NFe->obj->infNFe->set_versao(ConvNF::versaoNFToStr(m_nfe->configuracoes->get_VersaoNF()));
    ide();
    emite();
    dest();
    retirada();
    entrega();
    autXML();
    det();
    total();
    transp();
    cobr();
    pag();
    infIntermed();
    infAdic();
    exporta();
    compra();
    cana();
    infRespTec();
}

void Nota::ide()
{
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_cUF(11);
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_cNF(CppUtil::random(8));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_natOp("VENDA AO CONSUMIDOR");
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_mod(m_nfe->configuracoes->get_ModeloDF());
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_serie(1);
    //m_nfe->notafiscal->NFe->obj->infNFe->ide->set_nNF(); //numero será inserido por ultimo
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_dhEmi(QDateTime::currentDateTime());
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_dhSaiEnt(QDateTime::currentDateTime());
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_tpNF(ConvNF::strToTpNF("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_idDest(ConvNF::strToIdDest("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_cMunFG(1100205);
    //O tipo de relatório muda de acordo com o tipo.
    if (m_nfe->configuracoes->get_ModeloDF() == ModeloDF::NFCe)
        m_nfe->notafiscal->NFe->obj->infNFe->ide->set_tpImp(TpImp::NFCe);//ConvNF::strToTpImp("1")); //TpImp::Retrato
    else
        m_nfe->notafiscal->NFe->obj->infNFe->ide->set_tpImp(TpImp::Retrato);
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_tpEmis(ConvNF::strToTpEmis("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_tpAmb(m_nfe->configuracoes->webservices->get_tpAmb());//(ConvNF::strToTpAmb("2"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_finNFe(ConvNF::strToFinNFe("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_indFinal(ConvNF::strToIndFinal("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_indPres(ConvNF::strToIndPres("1"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_indIntermed(ConvNF::strToIndIntermed("0"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_procEmi(ConvNF::strToProcEmi("0"));
    m_nfe->notafiscal->NFe->obj->infNFe->ide->set_verProc("1.0.0.0");
    //--contingência
    //m_nfe->notafiscal->NFe->obj->infNFe->ide->set_dhCont(QDateTime::currentDateTime());
    //m_nfe->notafiscal->NFe->obj->infNFe->ide->set_xJust("Justificativa contingencia");

    //Grupo BA. Documento Fiscal Referenciado
    //---NFe referenciada --***CONTAINER*** 0-500
    /*
    //NFe/NFCe
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->set_refNFe("numero da chave de acesso");
    //CTe
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->set_refCTe("Chave de acesso do CT-e");

    //Informação da NF modelo 1/1A ou NF modelo 2 referenciada
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_cUF();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_AAMM();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_mod(); //01 ou 02
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_serie();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNF->set_nNF();

    //Informações da NF de produtorrural referenciada
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_cUF();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_AAMM();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_CPF();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_IE();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_mod(); //04=NF de Produtor
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_serie();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refNFP->set_nNF();

    //Informações do Cupom Fiscal referenciado
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refECF->set_mod(); //2B, 2C OU 2D
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refECF->set_nECF();
    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->obj->refECF->set_nCOO();

    m_nfe->notafiscal->NFe->obj->infNFe->ide->NFref->add(); //adicionando Referenciada
    */

}

void Nota::emite()
{
    /*
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CNPJ("CNPJ");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CPF("CPF");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_xNome("RAZÃO SOCIAL");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_xFant("FANTASIA");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IE("INSCRIÇÃO ESTADUAL");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IEST("IE do Substituto Tributário");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IM("INSCRIÇÃO MUNICIPAL");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CNAE("CNAE fiscal");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CRT(Crt::RegimeNormal);
    //****endereço
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xLgr("RUA, AVENIDA E ETC..");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_nro(); //NUMERO INTEIRO
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xCpl("complemento");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xBairro("BAIRRO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_cMun();//CODIGO DO MUNICIPIO NUMERO INTEIRO
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xMun("NOME DO MUNICIPIO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_UF("UF");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_CEP(); //CEP NUMERO INTEIRO
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_cPais(1058); //1058 BRASIL
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xPais("BRASIL");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_fone("TELEFONE");
    */

    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CNPJ("00000000000000");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CPF("CPF");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_xNome("RAZAO SOCIAL");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_xFant("FANTASIA");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IE("00000003283399");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IEST();
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_IM("");
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CNAE("");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->set_CRT(Crt::RegimeNormal);
    //****endereço
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xLgr("ENDEREÇO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_nro(0);
    //m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xCpl("complemento");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xBairro("BAISSO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_cMun(1100205);
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xMun("PORTO VELHO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_UF("RO");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_CEP(76800000);
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_cPais(1058);
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_xPais("BRASIL");
    m_nfe->notafiscal->NFe->obj->infNFe->emite->enderEmit->set_fone("123456789");

}

void Nota::dest()
{
    /*
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_CNPJ("CNPJ"); //PARA PESSOA JURIDICA
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_CPF("CPF"); //PARA PESSOA FISICA
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_idEstrangeiro("ID ESTRANGEIRO")
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_xNome("NOME DO DESTINATÁRIO");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_indIEDest(IndIEDest::NaoContribuinte);
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_IE("IE");
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_ISUF("suframa");
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_IM("insc municipal");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_email("email@.com");
    //Endereço
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xLgr("rua");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_nro(1234); //numero interio
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xCpl("complemento");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xBairro("BAIRRO");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_cMun(); //CODIGO DO MUNICÍPIO
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xMun("NOME DA CIDADE");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_UF("UF");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_CEP(); //NUMERO INTEIRO
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_cPais(1058); //BRASIL
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xPais("BRASIL");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_fone("TELEFONE"); //STRING
    */

    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_CNPJ("CNPJ");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_CPF("12345678910");
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_idEstrangeiro("ID ESTRANGEIRO")
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_xNome("FULANO DE TAL");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_indIEDest(IndIEDest::NaoContribuinte);
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_IE("IE");
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_ISUF("suframa");
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->set_IM("insc municipal");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->set_email("jetmorju@gmail.com");
    //Endereço
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xLgr("rua carlos");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_nro(01);
    //m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xCpl("complemento");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xBairro("BAIRRO");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_cMun(1100205);
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xMun("PORTO VELHO");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_UF("RO");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_CEP(76800000);
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_cPais(1058);
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_xPais("BRASIL");
    m_nfe->notafiscal->NFe->obj->infNFe->dest->enderDest->set_fone("123456789");


}

void Nota::retirada()
{
    /*
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_CPF();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_xNome();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_xLgr();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_nro();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_xCpl();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_xBairro();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_cMun();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_UF();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_CEP();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_cPais();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_xPais();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_fone();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_email();
    m_nfe->notafiscal->NFe->obj->infNFe->retirada->set_IE();
    */

}

void Nota::entrega()
{
    /*
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_CPF();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_xNome();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_xLgr();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_nro();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_xCpl();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_xBairro();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_cMun();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_UF();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_CEP();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_cPais();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_xPais();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_fone();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_email();
    m_nfe->notafiscal->NFe->obj->infNFe->entrega->set_IE();
    */

}

void Nota::autXML()
{
    /*
    m_nfe->notafiscal->NFe->obj->infNFe->autXML->obj->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->autXML->obj->get_CPF();
    m_nfe->notafiscal->NFe->obj->infNFe->autXML->add();
    */
}

void Nota::det()
{
    for (int i = 0; i <= 1; ++i) {
        det_produto(i);
        det_imposto();
        det_impostoDevol();
        det_obsItem();
        //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->set_infAdProd();

        m_nfe->notafiscal->NFe->obj->infNFe->det->add();

    }
}

void Nota::det_produto(const int &i)
{
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_nItem(i + 1);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_cProd("123");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_cEAN("SEM GTIN");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_xProd("PRODUTO TESTE");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_NCM("19021100");
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_NVE();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_CEST("1704900");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_indEscala(ConvNF::strToIndEscala("S"));
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_CNPJFab("");
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_cBenef("");
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_EXTIPI("");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_CFOP("5102");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_uCom("UNID");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_qCom(1.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vUnCom(1.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vProd(1.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_cEANTrib("SEM GTIN");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_uTrib("UNID");
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_qTrib(1.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vUnTrib(1.00);
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vFrete();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vSeg();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vDesc();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vOutro();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_indTot(ConvNF::strToIndTot("1"));
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vFrete();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vSeg();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vDesc();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_vOutro();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_indTot(ConvNF::strToIndTot("1"));
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_xPed();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_nItemPed();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_nFCI();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->set_nRECOPI();

    /*
    //Grupo I01. Produtos e Serviços / Declaração de Importação
    //***CONTAINER*** 0-100
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_nDI();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_dDI();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_xLocDesemb();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_UFDesemb();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_dDesemb();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_tpViaTransp();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_vAFRMM();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_tpIntermedio();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_UFTerceiro();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->set_cExportador();
        //---Adições(Dentro de DI)--***CONTAINER*** 1-100
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->obj->set_nAdicao();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->obj->set_nSeqAdic();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->obj->set_cFabricante();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->obj->set_vDescDI();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->obj->set_nDraw();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->obj->adi->add();//adicionando adi
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->DI->add();//adicionando DI
    */

    /*
    //Grupo I03. Produtos e Serviços / Grupo de Exportação
    //-***CONTAINER*** 0-500
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->detExport->obj->set_nDraw();
        //Grupo sobre exportação indireta
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->detExport->obj->exportInd->set_nRE();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->detExport->obj->exportInd->set_chNFe();
        m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->detExport->obj->exportInd->set_qExport();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->detExport->add();//adicionando detExport
    */

    /*
    //Grupo I80. Rastreabilidade de produto
    //--***CONTAINER*** 0-500
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->obj->set_nLote();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->obj->set_qLote();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->obj->set_dFab();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->obj->set_dVal();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->obj->set_cAgreg();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->rastro->add();//adicionando rastro
    */

    /*
    //Grupo JA. Detalhamento Específico de Veículos novos
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_tpOp();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_chassi();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_cCor();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_pot();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_cilin();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_pesoL();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_pesoB();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_nSerie();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_tpComb();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_nMotor();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_CMT();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_dist();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_anoMod();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_anoFab();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_tpPint();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_tpVeic();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_espVeic();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_VIN();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_condVeic();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_cMod();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_cCorDENATRAN();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_lota();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->veicProd->set_tpRest();
    */

    /*
    //Grupo K. Detalhamento Específico de Medicamento e de matérias-primas farmacêuticas
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->med->set_cProdANVISA();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->med->set_xMotivoIsencao();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->med->set_vPMC();
    */

    /*
    //Grupo L. Detalhamento Específico de Armamentos
    //--***CONTAINER*** 1-500
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->arma->obj->set_tpArma();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->arma->obj->set_nSerie();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->arma->obj->set_nCano();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->arma->obj->set_descr();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->arma->add();
    */

    /*
    //Grupo LA. Detalhamento Específico de Combustíveis
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_cProdANP();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_descANP();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_pGLP();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_pGNn();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_pGNi();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_vPart();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_CODIF();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_qTemp();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->set_UFCons();
    //LA07 - Informações da CIDE (obs:Dentro de comb)
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->CIDE->set_qBCProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->CIDE->set_vAliqProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->CIDE->set_vCIDE();
    //LA11 - Informações do grupo de “encerrante” (obs:Dentro de comb)
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->encerrante->set_nBico();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->encerrante->set_nBomba();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->encerrante->set_nTanque();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->encerrante->set_vEncIni();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->prod->comb->encerrante->set_vEncFin();
    */


}

void Nota::det_imposto()
{
    //Grupo M. Tributos incidentes no Produto ou Serviço
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->set_vTotTrib(0.10);
    //Grupo N01. ICMS Normal e ST
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_orig(ConvNF::strToOrig("0"));
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_CST(ConvNF::strToCstICMS("00"));
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_modBC(ConvNF::strToModBC("0"));
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_vBC(1.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_pICMS(17.50);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_vICMS(0.17);
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_pFCP();
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_vFCP();

    //simples nacional
    //m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMS->set_CSOSN(ConvNF::strToCsosnICMS("102"));

    /*
    //Grupo NA. ICMS para a UF de destino
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_vBCUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_vBCFCPUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_pFCPUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSInter();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSInterPart();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_vFCPUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_vICMSUFDest();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ICMSUFDest->set_vICMSUFRemet();
    */

    /*
    //Grupo O. Imposto sobre Produtos Industrializados
    //IPI
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_CNPJProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_cSelo();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_qSelo();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_cEnq();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_CST();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_pIPI();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_qUnid();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_vUnid();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->IPI->set_vIPI();
    */

    /*
    //Grupo P. Imposto de Importação
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->II->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->II->set_vDespAdu();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->II->set_vII();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->II->set_vIOF();
    */


    //Grupo Q. PIS
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PIS->set_CST(ConvNF::strToCstPIS("49"));
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PIS->set_vBC(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PIS->set_pPIS(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PIS->set_vPIS(0.00);


    /*
    //Grupo R. PIS ST
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_pPIS();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_qBCProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_vAliqProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_vAliqProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->PISST->set_vPIS();
    */

    //Grupo S. COFINS
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINS->set_CST(ConvNF::strToCstCOFINS("49"));
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINS->set_vBC(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINS->set_pCOFINS(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINS->set_vCOFINS(0.00);

    /*
    //Grupo T. COFINS ST
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINSST->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINSST->set_pCOFINS();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINSST->set_qBCProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINSST->set_vAliqProd();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->COFINSST->set_vCOFINS();
    */

    /*
    //Grupo U. ISSQN
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vAliq();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vISSQN();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_cMunFG();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_cListServ();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vDeducao();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vOutro();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vDescIncond();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vDescCond();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_vISSRet();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_indISS();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_cServico();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_cMun();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_cPais();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_nProcesso();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->imposto->ISSQN->set_indIncentivo();
    */

}

void Nota::det_impostoDevol()
{
    /*
    //Grupo UA. Tributos Devolvidos (para o item da NF-e)
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->impostoDevol->set_pDevol();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->impostoDevol->set_vIPIDevol();
    */

}

void Nota::det_obsItem()
{
    //Nota Técnica 2021.004

    /*
    //Grupo de observações de uso livre do Contribuinte
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->obsItem->obsCont->set_xCampo();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->obsItem->obsCont->set_xTexto();
    //Grupo de observações de uso livre do Fisco
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->obsItem->obsFisco->set_xCampo();
    m_nfe->notafiscal->NFe->obj->infNFe->det->obj->obsItem->obsFisco->set_xTexto();
    */

}

void Nota::total()
{
    //Grupo W. Total da NF-e
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vBC(2.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vICMS(0.34);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vICMSDeson(0.00);
    //m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vFCPUFDest();
    //m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vICMSUFDest();
    //m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vICMSUFRemet();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vFCP(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vBCST(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vST(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vFCPST(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vFCPSTRet(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vProd(2.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vFrete(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vSeg(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vDesc(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vII(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vIPI(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vIPIDevol(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vPIS(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vCOFINS(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vOutro(0.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vNF(2.00);
    m_nfe->notafiscal->NFe->obj->infNFe->total->ICMSTot->set_vTotTrib(0.20);

    /*
    //Grupo W01. Total da NF-e / ISSQN
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vServ();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vBC();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vISS();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vPIS();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vCOFINS();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_dCompet();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vDeducao();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vOutro();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vDescIncond();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vDescCond();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_vISSRet();
    m_nfe->notafiscal->NFe->obj->infNFe->total->ISSQNtot->set_cRegTrib();
    */

    /*
    //Grupo W02. Total da NF-e / Retenção de Tributos
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vRetPIS();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vRetCOFINS();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vRetCSLL();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vBCIRRF();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vIRRF();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vBCRetPrev();
    m_nfe->notafiscal->NFe->obj->infNFe->total->retTrib->set_vRetPrev();
    */

}

void Nota::transp()
{
    //Grupo X. Informações do Transporte da NF-e
    m_nfe->notafiscal->NFe->obj->infNFe->transp->set_modFrete(ConvNF::strToModFrete("9"));
    transp_transporta();
    transp_retTransp();
    transp_veicTransp();
    transp_reboque();
    //m_nfe->notafiscal->NFe->obj->infNFe->transp->set_vagao();
    //m_nfe->notafiscal->NFe->obj->infNFe->transp->set_balsa();
    transp_vol();

}

void Nota::transp_transporta()
{
    /*
    //Grupo Transportador
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_CPF();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_xNome();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_IE();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_xEnder();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_xMun();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->transporta->set_UF();
    */

}

void Nota::transp_retTransp()
{
    /*
    //Grupo Retenção ICMS transporte
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_vServ();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_vBCRet();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_pICMSRet();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_vICMSRet();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_CFOP();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->retTransp->set_cMunFG();
    */
}

void Nota::transp_veicTransp()
{
    /*
    //Grupo Veículo Transporte
    m_nfe->notafiscal->NFe->obj->infNFe->transp->veicTransp->set_placa();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->veicTransp->set_UF();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->veicTransp->set_RNTC();
    */

}

void Nota::transp_reboque()
{
    /*
    //Grupo Reboque
    m_nfe->notafiscal->NFe->obj->infNFe->transp->reboque->obj->set_placa();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->reboque->obj->set_UF();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->reboque->obj->set_RNTC();
    */
}

void Nota::transp_vol()
{
    /*
    //Grupo Volumes
    //--***CONTAINER*** 0-5000
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_qVol();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_esp();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_marca();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_nVol();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_pesoL();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->set_pesoB();
        //sub grupo de volume - Grupo Lacres
        //--***CONTAINER*** 0-5000
        m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->lacres->obj->set_nLacre();
        m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->obj->lacres->add();
    m_nfe->notafiscal->NFe->obj->infNFe->transp->vol->add();
    */

}

void Nota::cobr()
{
    /*
    //Grupo Y. Dados da Cobrança

    //Grupo Fatura
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->fat->set_nFat();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->fat->set_vOrig();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->fat->set_vDesc();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->fat->set_vLiq();

    //Grupo Parcelas
    //--***CONTAINER*** 0-120
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->dup->obj->set_nDup();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->dup->obj->set_dVenc();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->dup->obj->set_vDup();
    m_nfe->notafiscal->NFe->obj->infNFe->cobr->dup->add();
    */

}

void Nota::pag()
{
    //Grupo YA. Informações de Pagamento

    //Grupo Detalhamento do Pagamento
    //--***CONTAINER*** 1-100
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->set_indPag(ConvNF::strToIndPag("0"));
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->set_tPag(ConvNF::strToTPag("01"));
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->set_vPag(2.00);
    /*
    //Grupo de Cartões
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->card->set_tpIntegra();
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->card->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->card->set_tBand();
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->obj->card->set_cAut();
    */
    m_nfe->notafiscal->NFe->obj->infNFe->pag->detPag->add();

    m_nfe->notafiscal->NFe->obj->infNFe->pag->set_vTroco(0.00);

}

void Nota::infIntermed()
{
    /*
    //Grupo YB. Informações do Intermediador da Transação
    m_nfe->notafiscal->NFe->obj->infNFe->infIntermed->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->infIntermed->set_idCadIntTran();
    */
}

void Nota::infAdic()
{
    //Grupo Z. Informações Adicionais da NF-e
    //m_nfe->notafiscal->NFe->obj->infNFe->infAdic->set_infAdFisco();
    //m_nfe->notafiscal->NFe->obj->infNFe->infAdic->set_infCpl();

    /*
    //obsCont - Grupo Campo de uso livre do contribuinte
    //--***CONTAINER*** 0-10
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsCont->obj->set_xCampo();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsCont->obj->set_xTexto();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsCont->add();
    */

    /*
    //obsFisco - Grupo Campo de uso livre do Fisco(dentro de infAdic)
    //--***CONTAINER*** 0-10
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsFisco->obj->set_xCampo();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsFisco->obj->set_xTexto();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->obsFisco->add(); //adicionando obsFisco
    */

    /*
    //procRef - Grupo Processo referenciado
    //--***CONTAINER*** 0-100
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->procRef->obj->set_nProc();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->procRef->obj->set_indProc();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->procRef->obj->set_tpAto();
    m_nfe->notafiscal->NFe->obj->infNFe->infAdic->procRef->add(); //adicionando procRef
    */
}

void Nota::exporta()
{
    /*
     //Grupo ZA. Informações de Comércio Exterior
     m_nfe->notafiscal->NFe->obj->infNFe->exporta->set_UFSaidaPais();
     m_nfe->notafiscal->NFe->obj->infNFe->exporta->set_xLocExporta();
     m_nfe->notafiscal->NFe->obj->infNFe->exporta->set_xLocDespacho();
     */
}

void Nota::compra()
{
    /*
    //Grupo ZB. Informações de Compras
    m_nfe->notafiscal->NFe->obj->infNFe->compra->set_xNEmp();
    m_nfe->notafiscal->NFe->obj->infNFe->compra->set_xPed();
    m_nfe->notafiscal->NFe->obj->infNFe->compra->set_xCont();
    */

}

void Nota::cana()
{
    /*
    //Grupo ZC. Informações do Registro de Aquisição de Cana
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_safra();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_ref();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_qTotMes();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_qTotAnt();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_qTotGer();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_vFor();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_vTotDed();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->set_vLiqFor();

    //forDia - Grupo Fornecimento diário de cana
    //--***CONTAINER*** 1-31
    m_nfe->notafiscal->NFe->obj->infNFe->cana->forDia->obj->set_dia();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->forDia->obj->set_qtde();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->forDia->add();

    //deduc - Grupo Deduções – Taxas e Contribuições
    //--***CONTAINER*** 0-10
    m_nfe->notafiscal->NFe->obj->infNFe->cana->deduc->obj->set_xDed();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->deduc->obj->set_vDed();
    m_nfe->notafiscal->NFe->obj->infNFe->cana->deduc->add();//adicionando deduc
    */

}

void Nota::infRespTec()
{
    /*
    //Grupo ZD. Informações do Responsável Técnico
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_CNPJ();
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_xContato();
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_email();
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_fone();
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_idCSRT();
    m_nfe->notafiscal->NFe->obj->infNFe->infRespTec->set_hashCSRT();
    */
}


void Nota::onReqStatusServico()
{
    configurar(ModeloDF::NFe);
    QString _status;

    m_nfe->eventos->statusServico->clear();

    if (m_nfe->eventos->statusServico->get_status())
    {
        _status = "Versao: " + m_nfe->eventos->statusServico->retorno->get_versao() + "\n";
        _status += "cStat: " + QString::number(m_nfe->eventos->statusServico->retorno->get_cStat()) + "\n";
        _status += "cUF: " + QString::number(m_nfe->eventos->statusServico->retorno->get_cUF()) + "\n";
        _status += "dhRecbto: " + CppUtil::dateTimeToStr(m_nfe->eventos->statusServico->retorno->get_dhRecbto(),
                                                         DtH::DateTimeBr) + "\n";
        _status += "tpAmb: " + ConvNF::tpAmbToStr(m_nfe->eventos->statusServico->retorno->get_tpAmb()) + "\n";
        _status += "tMed: " + QString::number(m_nfe->eventos->statusServico->retorno->get_tMed()) + "\n";
        _status += "verAplic: " + m_nfe->eventos->statusServico->retorno->get_verAplic() + "\n";
        _status += "xMotivo: " + m_nfe->eventos->statusServico->retorno->get_xMotivo() + "\n";
        _status += "xObs: " + m_nfe->eventos->statusServico->retorno->get_xObs() + "\n";
        _status += "dhRetorno: " + CppUtil::dateTimeToStr(m_nfe->eventos->statusServico->retorno->get_dhRetorno(),
                                                          DtH::DateTimeBr) + "\n\n";

        emit retStatusServico(_status);

    }
}

void Nota::onReqGerarEnviar(const int &numNota, const int &tipoDocumento)
{
    //reconfigura
    ModeloDF _mod;
    if (tipoDocumento > 0)
        _mod = ConvNF::strToModeloDF(QString::number(tipoDocumento));
    else
        _mod = ModeloDF::NFe;//padrão

    configurar(_mod);

    //limpando tudo
    this->m_nfe->notafiscal->clear();
    //inserindo dados
    nfe(numNota);

    m_nfe->notafiscal->gerar();
    m_nfe->notafiscal->assinar();
    m_nfe->notafiscal->validar();

    QString _retLote, _xml;
    int _lote = 3;
    if (m_nfe->notafiscal->enviar(_lote))
    {
        _retLote = "Lote \n";
        _retLote += "Versao: " + m_nfe->notafiscal->retorno->get_versao() +"\n";
        _retLote += "tpAmb: " + ConvNF::tpAmbToStr(m_nfe->notafiscal->retorno->get_tpAmb()) +"\n";
        _retLote += "verAplic: " + m_nfe->notafiscal->retorno->get_verAplic() +"\n";
        _retLote += "nRec: " + m_nfe->notafiscal->retorno->get_nRec() +"\n";
        _retLote += "cStat: " + QString::number(m_nfe->notafiscal->retorno->get_cStat()) +"\n";
        _retLote += "xMotivo: " + m_nfe->notafiscal->retorno->get_xMotivo() +"\n";
        _retLote += "cUF: " + QString::number(m_nfe->notafiscal->retorno->get_cUF()) +"\n";
        _retLote += "dhRecbto: " + CppUtil::dateTimeToStr(m_nfe->notafiscal->retorno->get_dhRecbto(), DtH::DateTimeBr) +"\n";
        _retLote += "cMsg: " + QString::number(m_nfe->notafiscal->retorno->get_cMsg()) +"\n";
        _retLote += "xMsg: " + m_nfe->notafiscal->retorno->get_xMsg() + "\n\n";

        if(m_nfe->notafiscal->retorno->protNFe->items->count() > 0)
        {
             _retLote += "Informacoes das Notas \n\n";

             for (int i = 0; i < m_nfe->notafiscal->retorno->protNFe->items->count(); ++i) {
                 _retLote += "Versao: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_versao() +"\n";
                 _retLote += "ID: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_Id() +"\n";
                 _retLote += "tpAmb: " + ConvNF::tpAmbToStr(m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_tpAmb()) +"\n";
                 _retLote += "verAplic: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_verAplic() +"\n";
                 _retLote += "chNFe: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_chNFe() +"\n";
                 _retLote += "dhRecbto: " +  m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_dhRecbto().toString("dd/MM/yyyy hh:mm:ss") +"\n";
                 _retLote += "nProt: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_nProt() +"\n";
                 _retLote += "digVal: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_digVal() +"\n";
                 _retLote += "cStat: " + QString::number(m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_cStat()) +"\n";
                 _retLote += "xMotivo: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_xMotivo() +"\n";
                 _retLote += "cMsg: " + QString::number(m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_cMsg()) +"\n";
                 _retLote += "xMsg: " + m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_xMsg() +"\n";
                 _retLote += QStringLiteral("-------------------------------") + "\n\n";

                 _xml +=  m_nfe->notafiscal->retorno->protNFe->items->value(i)->get_xml();

             }

             emit retXML(_xml);

        }
        emit retLote(_retLote);
    }
}

void Nota::onWSChange(const WebServicesNF &webServicesNF)
{
    QString _ws;

    switch (webServicesNF)
    {
        case WebServicesNF::NFeAutorizacao : _ws = QStringLiteral("Enviando NFe/NFCe ...") + "\n";
            break;
        case WebServicesNF::NFeConsultaCadastro : _ws = QStringLiteral("Consultando Cadastro ...") + "\n";
            break;
        case WebServicesNF::NFeConsultaProtocolo : _ws = QStringLiteral("Consultando Protocolo ...") + "\n";
            break;
        case WebServicesNF::NFeInutilizacao : _ws = QStringLiteral("Enviando Inutilização ...") + "\n";
            break;
        case WebServicesNF::NFeRecepcaoEvento : _ws = QStringLiteral("Recebendo Eventos ...") + "\n";
            break;
        case WebServicesNF::NFeRetAutorizacao : _ws = QStringLiteral("Recebendo Retorno da NFe/NFCe ...") + "\n";
            break;
        case WebServicesNF::NFeStatusServico : _ws = QStringLiteral("Recebendo Status do Serviço ...") + "\n";
            break;
        case WebServicesNF::None : _ws = QStringLiteral("Finalizando Conexão...") + "\n";
            break;
        default: _ws = QStringLiteral("...") + "\n";
            break;
    }


    emit  retWSChange(_ws);


}

const CppNFe *Nota::getCppNFe()
{
    return this->m_nfe;
}
