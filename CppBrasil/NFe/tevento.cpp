#include "tevento.h"


//------------------------------------------------------------------------
// ItemPedProrrog class
//------------------------------------------------------------------------

AutXMLAtor::AutXMLAtor()
{

}

AutXMLAtor::~AutXMLAtor()
{

}

void AutXMLAtor::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
}

QString AutXMLAtor::get_CNPJ() const
{
    return this->m_CNPJ;
}

void AutXMLAtor::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString AutXMLAtor::get_CPF() const
{
    return this->m_CPF;
}

void AutXMLAtor::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

//------------------------------------------------------------------------
// ItemPedProrrog class
//------------------------------------------------------------------------

ItemPedProrrog::ItemPedProrrog(): m_numItem(0), m_qtdeItem(0.0)
{

}

ItemPedProrrog::~ItemPedProrrog()
{

}

void ItemPedProrrog::clear()
{
    this->m_numItem = 0;
    this->m_qtdeItem = 0.0;
}

int ItemPedProrrog::get_numItem() const
{
    return this->m_numItem;
}

void ItemPedProrrog::set_numItem(const int &numItem)
{
    this->m_numItem = numItem;
}

double ItemPedProrrog::get_qtdeItem() const
{
    return this->m_qtdeItem;
}

void ItemPedProrrog::set_qtdeItem(const double &qtdeItem)
{
    this->m_qtdeItem = qtdeItem;
}


//------------------------------------------------------------------------
// DestEPEC class
//------------------------------------------------------------------------

DestEPEC::DestEPEC(): m_UF(0)
{

}

DestEPEC::~DestEPEC()
{

}

void DestEPEC::clear()
{
    this->m_UF = 0;
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_idEstrangeiro.clear();
    this->m_IE.clear();
}

int DestEPEC::get_UF() const
{
    return this->m_UF;
}

void DestEPEC::set_UF(const int &uf)
{
    this->m_UF = uf;
}

QString DestEPEC::get_CNPJ() const
{
    return this->m_CNPJ;
}

void DestEPEC::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString DestEPEC::get_CPF() const
{
    return this->m_CPF;
}

void DestEPEC::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString DestEPEC::get_idEstrangeiro() const
{
    return this->m_idEstrangeiro;
}

void DestEPEC::set_idEstrangeiro(const QString &idEstrangeiro)
{
    this->m_idEstrangeiro = idEstrangeiro;
}

QString DestEPEC::get_IE() const
{
    return this->m_IE;
}

void DestEPEC::set_IE(const QString &IE)
{
    this->m_IE = IE;
}

//------------------------------------------------------------------------
// DetEvento class
//------------------------------------------------------------------------

DetEvento::DetEvento(): dest(new DestEPEC()), itemPedido(new Container<ItemPedProrrog>),
    autXML(new AutXMLAtor()), m_cOrgaoAutor(0), m_tpAutor(TpAutor::None),
    m_tpNF(TpNF::None), m_vNF(0.0), m_vICMS(0.0), m_vST(0.0), m_tpAutorizacao(TpAutorizacao::None)
{

}

DetEvento::~DetEvento()
{

}

void DetEvento::clear()
{
    this->dest->clear();
    this->itemPedido->clear();
    this->autXML->clear();
    this->m_versao.clear();
    this->m_descEvento.clear();
    this->m_cOrgaoAutor = 0;
    this->m_tpAutor = TpAutor::None;
    this->m_verAplic.clear();
    this->m_nProt.clear();
    this->m_xJust.clear();
    this->m_chNFeRef.clear();
    this->m_xCorrecao.clear();
    this->m_xCondUso.clear();
    //EPEC
    CppUtil::clearDateTime(this->m_dhEmi);
    this->m_tpNF = TpNF::None;
    this->m_IE.clear();
    this->m_vNF = 0.0;
    this->m_vICMS = 0.0;
    this->m_vST = 0.0;
    //Ator Interessado na NF-e
    this->m_tpAutorizacao = TpAutorizacao::None;
    //EntregaNFe
    this->m_nDoc.clear();
    this->m_xNome.clear();
    this->m_latGPS.clear();
    this->m_longGPS.clear();
    this->m_hashComprovante.clear();
    CppUtil::clearDateTime(this->m_dhHashComprovante);

}

QString DetEvento::get_descEvento() const
{
    return this->m_descEvento;
}

void DetEvento::set_descEvento(const QString &descEvento)
{
    this->m_descEvento = descEvento;
}

int DetEvento::get_cOrgaoAutor() const
{
    return this->m_cOrgaoAutor;
}

void DetEvento::set_cOrgaoAutor(const int &cOrgaoAutor)
{
    this->m_cOrgaoAutor = cOrgaoAutor;
}

TpAutor DetEvento::get_tpAutor() const
{
    return this->m_tpAutor;
}

void DetEvento::set_tpAutor(const TpAutor &tpAutor)
{
    this->m_tpAutor = tpAutor;
}

QString DetEvento::get_verAplic() const
{
    return this->m_verAplic;
}

void DetEvento::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

QString DetEvento::get_nProt() const
{
    return this->m_nProt;
}

void DetEvento::set_nProt(const QString &nProt)
{
    this->m_nProt = nProt;
}

QString DetEvento::get_xJust() const
{
    return this->m_xJust;
}

void DetEvento::set_xJust(const QString &xJust)
{
    this->m_xJust = xJust;
}

QString DetEvento::get_chNFeRef() const
{
    return this->m_chNFeRef;
}

void DetEvento::set_chNFeRef(const QString &chNFeRef)
{
    this->m_chNFeRef = chNFeRef;
}

QString DetEvento::get_xCorrecao() const
{
    return this->m_xCorrecao;
}

void DetEvento::set_xCorrecao(const QString &xCorrecao)
{
    this->m_xCorrecao = xCorrecao;
}

QString DetEvento::get_xCondUso() const
{
    return this->m_xCondUso;
}

void DetEvento::set_xCondUso(const QString &xCondUso)
{
    this->m_xCondUso = xCondUso;
}

QDateTime DetEvento::get_dhEmi() const
{
    return this->m_dhEmi;
}

void DetEvento::set_dhEmi(const QDateTime &dhEmi)
{
    this->m_dhEmi = dhEmi;
}

TpNF DetEvento::get_tpNF() const
{
    return this->m_tpNF;
}

void DetEvento::set_tpNF(const TpNF &tpNF)
{
    this->m_tpNF = tpNF;
}

QString DetEvento::get_IE() const
{
    return this->m_IE;
}

void DetEvento::set_IE(const QString &IE)
{
    this->m_IE = IE;
}

double DetEvento::get_vNF() const
{
    return this->m_vNF;
}

void DetEvento::set_vNF(const double &vNF)
{
    this->m_vNF = vNF;
}

double DetEvento::get_vICMS() const
{
    return this->m_vICMS;
}

void DetEvento::set_vICMS(const double &vICMS)
{
    this->m_vICMS = vICMS;
}

double DetEvento::get_vST() const
{
    return this->m_vST;
}

void DetEvento::set_vST(const double &vST)
{
    this->m_vST = vST;
}

TpAutorizacao DetEvento::get_tpAutorizacao() const
{
    return this->m_tpAutorizacao;
}

void DetEvento::set_tpAutorizacao(const TpAutorizacao &tpAutorizacao)
{
    this->m_tpAutorizacao = tpAutorizacao;
}

QString DetEvento::get_nDoc() const
{
    return this->m_nDoc;
}

void DetEvento::set_nDoc(const QString &nDoc)
{
    this->m_nDoc = nDoc;
}

QString DetEvento::get_xNome() const
{
    return this->m_xNome;
}

void DetEvento::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

QString DetEvento::get_latGPS() const
{
    return this->m_latGPS;
}

void DetEvento::set_latGPS(const QString &latGPS)
{
    this->m_latGPS = latGPS;
}

QString DetEvento::get_longGPS() const
{
    return this->m_longGPS;
}

void DetEvento::set_longGPS(const QString &longGPS)
{
    this->m_longGPS = longGPS;
}

QString DetEvento::get_hashComprovante() const
{
    return this->m_hashComprovante;
}

void DetEvento::set_hashComprovante(const QString &hashComprovante)
{
    this->m_hashComprovante = hashComprovante;
}

QDateTime DetEvento::get_dhHashComprovante() const
{
    return this->m_dhHashComprovante;
}

void DetEvento::set_dhHashComprovante(const QDateTime &dhHashComprovante)
{
    this->m_dhHashComprovante = dhHashComprovante;
}

//------------------------------------------------------------------------
// InfEvento class
//------------------------------------------------------------------------

InfEvento::InfEvento(): detEvento(new DetEvento()), m_cOrgao(0), m_tpAmb(TpAmb::None),
    m_tpEvento(TpEvento::None), m_nSeqEvento(0)
{

}

InfEvento::~InfEvento()
{
}

void InfEvento::clear()
{
    this->detEvento->clear();
    this->m_Id.clear();
    this->m_cOrgao = 0;
    this->m_tpAmb = TpAmb::None;
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_chNFe.clear();
    CppUtil::clearDateTime(this->m_dhEvento);
    this->m_tpEvento = TpEvento::None;
    this->m_nSeqEvento = 0;
    this->m_verEvento.clear();
}

QString InfEvento::get_Id() const
{
    return this->m_Id;
}

void InfEvento::set_Id(const QString &Id)
{
    this->m_Id = Id;
}

int InfEvento::get_cOrgao() const
{
    return this->m_cOrgao;
}

void InfEvento::set_cOrgao(const int &cOrgao)
{
    this->m_cOrgao = cOrgao;
}

TpAmb InfEvento::get_tpAmb() const
{
    return this->m_tpAmb;
}

void InfEvento::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

QString InfEvento::get_CNPJ() const
{
    return this->m_CNPJ;
}

void InfEvento::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString InfEvento::get_CPF() const
{
    return this->m_CPF;
}

void InfEvento::set_CPF(const QString &CPF)
{
    this->m_CNPJ = CPF;
}

QString InfEvento::get_chNFe() const
{
    return this->m_chNFe;
}

void InfEvento::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

QDateTime InfEvento::get_dhEvento() const
{
    return this->m_dhEvento;
}

void InfEvento::set_dhEvento(const QDateTime &dhEvento)
{
    this->m_dhEvento = dhEvento;
}

TpEvento InfEvento::get_tpEvento() const
{
    return this->m_tpEvento;
}

void InfEvento::set_tpEvento(const TpEvento &tpEvento)
{
    this->m_tpEvento = tpEvento;
}

int InfEvento::get_nSeqEvento() const
{
    return this->m_nSeqEvento;
}

void InfEvento::set_nSeqEvento(const int &nSeqEvento)
{
    this->m_nSeqEvento = nSeqEvento;
}

QString InfEvento::get_verEvento() const
{
    return this->m_verEvento;
}

void InfEvento::set_verEvento(const QString &verEvento)
{
    this->m_verEvento = verEvento;
}

//------------------------------------------------------------------------
// TEvento class
//------------------------------------------------------------------------

TEvento::TEvento(): infEvento(new InfEvento())
{

}

TEvento::~TEvento()
{

}

void TEvento::clear()
{
    this->infEvento->clear();
    this->m_versao.clear();
    this->m_xml_original.clear();
    this->m_xml_assinado.clear();
    this->m_error.clear();
}

QString TEvento::get_versao() const
{
    return this->m_versao;
}

void TEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

QString TEvento::get_XMLOriginal() const
{
    return this->m_xml_original;
}

void TEvento::set_XMLOriginal(const QString &xmlOriginal)
{
    this->m_xml_original = xmlOriginal;
}

QString TEvento::get_XMLAssinado() const
{
    return this->m_xml_assinado;
}

void TEvento::set_XMLAssinado(const QString &xmlAssinado)
{
    this->m_xml_assinado = xmlAssinado;
}

void TEvento::assinarXML(const ConfigNFe *config)
{
    HashType _sha1 = HashType::sha1;
    QByteArray _xmldoc, _schema, _nodedoc, _nodesign, _uri, _xml_assinado;
    XMLReturnType _returntype = XMLReturnType::XMLSign; //retornar xml assinado

    _xmldoc.append(this->m_xml_original.toLocal8Bit());
    _schema.append(config->arquivos->get_caminhoSchema().toLocal8Bit());
    _nodedoc.append("infEvento");
    _nodesign.append("SignedInfo");
    _uri.append(infEvento->get_Id().toLocal8Bit());

    CppXML * _libxml = new CppLibXml2(config->certificado->get_cryptoLib(), _schema);

    //assina o xml
    if (_libxml->signXML(_xmldoc, _sha1, _nodedoc, _nodesign, _uri,
                         _returntype, _xml_assinado))
    {
        this->m_xml_assinado.clear();
        this->m_xml_assinado.append(_xml_assinado);
    }
    else
    {
        this->m_error.append(_libxml->get_error());
    }

    delete _libxml;
}

void TEvento::gerarXML()
{
    QByteArray _strXML;
    QXmlStreamWriter _xmlw(&_strXML);

    _xmlw.writeStartElement("evento"); //abertura do grupo evento
    _xmlw.writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    _xmlw.writeAttribute("versao", this->m_versao);
    _xmlw.writeStartElement("infEvento"); //abertura do grupo infEvento
    _xmlw.writeAttribute("Id", this->infEvento->get_Id());
    _xmlw.writeTextElement("cOrgao", QString::number(this->infEvento->get_cOrgao()));

    //caso não esteja preenchido, pega de configurações
    _xmlw.writeTextElement("tpAmb", ConvNF::tpAmbToStr(this->infEvento->get_tpAmb()));

    if (!this->infEvento->get_CNPJ().isEmpty())
        _xmlw.writeTextElement("CNPJ", this->infEvento->get_CNPJ());
    if (!this->infEvento->get_CPF().isEmpty())
        _xmlw.writeTextElement("CPF", this->infEvento->get_CPF());
    _xmlw.writeTextElement("chNFe", this->infEvento->get_chNFe());
    _xmlw.writeTextElement("dhEvento", CppUtil::dateTimeToStr(this->infEvento->get_dhEvento(), DtH::DateTimeUTC));
    _xmlw.writeTextElement("tpEvento", ConvNF::tpEventoToStr(this->infEvento->get_tpEvento()));
    _xmlw.writeTextElement("nSeqEvento", QString::number(this->infEvento->get_nSeqEvento()));

    if (this->infEvento->get_verEvento().isEmpty())
        _xmlw.writeTextElement("verEvento", this->m_versao);
    else
        _xmlw.writeTextElement("verEvento", this->infEvento->get_verEvento());

    //grupo detalhe do evento
    _xmlw.writeStartElement("detEvento"); //abertura do grupo detEvento
    _xmlw.writeAttribute("versao", this->m_versao);

    //descrição obrigatório para todos
    _xmlw.writeTextElement("descEvento", this->infEvento->detEvento->get_descEvento());
    if (this->infEvento->detEvento->get_cOrgaoAutor() > 0)
        _xmlw.writeTextElement("cOrgaoAutor", QString::number(this->infEvento->detEvento->get_cOrgaoAutor()));
    if (this->infEvento->detEvento->get_tpAutor() != TpAutor::None)
        _xmlw.writeTextElement("tpAutor", ConvNF::tpAutorToStr(this->infEvento->detEvento->get_tpAutor()));
    if (!this->infEvento->detEvento->get_verAplic().isEmpty())
        _xmlw.writeTextElement("verAplic", this->infEvento->detEvento->get_verAplic());
    if (!this->infEvento->detEvento->get_nProt().isEmpty())
        _xmlw.writeTextElement("nProt", this->infEvento->detEvento->get_nProt());
    if (!this->infEvento->detEvento->get_xJust().isEmpty())
        _xmlw.writeTextElement("xJust", this->infEvento->detEvento->get_xJust());
    if (!this->infEvento->detEvento->get_chNFeRef().isEmpty())
        _xmlw.writeTextElement("chNFeRef", this->infEvento->detEvento->get_chNFeRef());

    //carta de coreção
    if (this->infEvento->get_tpEvento() == TpEvento::CartaCorrecao)
    {
        _xmlw.writeTextElement("xCorrecao", this->infEvento->detEvento->get_xCorrecao());
        _xmlw.writeTextElement("xCondUso", this->infEvento->detEvento->get_xCondUso());
    }
    //geral
    if (!this->infEvento->detEvento->get_dhEmi().isNull())
        _xmlw.writeTextElement("dhEmi", CppUtil::dateTimeToStr(this->infEvento->detEvento->get_dhEmi(), DtH::DateTimeUTC));
    if (this->infEvento->detEvento->get_tpNF() != TpNF::None)
        _xmlw.writeTextElement("tpNF", ConvNF::tpNFToStr(this->infEvento->detEvento->get_tpNF()));
    if (!this->infEvento->detEvento->get_IE().isEmpty())
        _xmlw.writeTextElement("IE", this->infEvento->detEvento->get_IE());

    //EPEC
    if (this->infEvento->get_tpEvento() == TpEvento::EPEC)
    {
        _xmlw.writeStartElement("dest"); //abertura do grupo dest
        if (this->infEvento->detEvento->dest->get_UF() > 0)
            _xmlw.writeTextElement("UF", QString::number(this->infEvento->detEvento->dest->get_UF()));
        if (!this->infEvento->detEvento->dest->get_CNPJ().isEmpty())
            _xmlw.writeTextElement("CNPJ", this->infEvento->detEvento->dest->get_CNPJ());
        if (!this->infEvento->detEvento->dest->get_CPF().isEmpty())
            _xmlw.writeTextElement("CPF", this->infEvento->detEvento->dest->get_CPF());
        if (!this->infEvento->detEvento->dest->get_idEstrangeiro().isEmpty())
            _xmlw.writeTextElement("idEstrangeiro", this->infEvento->detEvento->dest->get_idEstrangeiro());
        if (!this->infEvento->detEvento->dest->get_IE().isEmpty())
            _xmlw.writeTextElement("IE", this->infEvento->detEvento->dest->get_IE());
        _xmlw.writeEndElement(); //fechamento dest

        _xmlw.writeTextElement("vNF", CppUtility::doubleToStrDecimal(this->infEvento->detEvento->get_vNF(), 2));
        _xmlw.writeTextElement("vICMS", CppUtility::doubleToStrDecimal(this->infEvento->detEvento->get_vICMS(), 2));
        _xmlw.writeTextElement("vST", CppUtility::doubleToStrDecimal(this->infEvento->detEvento->get_vST(), 2));
    }
    //Pedido de Prorrogação
    if (this->infEvento->get_tpEvento() == TpEvento::CancelPedidoProrrogacao1 ||
        this->infEvento->get_tpEvento() == TpEvento::CancelPedidoProrrogacao2)
    {
        for (int i = 0; i < this->infEvento->detEvento->itemPedido->items->count(); ++i) {
            _xmlw.writeStartElement("itemPedido"); //abertura do grupo itemPedido - agrupamento dos ítens
            _xmlw.writeAttribute("numItem", QString::number(this->infEvento->detEvento->itemPedido->items->value(i)->get_numItem()));
            _xmlw.writeTextElement("qtdeItem", CppUtility::doubleToStrDecimal(this->infEvento->detEvento->itemPedido->items->value(i)->get_qtdeItem(), 4));
            _xmlw.writeEndElement(); //fechamento itemPedido - agrupamento dos ítens
        }
    }

    //Ator Interessado na NF-e - Transportador
    if (this->infEvento->get_tpEvento() == TpEvento::AtorInteressadoNFeTransp)
    {
        _xmlw.writeStartElement("autXML"); //abertura do grupo autXML
        if (!this->infEvento->detEvento->autXML->get_CNPJ().isEmpty())
            _xmlw.writeTextElement("CNPJ", this->infEvento->detEvento->autXML->get_CNPJ());
        if (!this->infEvento->detEvento->autXML->get_CPF().isEmpty())
            _xmlw.writeTextElement("CPF", this->infEvento->detEvento->autXML->get_CPF());
        _xmlw.writeEndElement(); //fechamento autXML

        _xmlw.writeTextElement("TpAutorizacao", ConvNF::tpAutorizacaoToStr(this->infEvento->detEvento->get_tpAutorizacao()));
        _xmlw.writeTextElement("xCondUso", this->infEvento->detEvento->get_xCondUso());
    }
    //Comprovante de Entrega
    if (this->infEvento->get_tpEvento() == TpEvento::ComprovanteEntregaNFe ||
        this->infEvento->get_tpEvento() == TpEvento::CancelComprovanteEntregaNFe)
    {
        if (!this->infEvento->detEvento->get_nDoc().isEmpty())
            _xmlw.writeTextElement("nDoc", this->infEvento->detEvento->get_nDoc());
        if (!this->infEvento->detEvento->get_xNome().isEmpty())
            _xmlw.writeTextElement("xNome", this->infEvento->detEvento->get_xNome());
        if (!this->infEvento->detEvento->get_latGPS().isEmpty())
            _xmlw.writeTextElement("latGPS", this->infEvento->detEvento->get_latGPS());
        if (!this->infEvento->detEvento->get_longGPS().isEmpty())
            _xmlw.writeTextElement("longGPS", this->infEvento->detEvento->get_longGPS());
        if (!this->infEvento->detEvento->get_hashComprovante().isEmpty())
            _xmlw.writeTextElement("hashComprovante", this->infEvento->detEvento->get_hashComprovante());
        if (!this->infEvento->detEvento->get_dhHashComprovante().isNull())
            _xmlw.writeTextElement("dhHashComprovante", CppUtil::dateTimeToStr(this->infEvento->detEvento->get_dhHashComprovante(), DtH::DateTimeUTC));
    }

    _xmlw.writeEndElement(); //fechamento detEvento
    _xmlw.writeEndElement(); //fechamento infEvento
    _xmlw.writeEndElement(); //fechamento evento

    this->m_xml_original.clear();
    this->m_xml_original.append(_strXML);
}

QString TEvento::get_error() const
{
    return this->m_error;
}
