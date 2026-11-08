/*
Copyright 2022 João Elson
jetmorju@gmail.com

Web-site: https://cppbrasil.com.br

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "xmlwrite.h"
#include "retornonfe.h"
#include <QStringList>

XmlWrite::XmlWrite(const InfNFe *infNFe, const ConfigNFe *confgNFe): m_infNFe(infNFe),
    m_confgNFe(confgNFe), m_xmlw(new QXmlStreamWriter(&this->m_strXML))
{
}

XmlWrite::~XmlWrite()
{
    delete m_xmlw;
}

bool XmlWrite::gerarXML(QByteArray *output)
{
    const auto &agropecuario = *m_infNFe->agropecuario;
    if (agropecuario.defensivo->count() > 20)
    {
        qWarning() << "O grupo agropecuario aceita até 20 defensivos.";
        return false;
    }
    if (!agropecuario.defensivo->isEmpty() && !agropecuario.guiaTransito->isEmpty())
    {
        qWarning() << "Informe defensivos ou guia de trânsito no grupo agropecuario.";
        return false;
    }
    if (!validarRTC())
        return false;

    try{
        gerarNFE();
        output->append(this->m_strXML.data());
    }catch( std::exception const& e) {
        qWarning() << "Erro: " << e.what();
        return false;
    }
    return true;
}

QByteArray XmlWrite::get_infNFeSupl_OFFLINE(const QString &chave, const VersaoQrCodeNF &versaoQrcodeNF,
  const TpAmb &tpAmb, const QDateTime &emissao, const double &vNF, const QString &digestValue,
  const QString &idCSC, const QString &CSC, const int &codigoUF, const bool &element_infNFeSupl)
{
    // Este método recebe os dados do CSC e do DigestValue usados na versão 2.
    if (versaoQrcodeNF == VersaoQrCodeNF::v300)
        return {};

    //infNFeSupl - ZX01
    QString _chave, _versaoQr, _ambiente, _diaEmiss, _valornf, _digest, _idcsc, _csc;
    QString _qrcode, _inputHash, _xml;
    QByteArray _resHash;
    _chave = chave;
    _versaoQr = ConvNF::versaoQrCodeNFToStr(versaoQrcodeNF).left(1);//apenas o primeiro numero da versão
    _ambiente = ConvNF::tpAmbToStr(tpAmb);
    _diaEmiss =  emissao.toString("dd");
    _valornf = CppUtility::doubleToStrDecimal(vNF, 2);
    _digest.append(digestValue.toLocal8Bit().toHex());

    if (!idCSC.isEmpty())
      _idcsc = QString::number(idCSC.toInt());

    _csc = CSC;

    _qrcode = _chave + QStringLiteral("|") + _versaoQr + QStringLiteral("|") +
              _ambiente + QStringLiteral("|") + _diaEmiss + QStringLiteral("|") +
              _valornf + QStringLiteral("|") + _digest  + QStringLiteral("|") +
              _idcsc ;

    _inputHash = _qrcode + _csc;
    //calc sha1
    HashType _sha1 = HashType::sha1;
    if (!CppOpenSSL::digest_s(_sha1, _inputHash.toLocal8Bit(), false, _resHash))
      qWarning() << "Erro ao gerar hash da tag infNFeSupl";

    _qrcode += QStringLiteral("|") + QString(_resHash.toHex().toUpper());

    //url
    QString _urlqrcode = UrlNFe::get_qrCode(ConvNF::intToUF(codigoUF), tpAmb, versaoQrcodeNF);
    QString _urlchave = UrlNFe::get_urlChave(ConvNF::intToUF(codigoUF), tpAmb, versaoQrcodeNF);

    _qrcode = _urlqrcode + QStringLiteral("?p=") + _qrcode;

    _xml += QStringLiteral("<a>");
      _xml += (element_infNFeSupl ? QStringLiteral("<infNFeSupl>") : "");
      _xml += QStringLiteral("<qrCode>"); _xml += _qrcode;   _xml += QStringLiteral("</qrCode>");
      _xml += QStringLiteral("<urlChave>"); _xml += _urlchave;   _xml += QStringLiteral("</urlChave>");
      _xml += (element_infNFeSupl ? QStringLiteral("</infNFeSupl>") : "");
    _xml += QStringLiteral("</a>");

    return _xml.toLocal8Bit();
}

VersaoQrCodeNF XmlWrite::versaoQrCode(const InfNFe &infNFe, const ConfigNFe &config)
{
    if (infNFe.ide->usaDanfeSimplificadoTipo2())
        return VersaoQrCodeNF::v300;
    if (!infNFe.emite->get_CPF().isEmpty() && ConvNF::intToUF(infNFe.ide->get_cUF()) != UF::PR)
        return VersaoQrCodeNF::v300;
    return config.get_VersaoQrCodeNF();
}

QByteArray XmlWrite::get_infNFeSupl_OFFLINE_V3(const QString &chave, const InfNFe &infNFe,
                                            CppCrypto &crypto, QString &erro)
{
    const auto &ide = *infNFe.ide;
    const auto &dest = *infNFe.dest;
    QString tipoDocumento;
    QString documento;
    if (!dest.get_CNPJ().isEmpty())
    {
        tipoDocumento = QStringLiteral("1");
        documento = dest.get_CNPJ();
    }
    else if (!dest.get_CPF().isEmpty())
    {
        tipoDocumento = QStringLiteral("2");
        documento = dest.get_CPF();
    }
    else if (!dest.get_idEstrangeiro().isEmpty())
        tipoDocumento = QStringLiteral("3");

    const QStringList campos{chave, QStringLiteral("3"), ConvNF::tpAmbToStr(ide.get_tpAmb()),
                             ide.get_dhEmi().toString(QStringLiteral("dd")),
                             CppUtility::doubleToStrDecimal(infNFe.total->ICMSTot->get_vNF(), 2),
                             tipoDocumento, documento};
    const QString parametros = campos.join(QLatin1Char('|'));
    QByteArray assinatura;
    if (!crypto.signature(HashType::sha1, parametros.toUtf8(), true, assinatura) || assinatura.isEmpty())
    {
        erro = QStringLiteral("Erro ao assinar o QR Code da nota em contingência off-line.");
        if (!crypto.get_error().isEmpty())
            erro += QLatin1Char(' ') + crypto.get_error();
        return {};
    }

    const auto uf = ConvNF::intToUF(ide.get_cUF());
    const auto versao = VersaoQrCodeNF::v300;
    const QString url = UrlNFe::get_qrCode(uf, ide.get_tpAmb(), versao) + QStringLiteral("?p=") +
                        parametros + QLatin1Char('|') + QString::fromLatin1(assinatura);
    QByteArray xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartElement("a");
    writer.writeStartElement("infNFeSupl");
    writer.writeTextElement("qrCode", url);
    writer.writeTextElement("urlChave", UrlNFe::get_urlChave(uf, ide.get_tpAmb(), versao));
    writer.writeEndElement();
    writer.writeEndElement();
    return xml;
}

QByteArray XmlWrite::get_protNFe(const QString &versao, const QString &Id,
                                 const TpAmb &tpAmb, const QString &verAplic,
                                 const QString &chNFe, const QDateTime &dhRecbto,
                                 const QString &nProt, const QString &digVal,
                                 const int &cStat, const QString &xMotivo,
                                 const int &cMsg, const QString &xMsg)
{
    ProtNFe protocolo;
    protocolo.set_versao(versao);
    protocolo.set_Id(Id);
    protocolo.set_tpAmb(tpAmb);
    protocolo.set_verAplic(verAplic);
    protocolo.set_chNFe(chNFe);
    protocolo.set_dhRecbto(dhRecbto);
    protocolo.set_nProt(nProt);
    protocolo.set_digVal(digVal);
    protocolo.set_cStat(cStat);
    protocolo.set_xMotivo(xMotivo);
    protocolo.set_cMsg(cMsg);
    protocolo.set_xMsg(xMsg);
    return get_protNFe(protocolo);
}

QByteArray XmlWrite::get_protNFe(const ProtNFe &protocolo)
{
    QByteArray xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartElement("a");
    writer.writeStartElement("protNFe");
    writer.writeAttribute("versao", protocolo.get_versao());
    writer.writeStartElement("infProt");
    if (!protocolo.get_Id().isEmpty())
        writer.writeAttribute("Id", protocolo.get_Id());
    writer.writeTextElement("tpAmb", ConvNF::tpAmbToStr(protocolo.get_tpAmb()));
    writer.writeTextElement("verAplic", protocolo.get_verAplic());
    writer.writeTextElement("chNFe", protocolo.get_chNFe());
    writer.writeTextElement("dhRecbto", CppUtility::dateTimeToStr(protocolo.get_dhRecbto(), DtH::DateTimeUTC));
    if (!protocolo.get_nProt().isEmpty())
        writer.writeTextElement("nProt", protocolo.get_nProt());
    if (!protocolo.get_digVal().isEmpty())
        writer.writeTextElement("digVal", protocolo.get_digVal());
    writer.writeTextElement("cStat", QString::number(protocolo.get_cStat()));
    writer.writeTextElement("xMotivo", protocolo.get_xMotivo());
    for (const auto &mensagem : protocolo.get_mensagens())
    {
        const QString texto = mensagem.get_texto();
        if (texto.isEmpty())
            continue;
        const QString codigo = mensagem.get_codigo();
        if (!codigo.isEmpty())
            writer.writeTextElement("cMsg", codigo);
        writer.writeTextElement("xMsg", texto);
    }
    writer.writeEndElement(); // infProt
    writer.writeEndElement(); // protNFe
    writer.writeEndElement(); // a
    return xml;
}

QByteArray XmlWrite::get_nfeProc(const QString &versao)
{
    QByteArray _strXML;
    std::unique_ptr<QXmlStreamWriter> _xmlw(new QXmlStreamWriter(&_strXML));
    _xmlw->writeStartElement("nfeProc");
    _xmlw->writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    _xmlw->writeAttribute("versao", versao);
    _xmlw->writeEndElement(); //fechamento infProt

    return _strXML;
}

void XmlWrite::get_infNFe()
{   //infNFe - A01
    this->m_xmlw->writeStartElement("infNFe");//abertura grupo infNFe
    this->m_xmlw->writeAttribute("versao", this->m_infNFe->get_versao());
    this->m_xmlw->writeAttribute("Id", this->m_infNFe->get_Id());
    get_ide();
    get_emit();
    get_avulsa();
    get_dest();
    get_retirada();
    get_entrega();
    get_autXML();
    get_det();
    get_total();
    get_transp();
    get_cobr();
    get_pag();
    get_infIntermed();
    get_infAdic();
    get_exporta();
    get_compra();
    get_cana();
    get_infRespTec();
    get_agropecuario();
    this->m_xmlw->writeEndElement();//fechamento grupo infNFe
}

void XmlWrite::get_ide()
{   //ide - B01
    Ide *_ide = this->m_infNFe->ide.get();
    this->m_xmlw->writeStartElement("ide"); //abertura grupo ide
    this->m_xmlw->writeTextElement("cUF", QString::number(_ide->get_cUF()));
    this->m_xmlw->writeTextElement("cNF", CppUtility::insZeroLeft(QString::number(_ide->get_cNF()), 8));
    this->m_xmlw->writeTextElement("natOp", _ide->get_natOp());
    this->m_xmlw->writeTextElement("mod", ConvNF::modeloDFToStr(_ide->get_mod()));
    this->m_xmlw->writeTextElement("serie", QString::number(_ide->get_serie()));
    this->m_xmlw->writeTextElement("nNF", QString::number(_ide->get_nNF()));
    this->m_xmlw->writeTextElement("dhEmi", CppUtility::dateTimeToStr(_ide->get_dhEmi(),
                                                                   DtH::DateTimeUTC));
    if (_ide->get_mod() == ModeloDF::NFe && _ide->get_dhSaiEnt().isValid())
        this->m_xmlw->writeTextElement("dhSaiEnt", CppUtility::dateTimeToStr(_ide->get_dhSaiEnt(),
                                                                          DtH::DateTimeUTC));
    //B10a - não informar para a NFC-e
    if (_ide->get_mod() == ModeloDF::NFe && _ide->get_dPrevEntrega().isValid())
        this->m_xmlw->writeTextElement("dPrevEntrega", _ide->get_dPrevEntrega().toString(Qt::ISODate));
    this->m_xmlw->writeTextElement("tpNF", ConvNF::tpNFToStr(_ide->get_tpNF()));
    this->m_xmlw->writeTextElement("idDest", ConvNF::idDestToStr(_ide->get_idDest()));
    this->m_xmlw->writeTextElement("cMunFG", QString::number(_ide->get_cMunFG()));
    if (_ide->get_cMunFGIBS() > 0) //B12a
        this->m_xmlw->writeTextElement("cMunFGIBS", QString::number(_ide->get_cMunFGIBS()));
    this->m_xmlw->writeTextElement("tpImp", ConvNF::tpImpToStr(_ide->get_tpImp()));
    this->m_xmlw->writeTextElement("tpEmis", ConvNF::tpEmisToStr(_ide->get_tpEmis()));
    this->m_xmlw->writeTextElement("cDV", _ide->get_cDV());
    this->m_xmlw->writeTextElement("tpAmb", ConvNF::tpAmbToStr(_ide->get_tpAmb()));
    this->m_xmlw->writeTextElement("finNFe", ConvNF::finNFeToStr(_ide->get_finNFe()));
    if (_ide->get_tpNFDebito() != TpNFDebito::None) //B25.1
        this->m_xmlw->writeTextElement("tpNFDebito", ConvNF::tpNFDebitoToStr(_ide->get_tpNFDebito()));
    if (_ide->get_tpNFCredito() != TpNFCredito::None) //B25.2
        this->m_xmlw->writeTextElement("tpNFCredito", ConvNF::tpNFCreditoToStr(_ide->get_tpNFCredito()));
    this->m_xmlw->writeTextElement("indFinal", ConvNF::indFinalToStr(_ide->get_indFinal()));
    this->m_xmlw->writeTextElement("indPres", ConvNF::indPresToStr(_ide->get_indPres()));
    if (_ide->get_indIntermed() != IndIntermed::None)
        this->m_xmlw->writeTextElement("indIntermed", ConvNF::indIntermedToStr(_ide->get_indIntermed()));
    if (!_ide->get_cIndOp().isEmpty()) //B25d
        this->m_xmlw->writeTextElement("cIndOp", _ide->get_cIndOp());
    this->m_xmlw->writeTextElement("procEmi", ConvNF::procEmiToStr(_ide->get_procEmi()));
    this->m_xmlw->writeTextElement("verProc", _ide->get_verProc());

    if (_ide->get_dhCont().isValid() || !_ide->get_xJust().isEmpty())
    {
        this->m_xmlw->writeTextElement("dhCont", CppUtility::dateTimeToStr(_ide->get_dhCont(),
                                                                        DtH::DateTimeUTC));
        this->m_xmlw->writeTextElement("xJust", _ide->get_xJust());
    }
    //grupo Documento fiscal referenciado
    get_NFref();

    //gCompraGov - BB01
    const GCompraGov *_compraGov = _ide->gCompraGov.get();
    if (!_compraGov->isEmpty())
    {
        this->m_xmlw->writeStartElement("gCompraGov");
        this->m_xmlw->writeTextElement("tpEnteGov", ConvNF::tpEnteGovToStr(_compraGov->get_tpEnteGov()));
        this->m_xmlw->writeTextElement("pRedutor", CppUtility::doubleToStrDecimal(_compraGov->get_pRedutor(), 4));
        this->m_xmlw->writeTextElement("tpOperGov", ConvNF::tpOperGovToStr(_compraGov->get_tpOperGov()));
        for (const QString &chave : _compraGov->get_refDFeAnt())
            this->m_xmlw->writeTextElement("refDFeAnt", chave);
        this->m_xmlw->writeEndElement();
    }
    //gPagAntecipado - BC01
    if (!_ide->gPagAntecipado->isEmpty())
    {
        this->m_xmlw->writeStartElement("gPagAntecipado");
        for (const QString &chave : _ide->gPagAntecipado->get_refNFe())
            this->m_xmlw->writeTextElement("refNFe", chave);
        this->m_xmlw->writeEndElement();
    }
    this->m_xmlw->writeEndElement();//fechamento grupo ide
}

void XmlWrite::get_NFref()
{   //NFref - BA01 - container 0-500
    if (this->m_infNFe->ide->NFref->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->ide->NFref->count(); ++i)
        {
            NFRef *_nfref = this->m_infNFe->ide->NFref->value(i);
            RefNF *_refNF = _nfref->refNF.get();
            RefNFP *_refNFP = _nfref->refNFP.get();
            RefECF *_refECF = _nfref->refECF.get();
            this->m_xmlw->writeStartElement("NFref");//abertura grupo NFref
            if (!_nfref->get_refNFe().isEmpty())
                this->m_xmlw->writeTextElement("refNFe", _nfref->get_refNFe());
            if (!_nfref->get_refNFeSig().isEmpty())
                this->m_xmlw->writeTextElement("refNFeSig", _nfref->get_refNFeSig());

            //grupo refNF - nota 1/1A
            //se exitir uma numeração
            if (_refNF->get_nNF() > 0)
            {
                this->m_xmlw->writeStartElement("refNF");//abertura grupo refNF
                this->m_xmlw->writeTextElement("cUF", QString::number(_refNF->get_cUF()));
                this->m_xmlw->writeTextElement("AAMM", _refNF->get_AAMM());
                this->m_xmlw->writeTextElement("CNPJ", _refNF->get_CNPJ());
                this->m_xmlw->writeTextElement("mod", _refNF->get_mod());
                this->m_xmlw->writeTextElement("serie", QString::number(_refNF->get_serie()));
                this->m_xmlw->writeTextElement("nNF", QString::number(_refNF->get_nNF()));
                this->m_xmlw->writeEndElement();//fechamento grupo refNF
            }
            //grupo refNFP - Informações da NF de produtor rural referenciada
            //se exitir uma numeração
            if (_refNFP->get_nNF() > 0)
            {
                this->m_xmlw->writeStartElement("refNFP");//abertura grupo refNFP
                this->m_xmlw->writeTextElement("cUF", QString::number(_refNFP->get_cUF()));
                this->m_xmlw->writeTextElement("AAMM", _refNFP->get_AAMM());
                this->m_xmlw->writeTextElement("CNPJ", _refNFP->get_CNPJ());
                this->m_xmlw->writeTextElement("CPF", _refNFP->get_CPF());
                this->m_xmlw->writeTextElement("IE", _refNFP->get_IE());
                this->m_xmlw->writeTextElement("mod", _refNFP->get_mod());
                this->m_xmlw->writeTextElement("serie", QString::number(_refNFP->get_serie()));
                this->m_xmlw->writeTextElement("nNF", QString::number(_refNFP->get_nNF()));
                this->m_xmlw->writeEndElement();//fechamento grupo refNFP
            }
            //CTe
            if (!_nfref->get_refCTe().isEmpty())
                this->m_xmlw->writeTextElement("refCTe", _nfref->get_refCTe());
            //grupo refECF - Informações do Cupom Fiscal referenciado
            if (!_refECF->get_mod().isEmpty())
            {
                this->m_xmlw->writeStartElement("refECF");//abertura grupo refECF
                this->m_xmlw->writeTextElement("mod", _refECF->get_mod());
                this->m_xmlw->writeTextElement("nECF", _refECF->get_nECF());
                this->m_xmlw->writeTextElement("nCOO", _refECF->get_nCOO());
                this->m_xmlw->writeEndElement();//fechamento grupo refECF
            }
            this->m_xmlw->writeEndElement();//fechamento grupo NFref
        }
    }
}

void XmlWrite::get_emit()
{   //emit - C01
    Emit *_emit = this->m_infNFe->emite.get();
    EnderEmit *_enderEmit = _emit->enderEmit.get();
    this->m_xmlw->writeStartElement("emit");//abertura grupo emit
    if (!_emit->get_CNPJ().isEmpty())
        this->m_xmlw->writeTextElement("CNPJ", _emit->get_CNPJ());
    if (!_emit->get_CPF().isEmpty())
        this->m_xmlw->writeTextElement("CPF", _emit->get_CPF());
    this->m_xmlw->writeTextElement("xNome", _emit->get_xNome());
    if (!_emit->get_xFant().isEmpty())
        this->m_xmlw->writeTextElement("xFant", _emit->get_xFant());
    //enderEmit - C05
    this->m_xmlw->writeStartElement("enderEmit"); //abertura grupo enderEmit
    this->m_xmlw->writeTextElement("xLgr", _enderEmit->get_xLgr());
    this->m_xmlw->writeTextElement("nro", QString::number(_enderEmit->get_nro()));
    if (!_enderEmit->get_xCpl().isEmpty())
        this->m_xmlw->writeTextElement("xCpl", _enderEmit->get_xCpl());
    this->m_xmlw->writeTextElement("xBairro", _enderEmit->get_xBairro());
    this->m_xmlw->writeTextElement("cMun", QString::number(_enderEmit->get_cMun()));
    this->m_xmlw->writeTextElement("xMun", _enderEmit->get_xMun());
    this->m_xmlw->writeTextElement("UF", _enderEmit->get_UF());
    this->m_xmlw->writeTextElement("CEP", QString::number(_enderEmit->get_CEP()));
    if (_enderEmit->get_cPais() > 0)
        this->m_xmlw->writeTextElement("cPais", QString::number(_enderEmit->get_cPais()));
    if (!_enderEmit->get_xPais().isEmpty())
        this->m_xmlw->writeTextElement("xPais", _enderEmit->get_xPais());
    if (!_enderEmit->get_fone().isEmpty())
        this->m_xmlw->writeTextElement("fone", _enderEmit->get_fone());
    this->m_xmlw->writeEndElement();//fechamento grupo enderEmit

    this->m_xmlw->writeTextElement("IE", _emit->get_IE());
    if (!_emit->get_IEST().isEmpty())
        this->m_xmlw->writeTextElement("IEST", _emit->get_IEST());

    if (!_emit->get_IM().isEmpty() ||
        !_emit->get_CNAE().isEmpty())
    {
        if (!_emit->get_IM().isEmpty())
            this->m_xmlw->writeTextElement("IM", _emit->get_IM());
        if (!_emit->get_CNAE().isEmpty())
            this->m_xmlw->writeTextElement("CNAE", _emit->get_CNAE());
    }
    this->m_xmlw->writeTextElement("CRT", ConvNF::crtToStr(_emit->get_CRT()));
    if (!_emit->get_ISUFEmit().isEmpty()) //C22
        this->m_xmlw->writeTextElement("ISUFEmit", _emit->get_ISUFEmit());

    this->m_xmlw->writeEndElement();//fechamento grupo emit
}

void XmlWrite::get_avulsa()
{   //avulsa - D01
    Avulsa *_avulsa = this->m_infNFe->avulsa.get();
    if (!_avulsa->get_CNPJ().isEmpty() &&
        !_avulsa->get_xOrgao().isEmpty())
    {
        this->m_xmlw->writeStartElement("avulsa");//abertura grupo avulsa
        this->m_xmlw->writeTextElement("CNPJ", _avulsa->get_CNPJ());
        this->m_xmlw->writeTextElement("xOrgao", _avulsa->get_xOrgao());
        this->m_xmlw->writeTextElement("matr", _avulsa->get_matr());
        this->m_xmlw->writeTextElement("xAgente", _avulsa->get_xAgente());
        if (!_avulsa->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", _avulsa->get_fone());
        this->m_xmlw->writeTextElement("UF", _avulsa->get_UF());
        if (!_avulsa->get_nDAR().isEmpty())
            this->m_xmlw->writeTextElement("nDAR", _avulsa->get_nDAR());
        if (_avulsa->get_dEmi().isValid())
            this->m_xmlw->writeTextElement("dEmi", CppUtility::dateTimeToStr(_avulsa->get_dEmi(),
                                                                          DtH::Date));
        if (_avulsa->get_vDAR() > 0)
            this->m_xmlw->writeTextElement("vDAR", CppUtility::doubleToStrDecimal(_avulsa->get_vDAR(), 13));
        this->m_xmlw->writeTextElement("repEmi", _avulsa->get_repEmi());
        if (!_avulsa->get_dPag().isValid())
            this->m_xmlw->writeTextElement("dPag", CppUtility::dateTimeToStr(_avulsa->get_dPag(),
                                                                             DtH::Date));
        this->m_xmlw->writeEndElement();//fechamento grupo avulsa
    }
}

void XmlWrite::get_dest()
{   //dest - E01
    Dest *_dest = this->m_infNFe->dest.get();
    EnderDest *_enderDest = _dest->enderDest.get();
    const auto &ide = *this->m_infNFe->ide;
    //Quando 65 só deverá existir caso algum dos campos abaixo não esteja vazio
    if (this->m_infNFe->ide->get_mod() == ModeloDF::NFCe)
    {
        if (!_dest->get_CNPJ().isEmpty() ||
            !_dest->get_CPF().isEmpty()  ||
            !_dest->get_idEstrangeiro().isEmpty() ||
            !_dest->get_xNome().isEmpty())
        {
            this->m_xmlw->writeStartElement("dest");//abertura grupo dest
            //cnpj ou cpf não os dois ou nenhum para 65
            if (!_dest->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", _dest->get_CNPJ());
            else if (!_dest->get_CPF().isEmpty())
                this->m_xmlw->writeTextElement("CPF", _dest->get_CPF());
            else if (!_dest->get_idEstrangeiro().isEmpty())
                this->m_xmlw->writeTextElement("idEstrangeiro", _dest->get_idEstrangeiro());
            //xNome opcional para a NFC-e->
            if (!_dest->get_xNome().isEmpty())
            {
                if (this->m_infNFe->ide->get_tpAmb() == TpAmb::Homologacao)
                   this->m_xmlw->writeTextElement("xNome", "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
                else
                   this->m_xmlw->writeTextElement("xNome", _dest->get_xNome());
            }
            //No caso de NFC-e informar indIEDest=9 e não informar a tag IE;
            this->m_xmlw->writeTextElement("indIEDest", ConvNF::indIEDestToStr(IndIEDest::NaoContribuinte));

            this->m_xmlw->writeEndElement();//fechamento grupo dest
        }
    }
    if (ide.get_mod() == ModeloDF::NFe)
    {
        const bool tipo2 = ide.usaDanfeSimplificadoTipo2();
        if (tipo2 && _dest->get_CNPJ().isEmpty() && _dest->get_CPF().isEmpty() &&
            _dest->get_idEstrangeiro().isEmpty())
            return;

        this->m_xmlw->writeStartElement("dest");//abertura grupo dest
        if (!_dest->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", _dest->get_CNPJ());
        else if (!_dest->get_CPF().isEmpty())
            this->m_xmlw->writeTextElement("CPF", _dest->get_CPF());
        else if (!_dest->get_idEstrangeiro().isNull() || this->m_infNFe->ide->get_idDest() == IdDest::Exterior)
            this->m_xmlw->writeTextElement("idEstrangeiro", _dest->get_idEstrangeiro());

        if (!tipo2 || !_dest->get_xNome().isEmpty())
        {
            if (ide.get_tpAmb() == TpAmb::Homologacao)
                this->m_xmlw->writeTextElement("xNome", "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
            else
                this->m_xmlw->writeTextElement("xNome", _dest->get_xNome());
        }

        //enderDest - E05
        if (!tipo2 || !_enderDest->get_xLgr().isEmpty())
        {
            this->m_xmlw->writeStartElement("enderDest");//abertura grupo enderDest
            this->m_xmlw->writeTextElement("xLgr", _enderDest->get_xLgr());
            this->m_xmlw->writeTextElement("nro", QString::number(_enderDest->get_nro()));
            if (!_enderDest->get_xCpl().isEmpty())
                this->m_xmlw->writeTextElement("xCpl", _enderDest->get_xCpl());
            this->m_xmlw->writeTextElement("xBairro", _enderDest->get_xBairro());
            this->m_xmlw->writeTextElement("cMun", QString::number(_enderDest->get_cMun()));
            this->m_xmlw->writeTextElement("xMun", _enderDest->get_xMun());
            this->m_xmlw->writeTextElement("UF", _enderDest->get_UF());
            if (_enderDest->get_CEP() > 0)
                this->m_xmlw->writeTextElement("CEP", QString::number(_enderDest->get_CEP()));
            if (_enderDest->get_cPais() > 0)
                this->m_xmlw->writeTextElement("cPais", QString::number(_enderDest->get_cPais()));
            if (!_enderDest->get_xPais().isEmpty())
                this->m_xmlw->writeTextElement("xPais", _enderDest->get_xPais());
            if (!_enderDest->get_fone().isEmpty())
                this->m_xmlw->writeTextElement("fone", _enderDest->get_fone());
            this->m_xmlw->writeEndElement();//fechamento grupo enderDest
        }

        const auto indIEDest = tipo2 && _dest->get_indIEDest() == IndIEDest::None
            ? IndIEDest::NaoContribuinte : _dest->get_indIEDest();
        this->m_xmlw->writeTextElement("indIEDest", ConvNF::indIEDestToStr(indIEDest));
        if (!_dest->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", _dest->get_IE());
        if (!_dest->get_ISUF().isEmpty())
            this->m_xmlw->writeTextElement("ISUF", _dest->get_ISUF());
        if (!_dest->get_IM().isEmpty())
            this->m_xmlw->writeTextElement("IM", _dest->get_IM());
        if (!_dest->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", _dest->get_email());
        this->m_xmlw->writeEndElement();//fechamento grupo dest
    }
}

void XmlWrite::get_retirada()
{   //retirada - F01
    Retirada *_retirada = this->m_infNFe->retirada.get();
    if (!_retirada->get_CNPJ().isEmpty() ||
        !_retirada->get_CPF().isEmpty()  ||
        !_retirada->get_xNome().isEmpty() ||
        !_retirada->get_xLgr().isEmpty())
    {
        this->m_xmlw->writeStartElement("retirada");//abertura grupo retirada
        //ou cnpj ou cpf não os dois
        if (!_retirada->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", _retirada->get_CNPJ());
        else if (!_retirada->get_CPF().isEmpty())
            this->m_xmlw->writeTextElement("CPF", _retirada->get_CPF());
        else
            this->m_xmlw->writeTextElement("CNPJ", QString());

        if (!_retirada->get_xNome().isEmpty())
            this->m_xmlw->writeTextElement("xNome", _retirada->get_xNome());
        this->m_xmlw->writeTextElement("xLgr", _retirada->get_xLgr());
        this->m_xmlw->writeTextElement("nro", _retirada->get_nroTexto());
        if (!_retirada->get_xCpl().isEmpty())
            this->m_xmlw->writeTextElement("xCpl", _retirada->get_xCpl());
        this->m_xmlw->writeTextElement("xBairro", _retirada->get_xBairro());
        this->m_xmlw->writeTextElement("cMun", QString::number(_retirada->get_cMun()));
        this->m_xmlw->writeTextElement("xMun", _retirada->get_xMun());
        this->m_xmlw->writeTextElement("UF", _retirada->get_UF());
        if (_retirada->get_CEP() > 0)
            this->m_xmlw->writeTextElement("CEP", QStringLiteral("%1").arg(_retirada->get_CEP(), 8, 10, QLatin1Char('0')));
        if (_retirada->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(_retirada->get_cPais()));
        if (!_retirada->get_xPais().isEmpty())
            this->m_xmlw->writeTextElement("xPais", _retirada->get_xPais());
        if (!_retirada->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", _retirada->get_fone());
        if (!_retirada->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", _retirada->get_email());
        if (!_retirada->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", _retirada->get_IE());
        this->m_xmlw->writeEndElement();//fechamento grupo retirada
    }
}

void XmlWrite::get_entrega()
{   //entrega - G01
    Entrega *_entrega = this->m_infNFe->entrega.get();
    if (!_entrega->get_CNPJ().isEmpty() ||
        !_entrega->get_CPF().isEmpty()  ||
        !_entrega->get_xNome().isEmpty() ||
        !_entrega->get_xLgr().isEmpty())
    {
        this->m_xmlw->writeStartElement("entrega");//abertura grupo entrega
        //ou cnpj ou cpf não os dois
        if (!_entrega->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", _entrega->get_CNPJ());
        else if (!_entrega->get_CPF().isEmpty())
            this->m_xmlw->writeTextElement("CPF", _entrega->get_CPF());
        else
            this->m_xmlw->writeTextElement("CNPJ", QString());

        if (!_entrega->get_xNome().isEmpty())
            this->m_xmlw->writeTextElement("xNome", _entrega->get_xNome());
        this->m_xmlw->writeTextElement("xLgr", _entrega->get_xLgr());
        this->m_xmlw->writeTextElement("nro", _entrega->get_nroTexto());
        if (!_entrega->get_xCpl().isEmpty())
            this->m_xmlw->writeTextElement("xCpl", _entrega->get_xCpl());
        this->m_xmlw->writeTextElement("xBairro", _entrega->get_xBairro());
        this->m_xmlw->writeTextElement("cMun", QString::number(_entrega->get_cMun()));
        this->m_xmlw->writeTextElement("xMun", _entrega->get_xMun());
        this->m_xmlw->writeTextElement("UF", _entrega->get_UF());
        if (_entrega->get_CEP() > 0)
            this->m_xmlw->writeTextElement("CEP", QStringLiteral("%1").arg(_entrega->get_CEP(), 8, 10, QLatin1Char('0')));
        if (_entrega->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(_entrega->get_cPais()));
        if (!_entrega->get_xPais().isEmpty())
            this->m_xmlw->writeTextElement("xPais", _entrega->get_xPais());
        if (!_entrega->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", _entrega->get_fone());
        if (!_entrega->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", _entrega->get_email());
        if (!_entrega->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", _entrega->get_IE());
        this->m_xmlw->writeEndElement();//fechamento grupo entrega
    }
}

void XmlWrite::get_autXML()
{   //autXML - GA01 - container 0-10
    if (this->m_infNFe->autXML->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->autXML->count(); ++i)
        {
            this->m_xmlw->writeStartElement("autXML"); //abertura grupo autXML
            if (!this->m_infNFe->autXML->value(i)->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ",this->m_infNFe->autXML->value(i)->get_CNPJ());
            else
                this->m_xmlw->writeTextElement("CPF",this->m_infNFe->autXML->value(i)->get_CPF());
            this->m_xmlw->writeEndElement();//fechamento grupo autXML
        }
    }
}

void XmlWrite::get_det()
{   //det - H01 - container 1-990
    for(int i = 0; i < this->m_infNFe->det->count(); ++i)
    {
        this->m_xmlw->writeStartElement("det"); //abertura grupo det
        get_prod(i);
        get_imposto(i);
        get_impostoDevol(i);
        if (!this->m_infNFe->det->value(i)->get_infAdProd().isEmpty())
            this->m_xmlw->writeTextElement("infAdProd", this->m_infNFe->det->value(i)->get_infAdProd());

        get_obsItem(i);

        const Det *_det = this->m_infNFe->det->value(i);
        if (_det->get_vItem() > 0) //VB01
            this->m_xmlw->writeTextElement("vItem", CppUtility::doubleToStrDecimal(_det->get_vItem(), 2));
        if (!_det->DFeReferenciado->get_chaveAcesso().isEmpty()) //VC01
        {
            this->m_xmlw->writeStartElement("DFeReferenciado");
            this->m_xmlw->writeTextElement("chaveAcesso", _det->DFeReferenciado->get_chaveAcesso());
            if (_det->DFeReferenciado->get_nItem() > 0)
                this->m_xmlw->writeTextElement("nItem", QString::number(_det->DFeReferenciado->get_nItem()));
            this->m_xmlw->writeEndElement();
        }
        this->m_xmlw->writeEndElement();//fechamento grupo det
    }//fim for det
}

void XmlWrite::get_prod(const int &i)
{
    Prod *_prod = this->m_infNFe->det->value(i)->prod.get();
    VeicProd *_veicProd = _prod->veicProd.get();
    Med *_med = _prod->med.get();
    Comb *_comb = _prod->comb.get();
    Encerrante *_encerrante = _comb->encerrante.get();
    Cide *_cide = _comb->CIDE.get();
    //nItem é um atributo da tag det, mas está dentro da classe prod
    this->m_xmlw->writeAttribute("nItem", QString::number(_prod->get_nItem()));
    //prod - I01
    this->m_xmlw->writeStartElement("prod"); //abertura grupo prod
    this->m_xmlw->writeTextElement("cProd", _prod->get_cProd());
    this->m_xmlw->writeTextElement("cEAN", _prod->get_cEAN());
    if (!_prod->get_cBarra().isEmpty())
        this->m_xmlw->writeTextElement("cBarra", _prod->get_cBarra());
    const auto &ide = *this->m_infNFe->ide;
    if (i == 0 && ide.get_tpAmb() == TpAmb::Homologacao &&
        (ide.get_mod() == ModeloDF::NFCe || ide.usaDanfeSimplificadoTipo2()))
        this->m_xmlw->writeTextElement("xProd", "NOTA FISCAL EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
    else
        this->m_xmlw->writeTextElement("xProd", _prod->get_xProd());

    this->m_xmlw->writeTextElement("NCM", _prod->get_NCM());
    if (!_prod->get_NVE().isEmpty())
        this->m_xmlw->writeTextElement("NVE", _prod->get_NVE());
    if (!_prod->get_CEST().isEmpty())
        this->m_xmlw->writeTextElement("CEST", _prod->get_CEST());
    if (_prod->get_indEscala() != IndEscala::None)
        this->m_xmlw->writeTextElement("indEscala", ConvNF::indEscalaToStr(_prod->get_indEscala()));
    if (!_prod->get_CNPJFab().isEmpty())
        this->m_xmlw->writeTextElement("CNPJFab", _prod->get_CNPJFab());
    if (!_prod->get_cBenef().isEmpty())
        this->m_xmlw->writeTextElement("cBenef", _prod->get_cBenef());
    for (const auto *credito : *_prod->gCred)
    {
        this->m_xmlw->writeStartElement("gCred");
        this->m_xmlw->writeTextElement("cCredPresumido", credito->get_cCredPresumido());
        this->m_xmlw->writeTextElement("pCredPresumido", CppUtility::doubleToStrDecimal(credito->get_pCredPresumido(), 4));
        this->m_xmlw->writeTextElement("vCredPresumido", CppUtility::doubleToStrDecimal(credito->get_vCredPresumido(), 2));
        this->m_xmlw->writeEndElement();
    }
    if (_prod->get_tpCredPresIBSZFM() != TpCredPresIBSZFM::None) //I05k
        this->m_xmlw->writeTextElement("tpCredPresIBSZFM", ConvNF::tpCredPresIBSZFMToStr(_prod->get_tpCredPresIBSZFM()));
    if (!_prod->get_EXTIPI().isEmpty())
        this->m_xmlw->writeTextElement("EXTIPI", _prod->get_EXTIPI());
    this->m_xmlw->writeTextElement("CFOP", _prod->get_CFOP());
    this->m_xmlw->writeTextElement("uCom", _prod->get_uCom());
    this->m_xmlw->writeTextElement("qCom", CppUtility::doubleToStrDecimal(_prod->get_qCom(), 4));
    this->m_xmlw->writeTextElement("vUnCom", CppUtility::doubleToStrDecimal(_prod->get_vUnCom(), 10));
    this->m_xmlw->writeTextElement("vProd", CppUtility::doubleToStrDecimal(_prod->get_vProd(), 2));
    this->m_xmlw->writeTextElement("cEANTrib", _prod->get_cEANTrib());
    if (!_prod->get_cBarraTrib().isEmpty())
        this->m_xmlw->writeTextElement("cBarraTrib", _prod->get_cBarraTrib());
    this->m_xmlw->writeTextElement("uTrib", _prod->get_uTrib());
    this->m_xmlw->writeTextElement("qTrib", CppUtility::doubleToStrDecimal(_prod->get_qTrib(), 4));
    this->m_xmlw->writeTextElement("vUnTrib", CppUtility::doubleToStrDecimal(_prod->get_vUnTrib(), 10));
    if (_prod->get_vFrete() > 0)
        this->m_xmlw->writeTextElement("vFrete", CppUtility::doubleToStrDecimal(_prod->get_vFrete(), 2));
    if (_prod->get_vSeg() > 0)
        this->m_xmlw->writeTextElement("vSeg", CppUtility::doubleToStrDecimal(_prod->get_vSeg(), 2));
    if (_prod->get_vDesc() > 0)
        this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(_prod->get_vDesc(), 2));
    if (_prod->get_vOutro() > 0)
        this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(_prod->get_vOutro(), 2));
    this->m_xmlw->writeTextElement("indTot", ConvNF::indTotToStr(_prod->get_indTot()));
    if (_prod->get_indBemMovelUsado() != IndBemMovelUsado::None) //I17c
        this->m_xmlw->writeTextElement("indBemMovelUsado", ConvNF::indBemMovelUsadoToStr(_prod->get_indBemMovelUsado()));

    //DI - I18 - container 0-100
    if (_prod->DI->count() > 0)
    {
        for(int j = 0; j < _prod->DI->count() ; ++j)
        {
            Di *_di = _prod->DI->value(j);
            this->m_xmlw->writeStartElement("DI");//abertura grupo DI
            this->m_xmlw->writeTextElement("nDI", _di->get_nDI());
            this->m_xmlw->writeTextElement("dDI", CppUtility::dateTimeToStr(_di->get_dDI(),
                                                                            DtH::Date));
            this->m_xmlw->writeTextElement("xLocDesemb", _di->get_xLocDesemb());
            this->m_xmlw->writeTextElement("UFDesemb", _di->get_UFDesemb());
            this->m_xmlw->writeTextElement("dDesemb", CppUtility::dateTimeToStr(_di->get_dDesemb(),
                                                                                DtH::Date));
            this->m_xmlw->writeTextElement("tpViaTransp", ConvNF::tpViaTranspToStr(_di->get_tpViaTransp()));
            if (_di->get_vAFRMM() > 0)
                this->m_xmlw->writeTextElement("vAFRMM", CppUtility::doubleToStrDecimal(_di->get_vAFRMM(), 2));
            this->m_xmlw->writeTextElement("tpIntermedio", ConvNF::tpIntermedioToStr(_di->get_tpIntermedio()));
            if (!_di->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", _di->get_CNPJ());
            if (!_di->get_CPF().isEmpty())
                this->m_xmlw->writeTextElement("CPF", _di->get_CPF());
            if (!_di->get_UFTerceiro().isEmpty())
                this->m_xmlw->writeTextElement("UFTerceiro", _di->get_UFTerceiro());
            this->m_xmlw->writeTextElement("cExportador", _di->get_cExportador());

            //adi - I25 - container 1-100
            if (_di->adi->count() > 0)
            {
                for(int l = 0; l < _di->adi->count() ; ++l)
                {
                    Adi *_adi = _di->adi->value(l);
                    this->m_xmlw->writeStartElement("adi");//abertura grupo adi
                    this->m_xmlw->writeTextElement("nAdicao", QString::number(_adi->get_nAdicao()));
                    this->m_xmlw->writeTextElement("nSeqAdic", QString::number(_adi->get_nSeqAdic()));
                    this->m_xmlw->writeTextElement("cFabricante", _adi->get_cFabricante());
                    if (_adi->get_vDescDI() > 0)
                        this->m_xmlw->writeTextElement("vDescDI", CppUtility::doubleToStrDecimal(_adi->get_vDescDI(), 2));
                    if (!_adi->get_nDraw().isEmpty())
                        this->m_xmlw->writeTextElement("nDraw", _adi->get_nDraw());
                    this->m_xmlw->writeEndElement();//fechamento grupo adi
                }
            }
            this->m_xmlw->writeEndElement();//fechamento grupo DI
        }
    }//fim DI

    //detExport - I50 - container 0-500
    if (_prod->detExport->count() > 0)
    {
        for(int j = 0; j < _prod->detExport->count() ; ++j)
        {
            DetExport *_detExport = _prod->detExport->value(j);
            ExportInd *_exportInd = _detExport->exportInd.get();
            this->m_xmlw->writeStartElement("detExport");//abertura grupo detExport
            if (!_detExport->get_nDraw().isEmpty())
                this->m_xmlw->writeTextElement("nDraw",_detExport->get_nDraw());
            //exportInd - I52
            if (!_exportInd->get_nRE().isEmpty() ||
                !_exportInd->get_chNFe().isEmpty() ||
                _exportInd->get_qExport() > 0)
            {
                this->m_xmlw->writeStartElement("exportInd");//abertura grupo exportInd
                this->m_xmlw->writeTextElement("nRE",_exportInd->get_nRE());
                this->m_xmlw->writeTextElement("chNFe",_exportInd->get_chNFe());
                this->m_xmlw->writeTextElement("qExport", CppUtility::doubleToStrDecimal(_exportInd->get_qExport(), 4));
                this->m_xmlw->writeEndElement();//fechamento grupo exportInd
            }
            this->m_xmlw->writeEndElement();//fechamento grupo detExport
        }
    }//fim detExport
    //continuação prod
    if (!_prod->get_xPed().isEmpty())
        this->m_xmlw->writeTextElement("xPed", _prod->get_xPed());
    if (_prod->get_nItemPed() > 0)
        this->m_xmlw->writeTextElement("nItemPed", QString::number(_prod->get_nItemPed()));
    if (!_prod->get_nFCI().isEmpty())
        this->m_xmlw->writeTextElement("nFCI", _prod->get_nFCI());

    //rastro - I80 - container 0-500
    if (_prod->rastro->count() > 0)
    {
        for(int j = 0; j < _prod->rastro->count() ; ++j)
        {
            Rastro *_rastro = _prod->rastro->value(j);
            this->m_xmlw->writeStartElement("rastro");//abertura grupo rastro
            this->m_xmlw->writeTextElement("nLote",_rastro->get_nLote());
            this->m_xmlw->writeTextElement("qLote",QString::number(_rastro->get_qLote()));
            this->m_xmlw->writeTextElement("dFab", CppUtility::dateTimeToStr(_rastro->get_dFab(),
                                                                            DtH::DateTime));
            this->m_xmlw->writeTextElement("dVal", CppUtility::dateTimeToStr(_rastro->get_dVal(),
                                                                            DtH::DateTime));
            if (!_rastro->get_cAgreg().isEmpty())
                this->m_xmlw->writeTextElement("cAgreg", _rastro->get_cAgreg());
            this->m_xmlw->writeEndElement();//fechamento grupo rastro
        }
    }

    //SequênciaXML - I90 -x-
    //veicProd - J01
    if (_veicProd->get_tpOp() != TpOp::None)
    {
        this->m_xmlw->writeStartElement("veicProd");//abertura grupo veicProd
        this->m_xmlw->writeTextElement("tpOp",ConvNF::tpOpToStr(_veicProd->get_tpOp()));
        this->m_xmlw->writeTextElement("chassi",_veicProd->get_chassi());
        this->m_xmlw->writeTextElement("cCor",_veicProd->get_cCor());
        this->m_xmlw->writeTextElement("xCor",_veicProd->get_xCor());
        this->m_xmlw->writeTextElement("pot",_veicProd->get_pot());
        this->m_xmlw->writeTextElement("cilin",_veicProd->get_cilin());
        this->m_xmlw->writeTextElement("pesoL",_veicProd->get_pesoL());
        this->m_xmlw->writeTextElement("pesoB",_veicProd->get_pesoB());
        this->m_xmlw->writeTextElement("nSerie",_veicProd->get_nSerie());
        this->m_xmlw->writeTextElement("tpComb",_veicProd->get_tpComb());
        this->m_xmlw->writeTextElement("nMotor",_veicProd->get_nMotor());
        this->m_xmlw->writeTextElement("CMT",_veicProd->get_CMT());
        this->m_xmlw->writeTextElement("dist",_veicProd->get_dist());
        this->m_xmlw->writeTextElement("anoMod",QString::number(_veicProd->get_anoMod()));
        this->m_xmlw->writeTextElement("anoFab",QString::number(_veicProd->get_anoFab()));
        this->m_xmlw->writeTextElement("tpPint",_veicProd->get_tpPint());
        this->m_xmlw->writeTextElement("tpVeic",_veicProd->get_tpVeic());
        this->m_xmlw->writeTextElement("espVeic",_veicProd->get_espVeic());
        this->m_xmlw->writeTextElement("VIN",_veicProd->get_VIN());
        this->m_xmlw->writeTextElement("condVeic",ConvNF::condVeicToStr(_veicProd->get_condVeic()));
        this->m_xmlw->writeTextElement("cMod",_veicProd->get_cMod());
        this->m_xmlw->writeTextElement("cCorDENATRAN",_veicProd->get_cCorDENATRAN());
        this->m_xmlw->writeTextElement("lota",QString::number(_veicProd->get_lota()));
        this->m_xmlw->writeTextElement("tpRest",ConvNF::tpRestToStr(_veicProd->get_tpRest()));
        this->m_xmlw->writeEndElement();//fechamento grupo veicProd
    }//fim veicProd

    //med - K01
    if (!_med->get_cProdANVISA().isEmpty())
    {
        this->m_xmlw->writeStartElement("med");//abertura grupo med
        this->m_xmlw->writeTextElement("cProdANVISA", _med->get_cProdANVISA());
        if (!_med->get_xMotivoIsencao().isEmpty())
            this->m_xmlw->writeTextElement("xMotivoIsencao", _med->get_xMotivoIsencao());
        this->m_xmlw->writeTextElement("vPMC", CppUtility::doubleToStrDecimal(_med->get_vPMC(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo med
    }

    //arma - L01 - container 1-500
    if (_prod->arma->count() > 0)
    {
        for(int j = 0; j < _prod->arma->count(); ++j)
        {
            Arma *_arma = _prod->arma->value(j);
            this->m_xmlw->writeStartElement("arma");//abertura grupo arma
            this->m_xmlw->writeTextElement("tpArma", ConvNF::tpArmaToStr(_arma->get_tpArma()));
            this->m_xmlw->writeTextElement("nSerie", _arma->get_nSerie());
            this->m_xmlw->writeTextElement("nCano", _arma->get_nCano());
            this->m_xmlw->writeTextElement("descr", _arma->get_descr());
            this->m_xmlw->writeEndElement();//fechamento grupo arma
        }
    }

    //comb - LA01
    if (_comb->get_cProdANP() > 0)
    {
        this->m_xmlw->writeStartElement("comb");//abertura grupo comb
        this->m_xmlw->writeTextElement("cProdANP", QString::number(_comb->get_cProdANP()));
        this->m_xmlw->writeTextElement("descANP", _comb->get_descANP());
        if (_comb->get_pGLP() > 0)
            this->m_xmlw->writeTextElement("pGLP", CppUtility::doubleToStrDecimal(_comb->get_pGLP(), 4));
        if (_comb->get_pGNn() > 0)
            this->m_xmlw->writeTextElement("pGNn", CppUtility::doubleToStrDecimal(_comb->get_pGNn(), 4));
        if (_comb->get_pGNi() > 0)
            this->m_xmlw->writeTextElement("pGNi", CppUtility::doubleToStrDecimal(_comb->get_pGNi(), 4));
        if (_comb->get_vPart() > 0)
            this->m_xmlw->writeTextElement("vPart", CppUtility::doubleToStrDecimal(_comb->get_vPart(), 2));
        if (!_comb->get_CODIF().isEmpty())
            this->m_xmlw->writeTextElement("CODIF", _comb->get_CODIF());
        if (_comb->get_qTemp() > 0)
            this->m_xmlw->writeTextElement("qTemp", CppUtility::doubleToStrDecimal(_comb->get_qTemp(), 4));
        if (!_comb->get_UFCons().isEmpty())
            this->m_xmlw->writeTextElement("UFCons", _comb->get_UFCons());
        //CIDE - LA07
        if (_cide->get_vCIDE() > 0)
        {
           this->m_xmlw->writeStartElement("CIDE");//abertura grupo CIDE
           this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_cide->get_qBCProd(), 4));
           this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_cide->get_vAliqProd(), 4));
           this->m_xmlw->writeTextElement("vCIDE", CppUtility::doubleToStrDecimal(_cide->get_vCIDE(), 2));
           this->m_xmlw->writeEndElement();//fechamento grupo CIDE
        }
        //encerrante - LA11
        if (_encerrante->get_nBico() > 0 ||
            _encerrante->get_vEncIni() > 0)
        {
            this->m_xmlw->writeStartElement("encerrante");//abertura grupo encerrante
            this->m_xmlw->writeTextElement("nBico", QString::number(_encerrante->get_nBico()));
            this->m_xmlw->writeTextElement("nBomba", QString::number(_encerrante->get_nBomba()));
            this->m_xmlw->writeTextElement("nTanque", QString::number(_encerrante->get_nTanque()));
            this->m_xmlw->writeTextElement("vEncIni", CppUtility::doubleToStrDecimal(_encerrante->get_vEncIni(), 3));
            this->m_xmlw->writeTextElement("vEncFin", CppUtility::doubleToStrDecimal(_encerrante->get_vEncFin(), 3));
            this->m_xmlw->writeEndElement();//fechamento grupo encerrante
        }
        //pBio - LA17 (filho de comb, independente de encerrante)
        if (_comb->get_pBio() != 0.0)
            this->m_xmlw->writeTextElement("pBio", CppUtility::doubleToStrDecimal(_comb->get_pBio(), 4));
        //origComb - LA18
        if (_comb->origComb->count() > 0)
        {
            for(int j = 0; j < _comb->origComb->count(); ++j)
            {
                OrigComb *_origComb = _comb->origComb->value(j);
                this->m_xmlw->writeStartElement("origComb");//abertura grupo arma
                this->m_xmlw->writeTextElement("indImport", ConvNF::indImportToStr(_origComb->get_indImport()));
                this->m_xmlw->writeTextElement("cUFOrig", QString::number(_origComb->get_cUFOrig()));
                this->m_xmlw->writeTextElement("pOrig", CppUtility::doubleToStrDecimal(_origComb->get_pOrig(), 4));
                this->m_xmlw->writeEndElement();//fechamento grupo arma
            }
        }


        this->m_xmlw->writeEndElement();//fechamento grupo comb
    }
    //nRECOPI LB01 - pertence ao I90-> Como o I90 nao e um grupo com tag, nRECOPI fica dentro de I01-prod
    if (!_prod->get_nRECOPI().isEmpty())
    {
        this->m_xmlw->writeTextElement("nRECOPI", _prod->get_nRECOPI());
    }

    this->m_xmlw->writeEndElement();//fechamento grupo prod
}

void XmlWrite::get_imposto(const int &i)
{   //imposto - M01
    this->m_xmlw->writeStartElement("imposto");//abertura grupo imposto
    if (this->m_infNFe->det->value(i)->imposto->get_vTotTrib() > 0)
        this->m_xmlw->writeTextElement("vTotTrib", CppUtility::doubleToStrDecimal(this->m_infNFe->det->value(i)->imposto->get_vTotTrib(), 2));
    get_ICMS(i);
    get_IPI(i);
    get_II(i);
    get_ISSQN(i);
    get_PIS(i);
    get_PISST(i);
    get_COFINS(i);
    get_COFINSST(i);
    get_ICMSUFDest(i);
    get_IS(i);
    get_IBSCBS(i);
    this->m_xmlw->writeEndElement();//fechamento grupo imposto
}

void XmlWrite::get_ICMS(const int &i)
{
    Icms *_icms = this->m_infNFe->det->value(i)->imposto->ICMS.get();
    if (_icms->get_CST() != CstICMS::None ||
        _icms->get_CSOSN() != CsosnICMS::None)
    {
        //Origem da mercadoria e igual pra todos, CST ou CSOSN
        QString orig = ConvNF::origToStr(_icms->get_orig());

        //ICMS - N01
        this->m_xmlw->writeStartElement("ICMS");//abertura grupo ICMS
        //CST
        if (_icms->get_CST() != CstICMS::None)
        {
            QString cst = ConvNF::cstICMSToStr(_icms->get_CST());
            QString cab;
            if (cst.length() == 2)
            {
                cab = (cst == "41" || cst == "50") ? QStringLiteral("ICMS40")
                                                       : QString("%1%2").arg("ICMS", cst);
            }
            else
            {
                if (cst.startsWith(QStringLiteral("Part")))
                    cab = "ICMSPart";
                else                    //ST41 ou ST60
                    cab = "ICMSST";
                cst = cst.right(2);
            }

            this->m_xmlw->writeStartElement(cab); //abertura grupo ICMS00, ICMS10, etc
            this->m_xmlw->writeTextElement("orig", orig);
            this->m_xmlw->writeTextElement("CST", cst);
            //ICMS00 - N02
            if (_icms->get_CST() == CstICMS::ICMS00)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));
                if (_icms->get_pFCP() > 0 ||
                    _icms->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                }
            }

            //ICMS02 - N02a
            if (_icms->get_CST() == CstICMS::ICMS02)
            {
                if (_icms->has_qBCMono())
                    this->m_xmlw->writeTextElement("qBCMono", CppUtility::doubleToStrDecimal(_icms->get_qBCMono(), 4));
                this->m_xmlw->writeTextElement("adRemICMS", CppUtility::doubleToStrDecimal(_icms->get_adRemICMS(), 4));
                this->m_xmlw->writeTextElement("vICMSMono", CppUtility::doubleToStrDecimal(_icms->get_vICMSMono(), 2));
            }

            //ICMS10 - N03
            if (_icms->get_CST() == CstICMS::ICMS10)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                if (_icms->get_vBCFCP() > 0 ||
                    _icms->get_pFCP() > 0   ||
                    _icms->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(_icms->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                }
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }

                if (_icms->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(_icms->get_motDesICMSST()));
                }
            }

            //ICMS15 - N03a
            if (_icms->get_CST() == CstICMS::ICMS15)
            {
                if (_icms->has_qBCMono())
                    this->m_xmlw->writeTextElement("qBCMono", CppUtility::doubleToStrDecimal(_icms->get_qBCMono(), 4));
                this->m_xmlw->writeTextElement("adRemICMS", CppUtility::doubleToStrDecimal(_icms->get_adRemICMS(), 4));
                this->m_xmlw->writeTextElement("vICMSMono", CppUtility::doubleToStrDecimal(_icms->get_vICMSMono(), 2));
                if (_icms->has_qBCMonoReten())
                    this->m_xmlw->writeTextElement("qBCMonoReten", CppUtility::doubleToStrDecimal(_icms->get_qBCMonoReten(), 4));
                this->m_xmlw->writeTextElement("adRemICMSReten", CppUtility::doubleToStrDecimal(_icms->get_adRemICMSReten(), 4));
                this->m_xmlw->writeTextElement("vICMSMonoReten", CppUtility::doubleToStrDecimal(_icms->get_vICMSMonoReten(), 2));

                if (_icms->has_pRedAdRem())
                    this->m_xmlw->writeTextElement("pRedAdRem", CppUtility::doubleToStrDecimal(_icms->get_pRedAdRem(), 2));

                if (_icms->get_motRedAdRem() != MotRedAdRem::None)
                    this->m_xmlw->writeTextElement("motRedAdRem", ConvNF::motRedAdRemToStr(_icms->get_motRedAdRem()));
            }

            //ICMS20 - N04
            if (_icms->get_CST() == CstICMS::ICMS20)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                if (_icms->get_vBCFCP() > 0 ||
                    _icms->get_pFCP() > 0   ||
                    _icms->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(_icms->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                }

                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }
            }

            //ICMS30 - N05
            if (_icms->get_CST() == CstICMS::ICMS30)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }

                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }
            }

            // ICMS40 - N06 - ICMS= 40, 41, 50
            if (_icms->get_CST() == CstICMS::ICMS40 ||
                _icms->get_CST() == CstICMS::ICMS41 ||
                _icms->get_CST() == CstICMS::ICMS50)
            {
                //ICMS=40, 41, 50
                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }
            }

            //ICMS51 - N07
            if (_icms->get_CST() == CstICMS::ICMS51)
            {
                if (_icms->get_modBC() != ModBC::None)
                    this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                if (_icms->has_pRedBC())
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                if (!_icms->get_cBenefRBC().isEmpty())
                    this->m_xmlw->writeTextElement("cBenefRBC", _icms->get_cBenefRBC());
                if (_icms->has_vBC())
                    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                if (_icms->has_pICMS())
                    this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                if (_icms->has_vICMSOp())
                    this->m_xmlw->writeTextElement("vICMSOp", CppUtility::doubleToStrDecimal(_icms->get_vICMSOp(), 2));
                if (_icms->has_pDif())
                    this->m_xmlw->writeTextElement("pDif", CppUtility::doubleToStrDecimal(_icms->get_pDif(), 4));
                if (_icms->has_vICMSDif())
                    this->m_xmlw->writeTextElement("vICMSDif", CppUtility::doubleToStrDecimal(_icms->get_vICMSDif(), 2));
                if (_icms->has_vICMS())
                    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                if (_icms->get_vBCFCP() > 0 ||
                    _icms->get_pFCP() > 0   ||
                    _icms->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(_icms->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                }

                if (_icms->get_pFCPDif() > 0 ||
                    _icms->get_vFCPDif() > 0)
                {
                    this->m_xmlw->writeTextElement("pFCPDif", CppUtility::doubleToStrDecimal(_icms->get_pFCPDif(), 4));
                    this->m_xmlw->writeTextElement("vFCPDif", CppUtility::doubleToStrDecimal(_icms->get_vFCPDif(), 2));
                }

                if (_icms->get_vFCPEfet() > 0)
                   this->m_xmlw->writeTextElement("vFCPEfet", CppUtility::doubleToStrDecimal(_icms->get_vFCPEfet(), 2));
            }

            //ICMS53 - N07a
            if (_icms->get_CST() == CstICMS::ICMS53)
            {
                if (_icms->has_qBCMono())
                    this->m_xmlw->writeTextElement("qBCMono", CppUtility::doubleToStrDecimal(_icms->get_qBCMono(), 4));
                if (_icms->has_adRemICMS())
                    this->m_xmlw->writeTextElement("adRemICMS", CppUtility::doubleToStrDecimal(_icms->get_adRemICMS(), 4));
                if (_icms->has_vICMSMonoOp())
                    this->m_xmlw->writeTextElement("vICMSMonoOp", CppUtility::doubleToStrDecimal(_icms->get_vICMSMonoOp(), 2));
                if (_icms->has_pDif())
                    this->m_xmlw->writeTextElement("pDif", CppUtility::doubleToStrDecimal(_icms->get_pDif(), 4));
                if (_icms->has_vICMSMonoDif())
                    this->m_xmlw->writeTextElement("vICMSMonoDif", CppUtility::doubleToStrDecimal(_icms->get_vICMSMonoDif(), 2));
                if (_icms->has_vICMSMono())
                    this->m_xmlw->writeTextElement("vICMSMono", CppUtility::doubleToStrDecimal(_icms->get_vICMSMono(), 2));
            }

            //ICMS60 - N08
            if (_icms->get_CST() == CstICMS::ICMS60)
            {
                // Regras N12-81 e N12a-50 da NT 2018.005 para o modelo 55.
                const bool emitirICMSSTRet = this->m_infNFe->ide->get_mod() == ModeloDF::NFe &&
                                            this->m_infNFe->ide->get_indFinal() == IndFinal::Normal;
                if (emitirICMSSTRet || _icms->has_ICMSSTRet() ||
                    _icms->has_pST() || _icms->has_vICMSSubstituto())
                {
                    this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCSTRet(), 2));
                    this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(_icms->get_pST(), 4));
                    if (emitirICMSSTRet || _icms->has_vICMSSubstituto())
                        this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(_icms->get_vICMSSubstituto(), 2));
                    this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTRet(), 2));
                }

                if (_icms->has_FCPSTRet())
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vFCPSTRet(), 2));
                }

                if (_icms->has_ICMSEfet())
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(_icms->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(_icms->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_vICMSEfet(), 2));
                }
            }

            //ICMS61 - N08a
            if (_icms->get_CST() == CstICMS::ICMS61)
            {
                if (_icms->has_qBCMonoRet())
                    this->m_xmlw->writeTextElement("qBCMonoRet", CppUtility::doubleToStrDecimal(_icms->get_qBCMonoRet(), 4));
                this->m_xmlw->writeTextElement("adRemICMSRet", CppUtility::doubleToStrDecimal(_icms->get_adRemICMSRet(), 4));
                this->m_xmlw->writeTextElement("vICMSMonoRet", CppUtility::doubleToStrDecimal(_icms->get_vICMSMonoRet(), 2));
            }

            //ICMSPart - N10a
            if (_icms->get_CST() == CstICMS::ICMSPart10 ||
                _icms->get_CST() == CstICMS::ICMSPart20 ||
                _icms->get_CST() == CstICMS::ICMSPart90)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                if (_icms->get_pRedBC() > 0)
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }

                this->m_xmlw->writeTextElement("pBCOp", CppUtility::doubleToStrDecimal(_icms->get_pBCOp(), 4));
                this->m_xmlw->writeTextElement("UFST", _icms->get_UFST());

                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }
            }

            //ICMS70 - N09
            if (_icms->get_CST() == CstICMS::ICMS70)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                if (_icms->get_vBCFCP() > 0 ||
                    _icms->get_pFCP() > 0   ||
                    _icms->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(_icms->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                }

                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }

                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }

                if (_icms->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(_icms->get_motDesICMSST()));
                }
            }

            //ICMS90 - N10
            if (_icms->get_CST() == CstICMS::ICMS90)
            {
                const bool diferimento = _icms->has_vICMSOp() || _icms->has_pDif() || _icms->has_vICMSDif();
                const bool fcp = _icms->get_vBCFCP() > 0 || _icms->get_pFCP() > 0 || _icms->get_vFCP() > 0;
                const bool diferimentoFCP = _icms->has_FCPDif();
                if (_icms->get_modBC() != ModBC::None ||
                    _icms->get_vBC() > 0 ||
                    _icms->get_pRedBC() > 0 ||
                    !_icms->get_cBenefRBC().isEmpty() ||
                    _icms->get_pICMS() > 0 ||
                    _icms->get_vICMS() > 0 ||
                    diferimento || fcp || diferimentoFCP)
                {
                    this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                    if (_icms->get_pRedBC() > 0)
                        this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                    if (!_icms->get_cBenefRBC().isEmpty())
                        this->m_xmlw->writeTextElement("cBenefRBC", _icms->get_cBenefRBC());
                    this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                    if (diferimento)
                    {
                        this->m_xmlw->writeTextElement("vICMSOp", CppUtility::doubleToStrDecimal(_icms->get_vICMSOp(), 2));
                        this->m_xmlw->writeTextElement("pDif", CppUtility::doubleToStrDecimal(_icms->get_pDif(), 4));
                        this->m_xmlw->writeTextElement("vICMSDif", CppUtility::doubleToStrDecimal(_icms->get_vICMSDif(), 2));
                    }
                    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                    if (fcp)
                    {
                        this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(_icms->get_vBCFCP(), 2));
                        this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(_icms->get_pFCP(), 4));
                        this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icms->get_vFCP(), 2));
                    }

                    if (diferimentoFCP)
                    {
                        this->m_xmlw->writeTextElement("pFCPDif", CppUtility::doubleToStrDecimal(_icms->get_pFCPDif(), 4));
                        this->m_xmlw->writeTextElement("vFCPDif", CppUtility::doubleToStrDecimal(_icms->get_vFCPDif(), 2));
                        if (_icms->has_vFCPEfet())
                            this->m_xmlw->writeTextElement("vFCPEfet", CppUtility::doubleToStrDecimal(_icms->get_vFCPEfet(), 2));
                    }
                }

                const bool fcpST = _icms->get_vBCFCPST() > 0 || _icms->get_pFCPST() > 0 || _icms->get_vFCPST() > 0;
                if (_icms->get_modBCST() != ModBCST::None ||
                    _icms->get_pMVAST() > 0   ||
                    _icms->get_pRedBCST() > 0 ||
                    _icms->get_vBCST() > 0    ||
                    _icms->get_pICMSST() > 0  ||
                    _icms->get_vICMSST() != 0 ||
                    fcpST)
                {
                    this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                    if (_icms->get_pMVAST() > 0)
                        this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                    if (_icms->get_pRedBCST() > 0)
                        this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                    this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                    this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                    if (fcpST)
                    {
                        this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                        this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                        this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                    }
                }

                if (_icms->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(_icms->get_motDesICMS()));
                    if (_icms->get_indDeduzDeson() != IndDeduzDeson::None)
                        this->m_xmlw->writeTextElement("indDeduzDeson", ConvNF::indDeduzDesonToStr(_icms->get_indDeduzDeson()));
                }

                if (_icms->get_motDesICMSST() != MotDesICMS::None || _icms->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(_icms->get_motDesICMSST()));
                }
            }

            //ICMSST - N10b
            if(_icms->get_CST() == CstICMS::ICMSST41 ||
               _icms->get_CST() == CstICMS::ICMSST60)
            {
                this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCSTRet(), 2));
                const bool emitirICMSSTRet = _icms->get_CST() == CstICMS::ICMSST60 &&
                                            this->m_infNFe->ide->get_mod() == ModeloDF::NFe &&
                                            this->m_infNFe->ide->get_indFinal() == IndFinal::Normal;
                if (emitirICMSSTRet || _icms->has_pST())
                    this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(_icms->get_pST(), 4));
                if (emitirICMSSTRet || _icms->has_vICMSSubstituto())
                    this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(_icms->get_vICMSSubstituto(), 2));

                this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTRet(), 2));

                if (_icms->has_FCPSTRet())
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vFCPSTRet(), 2));
                }

                this->m_xmlw->writeTextElement("vBCSTDest", CppUtility::doubleToStrDecimal(_icms->get_vBCSTDest(), 2));
                this->m_xmlw->writeTextElement("vICMSSTDest", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTDest(), 2));

                if (_icms->has_ICMSEfet())
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(_icms->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(_icms->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_vICMSEfet(), 2));
                }

            }

            this->m_xmlw->writeEndElement();//fechamento grupo ICMS00, ICMS10, etc->->->

        }
        else //CSOSN - Simples Nacional, incluindo CRT=4 (MEI)
        {
            QString csosn = ConvNF::csosnICMSToStr(_icms->get_CSOSN());
            QString cab = QString("%1%2").arg("ICMSSN", csosn);
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN103 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN300 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN400)
                cab = "ICMSSN102";

            this->m_xmlw->writeStartElement(cab); //abertura grupo ICMSSN101, ICMSSN102, etc->->->
            //NT 2024.001: apenas ICMSSN102 e ICMSSN900 permitem omitir a origem para MEI.
            const bool origemOpcional = this->m_infNFe->emite->get_CRT() == Crt::SimplesMEI &&
                (cab == "ICMSSN102" || cab == "ICMSSN900");
            if (!origemOpcional || _icms->get_orig() != Orig::None)
                this->m_xmlw->writeTextElement("orig", orig);
            this->m_xmlw->writeTextElement("CSOSN", csosn);

            //CSOSN=101
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN101)
            {
                this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(_icms->get_pCredSN(), 4));
                this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(_icms->get_vCredICMSSN(), 2));
            }
            //CSON 102, 103, 300 ou 400
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN102 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN103 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN300 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN400)
            {
                //não ha nada ate o momento
            }
            //CSON 201
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN201)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0   ||
                    _icms->get_pCredSN() > 0  ||
                    _icms->get_vCredICMSSN() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                    this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(_icms->get_pCredSN(), 4));
                    this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(_icms->get_vCredICMSSN(), 2));
                }
            }
            //CSON 202 ou 203
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN202 ||
                _icms->get_CSOSN() == CsosnICMS::ICMSSN203)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                if (_icms->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                if (_icms->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }
            }
            //CSON 500
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN500)
            {
                // Regras N12-81 e N12a-50 da NT 2018.005 para o modelo 55.
                const bool emitirICMSSTRet = this->m_infNFe->ide->get_mod() == ModeloDF::NFe &&
                                            this->m_infNFe->ide->get_indFinal() == IndFinal::Normal;
                if (emitirICMSSTRet || _icms->has_ICMSSTRet() ||
                    _icms->has_pST() || _icms->has_vICMSSubstituto())
                {
                    this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCSTRet(), 2));
                    this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(_icms->get_pST(), 4));
                    if (emitirICMSSTRet || _icms->has_vICMSSubstituto())
                        this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(_icms->get_vICMSSubstituto(), 2));
                    this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(_icms->get_vICMSSTRet(), 2));
                }

                if (_icms->has_FCPSTRet())
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(_icms->get_vFCPSTRet(), 2));
                }

                if (_icms->has_ICMSEfet())
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(_icms->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(_icms->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(_icms->get_vICMSEfet(), 2));
                }
            }
            //CSON 900
            if (_icms->get_CSOSN() == CsosnICMS::ICMSSN900)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(_icms->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icms->get_vBC(), 2));
                if (_icms->get_pRedBC() > 0)
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(_icms->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(_icms->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icms->get_vICMS(), 2));

                if (_icms->get_modBCST() != ModBCST::None ||
                    _icms->get_vBCST() > 0 ||
                    _icms->get_pICMSST() > 0 ||
                    _icms->get_vICMSST() > 0)
                {
                    this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(_icms->get_modBCST()));
                    if (_icms->get_pMVAST() > 0)
                        this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(_icms->get_pMVAST(), 4));
                    if (_icms->get_pRedBCST() > 0)
                        this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(_icms->get_pRedBCST(), 4));
                    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icms->get_vBCST(), 2));
                    this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(_icms->get_pICMSST(), 4));
                    this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(_icms->get_vICMSST(), 2));
                }

                if (_icms->get_vBCFCPST() > 0 ||
                    _icms->get_pFCPST() > 0   ||
                    _icms->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(_icms->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(_icms->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icms->get_vFCPST(), 2));
                }

                if (_icms->get_pCredSN() > 0  ||
                    _icms->get_vCredICMSSN() > 0)
                {
                    this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(_icms->get_pCredSN(), 4));
                    this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(_icms->get_vCredICMSSN(), 2));
                }
            }

            this->m_xmlw->writeEndElement();//fechamento grupo ICMSSN101, ICMSSN102, etc->->->
        }

        this->m_xmlw->writeEndElement();//fechamento grupo ICMS
    }
}

void XmlWrite::get_ICMSUFDest(const int &i)
{   //ICMSUFDest - NA01
    IcmsUFDest *_icmsUFDest = this->m_infNFe->det->value(i)->imposto->ICMSUFDest.get();
    if (_icmsUFDest->get_vBCUFDest() > 0      ||
        _icmsUFDest->get_vBCFCPUFDest() > 0   ||
        _icmsUFDest->get_pICMSUFDest() > 0    ||
        _icmsUFDest->get_pICMSInter() > 0     ||
        _icmsUFDest->get_pICMSInterPart() > 0 ||
        _icmsUFDest->get_vICMSUFDest() > 0    ||
        _icmsUFDest->get_vICMSUFRemet() > 0)
    {
        this->m_xmlw->writeStartElement("ICMSUFDest"); //abertura grupo ICMSUFDest
        this->m_xmlw->writeTextElement("vBCUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_vBCUFDest(), 2));
        this->m_xmlw->writeTextElement("vBCFCPUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_vBCFCPUFDest(), 2));
        if (_icmsUFDest->get_pFCPUFDest() > 0)
            this->m_xmlw->writeTextElement("pFCPUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_pFCPUFDest(), 4));
        this->m_xmlw->writeTextElement("pICMSUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_pICMSUFDest(), 4));
        this->m_xmlw->writeTextElement("pICMSInter", CppUtility::doubleToStrDecimal(_icmsUFDest->get_pICMSInter(), 2));
        this->m_xmlw->writeTextElement("pICMSInterPart", CppUtility::doubleToStrDecimal(_icmsUFDest->get_pICMSInterPart(), 4));
        if (_icmsUFDest->get_vFCPUFDest() > 0)
            this->m_xmlw->writeTextElement("vFCPUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_vFCPUFDest(), 2));
        this->m_xmlw->writeTextElement("vICMSUFDest", CppUtility::doubleToStrDecimal(_icmsUFDest->get_vICMSUFDest(), 2));
        this->m_xmlw->writeTextElement("vICMSUFRemet", CppUtility::doubleToStrDecimal(_icmsUFDest->get_vICMSUFRemet(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo ICMSUFDest
    }
}

void XmlWrite::get_IPI(const int &i)
{   //IPI - O01
    Ipi *_ipi = this->m_infNFe->det->value(i)->imposto->IPI.get();
    if (_ipi->get_CST() != CstIPI::None ||
        !_ipi->get_CNPJProd().isEmpty() ||
        !_ipi->get_cSelo().isEmpty()    ||
        _ipi->get_qSelo() > 0           ||
        !_ipi->get_cEnq().isEmpty())
    {
        this->m_xmlw->writeStartElement("IPI"); //abertura grupo IPI
        if (!_ipi->get_CNPJProd().isEmpty())
            this->m_xmlw->writeTextElement("CNPJProd", _ipi->get_CNPJProd());
        if (!_ipi->get_cSelo().isEmpty())
            this->m_xmlw->writeTextElement("cSelo", _ipi->get_cSelo());
        if (_ipi->get_qSelo() > 0)
            this->m_xmlw->writeTextElement("qSelo", QString::number(_ipi->get_qSelo()));
        this->m_xmlw->writeTextElement("cEnq", _ipi->get_cEnq());
        //IPITrib - O07
        if (_ipi->get_CST() == CstIPI::IPI00 ||
            _ipi->get_CST() == CstIPI::IPI49 ||
            _ipi->get_CST() == CstIPI::IPI50 ||
            _ipi->get_CST() == CstIPI::IPI99)
        {
            this->m_xmlw->writeStartElement("IPITrib"); //abertura grupo IPITrib
            this->m_xmlw->writeTextElement("CST", ConvNF::cstIPIToStr(_ipi->get_CST()));
            //Informar os campos O10 e O13 se o cálculo do IPI for por alíquota.
            if (_ipi->get_vBC() > 0 ||
                _ipi->get_pIPI() > 0)
            {
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_ipi->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pIPI", CppUtility::doubleToStrDecimal(_ipi->get_pIPI(), 4));
            }
            //Informar os campos O11 e O12 se o cálculo do IPI for de valor por unidade.
            if (_ipi->get_qUnid() > 0 ||
                _ipi->get_vUnid() > 0)
            {
                this->m_xmlw->writeTextElement("qUnid", CppUtility::doubleToStrDecimal(_ipi->get_qUnid(), 4));
                this->m_xmlw->writeTextElement("vUnid", CppUtility::doubleToStrDecimal(_ipi->get_vUnid(), 4));
            }
            this->m_xmlw->writeTextElement("vIPI", CppUtility::doubleToStrDecimal(_ipi->get_vIPI(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo IPITrib
        }
        //IPINT - O08
        if (_ipi->get_CST() == CstIPI::IPI01 ||
            _ipi->get_CST() == CstIPI::IPI02 ||
            _ipi->get_CST() == CstIPI::IPI03 ||
            _ipi->get_CST() == CstIPI::IPI04 ||
            _ipi->get_CST() == CstIPI::IPI51 ||
            _ipi->get_CST() == CstIPI::IPI52 ||
            _ipi->get_CST() == CstIPI::IPI53 ||
            _ipi->get_CST() == CstIPI::IPI54 ||
            _ipi->get_CST() == CstIPI::IPI55)
        {
            this->m_xmlw->writeStartElement("IPINT"); //abertura grupo IPINT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstIPIToStr(_ipi->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo IPINT
        }

        this->m_xmlw->writeEndElement();//fechamento grupo IPI

    }
}

void XmlWrite::get_II(const int &i)
{   //II - P01
    Ii *_ii = this->m_infNFe->det->value(i)->imposto->II.get();
    //para condição so o vBC basta ja que todos são obrigatorios, caso queira exibir o grupo
    if (_ii->get_vBC() > 0)
    {
        this->m_xmlw->writeStartElement("II"); //abertura grupo II
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_ii->get_vBC(), 2));
        this->m_xmlw->writeTextElement("vDespAdu", CppUtility::doubleToStrDecimal(_ii->get_vDespAdu(), 2));
        this->m_xmlw->writeTextElement("vII", CppUtility::doubleToStrDecimal(_ii->get_vII(), 2));
        this->m_xmlw->writeTextElement("vIOF", CppUtility::doubleToStrDecimal(_ii->get_vIOF(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo II
    }
}

void XmlWrite::get_PIS(const int &i)
{   //PIS - Q01
    Pis *_pis = this->m_infNFe->det->value(i)->imposto->PIS.get();
    //so o CST basta ja que e obrigatorio
    if (_pis->get_CST() != CstPIS::None)
    {
        this->m_xmlw->writeStartElement("PIS");//abertura grupo PIS
        //PISAliq - Q02
        if (_pis->get_CST() == CstPIS::PIS01 ||
            _pis->get_CST() == CstPIS::PIS02)
        {
            this->m_xmlw->writeStartElement("PISAliq");//abertura grupo PISAliq
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(_pis->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_pis->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(_pis->get_pPIS(), 4));
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_pis->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISAliq
        }
        //PISQtde - Q03
        if (_pis->get_CST() == CstPIS::PIS03)
        {
            this->m_xmlw->writeStartElement("PISQtde");//abertura grupo PISQtde
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(_pis->get_CST()));
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_pis->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_pis->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_pis->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISQtde
        }
        //PISNT - Q04
        if (_pis->get_CST() == CstPIS::PIS04 ||
            _pis->get_CST() == CstPIS::PIS05 ||
            _pis->get_CST() == CstPIS::PIS06 ||
            _pis->get_CST() == CstPIS::PIS07 ||
            _pis->get_CST() == CstPIS::PIS08 ||
            _pis->get_CST() == CstPIS::PIS09)
        {
            this->m_xmlw->writeStartElement("PISNT");//abertura grupo PISNT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(_pis->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo PISNT
        }
        //PISOutr - Q05
        if (_pis->get_CST() == CstPIS::PIS49 ||
            _pis->get_CST() == CstPIS::PIS50 ||
            _pis->get_CST() == CstPIS::PIS51 ||
            _pis->get_CST() == CstPIS::PIS52 ||
            _pis->get_CST() == CstPIS::PIS53 ||
            _pis->get_CST() == CstPIS::PIS54 ||
            _pis->get_CST() == CstPIS::PIS56 ||
            _pis->get_CST() == CstPIS::PIS56 ||
            _pis->get_CST() == CstPIS::PIS60 ||
            _pis->get_CST() == CstPIS::PIS61 ||
            _pis->get_CST() == CstPIS::PIS62 ||
            _pis->get_CST() == CstPIS::PIS63 ||
            _pis->get_CST() == CstPIS::PIS64 ||
            _pis->get_CST() == CstPIS::PIS65 ||
            _pis->get_CST() == CstPIS::PIS66 ||
            _pis->get_CST() == CstPIS::PIS67 ||
            _pis->get_CST() == CstPIS::PIS70 ||
            _pis->get_CST() == CstPIS::PIS71 ||
            _pis->get_CST() == CstPIS::PIS72 ||
            _pis->get_CST() == CstPIS::PIS73 ||
            _pis->get_CST() == CstPIS::PIS74 ||
            _pis->get_CST() == CstPIS::PIS75 ||
            _pis->get_CST() == CstPIS::PIS98 ||
            _pis->get_CST() == CstPIS::PIS99)
        {
            this->m_xmlw->writeStartElement("PISOutr");//abertura grupo PISOutr
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(_pis->get_CST()));
            //Informar os campos Q07 e Q08 se o cálculo do PIS em percentual.
            //caso não seja por valor o padrão será percentual
            if (_pis->get_qBCProd() == 0 ||
                _pis->get_vAliqProd() == 0)
            {
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_pis->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(_pis->get_pPIS(), 4));
            }
            else //Informar os campos Q10 e Q11 se o cálculo do PIS for em valor.
            {
                this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_pis->get_qBCProd(), 4));
                this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_pis->get_vAliqProd(), 4));
            }
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_pis->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISNT
        }
        this->m_xmlw->writeEndElement();//fechamento grupo PIS
    }
}

void XmlWrite::get_PISST(const int &i)
{   //PISST - R01
    PisST *_pisST = this->m_infNFe->det->value(i)->imposto->PISST.get();
    if (_pisST->get_vBC() > 0       ||
        _pisST->get_pPIS() > 0      ||
        _pisST->get_qBCProd() > 0   ||
        _pisST->get_vAliqProd() > 0 ||
        _pisST->get_vPIS() > 0       )
    {
        this->m_xmlw->writeStartElement("PISST");//abertura grupo PISST
        //Informar os campos R02 e R03 para cálculo do PIS em percentual.
        //caso não seja por valor o padrão será percentual
        if (_pisST->get_qBCProd() == 0 ||
            _pisST->get_vAliqProd() == 0)
        {
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_pisST->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(_pisST->get_pPIS(), 4));
        }
        else //Informar os campos R04 e R05 para cálculo do PIS em valor.
        {
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_pisST->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_pisST->get_vAliqProd(), 4));
        }
        this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_pisST->get_vPIS(), 2));
        this->m_xmlw->writeTextElement("indSomaPISST", ConvNF::indTotToStr(_pisST->get_indSomaPISST()));
        this->m_xmlw->writeEndElement();//fechamento grupo PISST
    }
}

void XmlWrite::get_COFINS(const int &i)
{   //COFINS - S01
    Cofins *_cofins = this->m_infNFe->det->value(i)->imposto->COFINS.get();
    //so o CST basta ja que e obrigatorio
    if (_cofins->get_CST() != CstCOFINS::None)
    {
        this->m_xmlw->writeStartElement("COFINS");//abertura grupo COFINS
        //COFINSAliq - S02
        if (_cofins->get_CST() == CstCOFINS::COFINS01 ||
            _cofins->get_CST() == CstCOFINS::COFINS02)
        {
            this->m_xmlw->writeStartElement("COFINSAliq");//abertura grupo COFINSAliq
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(_cofins->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_cofins->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(_cofins->get_pCOFINS(), 4));
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_cofins->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSAliq
        }
        //COFINSQtde - S03
        if (_cofins->get_CST() == CstCOFINS::COFINS03)
        {
            this->m_xmlw->writeStartElement("COFINSQtde");//abertura grupo COFINSQtde
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(_cofins->get_CST()));
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_cofins->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_cofins->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_cofins->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSQtde
        }
        //COFINSNT - S04
        if (_cofins->get_CST() == CstCOFINS::COFINS04 ||
            _cofins->get_CST() == CstCOFINS::COFINS05 ||
            _cofins->get_CST() == CstCOFINS::COFINS06 ||
            _cofins->get_CST() == CstCOFINS::COFINS07 ||
            _cofins->get_CST() == CstCOFINS::COFINS08 ||
            _cofins->get_CST() == CstCOFINS::COFINS09)
        {
            this->m_xmlw->writeStartElement("COFINSNT");//abertura grupo COFINSNT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(_cofins->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSNT
        }
        //COFINSOutr - S05
        if (_cofins->get_CST() == CstCOFINS::COFINS49 ||
            _cofins->get_CST() == CstCOFINS::COFINS50 ||
            _cofins->get_CST() == CstCOFINS::COFINS51 ||
            _cofins->get_CST() == CstCOFINS::COFINS52 ||
            _cofins->get_CST() == CstCOFINS::COFINS53 ||
            _cofins->get_CST() == CstCOFINS::COFINS54 ||
            _cofins->get_CST() == CstCOFINS::COFINS56 ||
            _cofins->get_CST() == CstCOFINS::COFINS56 ||
            _cofins->get_CST() == CstCOFINS::COFINS60 ||
            _cofins->get_CST() == CstCOFINS::COFINS61 ||
            _cofins->get_CST() == CstCOFINS::COFINS62 ||
            _cofins->get_CST() == CstCOFINS::COFINS63 ||
            _cofins->get_CST() == CstCOFINS::COFINS64 ||
            _cofins->get_CST() == CstCOFINS::COFINS65 ||
            _cofins->get_CST() == CstCOFINS::COFINS66 ||
            _cofins->get_CST() == CstCOFINS::COFINS67 ||
            _cofins->get_CST() == CstCOFINS::COFINS70 ||
            _cofins->get_CST() == CstCOFINS::COFINS71 ||
            _cofins->get_CST() == CstCOFINS::COFINS72 ||
            _cofins->get_CST() == CstCOFINS::COFINS73 ||
            _cofins->get_CST() == CstCOFINS::COFINS74 ||
            _cofins->get_CST() == CstCOFINS::COFINS75 ||
            _cofins->get_CST() == CstCOFINS::COFINS98 ||
            _cofins->get_CST() == CstCOFINS::COFINS99)
        {
            this->m_xmlw->writeStartElement("COFINSOutr");//abertura grupo COFINSOutr
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(_cofins->get_CST()));
            //Informar os campos S07 e S08 para cálculo da COFINS em percentual.
            //caso não seja por valor o padrão será percentual
            if (_cofins->get_qBCProd() == 0 ||
                _cofins->get_vAliqProd() == 0)
            {
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_cofins->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(_cofins->get_pCOFINS(), 4));
            }
            else//Informar os campos S09 e S10 para cálculo da COFINS em valor.
            {
                this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_cofins->get_qBCProd(), 4));
                this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_cofins->get_vAliqProd(), 4));
            }
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_cofins->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSNT
        }
        this->m_xmlw->writeEndElement();//fechamento grupo COFINS
    }
}

void XmlWrite::get_COFINSST(const int &i)
{   //COFINSST - T01
    CofinsST *_cofinsST = this->m_infNFe->det->value(i)->imposto->COFINSST.get();
    if (_cofinsST->get_vBC() > 0       ||
        _cofinsST->get_pCOFINS() > 0      ||
        _cofinsST->get_qBCProd() > 0   ||
        _cofinsST->get_vAliqProd() > 0 ||
        _cofinsST->get_vCOFINS() > 0)
    {
        this->m_xmlw->writeStartElement("COFINSST");//abertura grupo COFINSST
        //Informar os campos T02 e T03 para cálculo da COFINS Substituição Tributária em percentual.
        //caso não seja por valor o padrão será percentual
        if (_cofinsST->get_qBCProd() == 0 ||
            _cofinsST->get_vAliqProd() == 0)
        {
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_cofinsST->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(_cofinsST->get_pCOFINS(), 4));
        }
        else//Informar os campos T04 e T05 para cálculo da COFINS Substituição Tributária em valor.
        {
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(_cofinsST->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(_cofinsST->get_vAliqProd(), 4));
        }
        this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_cofinsST->get_vCOFINS(), 2));
        this->m_xmlw->writeTextElement("indSomaCOFINSST", ConvNF::indTotToStr(_cofinsST->get_indSomaCOFINSST()));
        this->m_xmlw->writeEndElement();//fechamento grupo COFINSST
    }
}

void XmlWrite::get_ISSQN(const int &i)
{   //ISSQN - U01
    IssQN *_issqn = this->m_infNFe->det->value(i)->imposto->ISSQN.get();
    if (this->m_infNFe->det->value(i)->imposto->ICMS->get_CST() == CstICMS::None &&
        this->m_infNFe->det->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::None)
    {
        this->m_xmlw->writeStartElement("ISSQN");//abertura grupo ISSQN
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_issqn->get_vBC(), 2));
        this->m_xmlw->writeTextElement("vAliq", CppUtility::doubleToStrDecimal(_issqn->get_vAliq(), 4));
        this->m_xmlw->writeTextElement("vISSQN", CppUtility::doubleToStrDecimal(_issqn->get_vISSQN(), 2));
        this->m_xmlw->writeTextElement("cMunFG", QString::number(_issqn->get_cMunFG()));
        this->m_xmlw->writeTextElement("cListServ", _issqn->get_cListServ());
        if (_issqn->get_vDeducao() > 0)
            this->m_xmlw->writeTextElement("vDeducao", CppUtility::doubleToStrDecimal(_issqn->get_vDeducao(), 2));
        if (_issqn->get_vOutro() > 0)
            this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(_issqn->get_vOutro(), 2));
        if (_issqn->get_vDescIncond() > 0)
            this->m_xmlw->writeTextElement("vDescIncond", CppUtility::doubleToStrDecimal(_issqn->get_vDescIncond(), 2));
        if (_issqn->get_vDescCond() > 0)
            this->m_xmlw->writeTextElement("vDescCond", CppUtility::doubleToStrDecimal(_issqn->get_vDescCond(), 2));
        if (_issqn->get_vISSRet() > 0)
            this->m_xmlw->writeTextElement("vISSRet", CppUtility::doubleToStrDecimal(_issqn->get_vISSRet(), 2));
        this->m_xmlw->writeTextElement("indISS", ConvNF::indISSToStr(_issqn->get_indISS()));
        if (!_issqn->get_cServico().isEmpty())
            this->m_xmlw->writeTextElement("cServico", _issqn->get_cServico());
        if (_issqn->get_cMun() > 0)
            this->m_xmlw->writeTextElement("cMun", QString::number(_issqn->get_cMun()));
        if (_issqn->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(_issqn->get_cPais()));
        if (!_issqn->get_nProcesso().isEmpty())
            this->m_xmlw->writeTextElement("nProcesso", _issqn->get_nProcesso());
        this->m_xmlw->writeTextElement("indIncentivo", ConvNF::indIncentivoToStr(_issqn->get_indIncentivo()));
        this->m_xmlw->writeEndElement();//fechamento grupo ISSQN
    }
}

void XmlWrite::get_impostoDevol(const int &i)
{   //impostoDevol - UA01
    if (this->m_infNFe->det->value(i)->impostoDevol->get_pDevol() > 0)
    {
        this->m_xmlw->writeStartElement("impostoDevol");//abertura grupo impostoDevol
        this->m_xmlw->writeTextElement("pDevol", CppUtility::doubleToStrDecimal(this->m_infNFe->det->value(i)->impostoDevol->get_pDevol(), 2));
        this->m_xmlw->writeStartElement("IPI");//abertura grupo IPI
        this->m_xmlw->writeTextElement("vIPIDevol", CppUtility::doubleToStrDecimal(this->m_infNFe->det->value(i)->impostoDevol->get_vIPIDevol(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo IPI
        this->m_xmlw->writeEndElement();//fechamento grupo impostoDevol
    }
}

void XmlWrite::get_obsItem(const int &i)
{   //obsItem - VA01
    ObsItem *_obsItem = this->m_infNFe->det->value(i)->obsItem.get();
    if (!_obsItem->obsCont->get_xCampo().isEmpty()  ||
        !_obsItem->obsCont->get_xTexto().isEmpty()  ||
        !_obsItem->obsFisco->get_xCampo().isEmpty() ||
        !_obsItem->obsFisco->get_xTexto().isEmpty())
    {
        //obsCont - VA02
        if (!_obsItem->obsCont->get_xCampo().isEmpty() ||
            !_obsItem->obsCont->get_xTexto().isEmpty())
        {
            this->m_xmlw->writeStartElement("obsCont");//abertura grupo obsCont
            this->m_xmlw->writeTextElement("xCampo", _obsItem->obsCont->get_xCampo());
            this->m_xmlw->writeTextElement("xTexto", _obsItem->obsCont->get_xTexto());
            this->m_xmlw->writeEndElement();//fechamento grupo obsCont
        }
        //obsFisco - VA05
        if (!_obsItem->obsFisco->get_xCampo().isEmpty() ||
            !_obsItem->obsFisco->get_xTexto().isEmpty())
        {
            this->m_xmlw->writeStartElement("obsFisco");//abertura grupo obsFisco
            this->m_xmlw->writeTextElement("xCampo", _obsItem->obsFisco->get_xCampo());
            this->m_xmlw->writeTextElement("xTexto", _obsItem->obsFisco->get_xTexto());
            this->m_xmlw->writeEndElement();//fechamento grupo obsFisco
        }
    }
}

void XmlWrite::get_total()
{
    IcmsTot *_icmsTot = this->m_infNFe->total->ICMSTot.get();
    IssQNtot *_issqnTot = this->m_infNFe->total->ISSQNtot.get();
    RetTrib *_retTrib = this->m_infNFe->total->retTrib.get();
    //total - W01
    this->m_xmlw->writeStartElement("total");//abertura grupo total

    //ICMSTot - W02
    this->m_xmlw->writeStartElement("ICMSTot");//abertura grupo ICMSTot
    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_icmsTot->get_vBC(), 2));
    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMS(), 2));
    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSDeson(), 2));
    if (_icmsTot->get_vFCPUFDest() > 0)
        this->m_xmlw->writeTextElement("vFCPUFDest", CppUtility::doubleToStrDecimal(_icmsTot->get_vFCPUFDest(), 2));
    if (_icmsTot->get_vICMSUFDest() > 0)
        this->m_xmlw->writeTextElement("vICMSUFDest", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSUFDest(), 2));
    if (_icmsTot->get_vICMSUFRemet() > 0)
        this->m_xmlw->writeTextElement("vICMSUFRemet", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSUFRemet(), 2));
    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(_icmsTot->get_vFCP(), 2));
    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(_icmsTot->get_vBCST(), 2));
    this->m_xmlw->writeTextElement("vST", CppUtility::doubleToStrDecimal(_icmsTot->get_vST(), 2));
    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(_icmsTot->get_vFCPST(), 2));
    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(_icmsTot->get_vFCPSTRet(), 2));
    this->m_xmlw->writeTextElement("qBCMono", CppUtility::doubleToStrDecimal(_icmsTot->get_qBCMono(), 2));
    this->m_xmlw->writeTextElement("vICMSMono", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSMono(), 2));
    this->m_xmlw->writeTextElement("qBCMonoReten", CppUtility::doubleToStrDecimal(_icmsTot->get_qBCMonoReten(), 2));
    this->m_xmlw->writeTextElement("vICMSMonoReten", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSMonoReten(), 2));
    this->m_xmlw->writeTextElement("qBCMonoRet", CppUtility::doubleToStrDecimal(_icmsTot->get_qBCMonoRet(), 2));
    this->m_xmlw->writeTextElement("vICMSMonoRet", CppUtility::doubleToStrDecimal(_icmsTot->get_vICMSMonoRet(), 2));
    this->m_xmlw->writeTextElement("vProd", CppUtility::doubleToStrDecimal(_icmsTot->get_vProd(), 2));
    this->m_xmlw->writeTextElement("vFrete", CppUtility::doubleToStrDecimal(_icmsTot->get_vFrete(), 2));
    this->m_xmlw->writeTextElement("vSeg", CppUtility::doubleToStrDecimal(_icmsTot->get_vSeg(), 2));
    this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(_icmsTot->get_vDesc(), 2));
    this->m_xmlw->writeTextElement("vII", CppUtility::doubleToStrDecimal(_icmsTot->get_vII(), 2));
    this->m_xmlw->writeTextElement("vIPI", CppUtility::doubleToStrDecimal(_icmsTot->get_vIPI(), 2));
    this->m_xmlw->writeTextElement("vIPIDevol", CppUtility::doubleToStrDecimal(_icmsTot->get_vIPIDevol(), 2));
    this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_icmsTot->get_vPIS(), 2));
    this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_icmsTot->get_vCOFINS(), 2));
    this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(_icmsTot->get_vOutro(), 2));
    this->m_xmlw->writeTextElement("vNF", CppUtility::doubleToStrDecimal(_icmsTot->get_vNF(), 2));
    if (_icmsTot->get_vTotTrib() > 0)
        this->m_xmlw->writeTextElement("vTotTrib", CppUtility::doubleToStrDecimal(_icmsTot->get_vTotTrib(), 2));
    this->m_xmlw->writeEndElement();//fechamento grupo ICMSTot

    //ISSQNtot - W17
    if(_issqnTot->get_vServ() > 0 ||
       _issqnTot->get_vBC () > 0  ||
       _issqnTot->get_vISS() > 0  ||
       _issqnTot->get_vPIS() > 0  ||
       _issqnTot->get_vCOFINS() > 0)
    {
        this->m_xmlw->writeStartElement("ISSQNtot");//abertura grupo ISSQNtot
        if (_issqnTot->get_vServ() > 0)
            this->m_xmlw->writeTextElement("vServ", CppUtility::doubleToStrDecimal(_issqnTot->get_vServ(), 2));
        if (_issqnTot->get_vBC() > 0)
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(_issqnTot->get_vBC(), 2));
        if (_issqnTot->get_vISS() > 0)
            this->m_xmlw->writeTextElement("vISS", CppUtility::doubleToStrDecimal(_issqnTot->get_vISS(), 2));
        if (_issqnTot->get_vPIS() > 0)
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(_issqnTot->get_vPIS(), 2));
        if (_issqnTot->get_vCOFINS() > 0)
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(_issqnTot->get_vCOFINS(), 2));
        this->m_xmlw->writeTextElement("dCompet", CppUtility::dateTimeToStr(_issqnTot->get_dCompet(), DtH::Date));
        if (_issqnTot->get_vDeducao() > 0)
            this->m_xmlw->writeTextElement("vDeducao", CppUtility::doubleToStrDecimal(_issqnTot->get_vDeducao(), 2));
        if (_issqnTot->get_vOutro() > 0)
            this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(_issqnTot->get_vOutro(), 2));
        if (_issqnTot->get_vDescIncond() > 0)
            this->m_xmlw->writeTextElement("vDescIncond", CppUtility::doubleToStrDecimal(_issqnTot->get_vDescIncond(), 2));
        if (_issqnTot->get_vDescCond() > 0)
            this->m_xmlw->writeTextElement("vDescCond", CppUtility::doubleToStrDecimal(_issqnTot->get_vDescCond(), 2));
        if (_issqnTot->get_vISSRet() > 0)
            this->m_xmlw->writeTextElement("vISSRet", CppUtility::doubleToStrDecimal(_issqnTot->get_vISSRet(), 2));
        if (_issqnTot->get_cRegTrib() != CRegTrib::None)
            this->m_xmlw->writeTextElement("cRegTrib", ConvNF::cRegTribToStr(_issqnTot->get_cRegTrib()));
        this->m_xmlw->writeEndElement();//fechamento grupo ISSQNtot
    }

    //retTrib - W23
    if (_retTrib->get_vRetPIS() > 0 ||
        _retTrib->get_vRetCOFINS() > 0 ||
        _retTrib->get_vRetCSLL() > 0 ||
        _retTrib->get_vBCIRRF() > 0 ||
        _retTrib->get_vIRRF() > 0 ||
        _retTrib->get_vBCRetPrev() > 0 ||
        _retTrib->get_vRetPrev() > 0 )
    {
        this->m_xmlw->writeStartElement("retTrib");//abertura grupo retTrib
        if (_retTrib->get_vRetPIS() > 0)
            this->m_xmlw->writeTextElement("vRetPIS", CppUtility::doubleToStrDecimal(_retTrib->get_vRetPIS(), 2));
        if (_retTrib->get_vRetCOFINS() > 0)
            this->m_xmlw->writeTextElement("vRetCOFINS", CppUtility::doubleToStrDecimal(_retTrib->get_vRetCOFINS(), 2));
        if (_retTrib->get_vRetCSLL() > 0)
            this->m_xmlw->writeTextElement("vRetCSLL", CppUtility::doubleToStrDecimal(_retTrib->get_vRetCSLL(), 2));
        if (_retTrib->get_vBCIRRF() > 0)
            this->m_xmlw->writeTextElement("vBCIRRF", CppUtility::doubleToStrDecimal(_retTrib->get_vBCIRRF(), 2));
        if (_retTrib->get_vIRRF() > 0)
            this->m_xmlw->writeTextElement("vIRRF", CppUtility::doubleToStrDecimal(_retTrib->get_vIRRF(), 2));
        if (_retTrib->get_vBCRetPrev() > 0)
            this->m_xmlw->writeTextElement("vBCRetPrev", CppUtility::doubleToStrDecimal(_retTrib->get_vBCRetPrev(), 2));
        if (_retTrib->get_vRetPrev() > 0)
            this->m_xmlw->writeTextElement("vRetPrev", CppUtility::doubleToStrDecimal(_retTrib->get_vRetPrev(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo retTrib
    }

    get_ISTot_IBSCBSTot();
    this->m_xmlw->writeEndElement();//fechamento grupo total
}

void XmlWrite::get_transp()
{   //transp - X01
    Transp *_transp = this->m_infNFe->transp.get();
    Transporta *_transporta = _transp->transporta.get();
    RetTransp *_retTransp = _transp->retTransp.get();
    VeicTransp *_veicTransp = _transp->veicTransp.get();
    this->m_xmlw->writeStartElement("transp");//abertura grupo transp
    this->m_xmlw->writeTextElement("modFrete", ConvNF::modFreteToStr(_transp->get_modFrete()));

    //transporta - X03
    if (_transp->get_modFrete() != ModFrete::SemFrete)
    {
        if (!_transporta->get_CNPJ().isEmpty() ||
            !_transporta->get_CPF().isEmpty() ||
            !_transporta->get_xNome().isEmpty() ||
            !_transporta->get_IE().isEmpty() ||
            !_transporta->get_xEnder().isEmpty() ||
            !_transporta->get_xMun().isEmpty() ||
            !_transporta->get_UF().isEmpty())
        {
            this->m_xmlw->writeStartElement("transporta");//abertura grupo transporta
            if (!_transporta->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", _transporta->get_CNPJ());
            else if (!_transporta->get_CPF().isEmpty())
                this->m_xmlw->writeTextElement("CPF", _transporta->get_CPF());
            if (!_transporta->get_xNome().isEmpty())
                this->m_xmlw->writeTextElement("xNome", _transporta->get_xNome());
            if (!_transporta->get_IE().isEmpty())
                this->m_xmlw->writeTextElement("IE", _transporta->get_IE());
            if (!_transporta->get_xEnder().isEmpty())
                this->m_xmlw->writeTextElement("xEnder", _transporta->get_xEnder());
            if (!_transporta->get_xMun().isEmpty())
                this->m_xmlw->writeTextElement("xMun", _transporta->get_xMun());
            if (!_transporta->get_UF().isEmpty())
                this->m_xmlw->writeTextElement("UF", _transporta->get_UF());
            this->m_xmlw->writeEndElement();//fechamento grupo transporta
        }
    }
    //retTransp - X11
    if (_retTransp->get_vServ() > 0 ||
        _retTransp->get_vBCRet() > 0 ||
        _retTransp->get_pICMSRet() > 0 ||
        _retTransp->get_vICMSRet() > 0 ||
        !_retTransp->get_CFOP().isEmpty() ||
        _retTransp->get_cMunFG() > 0 )
    {
        this->m_xmlw->writeStartElement("retTransp");//abertura grupo retTransp
        this->m_xmlw->writeTextElement("vServ", CppUtility::doubleToStrDecimal(_retTransp->get_vServ(), 2));
        this->m_xmlw->writeTextElement("vBCRet", CppUtility::doubleToStrDecimal(_retTransp->get_vBCRet(), 2));
        this->m_xmlw->writeTextElement("pICMSRet", CppUtility::doubleToStrDecimal(_retTransp->get_pICMSRet(), 4));
        this->m_xmlw->writeTextElement("vICMSRet", CppUtility::doubleToStrDecimal(_retTransp->get_vICMSRet(), 2));
        this->m_xmlw->writeTextElement("CFOP", _retTransp->get_CFOP());
        this->m_xmlw->writeTextElement("cMunFG", QString::number(_retTransp->get_cMunFG()));
        this->m_xmlw->writeEndElement();//fechamento grupo retTransp
    }
    //veicTransp - X18
    if (!_veicTransp->get_placa().isEmpty() ||
        !_veicTransp->get_UF().isEmpty() ||
        !_veicTransp->get_RNTC().isEmpty())
    {
        this->m_xmlw->writeStartElement("veicTransp");//abertura grupo veicTransp
        this->m_xmlw->writeTextElement("placa", _veicTransp->get_placa());
        this->m_xmlw->writeTextElement("UF", _veicTransp->get_UF());
        if (!_veicTransp->get_RNTC().isEmpty())
            this->m_xmlw->writeTextElement("RNTC", _veicTransp->get_RNTC());
        this->m_xmlw->writeEndElement();//fechamento grupo veicTransp
    }
    //reboque - X22
    if (_transp->reboque->count() > 0)
    {
        for(int i = 0; i < _transp->reboque->count(); ++i)
        {
            Reboque *_reboque = _transp->reboque->value(i);
            this->m_xmlw->writeStartElement("reboque");//abertura grupo reboque
            this->m_xmlw->writeTextElement("placa", _reboque->get_placa());
            this->m_xmlw->writeTextElement("UF", _reboque->get_UF());
            if (!_reboque->get_RNTC().isEmpty())
                this->m_xmlw->writeTextElement("RNTC", _reboque->get_RNTC());
            this->m_xmlw->writeEndElement();//fechamento grupo reboque
        }
    }
    //continuacao de X01
    if (!_transp->get_vagao().isEmpty())
        this->m_xmlw->writeTextElement("vagao", _transp->get_vagao());
    if (!_transp->get_balsa().isEmpty())
        this->m_xmlw->writeTextElement("balsa", _transp->get_balsa());
    //vol - X26
    if (_transp->vol->count() > 0)
    {
        for(int i = 0; i < _transp->vol->count(); ++i)
        {
            Vol *_vol = _transp->vol->value(i);
            this->m_xmlw->writeStartElement("vol");//abertura grupo vol
            if (_vol->get_qVol() > 0)
                this->m_xmlw->writeTextElement("qVol", QString::number(_vol->get_qVol()));
            if (!_vol->get_esp().isEmpty())
                this->m_xmlw->writeTextElement("esp", _vol->get_esp());
            if (!_vol->get_marca().isEmpty())
                this->m_xmlw->writeTextElement("marca", _vol->get_marca());
            if (!_vol->get_nVol().isEmpty())
                this->m_xmlw->writeTextElement("nVol", _vol->get_nVol());
            if (_vol->get_pesoL() > 0)
                this->m_xmlw->writeTextElement("pesoL", CppUtility::doubleToStrDecimal(_vol->get_pesoL(), 3));;
            if (_vol->get_pesoB() > 0)
                this->m_xmlw->writeTextElement("pesoB", CppUtility::doubleToStrDecimal(_vol->get_pesoB(), 3));;
            //lacres - X33
            if (_vol->lacres->count() > 0)
            {
                for(int j = 0; j < _vol->lacres->count(); ++j)
                {
                    this->m_xmlw->writeStartElement("lacres");//abertura grupo lacres
                    this->m_xmlw->writeTextElement("nLacre", _vol->lacres->value(j)->get_nLacre());
                    this->m_xmlw->writeEndElement();//fechamento grupo lacres
                }
            }
            this->m_xmlw->writeEndElement();//fechamento grupo vol
        }
    }

    this->m_xmlw->writeEndElement();//fechamento grupo transp
}

void XmlWrite::get_cobr()
{   //cobr - Y01
    Cobr *_cobr = this->m_infNFe->cobr.get();
    Fat *_fat = _cobr->fat.get();
    if (!_fat->get_nFat().isEmpty() ||
        _fat->get_vOrig() > 0 ||
        _fat->get_vDesc() > 0 ||
        _fat->get_vLiq() > 0 ||
        _cobr->dup->count() > 0)
    {
        this->m_xmlw->writeStartElement("cobr");//abertura grupo cobr
        //fat - Y02
        if (!_fat->get_nFat().isEmpty() ||
            _fat->get_vOrig() > 0 ||
            _fat->get_vDesc() > 0 ||
            _fat->get_vLiq() > 0)
        {
            this->m_xmlw->writeStartElement("fat");//abertura grupo fat
            if (!_fat->get_nFat().isEmpty())
                this->m_xmlw->writeTextElement("nFat", _fat->get_nFat());
            if (_fat->get_vOrig() > 0)
                this->m_xmlw->writeTextElement("vOrig", CppUtility::doubleToStrDecimal(_fat->get_vOrig(), 2));
            if (_fat->get_vDesc() > 0)
                this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(_fat->get_vDesc(), 2));
            if (_fat->get_vLiq() > 0)
                this->m_xmlw->writeTextElement("vLiq", CppUtility::doubleToStrDecimal(_fat->get_vLiq(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo fat
        }
        //dup - Y07
        if (_cobr->dup->count() > 0)
        {
            for(int i = 0; i < _cobr->dup->count(); ++i)
            {
                Dup *_dup = _cobr->dup->value(i);
                this->m_xmlw->writeStartElement("dup");//abertura grupo dup
                if (!_dup->get_nDup().isEmpty())
                    this->m_xmlw->writeTextElement("nDup", _dup->get_nDup());
                if (_dup->get_dVenc().isValid())
                    this->m_xmlw->writeTextElement("dVenc", CppUtility::dateTimeToStr(_dup->get_dVenc(), DtH::Date));
                this->m_xmlw->writeTextElement("vDup", CppUtility::doubleToStrDecimal(_dup->get_vDup(), 2));
                this->m_xmlw->writeEndElement();//fechamento grupo dup
            }
        }
        this->m_xmlw->writeEndElement();//fechamento grupo cobr
    }
}

void XmlWrite::get_pag()
{   //pag - YA01
    Pag *_pag = this->m_infNFe->pag.get();
    if (_pag->detPag->count() > 0 ||
        _pag->get_vTroco() > 0)
    {
        this->m_xmlw->writeStartElement("pag");//abertura grupo pag
        //detPag - YA01a
        if (_pag->detPag->count() > 0)
        {
            for(int i = 0; i < _pag->detPag->count(); ++i)
            {
                DetPag *_detPag = _pag->detPag->value(i);
                Card *_card = _detPag->card.get();
                this->m_xmlw->writeStartElement("detPag");//abertura grupo detPag
                if (_detPag->get_indPag() != IndPag::None)
                    this->m_xmlw->writeTextElement("indPag", ConvNF::indPagToStr(_detPag->get_indPag()));
                this->m_xmlw->writeTextElement("tPag", ConvNF::tPagToStr(_detPag->get_tPag()));
                if (!_detPag->get_xPag().isEmpty())
                    this->m_xmlw->writeTextElement("xPag", _detPag->get_xPag());
                this->m_xmlw->writeTextElement("vPag", CppUtility::doubleToStrDecimal(_detPag->get_vPag(), 2));
                if (_detPag->get_dPag().isValid())
                    this->m_xmlw->writeTextElement("dPag", _detPag->get_dPag().toString(Qt::ISODate));
                if (!_detPag->get_CNPJPag().isEmpty() || _detPag->get_UFPag() != UF::None)
                {
                    this->m_xmlw->writeTextElement("CNPJPag", _detPag->get_CNPJPag());
                    this->m_xmlw->writeTextElement("UFPag", ConvDF::ufToStr(_detPag->get_UFPag()));
                }
                //card - YA04
                if (_card->get_tpIntegra() != TpIntegra::None ||
                    !_card->get_CNPJ().isEmpty() ||
                    _card->get_tBand() != TBand::None ||
                    !_card->get_cAut().isEmpty() ||
                    !_card->get_CNPJReceb().isEmpty() ||
                    !_card->get_idTermPag().isEmpty())
                {
                    this->m_xmlw->writeStartElement("card");//abertura grupo card
                    this->m_xmlw->writeTextElement("tpIntegra", ConvNF::tpIntegraToStr(_card->get_tpIntegra()));
                    if (!_card->get_CNPJ().isEmpty())
                        this->m_xmlw->writeTextElement("CNPJ", _card->get_CNPJ());
                    if (_card->get_tBand() != TBand::None)
                        this->m_xmlw->writeTextElement("tBand", ConvNF::tBandToStr(_card->get_tBand()));
                    if (!_card->get_cAut().isEmpty())
                        this->m_xmlw->writeTextElement("cAut", _card->get_cAut());
                    if (!_card->get_CNPJReceb().isEmpty())
                        this->m_xmlw->writeTextElement("CNPJReceb", _card->get_CNPJReceb());
                    if (!_card->get_idTermPag().isEmpty())
                        this->m_xmlw->writeTextElement("idTermPag", _card->get_idTermPag());
                    this->m_xmlw->writeEndElement();//fechamento grupo card
                }

                this->m_xmlw->writeEndElement();//fechamento grupo detPag
            }
        }
        //vTroco dentro de YA01
        if (_pag->get_vTroco() > 0)
            this->m_xmlw->writeTextElement("vTroco", CppUtility::doubleToStrDecimal(_pag->get_vTroco(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo pag
    }
}

void XmlWrite::get_infIntermed()
{   //YB01 - infIntermed
    InfIntermed *_infIntermed = this->m_infNFe->infIntermed.get();
    if (!_infIntermed->get_CNPJ().isEmpty() ||
        !_infIntermed->get_idCadIntTran().isEmpty())
    {
        this->m_xmlw->writeStartElement("infIntermed");//abertura grupo infIntermed
        this->m_xmlw->writeTextElement("CNPJ", _infIntermed->get_CNPJ());
        this->m_xmlw->writeTextElement("idCadIntTran", _infIntermed->get_idCadIntTran());
        this->m_xmlw->writeEndElement();//fechamento grupo infIntermed
    }
}

void XmlWrite::get_infAdic()
{   //infAdic - Z01
    InfAdic *_infAdic = this->m_infNFe->infAdic.get();
    if (!_infAdic->get_infAdFisco().isEmpty() ||
        !_infAdic->get_infCpl().isEmpty() ||
        _infAdic->obsCont->count() > 0 ||
        _infAdic->obsFisco->count() > 0 ||
        _infAdic->procRef->count() > 0)
    {
        this->m_xmlw->writeStartElement("infAdic");//abertura grupo infAdic
        if (!_infAdic->get_infAdFisco().isEmpty())
            this->m_xmlw->writeTextElement("infAdFisco", _infAdic->get_infAdFisco());
        if (!_infAdic->get_infCpl().isEmpty())
            this->m_xmlw->writeTextElement("infCpl", _infAdic->get_infCpl());
        //obsCont - Z04
        if (_infAdic->obsCont->count() > 0)
        {
            for(int i = 0; i < _infAdic->obsCont->count(); ++i)
            {
                this->m_xmlw->writeStartElement("obsCont");//abertura grupo obsCont
                this->m_xmlw->writeTextElement("xCampo", _infAdic->obsCont->value(i)->get_xCampo());
                this->m_xmlw->writeTextElement("xTexto", _infAdic->obsCont->value(i)->get_xTexto());
                this->m_xmlw->writeEndElement();//fechamento grupo obsCont
            }
        }
        //obsFisco - Z07
        if (_infAdic->obsFisco->count() > 0)
        {
            for(int i = 0; i < _infAdic->obsFisco->count(); ++i)
            {
                this->m_xmlw->writeStartElement("obsFisco");//abertura grupo obsFisco
                this->m_xmlw->writeTextElement("xCampo", _infAdic->obsFisco->value(i)->get_xCampo());
                this->m_xmlw->writeTextElement("xTexto", _infAdic->obsFisco->value(i)->get_xTexto());
                this->m_xmlw->writeEndElement();//fechamento grupo obsFisco
            }
        }
        //procRef - Z10
        if (_infAdic->procRef->count() > 0)
        {
            for(int i = 0; i < _infAdic->procRef->count(); ++i)
            {
                ProcRef *_procRef = _infAdic->procRef->value(i);
                this->m_xmlw->writeStartElement("procRef");//abertura grupo procRef
                this->m_xmlw->writeTextElement("nProc", _procRef->get_nProc());
                this->m_xmlw->writeTextElement("indProc", ConvNF::indProcToStr(_procRef->get_indProc()));
                if (_procRef->get_tpAto() != TpAto::None)
                    this->m_xmlw->writeTextElement("tpAto", ConvNF::tpAtoToStr(_procRef->get_tpAto()));
                this->m_xmlw->writeEndElement();//fechamento grupo procRef
            }
        }

        this->m_xmlw->writeEndElement();//fechamento grupo infAdic
    }

}

void XmlWrite::get_exporta()
{   //exporta - ZA01
    Exporta *_exporta = this->m_infNFe->exporta.get();
    if (!_exporta->get_UFSaidaPais().isEmpty() ||
        !_exporta->get_xLocExporta().isEmpty() ||
        !_exporta->get_xLocDespacho().isEmpty())
    {
        this->m_xmlw->writeStartElement("exporta");//abertura grupo exporta
        this->m_xmlw->writeTextElement("UFSaidaPais", _exporta->get_UFSaidaPais());
        this->m_xmlw->writeTextElement("xLocExporta", _exporta->get_xLocExporta());
        if (!_exporta->get_xLocDespacho().isEmpty())
            this->m_xmlw->writeTextElement("xLocDespacho", _exporta->get_xLocDespacho());
        this->m_xmlw->writeEndElement();//fechamento grupo exporta
    }
}

void XmlWrite::get_compra()
{   //compra - ZB01
    Compra *_compra = this->m_infNFe->compra.get();
    if (!_compra->get_xNEmp().isEmpty() ||
        !_compra->get_xPed().isEmpty() ||
        !_compra->get_xCont().isEmpty())
    {
        this->m_xmlw->writeStartElement("compra");//abertura grupo compra
        if (!_compra->get_xNEmp().isEmpty())
            this->m_xmlw->writeTextElement("xNEmp", _compra->get_xNEmp());
        if (!_compra->get_xPed().isEmpty())
            this->m_xmlw->writeTextElement("xPed", _compra->get_xPed());
        if (!_compra->get_xCont().isEmpty())
            this->m_xmlw->writeTextElement("xCont", _compra->get_xCont());
        this->m_xmlw->writeEndElement();//fechamento grupo compra
    }
}

void XmlWrite::get_cana()
{   //cana - ZC01
    Cana *_cana = this->m_infNFe->cana.get();
    if (!_cana->get_safra().isEmpty() ||
        !_cana->get_ref().isEmpty())
    {
        this->m_xmlw->writeStartElement("cana");//abertura grupo cana
        this->m_xmlw->writeTextElement("safra", _cana->get_safra());
        this->m_xmlw->writeTextElement("ref", _cana->get_ref());
        //forDia - ZC04
        if (_cana->forDia->count() > 0)
        {
            for(int i = 0; i < _cana->forDia->count(); ++i)
            {
                this->m_xmlw->writeStartElement("forDia");//abertura grupo forDia
                this->m_xmlw->writeTextElement("dia", QString::number(_cana->forDia->value(i)->get_dia()));
                this->m_xmlw->writeTextElement("qtde", CppUtility::doubleToStrDecimal(_cana->forDia->value(i)->get_qtde(), 10));
                this->m_xmlw->writeEndElement();//fechamento grupo forDia
            }
        }
        //continuacao ZC01
        this->m_xmlw->writeTextElement("qTotMes", CppUtility::doubleToStrDecimal(_cana->get_qTotMes(), 10));
        this->m_xmlw->writeTextElement("qTotAnt", CppUtility::doubleToStrDecimal(_cana->get_qTotAnt(), 10));
        this->m_xmlw->writeTextElement("qTotGer", CppUtility::doubleToStrDecimal(_cana->get_qTotGer(), 10));

        //deduc - ZC10
        if (_cana->deduc->count() > 0)
        {
            for(int i = 0; i < _cana->deduc->count(); ++i)
            {
                this->m_xmlw->writeStartElement("deduc");//abertura grupo deduc
                this->m_xmlw->writeTextElement("xDed", _cana->deduc->value(i)->get_xDed());
                this->m_xmlw->writeTextElement("vDed", CppUtility::doubleToStrDecimal(_cana->deduc->value(i)->get_vDed(), 2));
                this->m_xmlw->writeEndElement();//fechamento grupo deduc
            }
        }
        //continuacao ZC01
        this->m_xmlw->writeTextElement("vFor", CppUtility::doubleToStrDecimal(_cana->get_vFor(), 2));
        this->m_xmlw->writeTextElement("vTotDed", CppUtility::doubleToStrDecimal(_cana->get_vTotDed(), 2));
        this->m_xmlw->writeTextElement("vLiqFor", CppUtility::doubleToStrDecimal(_cana->get_vLiqFor(), 2));

        this->m_xmlw->writeEndElement();//fechamento grupo cana
    }
}

void XmlWrite::get_infRespTec()
{   //infRespTec - ZD01
    InfRespTec *_infRespTec = this->m_infNFe->infRespTec.get();
    if (!_infRespTec->get_CNPJ().isEmpty())
    {
        this->m_xmlw->writeStartElement("infRespTec");//abertura grupo infRespTec
        this->m_xmlw->writeTextElement("CNPJ", _infRespTec->get_CNPJ());
        this->m_xmlw->writeTextElement("xContato", _infRespTec->get_xContato());
        this->m_xmlw->writeTextElement("email", _infRespTec->get_email());
        this->m_xmlw->writeTextElement("fone", _infRespTec->get_fone());
        //ZD07
        const QString hashCSRT = _infRespTec->hashCSRTParaChave(this->m_infNFe->get_Id().mid(3));
        if (_infRespTec->get_idCSRT() != 0 && !hashCSRT.isEmpty())
        {
            this->m_xmlw->writeTextElement("idCSRT",
                                          QStringLiteral("%1").arg(_infRespTec->get_idCSRT(), 2, 10, QLatin1Char('0')));
            this->m_xmlw->writeTextElement("hashCSRT", hashCSRT);
        }
        this->m_xmlw->writeEndElement();//fechamento grupo infRespTec
    }
}

void XmlWrite::get_agropecuario()
{
    const auto &agropecuario = *m_infNFe->agropecuario;
    const auto &defensivos = *agropecuario.defensivo;
    const auto &guia = *agropecuario.guiaTransito;
    const bool temDefensivos = !defensivos.isEmpty();
    const bool temGuia = !guia.isEmpty();
    if (!temDefensivos && !temGuia)
        return;

    m_xmlw->writeStartElement("agropecuario");
    for (const auto *defensivo : defensivos)
    {
        m_xmlw->writeStartElement("defensivo");
        m_xmlw->writeTextElement("nReceituario", defensivo->get_nReceituario());
        m_xmlw->writeTextElement("CPFRespTec", defensivo->get_CPFRespTec());
        m_xmlw->writeEndElement();
    }
    if (temGuia)
    {
        m_xmlw->writeStartElement("guiaTransito");
        m_xmlw->writeTextElement("tpGuia", QString::number(guia.get_tpGuia()));
        m_xmlw->writeTextElement("UFGuia", guia.get_UFGuia());
        if (!guia.get_serieGuia().isEmpty())
            m_xmlw->writeTextElement("serieGuia", guia.get_serieGuia());
        m_xmlw->writeTextElement("nGuia", guia.get_nGuia());
        m_xmlw->writeEndElement();
    }
    m_xmlw->writeEndElement();
}

namespace {
QString dec2(const double &valor)
{
    return CppUtility::doubleToStrDecimal(valor, 2);
}

QString dec4(const double &valor)
{
    return CppUtility::doubleToStrDecimal(valor, 4);
}
} // namespace

bool XmlWrite::validarRTC() const
{
    for (int i = 0; i < this->m_infNFe->det->count(); ++i)
    {
        const Imposto *_imposto = this->m_infNFe->det->value(i)->imposto.get();
        const int nItem = this->m_infNFe->det->value(i)->prod->get_nItem();
        const Is *_is = _imposto->IS.get();
        if (_is->get_CSTIS().isEmpty() && !_is->isEmpty())
        {
            qWarning() << "Item" << nItem << ": informe o CSTIS do grupo IS.";
            return false;
        }

        const IbsCbs *_ibscbs = _imposto->IBSCBS.get();
        if (_ibscbs->isEmpty())
            continue;
        if (_ibscbs->get_CST().isEmpty())
        {
            qWarning() << "Item" << nItem << ": informe o CST do grupo IBSCBS.";
            return false;
        }
        const int grupos = (_ibscbs->gIBSCBS->isEmpty() ? 0 : 1) +
                           (_ibscbs->gIBSCBSMono->isEmpty() ? 0 : 1) +
                           (_ibscbs->gTransfCred->isEmpty() ? 0 : 1) +
                           (_ibscbs->gAjusteCompet->isEmpty() ? 0 : 1);
        if (grupos > 1)
        {
            qWarning() << "Item" << nItem << ": informe apenas um dos grupos gIBSCBS, gIBSCBSMono,"
                       << "gTransfCred ou gAjusteCompet.";
            return false;
        }
        if (!_ibscbs->gCredPresOper->isEmpty() && !_ibscbs->gCredPresIBSZFM->isEmpty())
        {
            qWarning() << "Item" << nItem << ": informe apenas um dos grupos gCredPresOper ou gCredPresIBSZFM.";
            return false;
        }
        const GIBSCBSMono *_mono = _ibscbs->gIBSCBSMono.get();
        if ((!_mono->gIBSMonoAdRem->isEmpty() && !_mono->gIBSMonoAdValorem->isEmpty()) ||
            (!_mono->gCBSMonoAdRem->isEmpty() && !_mono->gCBSMonoAdValorem->isEmpty()))
        {
            qWarning() << "Item" << nItem << ": informe apenas um dos grupos AdRem ou AdValorem"
                       << "da monofasia para cada tributo.";
            return false;
        }
        if (!_ibscbs->gAjusteCompet->isEmpty() && !_ibscbs->gAjusteCompet->get_competApur().isValid())
        {
            qWarning() << "Item" << nItem << ": informe competApur do grupo gAjusteCompet.";
            return false;
        }
        const GCredPresIBSZFM *_zfm = _ibscbs->gCredPresIBSZFM.get();
        if (!_zfm->isEmpty() && (!_zfm->get_competApur().isValid() ||
                                 _zfm->get_tpCredPresIBSZFM() == TpCredPresIBSZFM::None))
        {
            qWarning() << "Item" << nItem << ": informe competApur e tpCredPresIBSZFM do grupo gCredPresIBSZFM.";
            return false;
        }
        const GCredPresOper *_credPres = _ibscbs->gCredPresOper.get();
        for (const GCredPres *_cp : {_credPres->gIBSCredPres.get(), _credPres->gCBSCredPres.get()})
        {
            if (_cp->get_vCredPres() > 0 && _cp->get_vCredPresCondSus() > 0)
            {
                qWarning() << "Item" << nItem << ": informe vCredPres ou vCredPresCondSus, não ambos.";
                return false;
            }
        }
    }
    return true;
}

void XmlWrite::get_IS(const int &i)
{   //IS - UB01
    const Is *_is = this->m_infNFe->det->value(i)->imposto->IS.get();
    if (_is->get_CSTIS().isEmpty())
        return;

    this->m_xmlw->writeStartElement("IS");
    this->m_xmlw->writeTextElement("CSTIS", _is->get_CSTIS());
    this->m_xmlw->writeTextElement("cClassTribIS", _is->get_cClassTribIS());
    if (!qFuzzyIsNull(_is->get_vBCIS()) || !qFuzzyIsNull(_is->get_pIS()) ||
        !qFuzzyIsNull(_is->get_adRemIS()) || !_is->get_uTrib().isEmpty() ||
        !qFuzzyIsNull(_is->get_qTrib()) || !qFuzzyIsNull(_is->get_vIS()))
    {
        this->m_xmlw->writeTextElement("vBCIS", dec2(_is->get_vBCIS()));
        this->m_xmlw->writeTextElement("pIS", dec4(_is->get_pIS()));
        if (_is->get_adRemIS() > 0)
            this->m_xmlw->writeTextElement("adRemIS", dec4(_is->get_adRemIS()));
        if (!_is->get_uTrib().isEmpty())
        {
            this->m_xmlw->writeTextElement("uTrib", _is->get_uTrib());
            this->m_xmlw->writeTextElement("qTrib", dec4(_is->get_qTrib()));
        }
        this->m_xmlw->writeTextElement("vIS", dec2(_is->get_vIS()));
    }
    this->m_xmlw->writeEndElement();//fechamento grupo IS
}

void XmlWrite::get_IBSCBS(const int &i)
{   //IBSCBS - UB12
    const IbsCbs *_ibscbs = this->m_infNFe->det->value(i)->imposto->IBSCBS.get();
    if (_ibscbs->get_CST().isEmpty())
        return;

    this->m_xmlw->writeStartElement("IBSCBS");
    this->m_xmlw->writeTextElement("CST", _ibscbs->get_CST());
    this->m_xmlw->writeTextElement("cClassTrib", _ibscbs->get_cClassTrib());
    if (_ibscbs->get_indDoacao() != IndDoacao::None)
        this->m_xmlw->writeTextElement("indDoacao", ConvNF::indDoacaoToStr(_ibscbs->get_indDoacao()));

    //grupos mutuamente exclusivos, verificados em validarRTC()
    if (!_ibscbs->gIBSCBS->isEmpty())
        get_gIBSCBS(*_ibscbs->gIBSCBS);
    else if (!_ibscbs->gIBSCBSMono->isEmpty())
        get_gIBSCBSMono(*_ibscbs->gIBSCBSMono);
    else if (!_ibscbs->gTransfCred->isEmpty())
    {   //gTransfCred - UB106
        this->m_xmlw->writeStartElement("gTransfCred");
        this->m_xmlw->writeTextElement("vIBS", dec2(_ibscbs->gTransfCred->get_vIBS()));
        this->m_xmlw->writeTextElement("vCBS", dec2(_ibscbs->gTransfCred->get_vCBS()));
        this->m_xmlw->writeEndElement();
    }
    else if (!_ibscbs->gAjusteCompet->isEmpty())
    {   //gAjusteCompet - UB112
        this->m_xmlw->writeStartElement("gAjusteCompet");
        this->m_xmlw->writeTextElement("competApur", _ibscbs->gAjusteCompet->get_competApur().toString("yyyy-MM"));
        this->m_xmlw->writeTextElement("vIBS", dec2(_ibscbs->gAjusteCompet->get_vIBS()));
        this->m_xmlw->writeTextElement("vCBS", dec2(_ibscbs->gAjusteCompet->get_vCBS()));
        this->m_xmlw->writeEndElement();
    }

    if (!_ibscbs->gEstornoCred->isEmpty())
    {   //gEstornoCred - UB116
        this->m_xmlw->writeStartElement("gEstornoCred");
        this->m_xmlw->writeTextElement("vIBSEstCred", dec2(_ibscbs->gEstornoCred->get_vIBSEstCred()));
        this->m_xmlw->writeTextElement("vCBSEstCred", dec2(_ibscbs->gEstornoCred->get_vCBSEstCred()));
        this->m_xmlw->writeEndElement();
    }

    if (!_ibscbs->gCredPresOper->isEmpty())
    {   //gCredPresOper - UB120
        const GCredPresOper *_credPres = _ibscbs->gCredPresOper.get();
        this->m_xmlw->writeStartElement("gCredPresOper");
        this->m_xmlw->writeTextElement("vBCCredPres", dec2(_credPres->get_vBCCredPres()));
        this->m_xmlw->writeTextElement("cCredPres", _credPres->get_cCredPres());
        if (!_credPres->gIBSCredPres->isEmpty())
            get_gCredPres(QStringLiteral("gIBSCredPres"), *_credPres->gIBSCredPres);
        if (!_credPres->gCBSCredPres->isEmpty())
            get_gCredPres(QStringLiteral("gCBSCredPres"), *_credPres->gCBSCredPres);
        this->m_xmlw->writeEndElement();
    }
    else if (!_ibscbs->gCredPresIBSZFM->isEmpty())
    {   //gCredPresIBSZFM - UB131
        const GCredPresIBSZFM *_zfm = _ibscbs->gCredPresIBSZFM.get();
        this->m_xmlw->writeStartElement("gCredPresIBSZFM");
        this->m_xmlw->writeTextElement("competApur", _zfm->get_competApur().toString("yyyy-MM"));
        this->m_xmlw->writeTextElement("tpCredPresIBSZFM", ConvNF::tpCredPresIBSZFMToStr(_zfm->get_tpCredPresIBSZFM()));
        this->m_xmlw->writeTextElement("vCredPresIBSZFM", dec2(_zfm->get_vCredPresIBSZFM()));
        this->m_xmlw->writeEndElement();
    }

    this->m_xmlw->writeEndElement();//fechamento grupo IBSCBS
}

void XmlWrite::get_gIBSCBS(const GIBSCBS &gIBSCBS)
{   //gIBSCBS - UB15
    this->m_xmlw->writeStartElement("gIBSCBS");
    this->m_xmlw->writeTextElement("vBC", dec2(gIBSCBS.get_vBC()));

    const GIBSUF *_uf = gIBSCBS.gIBSUF.get();
    this->m_xmlw->writeStartElement("gIBSUF"); //UB17
    this->m_xmlw->writeTextElement("pIBSUF", dec4(_uf->get_pIBSUF()));
    get_gDifDevTribRed(*_uf->gDif, *_uf->gDevTrib, *_uf->gRed);
    this->m_xmlw->writeTextElement("vIBSUF", dec2(_uf->get_vIBSUF()));
    this->m_xmlw->writeEndElement();

    const GIBSMun *_mun = gIBSCBS.gIBSMun.get();
    this->m_xmlw->writeStartElement("gIBSMun"); //UB36
    this->m_xmlw->writeTextElement("pIBSMun", dec4(_mun->get_pIBSMun()));
    get_gDifDevTribRed(*_mun->gDif, *_mun->gDevTrib, *_mun->gRed);
    this->m_xmlw->writeTextElement("vIBSMun", dec2(_mun->get_vIBSMun()));
    this->m_xmlw->writeEndElement();

    this->m_xmlw->writeTextElement("vIBS", dec2(gIBSCBS.get_vIBS()));

    const GCBS *_cbs = gIBSCBS.gCBS.get();
    this->m_xmlw->writeStartElement("gCBS"); //UB55
    this->m_xmlw->writeTextElement("pCBS", dec4(_cbs->get_pCBS()));
    get_gDifDevTribRed(*_cbs->gDif, *_cbs->gDevTrib, *_cbs->gRed);
    if (!_cbs->gALCZFMCBS->isEmpty())
    {   //gALCZFMCBS - UB66a
        const GALCZFMCBS *_alc = _cbs->gALCZFMCBS.get();
        this->m_xmlw->writeStartElement("gALCZFMCBS");
        this->m_xmlw->writeTextElement("tpALCZFMCBS", ConvNF::tpALCZFMCBSToStr(_alc->get_tpALCZFMCBS()));
        if (!_alc->get_nProcSuframa().isEmpty())
            this->m_xmlw->writeTextElement("nProcSuframa", _alc->get_nProcSuframa());
        this->m_xmlw->writeTextElement("pAliqEfetRegCBS", dec4(_alc->get_pAliqEfetRegCBS()));
        this->m_xmlw->writeTextElement("vTribRegCBS", dec2(_alc->get_vTribRegCBS()));
        this->m_xmlw->writeEndElement();
    }
    this->m_xmlw->writeTextElement("vCBS", dec2(_cbs->get_vCBS()));
    this->m_xmlw->writeEndElement();

    const GTribRegular *_reg = gIBSCBS.gTribRegular.get();
    if (!_reg->isEmpty())
    {   //gTribRegular - UB68
        this->m_xmlw->writeStartElement("gTribRegular");
        this->m_xmlw->writeTextElement("CSTReg", _reg->get_CSTReg());
        this->m_xmlw->writeTextElement("cClassTribReg", _reg->get_cClassTribReg());
        this->m_xmlw->writeTextElement("pAliqEfetRegIBSUF", dec4(_reg->get_pAliqEfetRegIBSUF()));
        this->m_xmlw->writeTextElement("vTribRegIBSUF", dec2(_reg->get_vTribRegIBSUF()));
        this->m_xmlw->writeTextElement("pAliqEfetRegIBSMun", dec4(_reg->get_pAliqEfetRegIBSMun()));
        this->m_xmlw->writeTextElement("vTribRegIBSMun", dec2(_reg->get_vTribRegIBSMun()));
        this->m_xmlw->writeTextElement("pAliqEfetRegCBS", dec4(_reg->get_pAliqEfetRegCBS()));
        this->m_xmlw->writeTextElement("vTribRegCBS", dec2(_reg->get_vTribRegCBS()));
        this->m_xmlw->writeEndElement();
    }

    const GTribCompraGov *_gov = gIBSCBS.gTribCompraGov.get();
    if (!_gov->isEmpty())
    {   //gTribCompraGov - UB82a
        this->m_xmlw->writeStartElement("gTribCompraGov");
        this->m_xmlw->writeTextElement("pAliqIBSUF", dec4(_gov->get_pAliqIBSUF()));
        this->m_xmlw->writeTextElement("vTribIBSUF", dec2(_gov->get_vTribIBSUF()));
        this->m_xmlw->writeTextElement("pAliqIBSMun", dec4(_gov->get_pAliqIBSMun()));
        this->m_xmlw->writeTextElement("vTribIBSMun", dec2(_gov->get_vTribIBSMun()));
        this->m_xmlw->writeTextElement("pAliqCBS", dec4(_gov->get_pAliqCBS()));
        this->m_xmlw->writeTextElement("vTribCBS", dec2(_gov->get_vTribCBS()));
        this->m_xmlw->writeEndElement();
    }
    this->m_xmlw->writeEndElement();//fechamento grupo gIBSCBS
}

void XmlWrite::get_gDifDevTribRed(const GDif &gDif, const GDevTrib &gDevTrib, const GRed &gRed)
{
    if (!gDif.isEmpty())
    {
        this->m_xmlw->writeStartElement("gDif");
        this->m_xmlw->writeTextElement("pDif", dec4(gDif.get_pDif()));
        this->m_xmlw->writeTextElement("vDif", dec2(gDif.get_vDif()));
        this->m_xmlw->writeEndElement();
    }
    if (!gDevTrib.isEmpty())
    {
        this->m_xmlw->writeStartElement("gDevTrib");
        if (gDevTrib.get_pDevTrib() > 0)
            this->m_xmlw->writeTextElement("pDevTrib", dec4(gDevTrib.get_pDevTrib()));
        this->m_xmlw->writeTextElement("vDevTrib", dec2(gDevTrib.get_vDevTrib()));
        this->m_xmlw->writeEndElement();
    }
    if (!gRed.isEmpty())
    {
        this->m_xmlw->writeStartElement("gRed");
        this->m_xmlw->writeTextElement("pRedAliq", dec4(gRed.get_pRedAliq()));
        this->m_xmlw->writeTextElement("pAliqEfet", dec4(gRed.get_pAliqEfet()));
        this->m_xmlw->writeEndElement();
    }
}

void XmlWrite::get_gIBSCBSMono(const GIBSCBSMono &gIBSCBSMono)
{   //gIBSCBSMono - UB84
    this->m_xmlw->writeStartElement("gIBSCBSMono");

    //IBS: AdRem (UB85a) ou AdValorem (UB90)
    const GIBSMonoAdRem *_ibsAdRem = gIBSCBSMono.gIBSMonoAdRem.get();
    const GIBSMonoAdValorem *_ibsAdVal = gIBSCBSMono.gIBSMonoAdValorem.get();
    const GIBSMonoRet *_ibsRet = nullptr;
    const GIBSpBioDiferenca *_ibsBio = nullptr;
    if (!_ibsAdRem->isEmpty())
    {
        this->m_xmlw->writeStartElement("gIBSMonoAdRem");
        if (!_ibsAdRem->gMonoPadrao->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoPadrao");
            this->m_xmlw->writeTextElement("qBCMono", dec4(_ibsAdRem->gMonoPadrao->get_qBCMono()));
            this->m_xmlw->writeTextElement("adRemIBS", dec4(_ibsAdRem->gMonoPadrao->get_adRemIBS()));
            this->m_xmlw->writeTextElement("vIBSMono", dec2(_ibsAdRem->gMonoPadrao->get_vIBSMono()));
            this->m_xmlw->writeEndElement();
        }
        if (!_ibsAdRem->gMonoReten->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoReten");
            this->m_xmlw->writeTextElement("qBCMonoReten", dec4(_ibsAdRem->gMonoReten->get_qBCMonoReten()));
            this->m_xmlw->writeTextElement("adRemIBSReten", dec4(_ibsAdRem->gMonoReten->get_adRemIBSReten()));
            this->m_xmlw->writeTextElement("vIBSMonoReten", dec2(_ibsAdRem->gMonoReten->get_vIBSMonoReten()));
            this->m_xmlw->writeEndElement();
        }
        _ibsRet = _ibsAdRem->gMonoRet.get();
        _ibsBio = _ibsAdRem->gpBioDiferenca.get();
    }
    else if (!_ibsAdVal->isEmpty())
    {
        this->m_xmlw->writeStartElement("gIBSMonoAdValorem");
        if (!_ibsAdVal->gMonoPadrao->isEmpty())
        {
            const GIBSMonoPadraoAdValorem *_p = _ibsAdVal->gMonoPadrao.get();
            this->m_xmlw->writeStartElement("gMonoPadrao");
            this->m_xmlw->writeTextElement("vBCMono", dec2(_p->get_vBCMono()));
            this->m_xmlw->writeTextElement("pAliqMonoUF", dec4(_p->get_pAliqMonoUF()));
            this->m_xmlw->writeTextElement("vIBSMonoUF", dec2(_p->get_vIBSMonoUF()));
            this->m_xmlw->writeTextElement("pAliqMonoMun", dec4(_p->get_pAliqMonoMun()));
            this->m_xmlw->writeTextElement("vIBSMonoMun", dec2(_p->get_vIBSMonoMun()));
            this->m_xmlw->writeTextElement("vIBSMono", dec2(_p->get_vIBSMono()));
            this->m_xmlw->writeEndElement();
        }
        if (!_ibsAdVal->gMonoReten->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoReten");
            this->m_xmlw->writeTextElement("vBCMonoReten", dec2(_ibsAdVal->gMonoReten->get_vBCMonoReten()));
            this->m_xmlw->writeTextElement("pAliqMonoReten", dec4(_ibsAdVal->gMonoReten->get_pAliqMonoReten()));
            this->m_xmlw->writeTextElement("vIBSMonoReten", dec2(_ibsAdVal->gMonoReten->get_vIBSMonoReten()));
            this->m_xmlw->writeEndElement();
        }
        _ibsRet = _ibsAdVal->gMonoRet.get();
        _ibsBio = _ibsAdVal->gpBioDiferenca.get();
    }
    if (_ibsRet)
    {
        if (!_ibsRet->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoRet");
            this->m_xmlw->writeTextElement("vIBSMonoRet", dec2(_ibsRet->get_vIBSMonoRet()));
            this->m_xmlw->writeEndElement();
        }
        if (!_ibsBio->isEmpty())
        {
            this->m_xmlw->writeStartElement("gpBioDiferenca");
            this->m_xmlw->writeTextElement("qBCBioComb", dec4(_ibsBio->get_qBCBioComb()));
            this->m_xmlw->writeTextElement("vIBSDiferenca", dec2(_ibsBio->get_vIBSDiferenca()));
            this->m_xmlw->writeEndElement();
        }
        this->m_xmlw->writeEndElement();//fechamento gIBSMonoAdRem/gIBSMonoAdValorem
    }

    //CBS: AdRem (UB95a) ou AdValorem (UB100)
    const GCBSMonoAdRem *_cbsAdRem = gIBSCBSMono.gCBSMonoAdRem.get();
    const GCBSMonoAdValorem *_cbsAdVal = gIBSCBSMono.gCBSMonoAdValorem.get();
    const GCBSMonoRet *_cbsRet = nullptr;
    const GCBSpBioDiferenca *_cbsBio = nullptr;
    if (!_cbsAdRem->isEmpty())
    {
        this->m_xmlw->writeStartElement("gCBSMonoAdRem");
        if (!_cbsAdRem->gMonoPadrao->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoPadrao");
            this->m_xmlw->writeTextElement("qBCMono", dec4(_cbsAdRem->gMonoPadrao->get_qBCMono()));
            this->m_xmlw->writeTextElement("adRemCBS", dec4(_cbsAdRem->gMonoPadrao->get_adRemCBS()));
            this->m_xmlw->writeTextElement("vCBSMono", dec2(_cbsAdRem->gMonoPadrao->get_vCBSMono()));
            this->m_xmlw->writeEndElement();
        }
        if (!_cbsAdRem->gMonoReten->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoReten");
            this->m_xmlw->writeTextElement("qBCMonoReten", dec4(_cbsAdRem->gMonoReten->get_qBCMonoReten()));
            this->m_xmlw->writeTextElement("adRemCBSReten", dec4(_cbsAdRem->gMonoReten->get_adRemCBSReten()));
            this->m_xmlw->writeTextElement("vCBSMonoReten", dec2(_cbsAdRem->gMonoReten->get_vCBSMonoReten()));
            this->m_xmlw->writeEndElement();
        }
        _cbsRet = _cbsAdRem->gMonoRet.get();
        _cbsBio = _cbsAdRem->gpBioDiferenca.get();
    }
    else if (!_cbsAdVal->isEmpty())
    {
        this->m_xmlw->writeStartElement("gCBSMonoAdValorem");
        if (!_cbsAdVal->gMonoPadrao->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoPadrao");
            this->m_xmlw->writeTextElement("vBCMono", dec2(_cbsAdVal->gMonoPadrao->get_vBCMono()));
            this->m_xmlw->writeTextElement("pAliqMonoCBS", dec4(_cbsAdVal->gMonoPadrao->get_pAliqMonoCBS()));
            this->m_xmlw->writeTextElement("vCBSMono", dec2(_cbsAdVal->gMonoPadrao->get_vCBSMono()));
            this->m_xmlw->writeEndElement();
        }
        if (!_cbsAdVal->gMonoReten->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoReten");
            this->m_xmlw->writeTextElement("vBCMonoReten", dec2(_cbsAdVal->gMonoReten->get_vBCMonoReten()));
            this->m_xmlw->writeTextElement("pAliqMonoReten", dec4(_cbsAdVal->gMonoReten->get_pAliqMonoReten()));
            this->m_xmlw->writeTextElement("vCBSMonoReten", dec2(_cbsAdVal->gMonoReten->get_vCBSMonoReten()));
            this->m_xmlw->writeEndElement();
        }
        _cbsRet = _cbsAdVal->gMonoRet.get();
        _cbsBio = _cbsAdVal->gpBioDiferenca.get();
    }
    if (_cbsRet)
    {
        if (!_cbsRet->isEmpty())
        {
            this->m_xmlw->writeStartElement("gMonoRet");
            this->m_xmlw->writeTextElement("vCBSMonoRet", dec2(_cbsRet->get_vCBSMonoRet()));
            this->m_xmlw->writeEndElement();
        }
        if (!_cbsBio->isEmpty())
        {
            this->m_xmlw->writeStartElement("gpBioDiferenca");
            this->m_xmlw->writeTextElement("qBCBioComb", dec4(_cbsBio->get_qBCBioComb()));
            this->m_xmlw->writeTextElement("vCBSDiferenca", dec2(_cbsBio->get_vCBSDiferenca()));
            this->m_xmlw->writeEndElement();
        }
        this->m_xmlw->writeEndElement();//fechamento gCBSMonoAdRem/gCBSMonoAdValorem
    }

    this->m_xmlw->writeTextElement("vTotIBSMonoItem", dec2(gIBSCBSMono.get_vTotIBSMonoItem()));
    this->m_xmlw->writeTextElement("vTotCBSMonoItem", dec2(gIBSCBSMono.get_vTotCBSMonoItem()));
    this->m_xmlw->writeEndElement();//fechamento grupo gIBSCBSMono
}

void XmlWrite::get_gCredPres(const QString &grupo, const GCredPres &gCredPres)
{   //gIBSCredPres - UB123 / gCBSCredPres - UB127
    this->m_xmlw->writeStartElement(grupo);
    this->m_xmlw->writeTextElement("pCredPres", dec4(gCredPres.get_pCredPres()));
    if (gCredPres.get_vCredPresCondSus() > 0)
        this->m_xmlw->writeTextElement("vCredPresCondSus", dec2(gCredPres.get_vCredPresCondSus()));
    else
        this->m_xmlw->writeTextElement("vCredPres", dec2(gCredPres.get_vCredPres()));
    this->m_xmlw->writeEndElement();
}

void XmlWrite::get_ISTot_IBSCBSTot()
{
    //os grupos de totais acompanham os grupos informados nos itens
    bool temIS = false, temIBSCBS = false, temGIBSCBS = false, temMono = false, temEstorno = false;
    for (const auto *_det : *this->m_infNFe->det)
    {
        const Imposto *_imposto = _det->imposto.get();
        temIS = temIS || !_imposto->IS->get_CSTIS().isEmpty();
        const IbsCbs *_ibscbs = _imposto->IBSCBS.get();
        if (_ibscbs->get_CST().isEmpty())
            continue;
        temIBSCBS = true;
        temGIBSCBS = temGIBSCBS || !_ibscbs->gIBSCBS->isEmpty() || !_ibscbs->gCredPresOper->isEmpty();
        temMono = temMono || !_ibscbs->gIBSCBSMono->isEmpty();
        temEstorno = temEstorno || !_ibscbs->gEstornoCred->isEmpty();
    }

    const Total *_total = this->m_infNFe->total.get();
    //ISTot - W31
    if (temIS || !_total->ISTot->isEmpty())
    {
        this->m_xmlw->writeStartElement("ISTot");
        this->m_xmlw->writeTextElement("vIS", dec2(_total->ISTot->get_vIS()));
        this->m_xmlw->writeEndElement();
    }

    //IBSCBSTot - W34
    const IbsCbsTot *_tot = _total->IBSCBSTot.get();
    if (temIBSCBS || !_tot->isEmpty())
    {
        this->m_xmlw->writeStartElement("IBSCBSTot");
        this->m_xmlw->writeTextElement("vBCIBSCBS", dec2(_tot->get_vBCIBSCBS()));
        if (temGIBSCBS || !_tot->gIBS->isEmpty())
        {   //gIBS - W36
            const GIBSTot *_ibs = _tot->gIBS.get();
            this->m_xmlw->writeStartElement("gIBS");
            this->m_xmlw->writeStartElement("gIBSUF");
            this->m_xmlw->writeTextElement("vDif", dec2(_ibs->gIBSUF->get_vDif()));
            this->m_xmlw->writeTextElement("vDevTrib", dec2(_ibs->gIBSUF->get_vDevTrib()));
            this->m_xmlw->writeTextElement("vIBSUF", dec2(_ibs->gIBSUF->get_vIBSUF()));
            this->m_xmlw->writeEndElement();
            this->m_xmlw->writeStartElement("gIBSMun");
            this->m_xmlw->writeTextElement("vDif", dec2(_ibs->gIBSMun->get_vDif()));
            this->m_xmlw->writeTextElement("vDevTrib", dec2(_ibs->gIBSMun->get_vDevTrib()));
            this->m_xmlw->writeTextElement("vIBSMun", dec2(_ibs->gIBSMun->get_vIBSMun()));
            this->m_xmlw->writeEndElement();
            this->m_xmlw->writeTextElement("vIBS", dec2(_ibs->get_vIBS()));
            this->m_xmlw->writeTextElement("vCredPres", dec2(_ibs->get_vCredPres()));
            this->m_xmlw->writeTextElement("vCredPresCondSus", dec2(_ibs->get_vCredPresCondSus()));
            this->m_xmlw->writeEndElement();
        }
        if (temGIBSCBS || !_tot->gCBS->isEmpty())
        {   //gCBS - W50
            const GCBSTot *_cbs = _tot->gCBS.get();
            this->m_xmlw->writeStartElement("gCBS");
            this->m_xmlw->writeTextElement("vDif", dec2(_cbs->get_vDif()));
            this->m_xmlw->writeTextElement("vDevTrib", dec2(_cbs->get_vDevTrib()));
            this->m_xmlw->writeTextElement("vCBS", dec2(_cbs->get_vCBS()));
            this->m_xmlw->writeTextElement("vCredPres", dec2(_cbs->get_vCredPres()));
            this->m_xmlw->writeTextElement("vCredPresCondSus", dec2(_cbs->get_vCredPresCondSus()));
            this->m_xmlw->writeEndElement();
        }
        if (temMono || !_tot->gMono->isEmpty())
        {   //gMono - W57
            const GMonoTot *_mono = _tot->gMono.get();
            this->m_xmlw->writeStartElement("gMono");
            this->m_xmlw->writeTextElement("vIBSMono", dec2(_mono->get_vIBSMono()));
            this->m_xmlw->writeTextElement("vCBSMono", dec2(_mono->get_vCBSMono()));
            this->m_xmlw->writeTextElement("vIBSMonoReten", dec2(_mono->get_vIBSMonoReten()));
            this->m_xmlw->writeTextElement("vCBSMonoReten", dec2(_mono->get_vCBSMonoReten()));
            this->m_xmlw->writeTextElement("vIBSMonoRet", dec2(_mono->get_vIBSMonoRet()));
            this->m_xmlw->writeTextElement("vCBSMonoRet", dec2(_mono->get_vCBSMonoRet()));
            this->m_xmlw->writeEndElement();
        }
        if (temEstorno || !_tot->gEstornoCred->isEmpty())
        {   //gEstornoCred - W59e
            this->m_xmlw->writeStartElement("gEstornoCred");
            this->m_xmlw->writeTextElement("vIBSEstCred", dec2(_tot->gEstornoCred->get_vIBSEstCred()));
            this->m_xmlw->writeTextElement("vCBSEstCred", dec2(_tot->gEstornoCred->get_vCBSEstCred()));
            this->m_xmlw->writeEndElement();
        }
        this->m_xmlw->writeEndElement();//fechamento grupo IBSCBSTot
    }

    //vNFTot - W60
    if (_total->get_vNFTot() > 0)
        this->m_xmlw->writeTextElement("vNFTot", dec2(_total->get_vNFTot()));
}

void XmlWrite::get_infNFeSupl_ONLINE()
{
    const auto &ide = *this->m_infNFe->ide;
    if (ide.get_mod() != ModeloDF::NFCe && !ide.usaDanfeSimplificadoTipo2())
        return;

    const auto versao = versaoQrCode(*this->m_infNFe, *this->m_confgNFe);
    if (versao == VersaoQrCodeNF::v300 ? ide.get_tpEmis() == TpEmis::Contingencia_Offline
                                     : ide.get_tpEmis() != TpEmis::Normal)
        return;

    const auto uf = ConvNF::intToUF(ide.get_cUF());
    QString parametros = this->m_infNFe->get_Id().mid(3) + QLatin1Char('|') +
                         ConvNF::versaoQrCodeNFToStr(versao).left(1) + QLatin1Char('|') +
                         ConvNF::tpAmbToStr(ide.get_tpAmb());
    if (versao != VersaoQrCodeNF::v300)
    {
        const QString idCSC = this->m_confgNFe->get_IdCSC();
        parametros += QLatin1Char('|');
        if (!idCSC.isEmpty())
            parametros += QString::number(idCSC.toInt());
        const QString entradaHash = parametros + this->m_confgNFe->get_CSC();
        QByteArray hash;
        if (!CppOpenSSL::digest_s(HashType::sha1, entradaHash.toLocal8Bit(), false, hash))
            qWarning() << "Erro ao gerar hash da tag infNFeSupl";
        parametros += QLatin1Char('|') + QString::fromLatin1(hash.toHex().toUpper());
    }

    const QString url = UrlNFe::get_qrCode(uf, ide.get_tpAmb(), versao) + QStringLiteral("?p=") + parametros;
    this->m_xmlw->writeStartElement("infNFeSupl");
    this->m_xmlw->writeTextElement("qrCode", url);
    this->m_xmlw->writeTextElement("urlChave", UrlNFe::get_urlChave(uf, ide.get_tpAmb(), versao));
    this->m_xmlw->writeEndElement();
}

void XmlWrite::gerarNFE()
{
    //cabeçalho
    this->m_xmlw->writeStartElement("NFe");
    this->m_xmlw->writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    get_infNFe();
    get_infNFeSupl_ONLINE();
    this->m_xmlw->writeEndElement(); //fechamento NFe
}

