#include "tevento.h"
#include <QRegularExpression>
#include <QSet>
#include <cmath>

namespace {
bool pedidoProrrogacao(TpEvento tipo)
{
    return tipo == TpEvento::PedidoProrrogacao1 || tipo == TpEvento::PedidoProrrogacao2;
}

bool cancelProrrogacao(TpEvento tipo)
{
    return tipo == TpEvento::CancelPedidoProrrogacao1 || tipo == TpEvento::CancelPedidoProrrogacao2;
}

bool fiscoProrrogacao(TpEvento tipo)
{
    return tipo == TpEvento::FiscoPedidoProrrogacao1 || tipo == TpEvento::FiscoPedidoProrrogacao2 ||
           tipo == TpEvento::FiscoCancelPedidoProrrogacao1 || tipo == TpEvento::FiscoCancelPedidoProrrogacao2;
}

// Destaca o detalhe com seu namespace, inclusive quando herdado do envelope.
QByteArray extrairDetalheEvento(const QString &xml)
{
    QXmlStreamReader reader(xml);
    QByteArray detalhe;
    QXmlStreamWriter writer(&detalhe);
    while (!reader.atEnd())
    {
        reader.readNext();
        if (!reader.isStartElement() || reader.name() != QStringLiteral("detEvento"))
            continue;
        writer.writeStartElement("detEvento");
        writer.writeDefaultNamespace(reader.namespaceUri().toString());
        writer.writeAttributes(reader.attributes());
        int nivel = 1;
        while (nivel > 0 && !reader.atEnd())
        {
            reader.readNext();
            if (reader.isStartElement()) ++nivel;
            if (reader.isEndElement()) --nivel;
            writer.writeCurrentToken(reader);
        }
        return reader.hasError() ? QByteArray() : detalhe;
    }
    return detalhe;
}
}

ItemRespPedido::ItemRespPedido() { clear(); }
void ItemRespPedido::clear()
{
    m_numItem = 0;
    m_statPedido = 0;
    m_justStatus = 0;
    m_justStaOutra.clear();
}
int ItemRespPedido::get_numItem() const { return m_numItem; }
void ItemRespPedido::set_numItem(const int &valor) { m_numItem = valor; }
int ItemRespPedido::get_statPedido() const { return m_statPedido; }
void ItemRespPedido::set_statPedido(const int &valor) { m_statPedido = valor; }
int ItemRespPedido::get_justStatus() const { return m_justStatus; }
void ItemRespPedido::set_justStatus(const int &valor) { m_justStatus = valor; }
QString ItemRespPedido::get_justStaOutra() const { return m_justStaOutra; }
void ItemRespPedido::set_justStaOutra(const QString &valor) { m_justStaOutra = valor; }

RespPedido::RespPedido(): itemPedido(new Container<ItemRespPedido>), m_statPrazo(-1) {}
void RespPedido::clear() { m_statPrazo = -1; itemPedido->clear(); }
int RespPedido::get_statPrazo() const { return m_statPrazo; }
void RespPedido::set_statPrazo(const int &valor) { m_statPrazo = valor; }

RespCancPedido::RespCancPedido() { clear(); }
void RespCancPedido::clear()
{
    m_statCancPedido = 0;
    m_justStatus = 0;
    m_justStaOutra.clear();
}
int RespCancPedido::get_statCancPedido() const { return m_statCancPedido; }
void RespCancPedido::set_statCancPedido(const int &valor) { m_statCancPedido = valor; }
int RespCancPedido::get_justStatus() const { return m_justStatus; }
void RespCancPedido::set_justStatus(const int &valor) { m_justStatus = valor; }
QString RespCancPedido::get_justStaOutra() const { return m_justStaOutra; }
void RespCancPedido::set_justStaOutra(const QString &valor) { m_justStaOutra = valor; }


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
    autXML(new AutXMLAtor()), detPag(new Container<DetPag>),
    respPedido(new RespPedido()), respCancPedido(new RespCancPedido()),
    m_cOrgaoAutor(0), m_tpAutor(TpAutor::None),
    m_tpNF(TpNF::None), m_vNF(0.0), m_vICMS(0.0), m_vST(0.0), m_tpAutorizacao(TpAutorizacao::None),
    m_nTentativa(0), m_tpMotivo(TpMotivo::None)
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
    this->detPag->clear();
    this->respPedido->clear();
    this->respCancPedido->clear();
    this->m_idPedidoCancelado.clear();
    this->m_idPedido.clear();
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
    //Insucesso na Entrega da NF-e
    CppUtil::clearDateTime(this->m_dhTentativaEntrega);
    this->m_nTentativa = 0;
    this->m_tpMotivo = TpMotivo::None;
    this->m_xJustMotivo.clear();
    this->m_hashTentativaEntrega.clear();
    CppUtil::clearDateTime(this->m_dhHashTentativaEntrega);

}

QString DetEvento::get_idPedidoCancelado() const { return m_idPedidoCancelado; }
void DetEvento::set_idPedidoCancelado(const QString &valor) { m_idPedidoCancelado = valor; }
QString DetEvento::get_idPedido() const { return m_idPedido; }
void DetEvento::set_idPedido(const QString &valor) { m_idPedido = valor; }

bool DetEvento::lerProrrogacao(QXmlStreamReader &xml)
{
    if (!xml.isStartElement()) return false;
    if (xml.name() == QStringLiteral("idPedidoCancelado"))
        set_idPedidoCancelado(xml.readElementText());
    else if (xml.name() == QStringLiteral("idPedido"))
        set_idPedido(xml.readElementText());
    else if (xml.name() == QStringLiteral("itemPedido"))
    {
        itemPedido->obj->set_numItem(xml.attributes().value("numItem").toInt());
        while (xml.readNextStartElement())
        {
            if (xml.name() == QStringLiteral("qtdeItem"))
                itemPedido->obj->set_qtdeItem(xml.readElementText().toDouble());
            else xml.skipCurrentElement();
        }
        itemPedido->add();
    }
    else if (xml.name() == QStringLiteral("respPedido"))
    {
        respPedido->clear();
        while (xml.readNextStartElement())
        {
            if (xml.name() == QStringLiteral("statPrazo"))
                respPedido->set_statPrazo(xml.readElementText().toInt());
            else if (xml.name() == QStringLiteral("itemPedido"))
            {
                auto *item = respPedido->itemPedido->obj;
                item->set_numItem(xml.attributes().value("numItem").toInt());
                while (xml.readNextStartElement())
                {
                    if (xml.name() == QStringLiteral("statPedido")) item->set_statPedido(xml.readElementText().toInt());
                    else if (xml.name() == QStringLiteral("justStatus")) item->set_justStatus(xml.readElementText().toInt());
                    else if (xml.name() == QStringLiteral("justStaOutra")) item->set_justStaOutra(xml.readElementText());
                    else xml.skipCurrentElement();
                }
                respPedido->itemPedido->add();
            }
            else xml.skipCurrentElement();
        }
    }
    else if (xml.name() == QStringLiteral("respCancPedido"))
    {
        respCancPedido->clear();
        while (xml.readNextStartElement())
        {
            if (xml.name() == QStringLiteral("statCancPedido")) respCancPedido->set_statCancPedido(xml.readElementText().toInt());
            else if (xml.name() == QStringLiteral("justStatus")) respCancPedido->set_justStatus(xml.readElementText().toInt());
            else if (xml.name() == QStringLiteral("justStaOutra")) respCancPedido->set_justStaOutra(xml.readElementText());
            else xml.skipCurrentElement();
        }
    }
    else return false;
    return true;
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

QDateTime DetEvento::get_dhEntrega() const
{
    return this->m_dhEntrega;
}

void DetEvento::set_dhEntrega(const QDateTime &dhEntrega)
{
    this->m_dhEntrega = dhEntrega;
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

QString DetEvento::get_nProtEvento() const
{
    return this->m_nProtEvento;
}

void DetEvento::set_nProtEvento(const QString &nProtEvento)
{
    this->m_nProtEvento = nProtEvento;
}

QDateTime DetEvento::get_dhTentativaEntrega() const
{
    return this->m_dhTentativaEntrega;
}

void DetEvento::set_dhTentativaEntrega(const QDateTime &dhTentativaEntrega)
{
    this->m_dhTentativaEntrega = dhTentativaEntrega;
}

int DetEvento::get_nTentativa() const
{
    return this->m_nTentativa;
}

void DetEvento::set_nTentativa(const int &nTentativa)
{
    this->m_nTentativa = nTentativa;
}

TpMotivo DetEvento::get_tpMotivo() const
{
    return this->m_tpMotivo;
}

void DetEvento::set_tpMotivo(const TpMotivo &tpMotivo)
{
    this->m_tpMotivo = tpMotivo;
}

QString DetEvento::get_xJustMotivo() const
{
    return this->m_xJustMotivo;
}

void DetEvento::set_xJustMotivo(const QString &xJustMotivo)
{
    this->m_xJustMotivo = xJustMotivo;
}

QString DetEvento::get_hashTentativaEntrega() const
{
    return this->m_hashTentativaEntrega;
}

void DetEvento::set_hashTentativaEntrega(const QString &hashTentativaEntrega)
{
    this->m_hashTentativaEntrega = hashTentativaEntrega;
}

QDateTime DetEvento::get_dhHashTentativaEntrega() const
{
    return this->m_dhHashTentativaEntrega;
}

void DetEvento::set_dhHashTentativaEntrega(const QDateTime &dhHashTentativaEntrega)
{
    this->m_dhHashTentativaEntrega = dhHashTentativaEntrega;
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
    this->m_CPF = CPF;
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

    std::unique_ptr<CppXML> _libxml(new CppLibXml2(config->certificado->get_cryptoLib(), _schema));

    const TpEvento tipo = infEvento->get_tpEvento();
    if (!ConvNF::descricaoManifestacao(tipo).isEmpty() &&
        !_libxml->parseXML(extrairDetalheEvento(m_xml_original),
                         ("e" + ConvNF::tpEventoToStr(tipo) + "_v1.00.xsd").toUtf8()))
    {
        m_error = _libxml->get_error();
        return;
    }

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
}

void TEvento::gerarXML()
{
    this->m_error.clear();
    this->m_xml_assinado.clear();
    this->m_xml_original.clear();
    const TpEvento tipo = infEvento->get_tpEvento();
    if (fiscoProrrogacao(tipo))
    {
        m_error = "Eventos 411500 a 411503 são exclusivos do Fisco; use lerXMLProrrogacao para importar a resposta.";
        return;
    }
    if ((pedidoProrrogacao(tipo) || cancelProrrogacao(tipo)) && !validarDadosProrrogacao())
        return;
    const QString descricaoManifestacao = ConvNF::descricaoManifestacao(tipo);
    const bool manifestacao = !descricaoManifestacao.isEmpty();
    if (manifestacao && !validarDadosManifestacao())
        return;
    if (infEvento->get_tpEvento() == TpEvento::AtorInteressadoNFeTransp)
    {
        const auto autor = infEvento->detEvento->get_tpAutor();
        const auto autorizacao = infEvento->detEvento->get_tpAutorizacao();
        if (infEvento->get_chNFe().mid(20, 2) != "55")
            m_error = "Ator interessado exige uma chave de NF-e modelo 55.";
        else if (autor != TpAutor::EmpresaEmitente && autor != TpAutor::EmpresaDestinataria &&
                 autor != TpAutor::EmpresaTerceiros)
            m_error = "Ator interessado exige tpAutor 1, 2 ou 3.";
        else if (autor == TpAutor::EmpresaTerceiros && autorizacao != TpAutorizacao::None)
            m_error = "Transportador contratado não pode informar tpAutorizacao.";
        else if (autor != TpAutor::EmpresaTerceiros &&
                 autorizacao != TpAutorizacao::NaoPermite && autorizacao != TpAutorizacao::PermiteTranspAutTransp)
            m_error = "Emitente e destinatário devem informar tpAutorizacao 0 ou 1.";
        else if (autorizacao != TpAutorizacao::PermiteTranspAutTransp &&
                 !infEvento->detEvento->get_xCondUso().isEmpty())
            m_error = "xCondUso só pode ser informado com tpAutorizacao 1.";
        if (!m_error.isEmpty())
            return;
    }
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
    _xmlw.writeAttribute("versao", this->infEvento->get_verEvento());

    _xmlw.writeTextElement("descEvento", manifestacao ? descricaoManifestacao :
                         this->infEvento->detEvento->get_descEvento());
    if (manifestacao)
    {
        if (tipo == TpEvento::OperacaoNaoRealizadaDest)
            _xmlw.writeTextElement("xJust", infEvento->detEvento->get_xJust());
    }
    else
    {
        if (this->infEvento->detEvento->get_cOrgaoAutor() > 0)
            _xmlw.writeTextElement("cOrgaoAutor", QString::number(this->infEvento->detEvento->get_cOrgaoAutor()));
        if (this->infEvento->detEvento->get_tpAutor() != TpAutor::None)
            _xmlw.writeTextElement("tpAutor", ConvNF::tpAutorToStr(this->infEvento->detEvento->get_tpAutor()));
        if (!this->infEvento->detEvento->get_verAplic().isEmpty())
            _xmlw.writeTextElement("verAplic", this->infEvento->detEvento->get_verAplic());
        if (cancelProrrogacao(tipo))
            _xmlw.writeTextElement("idPedidoCancelado", this->infEvento->detEvento->get_idPedidoCancelado());
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
    }

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
    if (pedidoProrrogacao(tipo))
    {
        for (int i = 0; i < this->infEvento->detEvento->itemPedido->count(); ++i) {
            _xmlw.writeStartElement("itemPedido"); //abertura do grupo itemPedido - agrupamento dos ítens
            _xmlw.writeAttribute("numItem", QString::number(this->infEvento->detEvento->itemPedido->value(i)->get_numItem()));
            _xmlw.writeTextElement("qtdeItem", CppUtility::doubleToStrDecimal(this->infEvento->detEvento->itemPedido->value(i)->get_qtdeItem(), 4));
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

        if (this->infEvento->detEvento->get_tpAutorizacao() != TpAutorizacao::None)
            _xmlw.writeTextElement("tpAutorizacao", ConvNF::tpAutorizacaoToStr(this->infEvento->detEvento->get_tpAutorizacao()));
        if (!this->infEvento->detEvento->get_xCondUso().isEmpty())
            _xmlw.writeTextElement("xCondUso", this->infEvento->detEvento->get_xCondUso());
    }
    //Comprovante de Entrega
    if (this->infEvento->get_tpEvento() == TpEvento::ComprovanteEntregaNFe ||
        this->infEvento->get_tpEvento() == TpEvento::CancelComprovanteEntregaNFe)
    {
        if (!this->infEvento->detEvento->get_dhEntrega().isNull())
            _xmlw.writeTextElement("dhEntrega", CppUtil::dateTimeToStr(this->infEvento->detEvento->get_dhEntrega(), DtH::DateTimeUTC));
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
        if (!this->infEvento->detEvento->get_nProtEvento().isEmpty())
            _xmlw.writeTextElement("nProtEvento", this->infEvento->detEvento->get_nProtEvento());

    }
    //Insucesso na Entrega da NF-e
    if (this->infEvento->get_tpEvento() == TpEvento::InsucessoEntregaNFe)
    {
        _xmlw.writeTextElement("dhTentativaEntrega", CppUtil::dateTimeToStr(this->infEvento->detEvento->get_dhTentativaEntrega(), DtH::DateTimeUTC));
        if (this->infEvento->detEvento->get_nTentativa() > 0)
            _xmlw.writeTextElement("nTentativa", QString::number(this->infEvento->detEvento->get_nTentativa()));
        _xmlw.writeTextElement("tpMotivo", ConvNF::tpMotivoToStr(this->infEvento->detEvento->get_tpMotivo()));
        //xJustMotivo - informar apenas para tpMotivo=4
        if (this->infEvento->detEvento->get_tpMotivo() == TpMotivo::Outros)
            _xmlw.writeTextElement("xJustMotivo", this->infEvento->detEvento->get_xJustMotivo());
        if (!this->infEvento->detEvento->get_latGPS().isEmpty())
            _xmlw.writeTextElement("latGPS", this->infEvento->detEvento->get_latGPS());
        if (!this->infEvento->detEvento->get_longGPS().isEmpty())
            _xmlw.writeTextElement("longGPS", this->infEvento->detEvento->get_longGPS());
        _xmlw.writeTextElement("hashTentativaEntrega", this->infEvento->detEvento->get_hashTentativaEntrega());
        if (!this->infEvento->detEvento->get_dhHashTentativaEntrega().isNull())
            _xmlw.writeTextElement("dhHashTentativaEntrega", CppUtil::dateTimeToStr(this->infEvento->detEvento->get_dhHashTentativaEntrega(), DtH::DateTimeUTC));
    }
    //Cancelamento Insucesso na Entrega da NF-e
    if (this->infEvento->get_tpEvento() == TpEvento::CancelInsucessoEntregaNFe)
    {
        _xmlw.writeTextElement("nProtEvento", this->infEvento->detEvento->get_nProtEvento());
    }

    //Conciliação Financeira (NT 2024.002)
    if (this->infEvento->get_tpEvento() == TpEvento::ECONF)
    {
        for (const DetPag *pag : *this->infEvento->detEvento->detPag)
        {
            _xmlw.writeStartElement("detPag");
            if (pag->get_indPag() != IndPag::None)
                _xmlw.writeTextElement("indPag", ConvNF::indPagToStr(pag->get_indPag()));
            _xmlw.writeTextElement("tPag", ConvNF::tPagToStr(pag->get_tPag()));
            if (!pag->get_xPag().isEmpty())
                _xmlw.writeTextElement("xPag", pag->get_xPag());
            _xmlw.writeTextElement("vPag", CppUtility::doubleToStrDecimal(pag->get_vPag(), 2));
            _xmlw.writeTextElement("dPag", pag->get_dPag().toString(Qt::ISODate));
            if (!pag->get_CNPJPag().isEmpty())
                _xmlw.writeTextElement("CNPJPag", pag->get_CNPJPag());
            if (pag->get_UFPag() != UF::None)
                _xmlw.writeTextElement("UFPag", ConvNF::ufToStr(pag->get_UFPag()));
            if (!pag->card->get_CNPJ().isEmpty())
                _xmlw.writeTextElement("CNPJIF", pag->card->get_CNPJ());
            if (pag->card->get_tBand() != TBand::None)
                _xmlw.writeTextElement("tBand", ConvNF::tBandToStr(pag->card->get_tBand()));
            if (!pag->card->get_cAut().isEmpty())
                _xmlw.writeTextElement("cAut", pag->card->get_cAut());
            if (!pag->get_CNPJReceb().isEmpty())
                _xmlw.writeTextElement("CNPJReceb", pag->get_CNPJReceb());
            if (pag->get_UFReceb() != UF::None)
                _xmlw.writeTextElement("UFReceb", ConvNF::ufToStr(pag->get_UFReceb()));
            _xmlw.writeEndElement();
        }
    }
    if (this->infEvento->get_tpEvento() == TpEvento::CancelECONF)
        _xmlw.writeTextElement("nProtEvento", this->infEvento->detEvento->get_nProtEvento());

    _xmlw.writeEndElement(); //fechamento detEvento
    _xmlw.writeEndElement(); //fechamento infEvento
    _xmlw.writeEndElement(); //fechamento evento

    this->m_xml_original.clear();
    this->m_xml_original.append(_strXML);
}

bool TEvento::validarDadosManifestacao()
{
    static const QRegularExpression formatoChave("^[0-9]{6}[0-9A-Z]{12}[0-9]{26}\\z");
    const QString chave = infEvento->get_chNFe();
    const TpEvento tipo = infEvento->get_tpEvento();
    const int sequencia = infEvento->get_nSeqEvento();
    const auto tamanhoJustificativa = infEvento->detEvento->get_xJust().size();
    if (!formatoChave.match(chave).hasMatch() || chave.mid(20, 2) != "55")
        m_error = "Informe uma chave de acesso de NF-e modelo 55 com 44 posições.";
    else if (chave.at(34) < QLatin1Char('1') || chave.at(34) > QLatin1Char('7'))
        m_error = "A manifestação exige tpEmis de 1 a 7 na chave da NF-e.";
    else if (infEvento->get_CNPJ().isEmpty() == infEvento->get_CPF().isEmpty())
        m_error = "Informe o CNPJ ou o CPF do destinatário da NF-e.";
    else if (!infEvento->get_CPF().isEmpty() && !CppUtility::cpfValido(infEvento->get_CPF()))
        m_error = "Informe um CPF válido para o destinatário da NF-e.";
    else if (sequencia != 1 && (sequencia != 2 || tipo == TpEvento::CienciaOperacaoDest))
        m_error = "Informe nSeqEvento 1 para Ciência da Operação ou 1 a 2 para manifestação conclusiva.";
    else if (tipo == TpEvento::OperacaoNaoRealizadaDest &&
             (tamanhoJustificativa < 15 || tamanhoJustificativa > 255))
        m_error = "Operação não Realizada exige uma justificativa de 15 a 255 caracteres.";

    if (m_error.isEmpty() &&
        CppUtility::dvModulo11(chave.left(43)) != chave.right(1).toInt())
        m_error = "Informe uma chave da NF-e com dígito verificador válido.";
    return m_error.isEmpty();
}

bool TEvento::validarDadosProrrogacao()
{
    const TpEvento tipo = infEvento->get_tpEvento();
    static const QRegularExpression chave("^[0-9]{6}[0-9A-Z]{12}[0-9]{26}\\z");
    auto *inf = infEvento.get();
    auto *det = inf->detEvento.get();
    if (!chave.match(inf->get_chNFe()).hasMatch() || inf->get_chNFe().mid(20, 2) != "55")
        m_error = "Prorrogação exige uma chave de acesso de NF-e modelo 55 com 44 posições.";
    else if (m_versao != "1.00" || inf->get_verEvento() != "1.00")
        m_error = "Prorrogação exige a versão 1.00 do evento e do detalhe.";
    else if (inf->get_cOrgao() != inf->get_chNFe().left(2).toInt())
        m_error = "O órgão do evento de prorrogação deve ser a UF da chave da NF-e.";
    else if (inf->get_nSeqEvento() < 1 || inf->get_nSeqEvento() > 99)
        m_error = "Prorrogação exige nSeqEvento de 1 a 99, controlado pela aplicação.";
    else if (cancelProrrogacao(tipo))
    {
        const QString referencia = det->get_idPedidoCancelado();
        const QString prefixo = tipo == TpEvento::CancelPedidoProrrogacao1 ? "ID111500" : "ID111501";
        static const QRegularExpression sequencia("^[0-9]{2}\\z");
        if (referencia.size() != 54 || !referencia.startsWith(prefixo) ||
            !sequencia.match(referencia.right(2)).hasMatch() ||
            referencia.mid(8, 44) != inf->get_chNFe() || referencia.right(2) == "00")
            m_error = "idPedidoCancelado deve identificar um pedido do mesmo prazo e da mesma NF-e.";
        else if (!det->itemPedido->isEmpty())
            m_error = "Cancelamento de pedido não permite itemPedido; informe idPedidoCancelado e o protocolo do pedido.";
    }
    else
    {
        if (!det->get_idPedidoCancelado().isEmpty())
            m_error = "idPedidoCancelado é exclusivo do cancelamento de pedido de prorrogação.";
        else if (det->itemPedido->count() < 1 || det->itemPedido->count() > 990)
            m_error = "Pedido de prorrogação exige de 1 a 990 itens.";
        QSet<int> numeros;
        for (const ItemPedProrrog *item : *det->itemPedido)
        {
            const int numero = item->get_numItem();
            if (numero < 1 || numero > 990 || numeros.contains(numero))
                m_error = "Número de item de prorrogação deve ser único e estar entre 1 e 990.";
            if (!std::isfinite(item->get_qtdeItem()) || item->get_qtdeItem() < 0 || item->get_qtdeItem() >= 100000000000.0)
                m_error = "Quantidade do item de prorrogação inválida.";
            numeros.insert(numero);
        }
    }
    return m_error.isEmpty();
}

bool TEvento::validarProrrogacao(const ConfigNFe *config)
{
    m_error.clear();
    const TpEvento tipo = infEvento->get_tpEvento();
    if (!config || (!pedidoProrrogacao(tipo) && !cancelProrrogacao(tipo) && !fiscoProrrogacao(tipo)))
    {
        m_error = "Informe a configuração de schemas e um evento da NT 2015.001.";
        return false;
    }
    if ((pedidoProrrogacao(tipo) || cancelProrrogacao(tipo)) && !validarDadosProrrogacao())
        return false;
    const QString xml = m_xml_assinado.isEmpty() ? m_xml_original : m_xml_assinado;
    QXmlStreamReader reader(xml);
    reader.readNextStartElement();
    const QString raiz = reader.name().toString();
    if (raiz != "evento" && raiz != "procEventoNFe")
    {
        m_error = "Informe um XML evento ou procEventoNFe.";
        return false;
    }
    CppLibXml2 validador(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    const QByteArray detalhe = extrairDetalheEvento(xml);
    QXmlStreamReader leitorDetalhe(detalhe);
    leitorDetalhe.readNextStartElement();
    if (leitorDetalhe.attributes().value("versao") != QStringLiteral("1.00"))
    {
        m_error = "O detalhe de prorrogação exige versao igual a 1.00.";
        return false;
    }
    QString envelope = xml;
    if (raiz == "evento")
    {
        envelope.remove(QRegularExpression("^\\s*<\\?xml[^?]*\\?>"));
        envelope = "<envEvento xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"1.00\"><idLote>0</idLote>" +
                   envelope + "</envEvento>";
    }
    if (!validador.parseXML(envelope.toUtf8(),
            raiz == "evento" ? QByteArray("envEvento_v1.00.xsd") : QByteArray("procEventoNFe_v1.00.xsd")) ||
        !validador.parseXML(detalhe,
            ("e" + ConvNF::tpEventoToStr(tipo) + "_v1.00.xsd").toUtf8()))
    {
        m_error = validador.get_error();
        return false;
    }
    const auto *inf = infEvento.get();
    const QString id = "ID" + ConvNF::tpEventoToStr(tipo) + inf->get_chNFe() +
                       QString::number(inf->get_nSeqEvento()).rightJustified(2, '0');
    if (m_versao != "1.00" || inf->get_verEvento() != "1.00" ||
        inf->get_chNFe().mid(20, 2) != "55" || inf->get_nSeqEvento() < 1 ||
        inf->get_nSeqEvento() > 99 || inf->get_Id() != id)
    {
        m_error = "Evento de prorrogação exige modelo 55, sequência de 1 a 99 e Id correspondente ao tipo, chave e sequência.";
        return false;
    }
    if (fiscoProrrogacao(tipo))
    {
        const QString pedido = inf->detEvento->get_idPedido();
        const QString prefixo = "ID" + QString::number(static_cast<int>(tipo) - 300000);
        if (pedido.left(8) != prefixo || pedido.mid(8, 44) != inf->get_chNFe() || pedido.right(2) == "00")
        {
            m_error = "idPedido da resposta do Fisco deve corresponder ao tipo e à NF-e do pedido original.";
            return false;
        }
        QSet<int> numeros;
        for (const ItemRespPedido *item : *inf->detEvento->respPedido->itemPedido)
        {
            if (item->get_numItem() < 1 || item->get_numItem() > 990 || numeros.contains(item->get_numItem()))
            {
                m_error = "Resposta do Fisco exige número de item único de 1 a 990.";
                return false;
            }
            numeros.insert(item->get_numItem());
        }
    }
    return true;
}

bool TEvento::lerXMLProrrogacao(const QString &xml, const ConfigNFe *config)
{
    clear();
    QXmlStreamReader reader(xml);
    bool encontrou = false;
    while (!reader.atEnd())
    {
        reader.readNext();
        if (!reader.isStartElement() || reader.name() != QStringLiteral("evento")) continue;
        if (encontrou)
        {
            reader.raiseError("Informe somente um evento ou procEventoNFe por importação.");
            break;
        }
        encontrou = true;
        set_versao(reader.attributes().value("versao").toString());
        while (reader.readNextStartElement())
        {
            if (reader.name() != QStringLiteral("infEvento")) { reader.skipCurrentElement(); continue; }
            infEvento->set_Id(reader.attributes().value("Id").toString());
            while (reader.readNextStartElement())
            {
                if (reader.name() == QStringLiteral("detEvento"))
                {
                    while (reader.readNextStartElement())
                    {
                        if (infEvento->detEvento->lerProrrogacao(reader)) continue;
                        if (reader.name() == QStringLiteral("descEvento")) infEvento->detEvento->set_descEvento(reader.readElementText());
                        else if (reader.name() == QStringLiteral("nProt")) infEvento->detEvento->set_nProt(reader.readElementText());
                        else reader.skipCurrentElement();
                    }
                }
                else
                {
                    const QString nome = reader.name().toString();
                    const QString valor = reader.readElementText();
                    if (nome == "cOrgao") infEvento->set_cOrgao(valor.toInt());
                    else if (nome == "tpAmb") infEvento->set_tpAmb(ConvNF::strToTpAmb(valor));
                    else if (nome == "CNPJ") infEvento->set_CNPJ(valor);
                    else if (nome == "CPF") infEvento->set_CPF(valor);
                    else if (nome == "chNFe") infEvento->set_chNFe(valor);
                    else if (nome == "dhEvento") infEvento->set_dhEvento(QDateTime::fromString(valor, Qt::ISODate));
                    else if (nome == "tpEvento") infEvento->set_tpEvento(ConvNF::strToTpEvento(valor));
                    else if (nome == "nSeqEvento") infEvento->set_nSeqEvento(valor.toInt());
                    else if (nome == "verEvento") infEvento->set_verEvento(valor);
                }
            }
        }
    }
    m_xml_original = xml; // Preserva integralmente o documento recebido, inclusive a assinatura.
    if (!encontrou || reader.hasError() || !validarProrrogacao(config))
    {
        const QString erro = reader.hasError() ? reader.errorString() :
            (m_error.isEmpty() ? "XML sem evento de prorrogação." : m_error);
        clear();
        m_error = erro;
        return false;
    }
    return true;
}

QString TEvento::get_error() const
{
    return this->m_error;
}
