#include "conssitnfe.h"

ConsSitNFe::ConsSitNFe(ConfigNFe *confgNFe, NotaFiscal *notafiscal) :
    retorno(new RetConsSitNFe()) , config(confgNFe), notafiscal(notafiscal)
{

}

ConsSitNFe::~ConsSitNFe()
{

}

void ConsSitNFe::clear()
{
    this->retorno->clear();
    this->m_error.clear();
    this->m_xml.clear();
    this->m_versao.clear();
    this->m_chNFe.clear();
}

bool ConsSitNFe::consultar(const QString &chNFe, const bool &salvarXMLNota, const bool &vincularEvento)
{
    bool _ret = true;
    if (gerarXML(chNFe))
    {
        //validar xml
        if (validarEvento())
        {
            //envia
            WSConsultaProtocolo* _ws = new WSConsultaProtocolo(this->config,
                                                               this->config->certificado->get_cryptoLib(),
                                                               this->retorno.get());

            connect(_ws, &WSConsultaProtocolo::wsChange, this, &ConsSitNFe::wsChange);
            connect(_ws, &WSConsultaProtocolo::errorOccurred, this, &ConsSitNFe::onError);

            bool _ret = _ws->send(this->m_xml.toLocal8Bit(),
                                  ConvNF::versaoLayout(WebServicesNF::NFeConsultaProtocolo));

            if (_ret)
              tratarRetorno(salvarXMLNota, vincularEvento);

            delete _ws;
        }
        else
        {
            _ret = false;
        }
    }
    else
    {
        _ret = false;
    }

    return _ret;
}

QString ConsSitNFe::get_versao() const
{
    return this->m_versao;
}

void ConsSitNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

QString ConsSitNFe::get_error() const
{
    return this->m_error;
}

void ConsSitNFe::onError(const QString &error)
{
    this->m_error = error;
    emit errorOccurred(error);
}

bool ConsSitNFe::gerarXML(const QString &chNFe)
{
    //versão - Caso não seja informado, será 4.00 que é a versão do  momento.
    if (this->m_versao.isEmpty())
      this->m_versao = "4.00";

    this->m_xml += "<consSitNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\""+ this->m_versao +"\">";
    this->m_xml += "<tpAmb>" + ConvNF::tpAmbToStr(config->webservices->get_tpAmb()) + "</tpAmb>";
    this->m_xml += "<xServ>CONSULTAR</xServ>";
    this->m_xml += "<chNFe>" + chNFe + "</chNFe>";
    this->m_xml += "</consSitNFe>" ;

    return true; //para manter o padrão será retornado o true, mas não há o que validar aqui
}

bool ConsSitNFe::validarEvento()
{
    bool _ret = true;
    QByteArray _schemaPath(config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray _schemaName;
    _schemaName.append("consSitNFe_v");
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

void ConsSitNFe::tratarRetorno(const bool &salvarXMLNota, const bool &vincularEvento)
{
    if (this->config->arquivos->get_salvar())
    {
        QString _nomeArqEvento =  this->retorno->get_chNFe() + "-consSitNFe";
        this->retorno->salvarXML(this->config->arquivos->get_caminhoConsulta(), _nomeArqEvento);
    }

    //atualiza e salva o xml da nota
    QByteArray _schema = this->config->arquivos->get_caminhoSchema().toLocal8Bit();
    CppXML * _libxml = new CppLibXml2(this->config->certificado->get_cryptoLib(), _schema);
    QByteArray _nfeProc, _protNFe, _xml;
    int _cStat = this->retorno->get_cStat();

    //salvando nota
    // 100 ou 150-Autorizado o Uso, 101-Cancelamento de NF-e Homologado, 110-Uso Denegado
    if (_cStat == 100 || _cStat == 101 || _cStat == 110 || _cStat == 150)
    {
        for (int i = 0; i < this->notafiscal->NFe->items->count(); ++i)
        {
            if (this->notafiscal->NFe->items->value(i)->get_chNFe() == retorno->get_chNFe())
            {
                _xml.clear();
                _xml.append("<a>");
                _xml.append(CppUtility::extractStr(this->notafiscal->NFe->items->value(i)->get_XMLOriginal(),
                                                   "<NFe xmlns", "</NFe>").toLocal8Bit());
                _xml.append("</a>");
                //montando xml
                _nfeProc = XmlWrite::get_nfeProc(this->retorno->get_versao());

                _protNFe = XmlWrite::get_protNFe(this->retorno->protNFe->get_versao(),
                                                 this->retorno->protNFe->get_Id(),
                                                 this->retorno->protNFe->get_tpAmb(),
                                                 this->retorno->protNFe->get_verAplic(),
                                                 this->retorno->protNFe->get_chNFe(),
                                                 this->retorno->protNFe->get_dhRecbto(),
                                                 this->retorno->protNFe->get_nProt(),
                                                 this->retorno->protNFe->get_digVal(),
                                                 this->retorno->protNFe->get_cStat(),
                                                 this->retorno->protNFe->get_xMotivo(),
                                                 this->retorno->protNFe->get_cMsg(),
                                                 this->retorno->protNFe->get_xMsg());

                //primeiro o xml original dentro de nfeProc
                if (_libxml->addChildXML(_nfeProc, _xml, QByteArray("nfeProc")))
                {
                    //agora o protNFe abaixo do xml original
                    if (_libxml->addChildXML(_nfeProc, _protNFe, QByteArray("nfeProc")))
                    {
                        //atualiza o xml de protNFe da classe Retorno
                        this->retorno->protNFe->set_xml(_nfeProc);
                        //atualiza xml de NFe
                        this->notafiscal->NFe->items->value(i)->set_XMLAutorizado(_nfeProc);
                    }
                }

                if (vincularEvento)
                {
                    QByteArray _xmlDFe, _xmlNota, _xmlEvento;
                    _xmlDFe.append("<DFe></DFe>");
                    _xmlNota.append("<a>");
                    _xmlNota.append(CppUtility::extractStr(_nfeProc, "<nfeProc", "nfeProc>"));
                    _xmlNota.append("</a>");
                    if (_libxml->addChildXML(_xmlDFe, _xmlNota, QByteArray("DFe")))
                    {
                        _xmlEvento.append("<a>");
                        _xmlEvento.append(this->retorno->get_xml().toLocal8Bit());
                        _xmlEvento.append("</a>");
                        if (_libxml->addChildXML(_xmlDFe, _xmlEvento, QByteArray("DFe")))
                        {
                            //atualiza o xml de protNFe da classe Retorno
                            this->retorno->protNFe->set_xml(_xmlDFe);
                            //atualiza xml de NFe
                            this->notafiscal->NFe->items->value(i)->set_XMLAutorizado(_xmlDFe);
                        }
                    }
                }
                if (salvarXMLNota)
                    this->retorno->protNFe->salvarXML(this->config->arquivos->get_caminhoNF(), "" );

            }
        }
    }
    delete _libxml;
}
