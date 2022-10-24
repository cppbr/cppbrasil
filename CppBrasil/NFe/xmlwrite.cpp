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
    //infNFeSupl - ZX01
    QString _chave, _versaoQr, _ambiente, _diaEmiss, _valornf, _digest, _idcsc, _csc;
    QString _qrcode, _inputHash, _xml;
    QByteArray _resHash;
    _chave = chave;
    _versaoQr = ConvNF::versaoQrCodeNFToStr(versaoQrcodeNF).at(0);//apenas o primeiro numero da versão
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

QByteArray XmlWrite::get_protNFe(const QString &versao, const QString &Id,
                                 const TpAmb &tpAmb, const QString &verAplic,
                                 const QString &chNFe, const QDateTime &dhRecbto,
                                 const QString &nProt, const QString &digVal,
                                 const int &cStat, const QString &xMotivo,
                                 const int &cMsg, const QString &xMsg)
{
    QByteArray _strXML;
    QXmlStreamWriter *_xmlw = new QXmlStreamWriter(&_strXML);

    _xmlw->writeStartElement("a");
    _xmlw->writeStartElement("protNFe");
    _xmlw->writeAttribute("versao", versao);
    _xmlw->writeStartElement("infProt");
    if (!Id.isEmpty())
       _xmlw->writeTextElement("Id", Id);
    _xmlw->writeTextElement("tpAmb", ConvNF::tpAmbToStr(tpAmb));
    _xmlw->writeTextElement("verAplic", verAplic);
    _xmlw->writeTextElement("chNFe", chNFe);
    _xmlw->writeTextElement("dhRecbto", CppUtility::dateTimeToStr(dhRecbto, DtH::DateTimeUTC));
    _xmlw->writeTextElement("nProt", nProt);
    _xmlw->writeTextElement("digVal", digVal);
    _xmlw->writeTextElement("cStat", QString::number(cStat));
    _xmlw->writeTextElement("xMotivo", xMotivo);
    if (cMsg != 0)
       _xmlw->writeTextElement("cMsg", QString::number(cMsg));
    if (!xMsg.isEmpty())
       _xmlw->writeTextElement("xMsg", xMsg);
    _xmlw->writeEndElement(); //fechamento infProt
    _xmlw->writeEndElement(); //fechamento protNFe
    _xmlw->writeEndElement(); //fechamento a

    delete _xmlw;
    return _strXML;
}

QByteArray XmlWrite::get_nfeProc(const QString &versao)
{
    QByteArray _strXML;
    QXmlStreamWriter *_xmlw = new QXmlStreamWriter(&_strXML);
    _xmlw->writeStartElement("nfeProc");
    _xmlw->writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    _xmlw->writeAttribute("versao", versao);
    _xmlw->writeEndElement(); //fechamento infProt

    delete _xmlw;
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
    this->m_xmlw->writeEndElement();//fechamento grupo infNFe
}

void XmlWrite::get_ide()
{   //ide - B01
    this->m_xmlw->writeStartElement("ide"); //abertura grupo ide
    this->m_xmlw->writeTextElement("cUF", QString::number(this->m_infNFe->ide->get_cUF()));
    this->m_xmlw->writeTextElement("cNF", QString::number(this->m_infNFe->ide->get_cNF()));
    this->m_xmlw->writeTextElement("natOp", this->m_infNFe->ide->get_natOp());
    this->m_xmlw->writeTextElement("mod", ConvNF::modeloDFToStr(this->m_infNFe->ide->get_mod()));
    this->m_xmlw->writeTextElement("serie", QString::number(this->m_infNFe->ide->get_serie()));
    this->m_xmlw->writeTextElement("nNF", QString::number(this->m_infNFe->ide->get_nNF()));
    this->m_xmlw->writeTextElement("dhEmi", CppUtility::dateTimeToStr(this->m_infNFe->ide->get_dhEmi(),
                                                                   DtH::DateTimeUTC));
    if (this->m_infNFe->ide->get_mod() == ModeloDF::NFe && this->m_infNFe->ide->get_dhSaiEnt().isValid())
        this->m_xmlw->writeTextElement("dhSaiEnt", CppUtility::dateTimeToStr(this->m_infNFe->ide->get_dhSaiEnt(),
                                                                          DtH::DateTimeUTC));
    this->m_xmlw->writeTextElement("tpNF", ConvNF::tpNFToStr(this->m_infNFe->ide->get_tpNF()));
    this->m_xmlw->writeTextElement("idDest", ConvNF::idDestToStr(this->m_infNFe->ide->get_idDest()));
    this->m_xmlw->writeTextElement("cMunFG", QString::number(this->m_infNFe->ide->get_cMunFG()));
    this->m_xmlw->writeTextElement("tpImp", ConvNF::tpImpToStr(this->m_infNFe->ide->get_tpImp()));
    this->m_xmlw->writeTextElement("tpEmis", ConvNF::tpEmisToStr(this->m_infNFe->ide->get_tpEmis()));
    this->m_xmlw->writeTextElement("cDV", this->m_infNFe->ide->get_cDV());
    this->m_xmlw->writeTextElement("tpAmb", ConvNF::tpAmbToStr(this->m_infNFe->ide->get_tpAmb()));
    this->m_xmlw->writeTextElement("finNFe", ConvNF::finNFeToStr(this->m_infNFe->ide->get_finNFe()));
    this->m_xmlw->writeTextElement("indFinal", ConvNF::indFinalToStr(this->m_infNFe->ide->get_indFinal()));
    this->m_xmlw->writeTextElement("indPres", ConvNF::indPresToStr(this->m_infNFe->ide->get_indPres()));
    if (this->m_infNFe->ide->get_indIntermed() != IndIntermed::None)
        this->m_xmlw->writeTextElement("indIntermed", ConvNF::indIntermedToStr(this->m_infNFe->ide->get_indIntermed()));
    this->m_xmlw->writeTextElement("procEmi", ConvNF::procEmiToStr(this->m_infNFe->ide->get_procEmi()));
    this->m_xmlw->writeTextElement("verProc", this->m_infNFe->ide->get_verProc());

    if (this->m_infNFe->ide->get_dhCont().isValid() || !this->m_infNFe->ide->get_xJust().isEmpty())
    {
        this->m_xmlw->writeTextElement("dhCont", CppUtility::dateTimeToStr(this->m_infNFe->ide->get_dhCont(),
                                                                        DtH::DateTimeUTC));
        this->m_xmlw->writeTextElement("xJust", this->m_infNFe->ide->get_xJust());
    }
    //grupo Documento fiscal referenciado
    get_NFref();
    this->m_xmlw->writeEndElement();//fechamento grupo ide
}

void XmlWrite::get_NFref()
{   //NFref - BA01 - container 0-500
    if (this->m_infNFe->ide->NFref->items->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->ide->NFref->items->count(); ++i)
        {
            this->m_xmlw->writeStartElement("NFref");//abertura grupo NFref
            if (!this->m_infNFe->ide->NFref->items->value(i)->get_refNFe().isEmpty())
                this->m_xmlw->writeTextElement("refNFe", this->m_infNFe->ide->NFref->items->value(i)->get_refNFe());
            //grupo refNF - nota 1/1A
            //se exitir uma numeração
            if (this->m_infNFe->ide->NFref->items->value(i)->refNF->get_nNF() > 0)
            {
                this->m_xmlw->writeStartElement("refNF");//abertura grupo refNF
                this->m_xmlw->writeTextElement("cUF", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNF->get_cUF()));
                this->m_xmlw->writeTextElement("AAMM", this->m_infNFe->ide->NFref->items->value(i)->refNF->get_AAMM());
                this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->ide->NFref->items->value(i)->refNF->get_CNPJ());
                this->m_xmlw->writeTextElement("mod", this->m_infNFe->ide->NFref->items->value(i)->refNF->get_mod());
                this->m_xmlw->writeTextElement("serie", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNF->get_serie()));
                this->m_xmlw->writeTextElement("nNF", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNF->get_nNF()));
                this->m_xmlw->writeEndElement();//fechamento grupo refNF
            }
            //grupo refNFP - Informações da NF de produtor rural referenciada
            //se exitir uma numeração
            if (this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_nNF() > 0)
            {
                this->m_xmlw->writeStartElement("refNFP");//abertura grupo refNFP
                this->m_xmlw->writeTextElement("cUF", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_cUF()));
                this->m_xmlw->writeTextElement("AAMM", this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_AAMM());
                this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_CNPJ());
                this->m_xmlw->writeTextElement("CPF", this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_CPF());
                this->m_xmlw->writeTextElement("IE", this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_IE());
                this->m_xmlw->writeTextElement("mod", this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_mod());
                this->m_xmlw->writeTextElement("serie", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_serie()));
                this->m_xmlw->writeTextElement("nNF", QString::number(this->m_infNFe->ide->NFref->items->value(i)->refNFP->get_nNF()));
                this->m_xmlw->writeEndElement();//fechamento grupo refNFP
            }
            //CTe
            if (!this->m_infNFe->ide->NFref->items->value(i)->get_refCTe().isEmpty())
                this->m_xmlw->writeTextElement("refCTe", this->m_infNFe->ide->NFref->items->value(i)->get_refCTe());
            //grupo refECF - Informações do Cupom Fiscal referenciado
            if (!this->m_infNFe->ide->NFref->items->value(i)->refECF->get_mod().isEmpty())
            {
                this->m_xmlw->writeStartElement("refECF");//abertura grupo refECF
                this->m_xmlw->writeTextElement("mod", this->m_infNFe->ide->NFref->items->value(i)->refECF->get_mod());
                this->m_xmlw->writeTextElement("nECF", this->m_infNFe->ide->NFref->items->value(i)->refECF->get_nECF());
                this->m_xmlw->writeTextElement("nCOO", this->m_infNFe->ide->NFref->items->value(i)->refECF->get_nCOO());
                this->m_xmlw->writeEndElement();//fechamento grupo refECF
            }
            this->m_xmlw->writeEndElement();//fechamento grupo NFref
        }
    }
}

void XmlWrite::get_emit()
{   //emit - C01
    this->m_xmlw->writeStartElement("emit");//abertura grupo emit
    if (!this->m_infNFe->emite->get_CNPJ().isEmpty())
        this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->emite->get_CNPJ());
    if (!this->m_infNFe->emite->get_CPF().isEmpty())
        this->m_xmlw->writeTextElement("CPF", this->m_infNFe->emite->get_CPF());
    this->m_xmlw->writeTextElement("xNome", this->m_infNFe->emite->get_xNome());
    if (!this->m_infNFe->emite->get_xFant().isEmpty())
        this->m_xmlw->writeTextElement("xFant", this->m_infNFe->emite->get_xFant());
    //enderEmit - C05
    this->m_xmlw->writeStartElement("enderEmit"); //abertura grupo enderEmit
    this->m_xmlw->writeTextElement("xLgr", this->m_infNFe->emite->enderEmit->get_xLgr());
    this->m_xmlw->writeTextElement("nro", QString::number(this->m_infNFe->emite->enderEmit->get_nro()));
    if (!this->m_infNFe->emite->enderEmit->get_xCpl().isEmpty())
        this->m_xmlw->writeTextElement("xCpl", this->m_infNFe->emite->enderEmit->get_xCpl());
    this->m_xmlw->writeTextElement("xBairro", this->m_infNFe->emite->enderEmit->get_xBairro());
    this->m_xmlw->writeTextElement("cMun", QString::number(this->m_infNFe->emite->enderEmit->get_cMun()));
    this->m_xmlw->writeTextElement("xMun", this->m_infNFe->emite->enderEmit->get_xMun());
    this->m_xmlw->writeTextElement("UF", this->m_infNFe->emite->enderEmit->get_UF());
    this->m_xmlw->writeTextElement("CEP", QString::number(this->m_infNFe->emite->enderEmit->get_CEP()));
    if (this->m_infNFe->emite->enderEmit->get_cPais() > 0)
        this->m_xmlw->writeTextElement("cPais", QString::number(this->m_infNFe->emite->enderEmit->get_cPais()));
    if (!this->m_infNFe->emite->enderEmit->get_xPais().isEmpty())
        this->m_xmlw->writeTextElement("xPais", this->m_infNFe->emite->enderEmit->get_xPais());
    if (!this->m_infNFe->emite->enderEmit->get_fone().isEmpty())
        this->m_xmlw->writeTextElement("fone", this->m_infNFe->emite->enderEmit->get_fone());
    this->m_xmlw->writeEndElement();//fechamento grupo enderEmit

    this->m_xmlw->writeTextElement("IE", this->m_infNFe->emite->get_IE());
    if (!this->m_infNFe->emite->get_IEST().isEmpty())
        this->m_xmlw->writeTextElement("IEST", this->m_infNFe->emite->get_IEST());

    if (!this->m_infNFe->emite->get_IM().isEmpty() ||
        !this->m_infNFe->emite->get_CNAE().isEmpty())
    {
        if (!this->m_infNFe->emite->get_IM().isEmpty())
            this->m_xmlw->writeTextElement("IM", this->m_infNFe->emite->get_IM());
        if (!this->m_infNFe->emite->get_CNAE().isEmpty())
            this->m_xmlw->writeTextElement("CNAE", this->m_infNFe->emite->get_CNAE());
    }
    this->m_xmlw->writeTextElement("CRT", ConvNF::crtToStr(this->m_infNFe->emite->get_CRT()));

    this->m_xmlw->writeEndElement();//fechamento grupo emit
}

void XmlWrite::get_avulsa()
{   //avulsa - D01
    if (!this->m_infNFe->avulsa->get_CNPJ().isEmpty() &&
        !this->m_infNFe->avulsa->get_xOrgao().isEmpty())
    {
        this->m_xmlw->writeStartElement("avulsa");//abertura grupo avulsa
        this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->avulsa->get_CNPJ());
        this->m_xmlw->writeTextElement("xOrgao", this->m_infNFe->avulsa->get_xOrgao());
        this->m_xmlw->writeTextElement("matr", this->m_infNFe->avulsa->get_matr());
        this->m_xmlw->writeTextElement("xAgente", this->m_infNFe->avulsa->get_xAgente());
        if (!this->m_infNFe->avulsa->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", this->m_infNFe->avulsa->get_fone());
        this->m_xmlw->writeTextElement("UF", this->m_infNFe->avulsa->get_UF());
        if (!this->m_infNFe->avulsa->get_nDAR().isEmpty())
            this->m_xmlw->writeTextElement("nDAR", this->m_infNFe->avulsa->get_nDAR());
        if (this->m_infNFe->avulsa->get_dEmi().isValid())
            this->m_xmlw->writeTextElement("dEmi", CppUtility::dateTimeToStr(this->m_infNFe->avulsa->get_dEmi(),
                                                                          DtH::Date));
        if (this->m_infNFe->avulsa->get_vDAR() > 0)
            this->m_xmlw->writeTextElement("vDAR", CppUtility::doubleToStrDecimal(this->m_infNFe->avulsa->get_vDAR(), 13));
        this->m_xmlw->writeTextElement("repEmi", this->m_infNFe->avulsa->get_repEmi());
        if (!this->m_infNFe->avulsa->get_dPag().isValid())
            this->m_xmlw->writeTextElement("dPag", CppUtility::dateTimeToStr(this->m_infNFe->avulsa->get_dPag(),
                                                                             DtH::Date));
        this->m_xmlw->writeEndElement();//fechamento grupo avulsa
    }
}

void XmlWrite::get_dest()
{   //dest - E01
    //Quando 65 só deverá existir caso algum dos campos abaixo não esteja vazio
    if (this->m_infNFe->ide->get_mod() == ModeloDF::NFCe)
    {
        if (!this->m_infNFe->dest->get_CNPJ().isEmpty() ||
            !this->m_infNFe->dest->get_CPF().isEmpty()  ||
            !this->m_infNFe->dest->get_xNome().isEmpty())
        {
            this->m_xmlw->writeStartElement("dest");//abertura grupo dest
            //cnpj ou cpf não os dois ou nenhum para 65
            if (!this->m_infNFe->dest->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->dest->get_CNPJ());
            else
                if (!this->m_infNFe->dest->get_CPF().isEmpty())
                    this->m_xmlw->writeTextElement("CPF", this->m_infNFe->dest->get_CPF());
            //xNome opcional para a NFC-e->
            if (!this->m_infNFe->dest->get_xNome().isEmpty())
            {
                if (this->m_infNFe->ide->get_tpAmb() == TpAmb::Homologacao)
                   this->m_xmlw->writeTextElement("xNome", "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
                else
                   this->m_xmlw->writeTextElement("xNome", this->m_infNFe->dest->get_xNome());
            }
            //No caso de NFC-e informar indIEDest=9 e não informar a tag IE;
            this->m_xmlw->writeTextElement("indIEDest", ConvNF::indIEDestToStr(IndIEDest::NaoContribuinte));

            this->m_xmlw->writeEndElement();//fechamento grupo dest
        }
    }
    //obgrigatório para 55
    if (this->m_infNFe->ide->get_mod() == ModeloDF::NFe)
    {
        this->m_xmlw->writeStartElement("dest");//abertura grupo dest
        //cnpj ou cpf não os dois, mas um tem que existir
        if (!this->m_infNFe->dest->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->dest->get_CNPJ());
        else
            this->m_xmlw->writeTextElement("CPF", this->m_infNFe->dest->get_CPF());

        if (!this->m_infNFe->dest->get_idEstrangeiro().isEmpty())
            this->m_xmlw->writeTextElement("idEstrangeiro", this->m_infNFe->dest->get_idEstrangeiro());

        if (this->m_infNFe->ide->get_tpAmb() == TpAmb::Homologacao)
           this->m_xmlw->writeTextElement("xNome", "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
        else
           this->m_xmlw->writeTextElement("xNome", this->m_infNFe->dest->get_xNome());

        //enderDest - E05
        this->m_xmlw->writeStartElement("enderDest");//abertura grupo enderDest
        this->m_xmlw->writeTextElement("xLgr", this->m_infNFe->dest->enderDest->get_xLgr());
        this->m_xmlw->writeTextElement("nro", QString::number(this->m_infNFe->dest->enderDest->get_nro()));
        if (!this->m_infNFe->dest->enderDest->get_xCpl().isEmpty())
            this->m_xmlw->writeTextElement("xCpl", this->m_infNFe->dest->enderDest->get_xCpl());
        this->m_xmlw->writeTextElement("xBairro", this->m_infNFe->dest->enderDest->get_xBairro());
        this->m_xmlw->writeTextElement("cMun", QString::number(this->m_infNFe->dest->enderDest->get_cMun()));
        this->m_xmlw->writeTextElement("xMun", this->m_infNFe->dest->enderDest->get_xMun());
        this->m_xmlw->writeTextElement("UF", this->m_infNFe->dest->enderDest->get_UF());
        if (this->m_infNFe->dest->enderDest->get_CEP() > 0)
            this->m_xmlw->writeTextElement("CEP", QString::number(this->m_infNFe->dest->enderDest->get_CEP()));
        if (this->m_infNFe->dest->enderDest->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(this->m_infNFe->dest->enderDest->get_cPais()));
        if (!this->m_infNFe->dest->enderDest->get_xPais().isEmpty())
            this->m_xmlw->writeTextElement("xPais", this->m_infNFe->dest->enderDest->get_xPais());
        if (!this->m_infNFe->dest->enderDest->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", this->m_infNFe->dest->enderDest->get_fone());
        this->m_xmlw->writeEndElement();//fechamento grupo enderDest

        this->m_xmlw->writeTextElement("indIEDest", ConvNF::indIEDestToStr(this->m_infNFe->dest->get_indIEDest()));
        if (!this->m_infNFe->dest->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", this->m_infNFe->dest->get_IE());
        if (!this->m_infNFe->dest->get_ISUF().isEmpty())
            this->m_xmlw->writeTextElement("ISUF", this->m_infNFe->dest->get_ISUF());
        if (!this->m_infNFe->dest->get_IM().isEmpty())
            this->m_xmlw->writeTextElement("IM", this->m_infNFe->dest->get_IM());
        if (!this->m_infNFe->dest->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", this->m_infNFe->dest->get_email());
        this->m_xmlw->writeEndElement();//fechamento grupo dest
    }
}

void XmlWrite::get_retirada()
{   //retirada - F01
    if (!this->m_infNFe->retirada->get_CNPJ().isEmpty() ||
        !this->m_infNFe->retirada->get_CPF().isEmpty()  ||
        !this->m_infNFe->retirada->get_xNome().isEmpty())
    {
        this->m_xmlw->writeStartElement("retirada");//abertura grupo retirada
        //ou cnpj ou cpf não os dois
        if (!this->m_infNFe->retirada->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->retirada->get_CNPJ());
        else
            this->m_xmlw->writeTextElement("CPF", this->m_infNFe->retirada->get_CPF());

        if (!this->m_infNFe->retirada->get_xNome().isEmpty())
            this->m_xmlw->writeTextElement("xNome", this->m_infNFe->retirada->get_xNome());
        this->m_xmlw->writeTextElement("xLgr", this->m_infNFe->retirada->get_xLgr());
        this->m_xmlw->writeTextElement("nro", QString::number(this->m_infNFe->retirada->get_nro()));
        if (!this->m_infNFe->retirada->get_xCpl().isEmpty())
            this->m_xmlw->writeTextElement("xCpl", this->m_infNFe->retirada->get_xCpl());
        this->m_xmlw->writeTextElement("xBairro", this->m_infNFe->retirada->get_xBairro());
        this->m_xmlw->writeTextElement("cMun", QString::number(this->m_infNFe->retirada->get_cMun()));
        this->m_xmlw->writeTextElement("xMun", this->m_infNFe->retirada->get_xMun());
        this->m_xmlw->writeTextElement("UF", this->m_infNFe->retirada->get_UF());
        if (this->m_infNFe->retirada->get_CEP() > 0)
            this->m_xmlw->writeTextElement("CEP", QString::number(this->m_infNFe->retirada->get_CEP()));
        if (this->m_infNFe->retirada->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(this->m_infNFe->retirada->get_cPais()));
        if (!this->m_infNFe->retirada->get_xPais().isEmpty())
            this->m_xmlw->writeTextElement("xPais", this->m_infNFe->retirada->get_xPais());
        if (!this->m_infNFe->retirada->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", this->m_infNFe->retirada->get_fone());
        if (!this->m_infNFe->retirada->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", this->m_infNFe->retirada->get_email());
        if (!this->m_infNFe->retirada->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", this->m_infNFe->retirada->get_IE());
        this->m_xmlw->writeEndElement();//fechamento grupo retirada
    }
}

void XmlWrite::get_entrega()
{   //entrega - G01
    if (!this->m_infNFe->entrega->get_CNPJ().isEmpty() ||
        !this->m_infNFe->entrega->get_CPF().isEmpty()  ||
        !this->m_infNFe->entrega->get_xNome().isEmpty())
    {
        this->m_xmlw->writeStartElement("entrega");//abertura grupo entrega
        //ou cnpj ou cpf não os dois
        if (!this->m_infNFe->entrega->get_CNPJ().isEmpty())
            this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->entrega->get_CNPJ());
        else
            this->m_xmlw->writeTextElement("CPF", this->m_infNFe->entrega->get_CPF());

        if (!this->m_infNFe->entrega->get_xNome().isEmpty())
            this->m_xmlw->writeTextElement("xNome", this->m_infNFe->entrega->get_xNome());
        this->m_xmlw->writeTextElement("xLgr", this->m_infNFe->entrega->get_xLgr());
        this->m_xmlw->writeTextElement("nro", QString::number(this->m_infNFe->entrega->get_nro()));
        if (!this->m_infNFe->entrega->get_xCpl().isEmpty())
            this->m_xmlw->writeTextElement("xCpl", this->m_infNFe->entrega->get_xCpl());
        this->m_xmlw->writeTextElement("xBairro", this->m_infNFe->entrega->get_xBairro());
        this->m_xmlw->writeTextElement("cMun", QString::number(this->m_infNFe->entrega->get_cMun()));
        this->m_xmlw->writeTextElement("xMun", this->m_infNFe->entrega->get_xMun());
        this->m_xmlw->writeTextElement("UF", this->m_infNFe->entrega->get_UF());
        if (this->m_infNFe->entrega->get_CEP() > 0)
            this->m_xmlw->writeTextElement("CEP", QString::number(this->m_infNFe->entrega->get_CEP()));
        if (this->m_infNFe->entrega->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(this->m_infNFe->entrega->get_cPais()));
        if (!this->m_infNFe->entrega->get_xPais().isEmpty())
            this->m_xmlw->writeTextElement("xPais", this->m_infNFe->entrega->get_xPais());
        if (!this->m_infNFe->entrega->get_fone().isEmpty())
            this->m_xmlw->writeTextElement("fone", this->m_infNFe->entrega->get_fone());
        if (!this->m_infNFe->entrega->get_email().isEmpty())
            this->m_xmlw->writeTextElement("email", this->m_infNFe->entrega->get_email());
        if (!this->m_infNFe->entrega->get_IE().isEmpty())
            this->m_xmlw->writeTextElement("IE", this->m_infNFe->entrega->get_IE());
        this->m_xmlw->writeEndElement();//fechamento grupo entrega
    }
}

void XmlWrite::get_autXML()
{   //autXML - GA01 - container 0-10
    if (this->m_infNFe->autXML->items->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->autXML->items->count(); ++i)
        {
            this->m_xmlw->writeStartElement("autXML"); //abertura grupo autXML
            if (!this->m_infNFe->autXML->items->value(i)->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ",this->m_infNFe->autXML->items->value(i)->get_CNPJ());
            else
                this->m_xmlw->writeTextElement("CPF",this->m_infNFe->autXML->items->value(i)->get_CPF());
            this->m_xmlw->writeEndElement();//fechamento grupo autXML
        }
    }
}

void XmlWrite::get_det()
{   //det - H01 - container 1-990
    for(int i = 0; i < this->m_infNFe->det->items->count(); ++i)
    {
        this->m_xmlw->writeStartElement("det"); //abertura grupo det
        get_prod(i);
        get_imposto(i);
        get_impostoDevol(i);
        if (!this->m_infNFe->det->items->value(i)->get_infAdProd().isEmpty())
            this->m_xmlw->writeTextElement("infAdProd", this->m_infNFe->det->items->value(i)->get_infAdProd());

        get_obsItem(i);
        this->m_xmlw->writeEndElement();//fechamento grupo det
    }//fim for det
}

void XmlWrite::get_prod(const int &i)
{
    //nItem é um atributo da tag det, mas está dentro da classe prod
    this->m_xmlw->writeAttribute("nItem", QString::number(this->m_infNFe->det->items->value(i)->prod->get_nItem()));
    //prod - I01
    this->m_xmlw->writeStartElement("prod"); //abertura grupo prod
    this->m_xmlw->writeTextElement("cProd", this->m_infNFe->det->items->value(i)->prod->get_cProd());
    this->m_xmlw->writeTextElement("cEAN", this->m_infNFe->det->items->value(i)->prod->get_cEAN());
    if (!this->m_infNFe->det->items->value(i)->prod->get_cBarra().isEmpty())
        this->m_xmlw->writeTextElement("cBarra", this->m_infNFe->det->items->value(i)->prod->get_cBarra());
    //NFCe Homologação o primeiro item deve ter a descrição diferente
    if ((this->m_infNFe->ide->get_tpAmb() == TpAmb::Homologacao) &&
        (this->m_infNFe->ide->get_mod() == ModeloDF::NFCe) && (i == 0))
        this->m_xmlw->writeTextElement("xProd", "NOTA FISCAL EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
    else
        this->m_xmlw->writeTextElement("xProd", this->m_infNFe->det->items->value(i)->prod->get_xProd());

    this->m_xmlw->writeTextElement("NCM", this->m_infNFe->det->items->value(i)->prod->get_NCM());
    if (!this->m_infNFe->det->items->value(i)->prod->get_NVE().isEmpty())
        this->m_xmlw->writeTextElement("NVE", this->m_infNFe->det->items->value(i)->prod->get_NVE());
    if (!this->m_infNFe->det->items->value(i)->prod->get_CEST().isEmpty())
        this->m_xmlw->writeTextElement("CEST", this->m_infNFe->det->items->value(i)->prod->get_CEST());
    if (this->m_infNFe->det->items->value(i)->prod->get_indEscala() != IndEscala::None)
        this->m_xmlw->writeTextElement("indEscala", ConvNF::indEscalaToStr(this->m_infNFe->det->items->value(i)->prod->get_indEscala()));
    if (!this->m_infNFe->det->items->value(i)->prod->get_CNPJFab().isEmpty())
        this->m_xmlw->writeTextElement("CNPJFab", this->m_infNFe->det->items->value(i)->prod->get_CNPJFab());
    if (!this->m_infNFe->det->items->value(i)->prod->get_cBenef().isEmpty())
        this->m_xmlw->writeTextElement("cBenef", this->m_infNFe->det->items->value(i)->prod->get_cBenef());
    if (!this->m_infNFe->det->items->value(i)->prod->get_EXTIPI().isEmpty())
        this->m_xmlw->writeTextElement("EXTIPI", this->m_infNFe->det->items->value(i)->prod->get_EXTIPI());
    this->m_xmlw->writeTextElement("CFOP", this->m_infNFe->det->items->value(i)->prod->get_CFOP());
    this->m_xmlw->writeTextElement("uCom", this->m_infNFe->det->items->value(i)->prod->get_uCom());
    this->m_xmlw->writeTextElement("qCom", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_qCom(), 4));
    this->m_xmlw->writeTextElement("vUnCom", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vUnCom(), 10));
    this->m_xmlw->writeTextElement("vProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vProd(), 2));
    this->m_xmlw->writeTextElement("cEANTrib", this->m_infNFe->det->items->value(i)->prod->get_cEANTrib());
    if (!this->m_infNFe->det->items->value(i)->prod->get_cBarraTrib().isEmpty())
        this->m_xmlw->writeTextElement("cBarraTrib", this->m_infNFe->det->items->value(i)->prod->get_cBarraTrib());
    this->m_xmlw->writeTextElement("uTrib", this->m_infNFe->det->items->value(i)->prod->get_uTrib());
    this->m_xmlw->writeTextElement("qTrib", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_qTrib(), 4));
    this->m_xmlw->writeTextElement("vUnTrib", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vUnTrib(), 10));
    if (this->m_infNFe->det->items->value(i)->prod->get_vFrete() > 0)
        this->m_xmlw->writeTextElement("vFrete", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vFrete(), 2));
    if (this->m_infNFe->det->items->value(i)->prod->get_vSeg() > 0)
        this->m_xmlw->writeTextElement("vSeg", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vSeg(), 2));
    if (this->m_infNFe->det->items->value(i)->prod->get_vDesc() > 0)
        this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vDesc(), 2));
    if (this->m_infNFe->det->items->value(i)->prod->get_vOutro() > 0)
        this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->get_vOutro(), 2));
    this->m_xmlw->writeTextElement("indTot", ConvNF::indTotToStr(this->m_infNFe->det->items->value(i)->prod->get_indTot()));

    //DI - I18 - container 0-100
    if (this->m_infNFe->det->items->value(i)->prod->DI->items->count() > 0)
    {
        for(int j = 0; j < this->m_infNFe->det->items->value(i)->prod->DI->items->count() ; ++j)
        {
            this->m_xmlw->writeStartElement("DI");//abertura grupo DI
            this->m_xmlw->writeTextElement("nDI", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_nDI());
            this->m_xmlw->writeTextElement("dDI", CppUtility::dateTimeToStr(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_dDI(),
                                                                            DtH::Date));
            this->m_xmlw->writeTextElement("xLocDesemb", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_xLocDesemb());
            this->m_xmlw->writeTextElement("UFDesemb", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_UFDesemb());
            this->m_xmlw->writeTextElement("UFDesemb", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_UFDesemb());
            this->m_xmlw->writeTextElement("dDesemb", CppUtility::dateTimeToStr(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_dDesemb(),
                                                                                DtH::Date));
            this->m_xmlw->writeTextElement("tpViaTransp", ConvNF::tpViaTranspToStr(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_tpViaTransp()));
            if (this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_vAFRMM() > 0)
                this->m_xmlw->writeTextElement("vAFRMM", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_vAFRMM(), 2));
            this->m_xmlw->writeTextElement("tpIntermedio", ConvNF::tpIntermedioToStr(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_tpIntermedio()));
            if (!this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_CNPJ());
            if (!this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_UFTerceiro().isEmpty())
                this->m_xmlw->writeTextElement("UFTerceiro", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_UFTerceiro());
            this->m_xmlw->writeTextElement("cExportador", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->get_cExportador());

            //adi - I25 - container 1-100
            if (this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->count() > 0)
            {
                for(int l = 0; l < this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->count() ; ++l)
                {
                    this->m_xmlw->writeStartElement("adi");//abertura grupo adi
                    this->m_xmlw->writeTextElement("nAdicao", QString::number(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_nAdicao()));
                    this->m_xmlw->writeTextElement("nSeqAdic", QString::number(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_nSeqAdic()));
                    this->m_xmlw->writeTextElement("cFabricante", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_cFabricante());
                    if (this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_vDescDI() > 0)
                        this->m_xmlw->writeTextElement("vDescDI", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_vDescDI(), 2));
                    if (!this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_nDraw().isEmpty())
                        this->m_xmlw->writeTextElement("nDraw", this->m_infNFe->det->items->value(i)->prod->DI->items->value(j)->adi->items->value(l)->get_nDraw());
                    this->m_xmlw->writeEndElement();//fechamento grupo adi
                }
            }
            this->m_xmlw->writeEndElement();//fechamento grupo DI
        }
    }//fim DI

    //detExport - I50 - container 0-500
    if (this->m_infNFe->det->items->value(i)->prod->detExport->items->count() > 0)
    {
        for(int j = 0; j < this->m_infNFe->det->items->value(i)->prod->detExport->items->count() ; ++j)
        {
            this->m_xmlw->writeStartElement("detExport");//abertura grupo detExport
            if (!this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->get_nDraw().isEmpty())
                this->m_xmlw->writeTextElement("nDraw",this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->get_nDraw());
            //exportInd - I52
            if (!this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_nRE().isEmpty() ||
                !this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_chNFe().isEmpty() ||
                this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_qExport() > 0)
            {
                this->m_xmlw->writeStartElement("exportInd");//abertura grupo exportInd
                this->m_xmlw->writeTextElement("nRE",this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_nRE());
                this->m_xmlw->writeTextElement("chNFe",this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_chNFe());
                this->m_xmlw->writeTextElement("qExport", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->detExport->items->value(j)->exportInd->get_qExport(), 4));
                this->m_xmlw->writeEndElement();//fechamento grupo exportInd
            }
            this->m_xmlw->writeEndElement();//fechamento grupo detExport
        }
    }//fim detExport
    //continuação prod
    if (!this->m_infNFe->det->items->value(i)->prod->get_xPed().isEmpty())
        this->m_xmlw->writeTextElement("xPed", this->m_infNFe->det->items->value(i)->prod->get_xPed());
    if (this->m_infNFe->det->items->value(i)->prod->get_nItemPed() > 0)
        this->m_xmlw->writeTextElement("nItemPed", QString::number(this->m_infNFe->det->items->value(i)->prod->get_nItemPed()));
    if (!this->m_infNFe->det->items->value(i)->prod->get_nFCI().isEmpty())
        this->m_xmlw->writeTextElement("nFCI", this->m_infNFe->det->items->value(i)->prod->get_nFCI());

    //rastro - I80 - container 0-500
    if (this->m_infNFe->det->items->value(i)->prod->rastro->items->count() > 0)
    {
        for(int j = 0; j < this->m_infNFe->det->items->value(i)->prod->rastro->items->count() ; ++j)
        {
            this->m_xmlw->writeStartElement("rastro");//abertura grupo rastro
            this->m_xmlw->writeTextElement("nLote",this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_nLote());
            this->m_xmlw->writeTextElement("qLote",QString::number(this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_qLote()));
            this->m_xmlw->writeTextElement("dFab", CppUtility::dateTimeToStr(this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_dFab(),
                                                                            DtH::DateTime));
            this->m_xmlw->writeTextElement("dVal", CppUtility::dateTimeToStr(this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_dVal(),
                                                                            DtH::DateTime));
            if (!this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_cAgreg().isEmpty())
                this->m_xmlw->writeTextElement("cAgreg", this->m_infNFe->det->items->value(i)->prod->rastro->items->value(j)->get_cAgreg());
            this->m_xmlw->writeEndElement();//fechamento grupo rastro
        }
    }

    //SequênciaXML - I90 -x-
    //veicProd - J01
    if (this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpOp() != TpOp::None)
    {
        this->m_xmlw->writeStartElement("veicProd");//abertura grupo veicProd
        this->m_xmlw->writeTextElement("tpOp",ConvNF::tpOpToStr(this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpOp()));
        this->m_xmlw->writeTextElement("chassi",this->m_infNFe->det->items->value(i)->prod->veicProd->get_chassi());
        this->m_xmlw->writeTextElement("cCor",this->m_infNFe->det->items->value(i)->prod->veicProd->get_cCor());
        this->m_xmlw->writeTextElement("xCor",this->m_infNFe->det->items->value(i)->prod->veicProd->get_xCor());
        this->m_xmlw->writeTextElement("pot",this->m_infNFe->det->items->value(i)->prod->veicProd->get_pot());
        this->m_xmlw->writeTextElement("cilin",this->m_infNFe->det->items->value(i)->prod->veicProd->get_cilin());
        this->m_xmlw->writeTextElement("pesoL",this->m_infNFe->det->items->value(i)->prod->veicProd->get_pesoL());
        this->m_xmlw->writeTextElement("pesoB",this->m_infNFe->det->items->value(i)->prod->veicProd->get_pesoB());
        this->m_xmlw->writeTextElement("nSerie",this->m_infNFe->det->items->value(i)->prod->veicProd->get_nSerie());
        this->m_xmlw->writeTextElement("tpComb",this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpComb());
        this->m_xmlw->writeTextElement("nMotor",this->m_infNFe->det->items->value(i)->prod->veicProd->get_nMotor());
        this->m_xmlw->writeTextElement("CMT",this->m_infNFe->det->items->value(i)->prod->veicProd->get_CMT());
        this->m_xmlw->writeTextElement("dist",this->m_infNFe->det->items->value(i)->prod->veicProd->get_dist());
        this->m_xmlw->writeTextElement("anoMod",QString::number(this->m_infNFe->det->items->value(i)->prod->veicProd->get_anoMod()));
        this->m_xmlw->writeTextElement("anoFab",QString::number(this->m_infNFe->det->items->value(i)->prod->veicProd->get_anoFab()));
        this->m_xmlw->writeTextElement("tpPint",this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpPint());
        this->m_xmlw->writeTextElement("tpVeic",this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpVeic());
        this->m_xmlw->writeTextElement("espVeic",this->m_infNFe->det->items->value(i)->prod->veicProd->get_espVeic());
        this->m_xmlw->writeTextElement("VIN",this->m_infNFe->det->items->value(i)->prod->veicProd->get_VIN());
        this->m_xmlw->writeTextElement("condVeic",ConvNF::condVeicToStr(this->m_infNFe->det->items->value(i)->prod->veicProd->get_condVeic()));
        this->m_xmlw->writeTextElement("cMod",this->m_infNFe->det->items->value(i)->prod->veicProd->get_cMod());
        this->m_xmlw->writeTextElement("cCorDENATRAN",this->m_infNFe->det->items->value(i)->prod->veicProd->get_cCorDENATRAN());
        this->m_xmlw->writeTextElement("lota",QString::number(this->m_infNFe->det->items->value(i)->prod->veicProd->get_lota()));
        this->m_xmlw->writeTextElement("tpRest",ConvNF::tpRestToStr(this->m_infNFe->det->items->value(i)->prod->veicProd->get_tpRest()));
        this->m_xmlw->writeEndElement();//fechamento grupo veicProd
    }//fim veicProd

    //med - K01
    if (!this->m_infNFe->det->items->value(i)->prod->med->get_cProdANVISA().isEmpty())
    {
        this->m_xmlw->writeStartElement("med");//abertura grupo med
        this->m_xmlw->writeTextElement("cProdANVISA", this->m_infNFe->det->items->value(i)->prod->med->get_cProdANVISA());
        if (!this->m_infNFe->det->items->value(i)->prod->med->get_xMotivoIsencao().isEmpty())
            this->m_xmlw->writeTextElement("xMotivoIsencao", this->m_infNFe->det->items->value(i)->prod->med->get_xMotivoIsencao());
        this->m_xmlw->writeTextElement("vPMC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->med->get_vPMC(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo med
    }

    //arma - L01 - container 1-500
    if (this->m_infNFe->det->items->value(i)->prod->arma->items->count() > 0)
    {
        for(int j = 0; j < this->m_infNFe->det->items->value(i)->prod->arma->items->count() ; ++j)
        {
            this->m_xmlw->writeStartElement("arma");//abertura grupo arma
            this->m_xmlw->writeTextElement("tpArma", ConvNF::tpArmaToStr(this->m_infNFe->det->items->value(i)->prod->arma->items->value(j)->get_tpArma()));
            this->m_xmlw->writeTextElement("nSerie", this->m_infNFe->det->items->value(i)->prod->arma->items->value(j)->get_nSerie());
            this->m_xmlw->writeTextElement("nCano", this->m_infNFe->det->items->value(i)->prod->arma->items->value(j)->get_nCano());
            this->m_xmlw->writeTextElement("descr", this->m_infNFe->det->items->value(i)->prod->arma->items->value(j)->get_descr());
            this->m_xmlw->writeEndElement();//fechamento grupo arma
        }
    }

    //comb - LA01
    if (this->m_infNFe->det->items->value(i)->prod->comb->get_cProdANP() > 0)
    {
        this->m_xmlw->writeStartElement("comb");//abertura grupo comb
        this->m_xmlw->writeTextElement("cProdANP", QString::number(this->m_infNFe->det->items->value(i)->prod->comb->get_cProdANP()));
        this->m_xmlw->writeTextElement("descANP", this->m_infNFe->det->items->value(i)->prod->comb->get_descANP());
        if (this->m_infNFe->det->items->value(i)->prod->comb->get_pGLP() > 0)
            this->m_xmlw->writeTextElement("pGLP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->get_pGLP(), 4));
        if (this->m_infNFe->det->items->value(i)->prod->comb->get_pGNn() > 0)
            this->m_xmlw->writeTextElement("pGNn", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->get_pGNn(), 4));
        if (this->m_infNFe->det->items->value(i)->prod->comb->get_pGNi() > 0)
            this->m_xmlw->writeTextElement("pGNi", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->get_pGNi(), 4));
        if (this->m_infNFe->det->items->value(i)->prod->comb->get_vPart() > 0)
            this->m_xmlw->writeTextElement("vPart", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->get_vPart(), 2));
        if (!this->m_infNFe->det->items->value(i)->prod->comb->get_CODIF().isEmpty())
            this->m_xmlw->writeTextElement("CODIF", this->m_infNFe->det->items->value(i)->prod->comb->get_CODIF());
        if (this->m_infNFe->det->items->value(i)->prod->comb->get_qTemp() > 0)
            this->m_xmlw->writeTextElement("qTemp", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->get_qTemp(), 4));
        if (!this->m_infNFe->det->items->value(i)->prod->comb->get_UFCons().isEmpty())
            this->m_xmlw->writeTextElement("UFCons", this->m_infNFe->det->items->value(i)->prod->comb->get_UFCons());
        //CIDE - LA07
        if (this->m_infNFe->det->items->value(i)->prod->comb->CIDE->get_vCIDE() > 0)
        {
           this->m_xmlw->writeStartElement("CIDE");//abertura grupo CIDE
           this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->CIDE->get_qBCProd(), 4));
           this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->CIDE->get_vAliqProd(), 4));
           this->m_xmlw->writeTextElement("vCIDE", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->CIDE->get_vCIDE(), 2));
           this->m_xmlw->writeEndElement();//fechamento grupo CIDE
        }
        //encerrante - LA11
        if (this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_nBico() > 0 ||
            this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_vEncIni() > 0)
        {
            this->m_xmlw->writeStartElement("encerrante");//abertura grupo encerrante
            this->m_xmlw->writeTextElement("nBico", QString::number(this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_nBico()));
            this->m_xmlw->writeTextElement("nBomba", QString::number(this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_nBomba()));
            this->m_xmlw->writeTextElement("nTanque", QString::number(this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_nTanque()));
            this->m_xmlw->writeTextElement("vEncIni", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_vEncIni(), 3));
            this->m_xmlw->writeTextElement("vEncFin", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->prod->comb->encerrante->get_vEncFin(), 3));
            this->m_xmlw->writeEndElement();//fechamento grupo encerrante
        }

        this->m_xmlw->writeEndElement();//fechamento grupo comb
    }
    //nRECOPI LB01 - pertence ao I90-> Como o I90 nao e um grupo com tag, nRECOPI fica dentro de I01-prod
    if (!this->m_infNFe->det->items->value(i)->prod->get_nRECOPI().isEmpty())
    {
        this->m_xmlw->writeTextElement("nRECOPI", this->m_infNFe->det->items->value(i)->prod->get_nRECOPI());
    }

    this->m_xmlw->writeEndElement();//fechamento grupo prod
}

void XmlWrite::get_imposto(const int &i)
{   //imposto - M01
    this->m_xmlw->writeStartElement("imposto");//abertura grupo imposto
    this->m_xmlw->writeTextElement("vTotTrib", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->get_vTotTrib(), 2));
    get_ICMS(i);
    get_ICMSUFDest(i);
    get_IPI(i);
    get_II(i);
    get_PIS(i);
    get_PISST(i);
    get_COFINS(i);
    get_COFINSST(i);
    get_ISSQN(i);
    this->m_xmlw->writeEndElement();//fechamento grupo imposto
}

void XmlWrite::get_ICMS(const int &i)
{
    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() != CstICMS::None ||
        this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() != CsosnICMS::None)
    {
        //Origem da mercadoria e igual pra todos, CST ou CSOSN
        QString orig = ConvNF::origToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_orig());

        //ICMS - N01
        this->m_xmlw->writeStartElement("ICMS");//abertura grupo ICMS
        //CST
        if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() != CstICMS::None)
        {
            QString cst = ConvNF::cstICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST());
            QString cab;
            if (cst.count() == 2)
            {
                cab = QString("%1%2").arg("ICMS", cst);
            }
            else
            {
                if (cst.count() > 4)
                    cab = "ICMSPart";
                else
                    cab = "ICMSST";
                cst = cst.right(2);
            }

            this->m_xmlw->writeStartElement(cab); //abertura grupo ICMS00, ICMS10, etc->->->
            this->m_xmlw->writeTextElement("orig", orig);
            this->m_xmlw->writeTextElement("CST", cst);
            //ICMS00 - N02
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS00)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }
            }

            //ICMS10 - N03
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS10)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMSST()));
                }
            }

            //ICMS20 - N04
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS20)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS()));
                }
            }

            //ICMS30 - N05
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS30)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS()));
                }
            }

            // ICMS40 - N06 - ICMS= 40, 41, 50
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS40 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS41 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS50)
            {
                //ICMS=40, 41, 50
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS()));
                }
            }

            //ICMS51 - N07
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS51)
            {
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC() != ModBC::None)
                    this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC() > 0)
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC() > 0)
                    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS() > 0)
                    this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSOp() > 0)
                    this->m_xmlw->writeTextElement("vICMSOp", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSOp(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pDif() > 0)
                    this->m_xmlw->writeTextElement("pDif", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pDif(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDif() > 0)
                    this->m_xmlw->writeTextElement("vICMSDif", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDif(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS() > 0)
                    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPDif() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPDif() > 0)
                {
                    this->m_xmlw->writeTextElement("pFCPDif", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPDif(), 4));
                    this->m_xmlw->writeTextElement("vFCPDif", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPDif(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPEfet() > 0)
                   this->m_xmlw->writeTextElement("vFCPEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPEfet(), 2));
            }

            //ICMS60 - N08
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS60)
            {
                //Nota Técnica 2018->005 - v 1->30 - Rejeicao 938
                if (this->m_infNFe->ide->get_indFinal() == IndFinal::Normal         ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTRet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST() > 0      ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTRet() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTRet(), 2));
                    this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST(), 4));
                    this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSubstituto(), 2));
                    this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTRet(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet() > 0    ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet() > 0)
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet(), 2));
                }
            }

            //ICMSPart - N10a
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSPart10 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSPart90)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC() > 0)
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));
                this->m_xmlw->writeTextElement("pBCOp", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pBCOp(), 4));
                this->m_xmlw->writeTextElement("UFST", this->m_infNFe->det->items->value(i)->imposto->ICMS->get_UFST());
            }

            //ICMS70 - N09
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS70)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }

                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS()));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMSST()));
                }
            }

            //ICMS90 - N10 e ICMSPart - N10a
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMS90)
            {
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC() != ModBC::None ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC() > 0    ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS() > 0)
                {
                    this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC() > 0)
                        this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                    this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCP(), 2));
                    this->m_xmlw->writeTextElement("pFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCP(), 4));
                    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCP(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST() != ModBCST::None ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST() > 0    ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST())
                {
                    this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                        this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                        this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                    this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                    this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS() != MotDesICMS::None)
                {
                    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMS", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMS()));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson() > 0)
                {
                    this->m_xmlw->writeTextElement("vICMSSTDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDeson(), 2));
                    this->m_xmlw->writeTextElement("motDesICMSST", ConvNF::motDesICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_motDesICMSST()));
                }
            }

            //ICMSST - N10b
            if(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSST41 ||
               this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSST60)
            {
                this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTRet(), 2));
                //so deve exibir caso seja maior que zero ou cst 60 e consumidor não final
                //Nota Técnica 2018->005 - v 1->30 - Rejeicao 938
                if ((this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST() > 0) ||
                    (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSST60 &&
                     this->m_infNFe->ide->get_indFinal() == IndFinal::Normal))
                        this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST(), 4));
                //so deve exibir caso seja maior que zero ou cst 60 e consumidor não final
                //Nota Técnica 2018->005 - v 1->30 - Rejeicao 938
                if ((this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSubstituto() > 0) ||
                    (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::ICMSST60 &&
                     this->m_infNFe->ide->get_indFinal() == IndFinal::Normal))
                        this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSubstituto(), 2));

                this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTRet(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTDest() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDest() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("vBCSTDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTDest(), 2));
                    this->m_xmlw->writeTextElement("vICMSSTDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTDest(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet() > 0    ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet() > 0)
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet(), 2));
                }

            }

            this->m_xmlw->writeEndElement();//fechamento grupo ICMS00, ICMS10, etc->->->

        }else //CSOSN Grupo CRT=1 – Simples Nacional
        {
            QString csosn = ConvNF::csosnICMSToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN());
            QString cab = QString("%1%2").arg("ICMSSN", csosn);

            this->m_xmlw->writeStartElement(cab); //abertura grupo ICMSSN101, ICMSSN102, etc->->->
            this->m_xmlw->writeTextElement("orig", orig);
            this->m_xmlw->writeTextElement("CSOSN", csosn);

            //CSOSN=101
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN101)
            {
                this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pCredSN(), 4));
                this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vCredICMSSN(), 2));
            }
            //CSON 102, 103, 300 ou 400
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN102 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN103 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN300 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN400)
            {
                //não ha nada ate o momento
            }
            //CSON 201
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN201)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pCredSN() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vCredICMSSN() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                    this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pCredSN(), 4));
                    this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vCredICMSSN(), 2));
                }
            }
            //CSON 202 ou 203
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN202 ||
                this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN203)
            {
                this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                    this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                    this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }
            }
            //CSON 500
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN500)
            {
                //Nota Técnica 2018->005 - v 1->30 - Rejeicao 938
                if (this->m_infNFe->ide->get_indFinal() == IndFinal::Normal      ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTRet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST() > 0      ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTRet() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCSTRet(), 2));
                    this->m_xmlw->writeTextElement("pST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pST(), 4));
                    this->m_xmlw->writeTextElement("vICMSSubstituto", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSubstituto(), 2));
                    this->m_xmlw->writeTextElement("vICMSSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSSTRet(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPSTRet(), 2));
                    this->m_xmlw->writeTextElement("pFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPSTRet(), 4));
                    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPSTRet(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet() > 0    ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet() > 0)
                {
                    this->m_xmlw->writeTextElement("pRedBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCEfet(), 4));
                    this->m_xmlw->writeTextElement("vBCEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCEfet(), 2));
                    this->m_xmlw->writeTextElement("pICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSEfet(), 4));
                    this->m_xmlw->writeTextElement("vICMSEfet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSEfet(), 2));
                }
            }
            //CSON 900
            if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::ICMSSN900)
            {
                this->m_xmlw->writeTextElement("modBC", ConvNF::modBCToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBC()));
                this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBC(), 2));
                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC() > 0)
                    this->m_xmlw->writeTextElement("pRedBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBC(), 4));
                this->m_xmlw->writeTextElement("pICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMS(), 4));
                this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMS(), 2));

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST() != ModBCST::None ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST() > 0)
                {
                    this->m_xmlw->writeTextElement("modBCST", ConvNF::modBCSTToStr(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_modBCST()));
                    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST() > 0)
                        this->m_xmlw->writeTextElement("pMVAST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pMVAST(), 4));
                    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST() > 0)
                        this->m_xmlw->writeTextElement("pRedBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pRedBCST(), 4));
                    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCST(), 2));
                    this->m_xmlw->writeTextElement("pICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pICMSST(), 4));
                    this->m_xmlw->writeTextElement("vICMSST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vICMSST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST() > 0 ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST() > 0   ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST() > 0)
                {
                    this->m_xmlw->writeTextElement("vBCFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vBCFCPST(), 2));
                    this->m_xmlw->writeTextElement("pFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pFCPST(), 4));
                    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vFCPST(), 2));
                }

                if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pCredSN() > 0  ||
                    this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vCredICMSSN() > 0)
                {
                    this->m_xmlw->writeTextElement("pCredSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_pCredSN(), 4));
                    this->m_xmlw->writeTextElement("vCredICMSSN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMS->get_vCredICMSSN(), 2));
                }
            }

            this->m_xmlw->writeEndElement();//fechamento grupo ICMSSN101, ICMSSN102, etc->->->
        }

        this->m_xmlw->writeEndElement();//fechamento grupo ICMS
    }
}

void XmlWrite::get_ICMSUFDest(const int &i)
{   //ICMSUFDest - NA01
    if (this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vBCUFDest() > 0      ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vBCFCPUFDest() > 0   ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSUFDest() > 0    ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSInter() > 0     ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSInterPart() > 0 ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vICMSUFDest() > 0    ||
        this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vICMSUFRemet() > 0)
    {
        this->m_xmlw->writeStartElement("ICMSUFDest"); //abertura grupo ICMSUFDest
        this->m_xmlw->writeTextElement("vBCUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vBCUFDest(), 2));
        this->m_xmlw->writeTextElement("vBCFCPUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vBCFCPUFDest(), 2));
        if (this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pFCPUFDest() > 0)
            this->m_xmlw->writeTextElement("pFCPUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pFCPUFDest(), 4));
        this->m_xmlw->writeTextElement("pICMSUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSUFDest(), 4));
        this->m_xmlw->writeTextElement("pICMSInter", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSInter(), 2));
        this->m_xmlw->writeTextElement("pICMSInterPart", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_pICMSInterPart(), 4));
        if (this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vFCPUFDest() > 0)
            this->m_xmlw->writeTextElement("vFCPUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vFCPUFDest(), 2));
        this->m_xmlw->writeTextElement("vICMSUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vICMSUFDest(), 2));
        this->m_xmlw->writeTextElement("vICMSUFRemet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ICMSUFDest->get_vICMSUFRemet(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo ICMSUFDest
    }
}

void XmlWrite::get_IPI(const int &i)
{   //IPI - O01
    if (this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() != CstIPI::None ||
        !this->m_infNFe->det->items->value(i)->imposto->IPI->get_CNPJProd().isEmpty() ||
        !this->m_infNFe->det->items->value(i)->imposto->IPI->get_cSelo().isEmpty()    ||
        this->m_infNFe->det->items->value(i)->imposto->IPI->get_qSelo() > 0           ||
        !this->m_infNFe->det->items->value(i)->imposto->IPI->get_cEnq().isEmpty())
    {
        this->m_xmlw->writeStartElement("IPI"); //abertura grupo IPI
        if (!this->m_infNFe->det->items->value(i)->imposto->IPI->get_CNPJProd().isEmpty())
            this->m_xmlw->writeTextElement("CNPJProd", this->m_infNFe->det->items->value(i)->imposto->IPI->get_CNPJProd());
        if (!this->m_infNFe->det->items->value(i)->imposto->IPI->get_cSelo().isEmpty())
            this->m_xmlw->writeTextElement("cSelo", this->m_infNFe->det->items->value(i)->imposto->IPI->get_cSelo());
        if (this->m_infNFe->det->items->value(i)->imposto->IPI->get_qSelo() > 0)
            this->m_xmlw->writeTextElement("qSelo", QString::number(this->m_infNFe->det->items->value(i)->imposto->IPI->get_qSelo()));
        this->m_xmlw->writeTextElement("cEnq", this->m_infNFe->det->items->value(i)->imposto->IPI->get_cEnq());
        //IPITrib - O07
        if (this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI00 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI49 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI50 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI99)
        {
            this->m_xmlw->writeStartElement("IPITrib"); //abertura grupo IPITrib
            this->m_xmlw->writeTextElement("CST", ConvNF::cstIPIToStr(this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->IPI->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pIPI", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->IPI->get_pIPI(), 4));
            this->m_xmlw->writeTextElement("qUnid", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->IPI->get_qUnid(), 4));
            this->m_xmlw->writeTextElement("vUnid", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->IPI->get_vUnid(), 4));
            this->m_xmlw->writeTextElement("vIPI", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->IPI->get_vIPI(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo IPITrib
        }
        //IPINT - O08
        if (this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI01 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI02 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI03 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI04 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI51 ||
            this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST() == CstIPI::IPI52)
        {
            this->m_xmlw->writeStartElement("IPINT"); //abertura grupo IPINT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstIPIToStr(this->m_infNFe->det->items->value(i)->imposto->IPI->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo IPINT
        }

        this->m_xmlw->writeEndElement();//fechamento grupo IPI

    }
}

void XmlWrite::get_II(const int &i)
{   //II - P01
    //para condição so o vBC basta ja que todos são obrigatorios, caso queira exibir o grupo
    if (this->m_infNFe->det->items->value(i)->imposto->II->get_vBC() > 0)
    {
        this->m_xmlw->writeStartElement("II"); //abertura grupo II
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->II->get_vBC(), 2));
        this->m_xmlw->writeTextElement("vDespAdu", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->II->get_vDespAdu(), 2));
        this->m_xmlw->writeTextElement("vII", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->II->get_vII(), 2));
        this->m_xmlw->writeTextElement("vIOF", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->II->get_vIOF(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo II
    }
}

void XmlWrite::get_PIS(const int &i)
{   //PIS - Q01
    //so o CST basta ja que e obrigatorio
    if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() != CstPIS::None)
    {
        this->m_xmlw->writeStartElement("PIS");//abertura grupo PIS
        //PISAliq - Q02
        if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS01 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS02)
        {
            this->m_xmlw->writeStartElement("PISAliq");//abertura grupo PISAliq
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_pPIS(), 4));
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISAliq
        }
        //PISQtde - Q03
        if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS03)
        {
            this->m_xmlw->writeStartElement("PISQtde");//abertura grupo PISQtde
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST()));
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISQtde
        }
        //PISNT - Q04
        if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS04 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS05 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS06 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS07 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS08 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS09)
        {
            this->m_xmlw->writeStartElement("PISNT");//abertura grupo PISNT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo PISNT
        }
        //PISOutr - Q05
        if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS49 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS50 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS51 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS52 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS53 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS54 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS56 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS56 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS60 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS61 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS62 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS63 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS64 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS65 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS66 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS67 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS70 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS71 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS72 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS73 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS74 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS75 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS98 ||
            this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST() == CstPIS::PIS99)
        {
            this->m_xmlw->writeStartElement("PISOutr");//abertura grupo PISOutr
            this->m_xmlw->writeTextElement("CST", ConvNF::cstPISToStr(this->m_infNFe->det->items->value(i)->imposto->PIS->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_pPIS(), 4));
            if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_qBCProd() > 0)
                this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_qBCProd(), 4));
            if (this->m_infNFe->det->items->value(i)->imposto->PIS->get_vAliqProd() > 0)
                this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PIS->get_vPIS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo PISNT
        }
        this->m_xmlw->writeEndElement();//fechamento grupo PIS
    }
}

void XmlWrite::get_PISST(const int &i)
{   //PISST - R01
    if (this->m_infNFe->det->items->value(i)->imposto->PISST->get_vBC() > 0       ||
        this->m_infNFe->det->items->value(i)->imposto->PISST->get_pPIS() > 0      ||
        this->m_infNFe->det->items->value(i)->imposto->PISST->get_qBCProd() > 0   ||
        this->m_infNFe->det->items->value(i)->imposto->PISST->get_vAliqProd() > 0 ||
        this->m_infNFe->det->items->value(i)->imposto->PISST->get_vPIS() > 0       )
    {
        this->m_xmlw->writeStartElement("PISST");//abertura grupo PISST
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PISST->get_vBC(), 2));
        this->m_xmlw->writeTextElement("pPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PISST->get_pPIS(), 4));
        this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PISST->get_qBCProd(), 4));
        this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PISST->get_vAliqProd(), 4));
        this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->PISST->get_vPIS(), 2));
        this->m_xmlw->writeTextElement("indSomaPISST", ConvNF::indTotToStr(this->m_infNFe->det->items->value(i)->imposto->PISST->get_indSomaPISST()));
        this->m_xmlw->writeEndElement();//fechamento grupo PISST
    }
}

void XmlWrite::get_COFINS(const int &i)
{   //COFINS - S01
    //so o CST basta ja que e obrigatorio
    if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() != CstCOFINS::None)
    {
        this->m_xmlw->writeStartElement("COFINS");//abertura grupo COFINS
        //COFINSAliq - S02
        if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS01 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS02)
        {
            this->m_xmlw->writeStartElement("COFINSAliq");//abertura grupo COFINSAliq
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_pCOFINS(), 4));
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSAliq
        }
        //COFINSQtde - S03
        if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS03)
        {
            this->m_xmlw->writeStartElement("COFINSQtde");//abertura grupo COFINSQtde
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST()));
            this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_qBCProd(), 4));
            this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSQtde
        }
        //COFINSNT - S04
        if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS04 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS05 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS06 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS07 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS08 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS09)
        {
            this->m_xmlw->writeStartElement("COFINSNT");//abertura grupo COFINSNT
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST()));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSNT
        }
        //COFINSOutr - S05
        if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS49 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS50 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS51 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS52 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS53 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS54 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS56 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS56 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS60 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS61 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS62 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS63 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS64 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS65 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS66 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS67 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS70 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS71 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS72 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS73 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS74 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS75 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS98 ||
            this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST() == CstCOFINS::COFINS99)
        {
            this->m_xmlw->writeStartElement("COFINSOutr");//abertura grupo COFINSOutr
            this->m_xmlw->writeTextElement("CST", ConvNF::cstCOFINSToStr(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_CST()));
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vBC(), 2));
            this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_pCOFINS(), 4));
            if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_qBCProd() > 0)
                this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_qBCProd(), 4));
            if (this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vAliqProd() > 0)
                this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vAliqProd(), 4));
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINS->get_vCOFINS(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo COFINSNT
        }
        this->m_xmlw->writeEndElement();//fechamento grupo COFINS
    }
}

void XmlWrite::get_COFINSST(const int &i)
{   //COFINSST - T01
    if (this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vBC() > 0       ||
        this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_pCOFINS() > 0      ||
        this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_qBCProd() > 0   ||
        this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vAliqProd() > 0 ||
        this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vCOFINS() > 0)
    {
        this->m_xmlw->writeStartElement("COFINSST");//abertura grupo COFINSST
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vBC(), 2));
        this->m_xmlw->writeTextElement("pCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_pCOFINS(), 4));
        this->m_xmlw->writeTextElement("qBCProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_qBCProd(), 4));
        this->m_xmlw->writeTextElement("vAliqProd", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vAliqProd(), 4));
        this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_vCOFINS(), 2));
        this->m_xmlw->writeTextElement("indSomaCOFINSST", ConvNF::indTotToStr(this->m_infNFe->det->items->value(i)->imposto->COFINSST->get_indSomaCOFINSST()));
        this->m_xmlw->writeEndElement();//fechamento grupo COFINSST
    }
}

void XmlWrite::get_ISSQN(const int &i)
{   //ISSQN - U01
    if (this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CST() == CstICMS::None &&
        this->m_infNFe->det->items->value(i)->imposto->ICMS->get_CSOSN() == CsosnICMS::None)
    {
        this->m_xmlw->writeStartElement("ISSQN");//abertura grupo ISSQN
        this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vBC(), 2));
        this->m_xmlw->writeTextElement("vAliq", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vAliq(), 4));
        this->m_xmlw->writeTextElement("vISSQN", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vISSQN(), 2));
        this->m_xmlw->writeTextElement("cMunFG", QString::number(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cMunFG()));
        this->m_xmlw->writeTextElement("cListServ", this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cListServ());
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDeducao() > 0)
            this->m_xmlw->writeTextElement("vDeducao", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDeducao(), 2));
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vOutro() > 0)
            this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vOutro(), 2));
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDescIncond() > 0)
            this->m_xmlw->writeTextElement("vDescIncond", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDescIncond(), 2));
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDescCond() > 0)
            this->m_xmlw->writeTextElement("vDescCond", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vDescCond(), 2));
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vISSRet() > 0)
            this->m_xmlw->writeTextElement("vISSRet", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_vISSRet(), 2));
        this->m_xmlw->writeTextElement("indISS", ConvNF::indISSToStr(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_indISS()));
        if (!this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cServico().isEmpty())
            this->m_xmlw->writeTextElement("cServico", this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cServico());
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cMun() > 0)
            this->m_xmlw->writeTextElement("cMun", QString::number(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cMun()));
        if (this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cPais() > 0)
            this->m_xmlw->writeTextElement("cPais", QString::number(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_cPais()));
        if (!this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_nProcesso().isEmpty())
            this->m_xmlw->writeTextElement("nProcesso", this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_nProcesso());
        this->m_xmlw->writeTextElement("indIncentivo", ConvNF::indIncentivoToStr(this->m_infNFe->det->items->value(i)->imposto->ISSQN->get_indIncentivo()));
        this->m_xmlw->writeEndElement();//fechamento grupo ISSQN
    }
}

void XmlWrite::get_impostoDevol(const int &i)
{   //impostoDevol - UA01
    if (this->m_infNFe->det->items->value(i)->impostoDevol->get_pDevol() > 0)
    {
        this->m_xmlw->writeStartElement("impostoDevol");//abertura grupo impostoDevol
        this->m_xmlw->writeTextElement("pDevol", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->impostoDevol->get_pDevol(), 2));
        this->m_xmlw->writeStartElement("IPI");//abertura grupo IPI
        this->m_xmlw->writeTextElement("vIPIDevol", CppUtility::doubleToStrDecimal(this->m_infNFe->det->items->value(i)->impostoDevol->get_vIPIDevol(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo IPI
        this->m_xmlw->writeEndElement();//fechamento grupo impostoDevol
    }
}

void XmlWrite::get_obsItem(const int &i)
{   //obsItem - VA01
    if (!this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xCampo().isEmpty()  ||
        !this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xTexto().isEmpty()  ||
        !this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xCampo().isEmpty() ||
        !this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xTexto().isEmpty())
    {
        //obsCont - VA02
        if (!this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xCampo().isEmpty() ||
            !this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xTexto().isEmpty())
        {
            this->m_xmlw->writeStartElement("obsCont");//abertura grupo obsCont
            this->m_xmlw->writeTextElement("xCampo", this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xCampo());
            this->m_xmlw->writeTextElement("xTexto", this->m_infNFe->det->items->value(i)->obsItem->obsCont->get_xTexto());
            this->m_xmlw->writeEndElement();//fechamento grupo obsCont
        }
        //obsFisco - VA05
        if (!this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xCampo().isEmpty() ||
            !this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xTexto().isEmpty())
        {
            this->m_xmlw->writeStartElement("obsFisco");//abertura grupo obsFisco
            this->m_xmlw->writeTextElement("xCampo", this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xCampo());
            this->m_xmlw->writeTextElement("xTexto", this->m_infNFe->det->items->value(i)->obsItem->obsFisco->get_xTexto());
            this->m_xmlw->writeEndElement();//fechamento grupo obsFisco
        }
    }
}

void XmlWrite::get_total()
{
    //total - W01
    this->m_xmlw->writeStartElement("total");//abertura grupo total

    //ICMSTot - W02
    this->m_xmlw->writeStartElement("ICMSTot");//abertura grupo ICMSTot
    this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vBC(), 2));
    this->m_xmlw->writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vICMS(), 2));
    this->m_xmlw->writeTextElement("vICMSDeson", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vICMSDeson(), 2));
    if (this->m_infNFe->total->ICMSTot->get_vFCPUFDest() > 0)
        this->m_xmlw->writeTextElement("vFCPUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vFCPUFDest(), 2));
    if (this->m_infNFe->total->ICMSTot->get_vICMSUFDest() > 0)
        this->m_xmlw->writeTextElement("vICMSUFDest", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vICMSUFDest(), 2));
    if (this->m_infNFe->total->ICMSTot->get_vICMSUFRemet() > 0)
        this->m_xmlw->writeTextElement("vICMSUFRemet", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vICMSUFRemet(), 2));
    this->m_xmlw->writeTextElement("vFCP", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vFCP(), 2));
    this->m_xmlw->writeTextElement("vBCST", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vBCST(), 2));
    this->m_xmlw->writeTextElement("vST", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vST(), 2));
    this->m_xmlw->writeTextElement("vFCPST", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vFCPST(), 2));
    this->m_xmlw->writeTextElement("vFCPSTRet", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vFCPSTRet(), 2));
    this->m_xmlw->writeTextElement("vProd", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vProd(), 2));
    this->m_xmlw->writeTextElement("vFrete", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vFrete(), 2));
    this->m_xmlw->writeTextElement("vSeg", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vSeg(), 2));
    this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vDesc(), 2));
    this->m_xmlw->writeTextElement("vII", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vII(), 2));
    this->m_xmlw->writeTextElement("vIPI", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vIPI(), 2));
    this->m_xmlw->writeTextElement("vIPIDevol", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vIPIDevol(), 2));
    this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vPIS(), 2));
    this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vCOFINS(), 2));
    this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vOutro(), 2));
    this->m_xmlw->writeTextElement("vNF", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vNF(), 2));
    if (this->m_infNFe->total->ICMSTot->get_vTotTrib() > 0)
        this->m_xmlw->writeTextElement("vTotTrib", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ICMSTot->get_vTotTrib(), 2));
    this->m_xmlw->writeEndElement();//fechamento grupo ICMSTot

    //ISSQNtot - W17
    if(this->m_infNFe->total->ISSQNtot->get_vServ() > 0 ||
       this->m_infNFe->total->ISSQNtot->get_vBC () > 0  ||
       this->m_infNFe->total->ISSQNtot->get_vISS() > 0  ||
       this->m_infNFe->total->ISSQNtot->get_vPIS() > 0  ||
       this->m_infNFe->total->ISSQNtot->get_vCOFINS() > 0)
    {
        this->m_xmlw->writeStartElement("ISSQNtot");//abertura grupo ISSQNtot
        if (this->m_infNFe->total->ISSQNtot->get_vServ() > 0)
            this->m_xmlw->writeTextElement("vServ", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vServ(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vBC() > 0)
            this->m_xmlw->writeTextElement("vBC", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vBC(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vISS() > 0)
            this->m_xmlw->writeTextElement("vISS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vISS(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vPIS() > 0)
            this->m_xmlw->writeTextElement("vPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vPIS(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vCOFINS() > 0)
            this->m_xmlw->writeTextElement("vCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vCOFINS(), 2));
        this->m_xmlw->writeTextElement("dCompet", CppUtility::dateTimeToStr(this->m_infNFe->total->ISSQNtot->get_dCompet(), DtH::Date));
        if (this->m_infNFe->total->ISSQNtot->get_vDeducao() > 0)
            this->m_xmlw->writeTextElement("vDeducao", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vDeducao(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vOutro() > 0)
            this->m_xmlw->writeTextElement("vOutro", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vOutro(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vDescIncond() > 0)
            this->m_xmlw->writeTextElement("vDescIncond", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vDescIncond(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vDescCond() > 0)
            this->m_xmlw->writeTextElement("vDescCond", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vDescCond(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_vISSRet() > 0)
            this->m_xmlw->writeTextElement("vISSRet", CppUtility::doubleToStrDecimal(this->m_infNFe->total->ISSQNtot->get_vISSRet(), 2));
        if (this->m_infNFe->total->ISSQNtot->get_cRegTrib() != CRegTrib::None)
            this->m_xmlw->writeTextElement("cRegTrib", ConvNF::cRegTribToStr(this->m_infNFe->total->ISSQNtot->get_cRegTrib()));
        this->m_xmlw->writeEndElement();//fechamento grupo ISSQNtot
    }

    //retTrib - W23
    if (this->m_infNFe->total->retTrib->get_vRetPIS() > 0 ||
        this->m_infNFe->total->retTrib->get_vRetCOFINS() > 0 ||
        this->m_infNFe->total->retTrib->get_vRetCSLL() > 0 ||
        this->m_infNFe->total->retTrib->get_vBCIRRF() > 0 ||
        this->m_infNFe->total->retTrib->get_vIRRF() > 0 ||
        this->m_infNFe->total->retTrib->get_vBCRetPrev() > 0 ||
        this->m_infNFe->total->retTrib->get_vRetPrev() > 0 )
    {
        this->m_xmlw->writeStartElement("retTrib");//abertura grupo retTrib
        if (this->m_infNFe->total->retTrib->get_vRetPIS() > 0)
            this->m_xmlw->writeTextElement("vRetPIS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vRetPIS(), 2));
        if (this->m_infNFe->total->retTrib->get_vRetCOFINS() > 0)
            this->m_xmlw->writeTextElement("vRetCOFINS", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vRetCOFINS(), 2));
        if (this->m_infNFe->total->retTrib->get_vRetCSLL() > 0)
            this->m_xmlw->writeTextElement("vRetCSLL", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vRetCSLL(), 2));
        if (this->m_infNFe->total->retTrib->get_vBCIRRF() > 0)
            this->m_xmlw->writeTextElement("vBCIRRF", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vBCIRRF(), 2));
        if (this->m_infNFe->total->retTrib->get_vIRRF() > 0)
            this->m_xmlw->writeTextElement("vIRRF", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vIRRF(), 2));
        if (this->m_infNFe->total->retTrib->get_vBCRetPrev() > 0)
            this->m_xmlw->writeTextElement("vBCRetPrev", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vBCRetPrev(), 2));
        if (this->m_infNFe->total->retTrib->get_vRetPrev() > 0)
            this->m_xmlw->writeTextElement("vRetPrev", CppUtility::doubleToStrDecimal(this->m_infNFe->total->retTrib->get_vRetPrev(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo retTrib
    }

    this->m_xmlw->writeEndElement();//fechamento grupo total
}

void XmlWrite::get_transp()
{   //transp - X01
    this->m_xmlw->writeStartElement("transp");//abertura grupo transp
    this->m_xmlw->writeTextElement("modFrete", ConvNF::modFreteToStr(this->m_infNFe->transp->get_modFrete()));

    //transporta - X03
    if (this->m_infNFe->transp->get_modFrete() != ModFrete::SemFrete)
    {
        if (!this->m_infNFe->transp->transporta->get_CNPJ().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_CPF().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_xNome().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_IE().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_xEnder().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_xMun().isEmpty() ||
            !this->m_infNFe->transp->transporta->get_UF().isEmpty())
        {
            this->m_xmlw->writeStartElement("transp");//abertura grupo transporta
            if (!this->m_infNFe->transp->transporta->get_CNPJ().isEmpty())
                this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->transp->transporta->get_CNPJ());
            if (!this->m_infNFe->transp->transporta->get_CPF().isEmpty())
                this->m_xmlw->writeTextElement("CPF", this->m_infNFe->transp->transporta->get_CPF());
            if (!this->m_infNFe->transp->transporta->get_xNome().isEmpty())
                this->m_xmlw->writeTextElement("xNome", this->m_infNFe->transp->transporta->get_xNome());
            if (!this->m_infNFe->transp->transporta->get_IE().isEmpty())
                this->m_xmlw->writeTextElement("IE", this->m_infNFe->transp->transporta->get_IE());
            if (!this->m_infNFe->transp->transporta->get_xEnder().isEmpty())
                this->m_xmlw->writeTextElement("xEnder", this->m_infNFe->transp->transporta->get_xEnder());
            if (!this->m_infNFe->transp->transporta->get_xMun().isEmpty())
                this->m_xmlw->writeTextElement("xMun", this->m_infNFe->transp->transporta->get_xMun());
            if (!this->m_infNFe->transp->transporta->get_UF().isEmpty())
                this->m_xmlw->writeTextElement("UF", this->m_infNFe->transp->transporta->get_UF());
            this->m_xmlw->writeEndElement();//fechamento grupo transporta
        }
    }
    //retTransp - X11
    if (this->m_infNFe->transp->retTransp->get_vServ() > 0 ||
        this->m_infNFe->transp->retTransp->get_vBCRet() > 0 ||
        this->m_infNFe->transp->retTransp->get_pICMSRet() > 0 ||
        this->m_infNFe->transp->retTransp->get_vICMSRet() > 0 ||
        !this->m_infNFe->transp->retTransp->get_CFOP().isEmpty() ||
        this->m_infNFe->transp->retTransp->get_cMunFG() > 0 )
    {
        this->m_xmlw->writeStartElement("retTransp");//abertura grupo retTransp
        this->m_xmlw->writeTextElement("vServ", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->retTransp->get_vServ(), 2));
        this->m_xmlw->writeTextElement("vBCRet", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->retTransp->get_vBCRet(), 2));
        this->m_xmlw->writeTextElement("pICMSRet", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->retTransp->get_pICMSRet(), 4));
        this->m_xmlw->writeTextElement("vICMSRet", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->retTransp->get_vICMSRet(), 2));
        this->m_xmlw->writeTextElement("CFOP", this->m_infNFe->transp->retTransp->get_CFOP());
        this->m_xmlw->writeTextElement("cMunFG", QString::number(this->m_infNFe->transp->retTransp->get_cMunFG()));
        this->m_xmlw->writeEndElement();//fechamento grupo retTransp
    }
    //veicTransp - X18
    if (!this->m_infNFe->transp->veicTransp->get_placa().isEmpty() ||
        !this->m_infNFe->transp->veicTransp->get_UF().isEmpty() ||
        !this->m_infNFe->transp->veicTransp->get_RNTC().isEmpty())
    {
        this->m_xmlw->writeStartElement("veicTransp");//abertura grupo veicTransp
        this->m_xmlw->writeTextElement("placa", this->m_infNFe->transp->veicTransp->get_placa());
        this->m_xmlw->writeTextElement("UF", this->m_infNFe->transp->veicTransp->get_UF());
        if (!this->m_infNFe->transp->veicTransp->get_RNTC().isEmpty())
            this->m_xmlw->writeTextElement("RNTC", this->m_infNFe->transp->veicTransp->get_RNTC());
        this->m_xmlw->writeEndElement();//fechamento grupo veicTransp
    }
    //reboque - X22
    if (this->m_infNFe->transp->reboque->items->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->transp->reboque->items->count(); ++i)
        {
            this->m_xmlw->writeStartElement("reboque");//abertura grupo reboque
            this->m_xmlw->writeTextElement("placa", this->m_infNFe->transp->reboque->items->value(i)->get_placa());
            this->m_xmlw->writeTextElement("UF", this->m_infNFe->transp->reboque->items->value(i)->get_UF());
            if (!this->m_infNFe->transp->reboque->items->value(i)->get_RNTC().isEmpty())
                this->m_xmlw->writeTextElement("RNTC", this->m_infNFe->transp->reboque->items->value(i)->get_RNTC());
            this->m_xmlw->writeEndElement();//fechamento grupo reboque
        }
    }
    //continuacao de X01
    if (!this->m_infNFe->transp->get_vagao().isEmpty())
        this->m_xmlw->writeTextElement("vagao", this->m_infNFe->transp->get_vagao());
    if (!this->m_infNFe->transp->get_balsa().isEmpty())
        this->m_xmlw->writeTextElement("balsa", this->m_infNFe->transp->get_balsa());
    //vol - X26
    if (this->m_infNFe->transp->vol->items->count() > 0)
    {
        for(int i = 0; i < this->m_infNFe->transp->vol->items->count(); ++i)
        {
            this->m_xmlw->writeStartElement("vol");//abertura grupo vol
            if (this->m_infNFe->transp->vol->items->value(i)->get_qVol() > 0)
                this->m_xmlw->writeTextElement("qVol", QString::number(this->m_infNFe->transp->vol->items->value(i)->get_qVol()));
            if (!this->m_infNFe->transp->vol->items->value(i)->get_esp().isEmpty())
                this->m_xmlw->writeTextElement("esp", this->m_infNFe->transp->vol->items->value(i)->get_esp());
            if (!this->m_infNFe->transp->vol->items->value(i)->get_marca().isEmpty())
                this->m_xmlw->writeTextElement("marca", this->m_infNFe->transp->vol->items->value(i)->get_marca());
            if (!this->m_infNFe->transp->vol->items->value(i)->get_nVol().isEmpty())
                this->m_xmlw->writeTextElement("nVol", this->m_infNFe->transp->vol->items->value(i)->get_nVol());
            if (this->m_infNFe->transp->vol->items->value(i)->get_pesoL() > 0)
                this->m_xmlw->writeTextElement("pesoL", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->vol->items->value(i)->get_pesoL(), 3));;
            if (this->m_infNFe->transp->vol->items->value(i)->get_pesoB() > 0)
                this->m_xmlw->writeTextElement("pesoB", CppUtility::doubleToStrDecimal(this->m_infNFe->transp->vol->items->value(i)->get_pesoB(), 3));;
            //lacres - X33
            if (this->m_infNFe->transp->vol->items->value(i)->lacres->items->count() > 0)
            {
                for(int j = 0; j < this->m_infNFe->transp->vol->items->value(i)->lacres->items->count(); ++j)
                {
                    this->m_xmlw->writeStartElement("lacres");//abertura grupo lacres
                    this->m_xmlw->writeTextElement("nLacre", this->m_infNFe->transp->vol->items->value(i)->lacres->items->value(j)->get_nLacre());
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
    if (!this->m_infNFe->cobr->fat->get_nFat().isEmpty() ||
        this->m_infNFe->cobr->fat->get_vOrig() > 0 ||
        this->m_infNFe->cobr->fat->get_vDesc() > 0 ||
        this->m_infNFe->cobr->fat->get_vLiq() > 0 ||
        this->m_infNFe->cobr->dup->items->count() > 0)
    {
        this->m_xmlw->writeStartElement("cobr");//abertura grupo cobr
        //fat - Y02
        if (!this->m_infNFe->cobr->fat->get_nFat().isEmpty() ||
            this->m_infNFe->cobr->fat->get_vOrig() > 0 ||
            this->m_infNFe->cobr->fat->get_vDesc() > 0 ||
            this->m_infNFe->cobr->fat->get_vLiq() > 0)
        {
            this->m_xmlw->writeStartElement("fat");//abertura grupo fat
            if (!this->m_infNFe->cobr->fat->get_nFat().isEmpty())
                this->m_xmlw->writeTextElement("nFat", this->m_infNFe->cobr->fat->get_nFat());
            if (this->m_infNFe->cobr->fat->get_vOrig() > 0)
                this->m_xmlw->writeTextElement("vOrig", CppUtility::doubleToStrDecimal(this->m_infNFe->cobr->fat->get_vOrig(), 2));
            if (this->m_infNFe->cobr->fat->get_vDesc() > 0)
                this->m_xmlw->writeTextElement("vDesc", CppUtility::doubleToStrDecimal(this->m_infNFe->cobr->fat->get_vDesc(), 2));
            if (this->m_infNFe->cobr->fat->get_vLiq() > 0)
                this->m_xmlw->writeTextElement("vLiq", CppUtility::doubleToStrDecimal(this->m_infNFe->cobr->fat->get_vLiq(), 2));
            this->m_xmlw->writeEndElement();//fechamento grupo fat
        }
        //dup - Y07
        if (this->m_infNFe->cobr->dup->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->cobr->dup->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("dup");//abertura grupo dup
                if (!this->m_infNFe->cobr->dup->items->value(i)->get_nDup().isEmpty())
                    this->m_xmlw->writeTextElement("nDup", this->m_infNFe->cobr->dup->items->value(i)->get_nDup());
                if (this->m_infNFe->cobr->dup->items->value(i)->get_dVenc().isValid())
                    this->m_xmlw->writeTextElement("dVenc", CppUtility::dateTimeToStr(this->m_infNFe->cobr->dup->items->value(i)->get_dVenc(), DtH::Date));
                this->m_xmlw->writeTextElement("vDup", CppUtility::doubleToStrDecimal(this->m_infNFe->cobr->dup->items->value(i)->get_vDup(), 2));
                this->m_xmlw->writeEndElement();//fechamento grupo dup
            }
        }
        this->m_xmlw->writeEndElement();//fechamento grupo cobr
    }
}

void XmlWrite::get_pag()
{   //pag - YA01
    if (this->m_infNFe->pag->detPag->items->count() > 0 ||
        this->m_infNFe->pag->get_vTroco() > 0)
    {
        this->m_xmlw->writeStartElement("pag");//abertura grupo pag
        //detPag - YA01a
        if (this->m_infNFe->pag->detPag->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->pag->detPag->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("detPag");//abertura grupo detPag
                if (this->m_infNFe->pag->detPag->items->value(i)->get_indPag() != IndPag::None)
                    this->m_xmlw->writeTextElement("indPag", ConvNF::indPagToStr(this->m_infNFe->pag->detPag->items->value(i)->get_indPag()));
                this->m_xmlw->writeTextElement("tPag", ConvNF::tPagToStr(this->m_infNFe->pag->detPag->items->value(i)->get_tPag()));
                if (!this->m_infNFe->pag->detPag->items->value(i)->get_xPag().isEmpty())
                    this->m_xmlw->writeTextElement("xPag", this->m_infNFe->pag->detPag->items->value(i)->get_xPag());
                this->m_xmlw->writeTextElement("vPag", CppUtility::doubleToStrDecimal(this->m_infNFe->pag->detPag->items->value(i)->get_vPag(), 2));
                //card - YA04
                if (this->m_infNFe->pag->detPag->items->value(i)->card->get_tpIntegra() != TpIntegra::None ||
                    !this->m_infNFe->pag->detPag->items->value(i)->card->get_CNPJ().isEmpty() ||
                    this->m_infNFe->pag->detPag->items->value(i)->card->get_tBand() != TBand::None ||
                    !this->m_infNFe->pag->detPag->items->value(i)->card->get_cAut().isEmpty())
                {
                    this->m_xmlw->writeStartElement("card");//abertura grupo card
                    this->m_xmlw->writeTextElement("tpIntegra", ConvNF::tpIntegraToStr(this->m_infNFe->pag->detPag->items->value(i)->card->get_tpIntegra()));
                    if (!this->m_infNFe->pag->detPag->items->value(i)->card->get_CNPJ().isEmpty())
                        this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->pag->detPag->items->value(i)->card->get_CNPJ());
                    if (this->m_infNFe->pag->detPag->items->value(i)->card->get_tBand() != TBand::None)
                        this->m_xmlw->writeTextElement("tBand", ConvNF::tBandToStr(this->m_infNFe->pag->detPag->items->value(i)->card->get_tBand()));
                    if (!this->m_infNFe->pag->detPag->items->value(i)->card->get_cAut().isEmpty())
                        this->m_xmlw->writeTextElement("cAut", this->m_infNFe->pag->detPag->items->value(i)->card->get_cAut());
                    this->m_xmlw->writeEndElement();//fechamento grupo card
                }

                this->m_xmlw->writeEndElement();//fechamento grupo detPag
            }
        }
        //vTroco dentro de YA01
        if (this->m_infNFe->pag->get_vTroco() > 0)
            this->m_xmlw->writeTextElement("vTroco", CppUtility::doubleToStrDecimal(this->m_infNFe->pag->get_vTroco(), 2));
        this->m_xmlw->writeEndElement();//fechamento grupo pag
    }
}

void XmlWrite::get_infIntermed()
{   //YB01 - infIntermed
    if (!this->m_infNFe->infIntermed->get_CNPJ().isEmpty() ||
        !this->m_infNFe->infIntermed->get_idCadIntTran().isEmpty())
    {
        this->m_xmlw->writeStartElement("infIntermed");//abertura grupo infIntermed
        this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->infIntermed->get_CNPJ());
        this->m_xmlw->writeTextElement("idCadIntTran", this->m_infNFe->infIntermed->get_idCadIntTran());
        this->m_xmlw->writeEndElement();//fechamento grupo infIntermed
    }
}

void XmlWrite::get_infAdic()
{   //infAdic - Z01
    if (!this->m_infNFe->infAdic->get_infAdFisco().isEmpty() ||
        !this->m_infNFe->infAdic->get_infCpl().isEmpty() ||
        this->m_infNFe->infAdic->obsCont->items->count() > 0 ||
        this->m_infNFe->infAdic->obsFisco->items->count() > 0 ||
        this->m_infNFe->infAdic->procRef->items->count() > 0)
    {
        this->m_xmlw->writeStartElement("infAdic");//abertura grupo infAdic
        if (!this->m_infNFe->infAdic->get_infAdFisco().isEmpty())
            this->m_xmlw->writeTextElement("infAdFisco", this->m_infNFe->infAdic->get_infAdFisco());
        if (!this->m_infNFe->infAdic->get_infCpl().isEmpty())
            this->m_xmlw->writeTextElement("infCpl", this->m_infNFe->infAdic->get_infCpl());
        //obsCont - Z04
        if (this->m_infNFe->infAdic->obsCont->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->infAdic->obsCont->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("obsCont");//abertura grupo obsCont
                this->m_xmlw->writeTextElement("xCampo", this->m_infNFe->infAdic->obsCont->items->value(i)->get_xCampo());
                this->m_xmlw->writeTextElement("xTexto", this->m_infNFe->infAdic->obsCont->items->value(i)->get_xTexto());
                this->m_xmlw->writeEndElement();//fechamento grupo obsCont
            }
        }
        //obsFisco - Z07
        if (this->m_infNFe->infAdic->obsFisco->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->infAdic->obsFisco->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("obsFisco");//abertura grupo obsFisco
                this->m_xmlw->writeTextElement("xCampo", this->m_infNFe->infAdic->obsFisco->items->value(i)->get_xCampo());
                this->m_xmlw->writeTextElement("xTexto", this->m_infNFe->infAdic->obsFisco->items->value(i)->get_xTexto());
                this->m_xmlw->writeEndElement();//fechamento grupo obsFisco
            }
        }
        //procRef - Z10
        if (this->m_infNFe->infAdic->procRef->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->infAdic->procRef->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("procRef");//abertura grupo procRef
                this->m_xmlw->writeTextElement("nProc", this->m_infNFe->infAdic->procRef->items->value(i)->get_nProc());
                this->m_xmlw->writeTextElement("indProc", ConvNF::indProcToStr(this->m_infNFe->infAdic->procRef->items->value(i)->get_indProc()));
                if (this->m_infNFe->infAdic->procRef->items->value(i)->get_tpAto() != TpAto::None)
                    this->m_xmlw->writeTextElement("tpAto", ConvNF::tpAtoToStr(this->m_infNFe->infAdic->procRef->items->value(i)->get_tpAto()));
                this->m_xmlw->writeEndElement();//fechamento grupo procRef
            }
        }

        this->m_xmlw->writeEndElement();//fechamento grupo infAdic
    }

}

void XmlWrite::get_exporta()
{   //exporta - ZA01
    if (!this->m_infNFe->exporta->get_UFSaidaPais().isEmpty() ||
        !this->m_infNFe->exporta->get_xLocExporta().isEmpty() ||
        !this->m_infNFe->exporta->get_xLocDespacho().isEmpty())
    {
        this->m_xmlw->writeStartElement("exporta");//abertura grupo exporta
        this->m_xmlw->writeTextElement("UFSaidaPais", this->m_infNFe->exporta->get_UFSaidaPais());
        this->m_xmlw->writeTextElement("xLocExporta", this->m_infNFe->exporta->get_xLocExporta());
        if (!this->m_infNFe->exporta->get_xLocDespacho().isEmpty())
            this->m_xmlw->writeTextElement("xLocDespacho", this->m_infNFe->exporta->get_xLocDespacho());
        this->m_xmlw->writeEndElement();//fechamento grupo exporta
    }
}

void XmlWrite::get_compra()
{   //compra - ZB01
    if (!this->m_infNFe->compra->get_xNEmp().isEmpty() ||
        !this->m_infNFe->compra->get_xPed().isEmpty() ||
        !this->m_infNFe->compra->get_xCont().isEmpty())
    {
        this->m_xmlw->writeStartElement("compra");//abertura grupo compra
        if (!this->m_infNFe->compra->get_xNEmp().isEmpty())
            this->m_xmlw->writeTextElement("xNEmp", this->m_infNFe->compra->get_xNEmp());
        if (!this->m_infNFe->compra->get_xPed().isEmpty())
            this->m_xmlw->writeTextElement("xPed", this->m_infNFe->compra->get_xPed());
        if (!this->m_infNFe->compra->get_xCont().isEmpty())
            this->m_xmlw->writeTextElement("xCont", this->m_infNFe->compra->get_xCont());
        this->m_xmlw->writeEndElement();//fechamento grupo compra
    }
}

void XmlWrite::get_cana()
{   //cana - ZC01
    if (!this->m_infNFe->cana->get_safra().isEmpty() ||
        !this->m_infNFe->cana->get_ref().isEmpty())
    {
        this->m_xmlw->writeStartElement("cana");//abertura grupo cana
        this->m_xmlw->writeTextElement("safra", this->m_infNFe->cana->get_safra());
        this->m_xmlw->writeTextElement("ref", this->m_infNFe->cana->get_ref());
        //forDia - ZC04
        if (this->m_infNFe->cana->forDia->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->cana->forDia->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("forDia");//abertura grupo forDia
                this->m_xmlw->writeTextElement("dia", QString::number(this->m_infNFe->cana->forDia->items->value(i)->get_dia()));
                this->m_xmlw->writeTextElement("qtde", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->forDia->items->value(i)->get_qtde(), 10));
                this->m_xmlw->writeEndElement();//fechamento grupo forDia
            }
        }
        //continuacao ZC01
        this->m_xmlw->writeTextElement("qTotMes", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_qTotMes(), 10));
        this->m_xmlw->writeTextElement("qTotAnt", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_qTotAnt(), 10));
        this->m_xmlw->writeTextElement("qTotGer", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_qTotGer(), 10));

        //deduc - ZC10
        if (this->m_infNFe->cana->deduc->items->count() > 0)
        {
            for(int i = 0; i < this->m_infNFe->cana->deduc->items->count(); ++i)
            {
                this->m_xmlw->writeStartElement("deduc");//abertura grupo deduc
                this->m_xmlw->writeTextElement("xDed", this->m_infNFe->cana->deduc->items->value(i)->get_xDed());
                this->m_xmlw->writeTextElement("vDed", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->deduc->items->value(i)->get_vDed(), 2));
                this->m_xmlw->writeEndElement();//fechamento grupo deduc
            }
        }
        //continuacao ZC01
        this->m_xmlw->writeTextElement("vFor", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_vFor(), 2));
        this->m_xmlw->writeTextElement("vTotDed", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_vTotDed(), 2));
        this->m_xmlw->writeTextElement("vLiqFor", CppUtility::doubleToStrDecimal(this->m_infNFe->cana->get_vLiqFor(), 2));

        this->m_xmlw->writeEndElement();//fechamento grupo cana
    }
}

void XmlWrite::get_infRespTec()
{   //infRespTec - ZD01
    if (!this->m_infNFe->infRespTec->get_CNPJ().isEmpty())
    {
        this->m_xmlw->writeStartElement("infRespTec");//abertura grupo infRespTec
        this->m_xmlw->writeTextElement("CNPJ", this->m_infNFe->infRespTec->get_CNPJ());
        this->m_xmlw->writeTextElement("xContato", this->m_infNFe->infRespTec->get_xContato());
        this->m_xmlw->writeTextElement("email", this->m_infNFe->infRespTec->get_email());
        this->m_xmlw->writeTextElement("fone", this->m_infNFe->infRespTec->get_fone());
        //ZD07
        if (this->m_infNFe->infRespTec->get_idCSRT() != 0 &&
            !this->m_infNFe->infRespTec->get_hashCSRT().isEmpty())
        {
            this->m_xmlw->writeTextElement("idCSRT", QString::number(this->m_infNFe->infRespTec->get_idCSRT()));
            this->m_xmlw->writeTextElement("hashCSRT", this->m_infNFe->infRespTec->get_hashCSRT());
        }
        this->m_xmlw->writeEndElement();//fechamento grupo infRespTec
    }
}

void XmlWrite::get_infNFeSupl_ONLINE()
{
    //infNFeSupl - ZX01
    //Caso NFCe e Normal, já devolve o xml com o nó infNFeSupl
     if ((this->m_infNFe->ide->get_mod() == ModeloDF::NFCe) &&
         (this->m_infNFe->ide->get_tpEmis() == TpEmis::Normal))
     {
         QString _chave, _versaoQr, _ambiente, _idcsc, _csc, _qrcode, _inputHash;
         QByteArray _resHash;
         _chave = QString(this->m_infNFe->get_Id()).right(this->m_infNFe->get_Id().length() - 3); //remove NFe da string
         _versaoQr = ConvNF::versaoQrCodeNFToStr(this->m_confgNFe->get_VersaoQrCodeNF()).at(0);//apenas o primeiro numero da versão
         _ambiente = ConvNF::tpAmbToStr(this->m_infNFe->ide->get_tpAmb());
         if (!this->m_confgNFe->get_IdCSC().isEmpty()) //não pode ser empty se não daria erro na conversao para inteiro
           _idcsc = QString::number(this->m_confgNFe->get_IdCSC().toInt());
         _csc = this->m_confgNFe->get_CSC();

         _qrcode = _chave + QStringLiteral("|") + _versaoQr + QStringLiteral("|") +
                   _ambiente + QStringLiteral("|") + _idcsc ;
         _inputHash = _qrcode + _csc;
         //calc sha1
         HashType _sha1 = HashType::sha1;
         if (!CppOpenSSL::digest_s(_sha1, _inputHash.toLocal8Bit(), false, _resHash))
           qWarning() << "Erro ao gerar hash da tag infNFeSupl";

         _qrcode += QStringLiteral("|") + QString(_resHash.toHex().toUpper());
         //url
         QString _urlqrcode = UrlNFe::get_qrCode(ConvNF::intToUF(this->m_infNFe->ide->get_cUF()), this->m_infNFe->ide->get_tpAmb(), this->m_confgNFe->get_VersaoQrCodeNF());
         QString _urlchave = UrlNFe::get_urlChave(ConvNF::intToUF(this->m_infNFe->ide->get_cUF()), this->m_infNFe->ide->get_tpAmb(), this->m_confgNFe->get_VersaoQrCodeNF());

         _qrcode = _urlqrcode + QStringLiteral("?p=") + _qrcode;


         this->m_xmlw->writeStartElement("infNFeSupl");//abertura grupo infNFeSupl
         this->m_xmlw->writeTextElement("qrCode", _qrcode);
         this->m_xmlw->writeTextElement("urlChave", _urlchave);
         this->m_xmlw->writeEndElement();//fechamento grupo infNFeSupl
     }
}

void XmlWrite::gerarNFE()
{
    //cabeçalho
    //this->m_xmlw->setCodec(QTextCodec::codecForName("UTF-8"));
    this->m_xmlw->writeStartElement("NFe");
    this->m_xmlw->writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    get_infNFe();
    //só gera o nó infNFeSupl caso for nfce e normal, a offline só será gerada
    //após a assinatura da nfce
    if (this->m_infNFe->ide->get_mod() == ModeloDF::NFCe)
    {
        if (this->m_infNFe->ide->get_tpEmis() == TpEmis::Normal)
            get_infNFeSupl_ONLINE();
    }
    this->m_xmlw->writeEndElement(); //fechamento NFe
}

