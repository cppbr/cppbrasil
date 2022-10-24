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


#include "envevento.h"

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
    CppUtil::clearDateTime(m_dhEmi);
    this->m_tpNF = TpNF::None;
    this->m_IE.clear();
    this->m_vNF = 0.0;
    this->m_vICMS = 0.0;
    this->m_vST = 0.0;
    //Ator Interessado na NF-e
    this->m_tpAutorizacao = TpAutorizacao::None;

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
    this->m_Id.clear();
    this->m_cOrgao = 0;
    this->m_tpAmb = TpAmb::None;
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_chNFe.clear();
    CppUtil::clearDateTime(this->m_dhEvento);
    this->m_tpEvento = TpEvento::None;
    this->m_nSeqEvento = 0;
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

//------------------------------------------------------------------------
// EnvEventoEvento class
//------------------------------------------------------------------------

EnvEventoEvento::EnvEventoEvento(): infEvento(new InfEvento())
{

}

EnvEventoEvento::~EnvEventoEvento()
{

}

void EnvEventoEvento::clear()
{
    this->infEvento->clear();
    this->m_versao.clear();
    this->m_xml_original.clear();
    this->m_xml_assinado.clear();
    this->m_error.clear();
}

QString EnvEventoEvento::get_versao() const
{
    return this->m_versao;
}

void EnvEventoEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

QString EnvEventoEvento::get_XMLOriginal() const
{
    return this->m_xml_original;
}

QString EnvEventoEvento::get_XMLAssinado() const
{
    return this->m_xml_assinado;
}

void EnvEventoEvento::assinarXML(const ConfigNFe *config)
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
    } else
    {
        this->m_error.append(_libxml->get_error());
    }

    delete _libxml;
}

void EnvEventoEvento::gerarXML()
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

    _xmlw.writeTextElement("verEvento", this->m_versao);

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

    if (this->infEvento->get_tpEvento() == TpEvento::CartaCorrecao)
    {
        _xmlw.writeTextElement("xCorrecao", this->infEvento->detEvento->get_xCorrecao());
        _xmlw.writeTextElement("xCondUso", this->infEvento->detEvento->get_xCondUso());
    }
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
    if (this->infEvento->get_tpEvento() == TpEvento::CancelaPedidoProrrogacao1 ||
        this->infEvento->get_tpEvento() == TpEvento::CancelaPedidoProrrogacao2)
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

    _xmlw.writeEndElement(); //fechamento detEvento
    _xmlw.writeEndElement(); //fechamento infEvento
    _xmlw.writeEndElement(); //fechamento evento

    this->m_xml_original.clear();
    this->m_xml_original.append(_strXML);
}

void EnvEventoEvento::set_XMLOriginal(const QString &xmlOriginal)
{
    this->m_xml_original = xmlOriginal;
}

void EnvEventoEvento::set_XMLAssinado(const QString &xmlAssinado)
{
    this->m_xml_assinado = xmlAssinado;
}

QString EnvEventoEvento::get_error() const
{
    return this->m_error;
}

//------------------------------------------------------------------------
// EnvEvento class
//------------------------------------------------------------------------

EnvEvento::EnvEvento(ConfigNFe *confgNFe, NotaFiscal *notafiscal): evento(new Container<EnvEventoEvento>),
   retorno(new RetEnvEvento()), config(confgNFe), notafiscal(notafiscal)
{
}

EnvEvento::~EnvEvento()
{
}

void EnvEvento::clear()
{
    this->evento->clear();
    this->retorno->clear();
    this->m_error.clear();
    this->m_xml.clear();
    this->m_versao.clear();
}

bool EnvEvento::enviarEvento(const int &idLote, const bool &vincularNota, const bool &salvarNotaVinculada)
{
    bool _ret = true;
    if (gerarXML(idLote))
    {
        //validar xml
        if (validarEvento())
        {
            //envia
            WSEvento* _ws = new WSEvento(this->config,
                                      this->config->certificado->get_cryptoLib(),
                                      this->retorno.get());

            //connect(_ws, SIGNAL(wsChange(WebServicesNF)), this, SIGNAL(wsChange(WebServicesNF)));
            connect(_ws, &WSEvento::wsChange, this, &EnvEvento::wsChange);
            connect(_ws, &WSEvento::errorOccurred, [=]( const QString &error ) {
                this->m_error = error;
            } );

            bool _ret = _ws->send(this->m_xml.toLocal8Bit(),
                                  this->evento->items->count(),
                                  ConvNF::versaoLayout(WebServicesNF::NFeRecepcaoEvento));

            if (_ret)
              tratarRetorno(vincularNota, salvarNotaVinculada);

            delete _ws;

        } else {
            emit errorOccurred(this->m_error);
            _ret = false;
        }

    } else {
        emit errorOccurred(this->m_error);
        _ret = false;
    }

    return _ret;

}

QString EnvEvento::get_versao() const
{
    return this->m_versao;
}

void EnvEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

bool EnvEvento::assinarEvento(const QString &evento, const QString &uri, QString &output)
{
    HashType _sha1 = HashType::sha1;
    QByteArray _xmldoc, _schema, _nodedoc, _nodesign, _uri, _xml_assinado;
    XMLReturnType _returntype = XMLReturnType::XMLSign; //retornar xml assinado
    bool _ret = true;

    _xmldoc.append(evento.toLocal8Bit());
    _schema.append(config->arquivos->get_caminhoSchema().toLocal8Bit());
    _nodedoc.append("infEvento");
    _nodesign.append("SignedInfo");
    _uri.append(uri.toLocal8Bit());

    CppXML * _libxml = new CppLibXml2(config->certificado->get_cryptoLib(), _schema);

    //assina o xml
    if (_libxml->signXML(_xmldoc, _sha1, _nodedoc, _nodesign, _uri,
                         _returntype, _xml_assinado))
    {
        output.append(_xml_assinado);
    } else
    {
        _ret = false;
        //retorna o erro ao invés do xml assinado
        output = _libxml->get_error();
    }

    delete _libxml;
    return _ret;
}

bool EnvEvento::gerarXML(const int &idLote)
{
    bool _ret = true;
    QString _evento;
    this->m_error.clear();

    //versão - Caso não seja informado, será 1.00 que é a versão do  momento.
    if (this->m_versao.isEmpty())
      this->m_versao = "1.00";

    for (int i = 0; i < evento->items->count(); ++i)
    {
        //Só pode gerar o xml depois de fazer a atualização dos eventos, devido a alguns
        //campos que necessitam de ajustes.
        updateEvento(i);
        evento->items->value(i)->gerarXML();
        evento->items->value(i)->assinarXML(this->config);
        //assina xml
        if (evento->items->value(i)->get_error().isEmpty())
        {
            _evento.append(CppUtility::extractStr(evento->items->value(i)->get_XMLAssinado(), "<evento", "/evento>"));

        } else  {
            //o erro será retornado em output
            this->m_error += "Evento: " + QString::number(i) + " - " +
                             evento->items->value(i)->get_error();
            _ret = false;
        }
    }
    //só completa o xml caso não tenha ocorrido erro.
    if (_ret)
    {
        this->m_xml += "<envEvento xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\""+ this->m_versao +"\">";
        this->m_xml += "<idLote>" + CppUtil::insZeroLeft(QString::number(idLote),15) + "</idLote>";
        this->m_xml += _evento;
        this->m_xml += "</envEvento>" ;
    }

    return _ret;
}


void EnvEvento::updateEvento(int &i)
{
    //versão
    evento->items->value(i)->set_versao(this->m_versao);
    //nSeqEvento deve ser tratado pelo software do usuário e não pela lib, portanto,
    //se não for informado ocorrerá erro na valização.
    //caso seja maior que 0 e menor que 9, será acrescido zero a esquerda.
    //nSeqEvento deve ter dois algarismos, para não ocorrer erro no ID.
    QString _strSeq;
    if ((evento->items->value(i)->infEvento->get_nSeqEvento() > 0) &&
        (evento->items->value(i)->infEvento->get_nSeqEvento() < 10))
    {
        _strSeq = CppUtil::insZeroLeft(QString::number(evento->items->value(i)->infEvento->get_nSeqEvento()), 2);
    } else {
        _strSeq = QString::number(evento->items->value(i)->infEvento->get_nSeqEvento());
    }
    QString _id = "ID" + ConvNF::tpEventoToStr(evento->items->value(i)->infEvento->get_tpEvento())
                 + evento->items->value(i)->infEvento->get_chNFe()
                 + _strSeq;

    evento->items->value(i)->infEvento->set_Id(_id);

    //se não informou o ambiente, será o padrão de configurações
    if (evento->items->value(i)->infEvento->get_tpAmb() == TpAmb::None)
        evento->items->value(i)->infEvento->set_tpAmb(config->webservices->get_tpAmb());
}


bool EnvEvento::validarEvento()
{
    bool _ret = true;
    QByteArray _schemaPath(config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray _schemaName;
    _schemaName.append("envEvento_v");
    _schemaName.append(this->m_versao.toLocal8Bit());
    _schemaName.append(".xsd");
    CppXML * _libxml = new CppLibXml2(config->certificado->get_cryptoLib(), _schemaPath);

    this->m_error.clear(); //limpando erro
    if (!_libxml->parseXML(this->m_xml.toLocal8Bit(), _schemaName))
    {
       this->m_error = _libxml->get_error();
        _ret = false;
    }

    delete _libxml;
    return _ret;
}

void EnvEvento::tratarRetorno(const bool &vincularNota, const bool &salvarNotaVinculada)
{
    //atualiza e salva o xml
    QByteArray _schema = this->config->arquivos->get_caminhoSchema().toLocal8Bit();
    CppXML * _libxml = new CppLibXml2(this->config->certificado->get_cryptoLib(), _schema);
    //unindo xml assinado com o retorno do web services

    if (this->retorno->retEvento->items->count() > 0)
    {
        for (int i = 0; i < this->retorno->retEvento->items->count(); ++i)
        {
            int _cStat = this->retorno->retEvento->items->value(i)->infEvento->get_cStat();

            for (int j = 0; j < evento->items->count(); ++j)
            {
                //verifica se é o mesmo evento
                if ((this->retorno->retEvento->items->value(i)->infEvento->get_chNFe() ==
                     this->evento->items->value(j)->infEvento->get_chNFe()) &&
                    (this->retorno->retEvento->items->value(i)->infEvento->get_nSeqEvento() ==
                     this->evento->items->value(j)->infEvento->get_nSeqEvento()))
                {
                    if (_cStat == 135)
                    {
                        QByteArray _procEventoNFe, _xmlOriginal, _retEvento;
                        _procEventoNFe.append(QString("<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao =\"" + get_versao() + "\">").toLocal8Bit());
                        _procEventoNFe.append("</procEventoNFe>");

                        _xmlOriginal.append("<a>");
                        _xmlOriginal.append(CppUtility::extractStr(this->evento->items->value(j)->get_XMLAssinado().toLocal8Bit(), "<evento", "/evento>").toLocal8Bit());
                        _xmlOriginal.append("</a>");

                        //xml do evento é inserido ao retornar do webservices.
                        _retEvento.append("<a>");
                        _retEvento.append(CppUtility::extractStr(this->retorno->retEvento->items->value(i)->get_XML().toLocal8Bit(), "<retEvento", "/retEvento>").toLocal8Bit());
                        _retEvento.append("</a>");

                        //primeiro o xml original dentro de procEventoNFe
                        if (_libxml->addChildXML(_procEventoNFe, _xmlOriginal, QByteArray("procEventoNFe")))
                        {
                            //agora o retEvento abaixo do xml original
                            if (_libxml->addChildXML(_procEventoNFe, _retEvento, QByteArray("procEventoNFe")))
                            {
                                //atualiza o xml de retEvento da classe RetEnvEvento com o xml completo
                                this->retorno->retEvento->items->value(i)->set_XML(_procEventoNFe);
                                //caso esteja configurado para salvar
                                if (this->config->arquivos->get_salvar())
                                {
                                    //O nome do arquivo deve ser o Id enviado, já que o Id retornado é o mesmo protocolo.
                                    QString _nomeArquivo =  evento->items->value(j)->infEvento->get_Id().mid(3, evento->items->value(j)->infEvento->get_Id().length() - 3);
                                    _nomeArquivo += "-procEventoNFe";
                                    this->retorno->retEvento->items->value(i)->salvarXML(this->config->get_caminhoEvento(), _nomeArquivo);
                                }
                            }
                        }

                        //atualiza o xml da nota, caso esteja vinculado e exista
                        if (vincularNota && this->notafiscal->NFe->items->count() > 0)
                        {
                            //não será usado o l para não confundir com o i
                            for (int m = 0; m < this->notafiscal->NFe->items->count(); ++m)
                            {
                                if (this->notafiscal->NFe->items->value(m)->get_chNFe() ==
                                     retorno->retEvento->items->value(i)->infEvento->get_chNFe())
                                {
                                    //só atualiza caso o xml já tenha sido autorizado.
                                    if (!this->notafiscal->NFe->items->value(m)->get_XMLAutorizado().isEmpty())
                                    {
                                        QByteArray _xmlNota;
                                        _xmlNota.append(CppUtility::extractStr(this->notafiscal->NFe->items->value(m)->get_XMLAutorizado().toLocal8Bit(),
                                                                               "<nfeProc", "/nfeProc>").toLocal8Bit());

                                        if (_libxml->addChildXML(_xmlNota, _retEvento, QByteArray("nfeProc")))
                                        {
                                            this->notafiscal->NFe->items->value(m)->set_XMLAutorizado(_xmlNota);
                                            //salva o xml vinculado caso deseje
                                            if (salvarNotaVinculada)
                                                CppUtility::saveFile(this->config->get_caminhoNF(),
                                                                     QString(this->notafiscal->NFe->items->value(m)->get_chNFe() + "-nfe"),
                                                                     TipoArquivo::XML,
                                                                     _xmlNota);
                                        }
                                    }

                                    break;
                                }
                            }
                        }


                    }

                    break;
                }
            }

        }
    }

    delete _libxml;
}

