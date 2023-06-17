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

bool ConsSitNFe::consultar(const QString &chNFe, const bool &salvarNotaVinculada)
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
              tratarRetorno(salvarNotaVinculada);

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

void ConsSitNFe::tratarRetorno(const bool &salvarNotaVinculada)
{
    //tratar retorno
    Q_UNUSED(salvarNotaVinculada);
}
