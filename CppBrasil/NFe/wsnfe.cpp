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


#include "wsnfe.h"

WSNFeBase::WSNFeBase(ConfigNFe* confgNFe, CppCrypto* crypto):
    WebServices(crypto, confgNFe->webservices->get_proxy(),
                confgNFe->webservices->get_protocoloSSL(),
                confgNFe->webservices->get_httpType(),
                confgNFe->webservices->get_timeoutConexao(),
                confgNFe->webservices->get_verificarSslSocket()),
    confgNFe(confgNFe), crypto(crypto)
{
}

WSNFeBase::~WSNFeBase()
{
}

QByteArray WSNFeBase::get_mensagemRetorno() const
{
    return this->m_msgRetorno;

}

QString WSNFeBase::get_verLayout() const
{
    return this->m_verLayout;
}

void WSNFeBase::set_verLayout(const QString &verLayout)
{
    this->m_verLayout = verLayout;
}

QString WSNFeBase::get_namespaceBase() const
{
    return this->m_nameSpaceBase;
}

QString WSNFeBase::get_urlNamespaceBase() const
{
    return this->m_urlNameSpaceBase;
}

QString WSNFeBase::get_urlNamespaceWS(const WebServicesNF &webServicesNF) const
{
    //pega o nome do webservices e coloca a versão no final da string
    //com apenas o primeiro numero
    QString _versaoWS = ConvNF::versaoNFToStr(confgNFe->get_VersaoNF());
    _versaoWS = (_versaoWS.isEmpty() ? "4" : QString(_versaoWS.at(0)));
    QString _res = get_urlNamespaceBase() +
                   QStringLiteral("/wsdl/") +
                   ConvNF::webServicesNFToMetodo(webServicesNF) + _versaoWS;
    return _res;
}

QString WSNFeBase::get_urlServidorWS(const WebServicesNF &webServicesNF) const
{
    QString _url = UrlNFe::get_urlServidor(confgNFe->webservices->get_uf(),
                                           confgNFe->webservices->get_tpAmb(),
                                           confgNFe->get_TipoEmissao(),
                                           confgNFe->get_ModeloDF(),
                                           confgNFe->get_VersaoNF(),
                                           webServicesNF);
    return _url;

}

bool WSNFeBase::validarXML(const QString &schemaName, const QString &nomeGrupo, const QByteArray &xml)
{
    QByteArray _schemaPath(this->confgNFe->arquivos->get_caminhoSchema().toLocal8Bit());
    bool _ret = true;
    CppXML * _libxml = new CppLibXml2(this->crypto, _schemaPath);

    if (nomeGrupo.isEmpty())
    {
         _ret = _libxml->parseXML(xml, schemaName.toLocal8Bit());
    }
    else
    {
        //caso informe o nome do grupo, deve gerar um novo xml a partir do grupo indicado.
        //Isso acontece no caso das mensagens de retorno que virá com o envelope soap
        //junto e dará erro na validação
        QByteArray _xml;
        _xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        _xml.append(CppUtility::extractStr(xml, "<" + nomeGrupo, nomeGrupo + ">").toLocal8Bit());
        _ret = _libxml->parseXML(_xml, schemaName.toLocal8Bit());
    }

    if (!_ret)
    {
       set_error(_libxml->get_error());
        //qWarning() << get_error(); //remover isso
    }

    delete _libxml;
    return _ret;
}

void WSNFeBase::salvarLogs(const TipoArquivo &tipoArquivo, const TipoMsgLog &tipoMsgLog,
                           const QString &nomeGrupo, const WebServicesNF &webServicesNF,
                           const QByteArray &dadosLog)
{
    if (confgNFe->arquivos->get_salvarLogs())
    {
        //para não fazer nenhum tipo de validação aqui nesta função, será melhor que
        //receba o formato que deverá ser salvo. Lembrando que o web services retornará html,
        //caso ocorra algum erro SSL.
        QString _ext, _ini, _file, _strDate;

        switch (tipoMsgLog) {
            case TipoMsgLog::MsgErro:
                _ini = "err_";
                break;
            case TipoMsgLog::MsgEnvio :
                _ini = "env_";
                break;
            case TipoMsgLog::MsgRetorno :
                _ini = "ret_";
                break;
        }

        _strDate = CppUtility::dateTimeToStr(QDateTime::currentDateTime(), DtH::DateTimeNumber);
        _file = _ini + ConvNF::webServicesNFToMetodo(webServicesNF) + "_" + _strDate + _ext;

        //se não tiver o nome do grupo, será salvo o xml da forma que retornar do web services
        if (nomeGrupo.isEmpty())
        {
            CppUtility::saveFile(confgNFe->arquivos->get_caminhoLogs(), _file, tipoArquivo, dadosLog);
        }
        else
        {
            QString _xml;
            _xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            _xml.append(CppUtility::extractStr(dadosLog, "<" + nomeGrupo, nomeGrupo + ">"));
            CppUtility::saveFile(confgNFe->arquivos->get_caminhoLogs(), _file, tipoArquivo, _xml.toLocal8Bit());
        }
    }
}

//WSNFe---------------------------------------------------

WSNFe::WSNFe(ConfigNFe* confgNFe, CppCrypto* crypto, RetConsReciNFe* retorno):
    WSNFeBase(confgNFe, crypto), retorno(retorno)
{
}

WSNFe::~WSNFe()
{
}

bool WSNFe::send(const int &numLote, const QByteArray &xml, const QString &verLayout)
{
    //versão do layout
    set_verLayout(verLayout);
    StatusRetorno _ret = StatusRetorno::EmProcessamento;
    QString _url = get_urlServidorWS(WebServicesNF::NFeAutorizacao);
    //se não for informado quantas tentativas, o padrão será ao menos 2
    int _tentativas = this->confgNFe->webservices->get_tentativas();
    if (_tentativas <= 0)
      _tentativas = 2;
    //se não for informado o timeout, o padrão será 8000 milisegundos (8 segundos)
    int _timeout = this->confgNFe->webservices->get_timeoutAssincrono();
    if (_timeout <= 0)
      _timeout = 8000;

    //gera xml do lote na var m_msgEnvio
    if (gerarXMLlote(numLote, xml))
    {
        //enviando mensagem
        emit wsChange(WebServicesNF::NFeAutorizacao);
        this->m_msgRetorno.clear();
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio);
        //se não houver erro
        if (this->get_error().isEmpty())
        {
            //caso seja sincrono, apenas trata o retorno já que o lote vem junto
            if (this->confgNFe->get_indicadorSincrono() == IndSinc::Sim)
            {
                _ret = tratarRetorno(TipoRetorno::retEnviNFe);
            }
            else
            {
                //trata primeiro recibo
                _ret = tratarRetorno(TipoRetorno::retEnviNFe);
                if (_ret != StatusRetorno::Erro)
                {
                    //gera o xml de recibo na var m_msgEnvio
                    if (gerarXMLrecibo())
                    {
                        //aqui será feito quantas consultas forem informados em tentativas
                        //caso a consulta retorne EmProcessamento, deverá esperar outro timeout para consultar novamente
                        for (int i = 0; i < _tentativas; ++i)
                        {
                            //aguarda o timeout para fazer a consulta do lote
                            if (timeout(_timeout))
                            {
                                _url = get_urlServidorWS(WebServicesNF::NFeRetAutorizacao);
                                //enviando mensagem
                                emit wsChange(WebServicesNF::NFeRetAutorizacao);
                                this->m_msgRetorno.clear();
                                this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio);
                                //se não houver erro
                                if (this->get_error().isEmpty())
                                {
                                    _ret = tratarRetorno(TipoRetorno::retConsReciNFe);
                                    //se retornar algo diferente de EmProcessamento, deverá parar, já que foi processado
                                    if (_ret != StatusRetorno::EmProcessamento)
                                      break;
                                }
                                else
                                {
                                    _ret = StatusRetorno::Erro;
                                    break;
                                    set_error("Erro ao enviar lote.");
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            _ret = StatusRetorno::Erro;
            set_error("Erro ao enviar lote.");
        }
    }
    emit wsChange(WebServicesNF::None);
    return (_ret == StatusRetorno::Erro) ? false : true;
}

bool WSNFe::gerarXMLlote(const int &numLote, const QByteArray &xml)
{
    //envelop soap
    QString _namespace = get_namespaceBase();
    QString _urlNamespace = get_urlNamespaceWS(WebServicesNF::NFeAutorizacao);
    QString _versao = ConvNF::versaoNFToStr(this->confgNFe->get_VersaoNF());
    QString _indSinc = ConvNF::indSincToStr(this->confgNFe->get_indicadorSincrono());
    QString _env;
    _env += QStringLiteral("<enviNFe xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _env += QStringLiteral("<idLote>") + QString::number(numLote) +  QStringLiteral("</idLote>");
    _env += QStringLiteral("<indSinc>") + _indSinc +  QStringLiteral("</indSinc>");
    _env += QString(xml);
    _env += QStringLiteral("</enviNFe>");

    //nome do schema
    QString _schemaName = "enviNFe_v" + get_verLayout() + ".xsd";
    //validar xml com schema
    bool _ret = validarXML(_schemaName, "", _env.toLocal8Bit());
    if (_ret)
    {
        //se deseja compactar
        if (this->confgNFe->webservices->get_compactar())
        {
            int _size = this->confgNFe->webservices->get_compactarAcimaDe();
            //size dever ser ao menos 1
            if (_size <= 0)
               _size = 1;
            //tamanho minimo do pacote a ser compactado
            int _minSize = 1024 * _size;
            //se _env for maior que o tamanho mínimo do pacote, deve ser compactado.
            //_env deve ser, pelo menos, maior que 1KB, se não, não há necessidade de compactar.
            if (_env.size() > _minSize)
            {
                QByteArray _tmp;
                if (CppZip::compress(_env.toLocal8Bit(), _tmp))
                {
                    _env.clear();
                    _env.append(_tmp.toBase64());
                }
                _namespace += "Zip";
            }
        }

        this->m_msgEnvio.clear();
        _ret = get_soapEnvelop(_env.toLocal8Bit(), _namespace.toLocal8Bit(),
                               _urlNamespace.toLocal8Bit(), this->m_msgEnvio);
        //salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,_namespace,//"enviNFe",
                   WebServicesNF::NFeAutorizacao, this->m_msgEnvio);
    }
    else
    {
        //caso der erro na validação, também salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgErro ,_namespace,//,"enviNFe",
                   WebServicesNF::NFeAutorizacao, this->m_msgEnvio);
    }

    return _ret;
}

bool WSNFe::gerarXMLrecibo()
{
    //envelop soap
    QString _urlBodyElement = get_urlNamespaceWS(WebServicesNF::NFeRetAutorizacao);
    QString _versao = ConvNF::versaoNFToStr(this->confgNFe->get_VersaoNF());
    QString _tpAmb = ConvNF::tpAmbToStr(this->confgNFe->webservices->get_tpAmb());
    QString _nRec = this->retorno->get_nRec();
    QString _rec;
    _rec += QStringLiteral("<consReciNFe xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _rec += QStringLiteral("<tpAmb>") + _tpAmb +  QStringLiteral("</tpAmb>");
    _rec += QStringLiteral("<nRec>") + _nRec +  QStringLiteral("</nRec>");
    _rec += QStringLiteral("</consReciNFe>");

    //nome do schema
    QString _schemaName = "consReciNFe_v" + get_verLayout() + ".xsd";
    //validar xml com schema
    bool _ret = validarXML(_schemaName,"", _rec.toLocal8Bit());
    if (_ret)
    {
        this->m_msgEnvio.clear();
        _ret = get_soapEnvelop(_rec.toLocal8Bit(), get_namespaceBase().toLocal8Bit(),
                                    _urlBodyElement.toLocal8Bit(), this->m_msgEnvio);
        //salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,"consReciNFe",
                   WebServicesNF::NFeRetAutorizacao, this->m_msgEnvio);
    }
    else
    {
        //caso der erro na validação, também salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgErro ,"consReciNFe",
                   WebServicesNF::NFeRetAutorizacao, this->m_msgEnvio);
    }

    return _ret;
}

WSNFeBase::StatusRetorno WSNFe::tratarRetorno(const TipoRetorno &tipo)
{
    QString _parent = (tipo == TipoRetorno::retEnviNFe) ? "retEnviNFe" : "retConsReciNFe";

    //consultar schema e analizar o resultado
    QString _schemaName = _parent + "_v" + get_verLayout() + ".xsd";
    StatusRetorno _ret = (validarXML(_schemaName, _parent, this->m_msgRetorno) ? StatusRetorno::Processado : StatusRetorno::Erro);
    if (_ret == StatusRetorno::Processado)
    {
        QXmlStreamReader _xml(this->m_msgRetorno);
        QXmlStreamAttributes attributes;
        QString _valor;
        //salvar log do retorno
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgRetorno ,_parent,
                   WebServicesNF::NFeRetAutorizacao, this->m_msgRetorno);

        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == _parent))
        {
            attributes = _xml.attributes();
            if(attributes.hasAttribute("versao"))
            {
                _valor = attributes.value("versao").toString();
                if (!_valor.isEmpty())
                    this->retorno->set_versao(_valor);
            }

            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("nRec"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_nRec(_valor);
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_cStat(_valor.toInt());

                if (_valor.toInt() == 105) //Lote em processamento
                    _ret = StatusRetorno::EmProcessamento;
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("cUF"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_cUF(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRecbto"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->retorno->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("cMsg"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_cMsg(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMsg"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->retorno->set_xMsg(_valor);
            }
            //assincrono retorna o numero do recibo como filho do elemento infRec
            if(_xml.name() == QStringLiteral("infRec"))
            {
                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infRec")))
                {
                    if(_xml.name() == QStringLiteral("nRec"))
                    {
                        _valor = _xml.readElementText();
                        if (!_valor.isEmpty())
                          this->retorno->set_nRec(_valor);
                    }
                    _xml.readNext();
                }
            }
            //lote
            if(_xml.name() == QStringLiteral("protNFe"))
            {
                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("protNFe")))
                {
                    attributes = _xml.attributes();
                    if(attributes.hasAttribute("versao"))
                    {
                        _valor = attributes.value("versao").toString();
                        if (!_valor.isEmpty())
                            this->retorno->protNFe->obj->set_versao(_valor);
                    }
                    if(_xml.name() == QStringLiteral("infProt"))
                    {
                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infProt")))
                        {
                            //attributes = _xml.attributes();
                            if(attributes.hasAttribute("Id"))
                            {
                                _valor = attributes.value("Id").toString();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_Id(_valor);
                            }

                            if(_xml.name() == QStringLiteral("tpAmb"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_tpAmb(ConvNF::strToTpAmb(_valor));
                            }
                            if(_xml.name() == QStringLiteral("verAplic"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_verAplic(_valor);
                            }
                            if(_xml.name() == QStringLiteral("chNFe"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_chNFe(_valor);
                            }
                            if(_xml.name() == QStringLiteral("dhRecbto"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                   this->retorno->protNFe->obj->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                            }
                            if(_xml.name() == QStringLiteral("nProt"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                  this->retorno->protNFe->obj->set_nProt(_valor);
                            }
                            if(_xml.name() == QStringLiteral("digVal"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_digVal(_valor);
                            }
                            if(_xml.name() == QStringLiteral("cStat"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_cStat(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("xMotivo"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_xMotivo(_valor);
                            }
                            //Sequência XML - PR13 - dentro de PR03
                            if(_xml.name() == QStringLiteral("cMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_cMsg(_valor.toInt());

                            }
                            if(_xml.name() == QStringLiteral("xMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->obj->set_xMsg(_valor);
                            }

                            _xml.readNext();
                        }
                    }

                    _xml.readNext();
                }

                this->retorno->protNFe->add();
            }

            _xml.readNext();
        }
    }
    else
    {
        _ret = StatusRetorno::Erro;
        set_error("Erro: O XML retornado pelo web services não é válido.");
        //caso errado, salvar log do retorno em formato html
        salvarLogs(TipoArquivo::HTML,TipoMsgLog::MsgErro ,"",
                   WebServicesNF::NFeRetAutorizacao, this->m_msgRetorno);
    }

    return _ret;
}


//WSStatus---------------------------------------------------

WSStatus::WSStatus(ConfigNFe *confgNFe, CppCrypto *crypto, RetConsStatServ *retorno) :
  WSNFeBase(confgNFe, crypto), retorno(retorno)
{
}

WSStatus::~WSStatus()
{
}

bool WSStatus::send(const QString &verLayout)
{
    set_verLayout(verLayout);
    StatusRetorno _ret;
    if (gerarXML())
    {
        QString _url = get_urlServidorWS(WebServicesNF::NFeStatusServico);
        emit wsChange(WebServicesNF::NFeStatusServico);
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio);

        //se não houve erro no envio, trata o retorno
        if (this->get_error().isEmpty())
            _ret = tratarRetorno();
        else
            _ret = StatusRetorno::Erro;
    }
    else //se houve erro ao gerar xml
       _ret = StatusRetorno::Erro;

    emit wsChange(WebServicesNF::None);
    return (_ret == StatusRetorno::Erro) ? false : true;
}

bool WSStatus::gerarXML()
{
    //envelop soap
    QString _urlBodyElement = get_urlNamespaceWS(WebServicesNF::NFeStatusServico);
    QString _versao = ConvNF::versaoNFToStr(this->confgNFe->get_VersaoNF());
    QString _tpAmb = ConvNF::tpAmbToStr(this->confgNFe->webservices->get_tpAmb());
    int _cUF = ConvNF::ufToInt(this->confgNFe->webservices->get_uf());
    QString _env;
    _env += QStringLiteral("<consStatServ xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _env += QStringLiteral("<tpAmb>") + _tpAmb +  QStringLiteral("</tpAmb>");
    _env += QStringLiteral("<cUF>") + QString::number(_cUF) +  QStringLiteral("</cUF>");
    _env += QStringLiteral("<xServ>STATUS</xServ>");
    _env += QStringLiteral("</consStatServ>");

    //validar o _env
    QString _schemaName = "consStatServ_v" + get_verLayout() + ".xsd";
    bool _ret = validarXML(_schemaName, "", _env.toLocal8Bit());
    if (_ret)
    {
        this->m_msgEnvio.clear();
        _ret = get_soapEnvelop(_env.toLocal8Bit(), get_namespaceBase().toLocal8Bit(),
                                    _urlBodyElement.toLocal8Bit(), this->m_msgEnvio);
        //salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,"consStatServ",
                   WebServicesNF::NFeStatusServico, this->m_msgEnvio);
    }
    else
    {
        //caso der erro na validação, também salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgErro ,"consStatServ",
                   WebServicesNF::NFeStatusServico, this->m_msgEnvio);
    }

    return _ret;

}

WSNFeBase::StatusRetorno WSStatus::tratarRetorno()
{
    //consultar schema e analizar o resultado
    QString _schemaName = "retConsStatServ_v" + get_verLayout() + ".xsd";
    StatusRetorno _ret = (validarXML(_schemaName, "retConsStatServ", this->m_msgRetorno) ? StatusRetorno::Processado : StatusRetorno::Erro );

    if (_ret == StatusRetorno::Processado)
    {
        QXmlStreamReader _xml(this->m_msgRetorno);
        QString _valor;
        //salvar log do retorno
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgRetorno ,"retConsStatServ",
                   WebServicesNF::NFeStatusServico, this->m_msgRetorno);
        QXmlStreamAttributes attributes;
        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retConsStatServ")))
        {
            attributes = _xml.attributes();
            if(attributes.hasAttribute("versao"))
            {
                 _valor = attributes.value("versao").toString();
                 if (!_valor.isEmpty())
                    this->retorno->set_versao(_valor);
            }

            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_cStat(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("cUF"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_cUF(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRecbto"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->retorno->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("tMed"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_tMed(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRetorno"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->retorno->set_dhRetorno(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("xObs"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_xObs(_valor);
            }

            _xml.readNext();
        }
    }
    else
    {
        set_error("Erro: O XML retornado pelo web services não é válido.");
        //caso errado, salvar log do retorno em formato html
        salvarLogs(TipoArquivo::HTML,TipoMsgLog::MsgErro ,"",
                   WebServicesNF::NFeStatusServico, this->m_msgRetorno);

    }

    return _ret;
}

//WSStatus---------------------------------------------------

WSEvento::WSEvento(ConfigNFe *confgNFe, CppCrypto *crypto, RetEnvEvento *retorno):
    WSNFeBase(confgNFe, crypto), retorno(retorno)
{

}

WSEvento::~WSEvento()
{

}

bool WSEvento::send(const QByteArray &xml, const QString &verLayout)
{
    set_verLayout(verLayout);
    StatusRetorno _ret = StatusRetorno::EmProcessamento;
    QString _url = get_urlServidorWS(WebServicesNF::NFeRecepcaoEvento);
    //gera xml do lote na var m_msgEnvio
    if (gerarXML(xml))
    {
        //enviando mensagem
        emit wsChange(WebServicesNF::NFeRecepcaoEvento);
        this->m_msgRetorno.clear();
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio);
        //se não houver erro
        if (this->get_error().isEmpty())
        {
            //envio de eventos é síncrono
            _ret = tratarRetorno();
        }
        else
        {
            _ret = StatusRetorno::Erro;
            set_error("Erro ao enviar evento.");
        }
    }
    emit wsChange(WebServicesNF::None);
    return (_ret == StatusRetorno::Erro) ? false : true;
}

bool WSEvento::gerarXML(const QByteArray &xml)
{
    //só será necessário gerar o envelop soap, já que o xml foi assinado e validado na sua criação
    bool _ret = true;
    //envelop soap
    QString _urlBodyElement = get_urlNamespaceWS(WebServicesNF::NFeRecepcaoEvento);

    this->m_msgEnvio.clear();
    _ret = get_soapEnvelop(xml, get_namespaceBase().toLocal8Bit(),
                                _urlBodyElement.toLocal8Bit(), this->m_msgEnvio);
    //salva log
    salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,"envEvento",
               WebServicesNF::NFeRecepcaoEvento, this->m_msgEnvio);

    return _ret;
}

WSNFeBase::StatusRetorno WSEvento::tratarRetorno()
{
    //consultar schema e analizar o resultado
    QString _schemaName = "retEnvEvento_v" + get_verLayout() + ".xsd";
    StatusRetorno _ret = (validarXML(_schemaName, "retEnvEvento", this->m_msgRetorno) ? StatusRetorno::Processado : StatusRetorno::Erro );

    if (_ret == StatusRetorno::Processado)
    {
        QXmlStreamReader _xml(this->m_msgRetorno);
        QString _valor;
        //salvar log do retorno
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgRetorno ,"retEnvEvento",
                   WebServicesNF::NFeRecepcaoEvento, this->m_msgRetorno);
        QXmlStreamAttributes attributes;
        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retEnvEvento")))
        {
            attributes = _xml.attributes();
            if(attributes.hasAttribute("versao"))
            {
                 _valor = attributes.value("versao").toString();
                 if (!_valor.isEmpty())
                    this->retorno->set_versao(_valor);
            }
            if(_xml.name() == QStringLiteral("idLote"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_idLote(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("cOrgao"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_cOrgao(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_cStat(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("retEvento"))
            {
                //montando xml apenas do retEvento.
                QByteArray _strXMLRetEvento;
                QXmlStreamWriter _xmlRetEvento(&_strXMLRetEvento);
                _xmlRetEvento.writeStartElement("retEvento");

                if(attributes.hasAttribute("versao"))
                {
                    _valor = attributes.value("versao").toString();
                    if (!_valor.isEmpty())
                       this->retorno->retEvento->obj->set_versao(_valor);
                    _xmlRetEvento.writeAttribute("versao", _valor);
                }

                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retEvento")))
                {
                    if(_xml.name() == QStringLiteral("infEvento"))
                    {
                        _xmlRetEvento.writeStartElement("infEvento");

                        //attributes = _xml.attributes();
                        if(attributes.hasAttribute("Id"))
                        {
                           _valor = attributes.value("Id").toString();
                           if (!_valor.isEmpty())
                               this->retorno->retEvento->obj->infEvento->set_Id(_valor);

                           _xmlRetEvento.writeAttribute("Id", _valor);
                        }

                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infEvento")))
                        {
                            if(_xml.name() == QStringLiteral("tpAmb"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_tpAmb(ConvNF::strToTpAmb(_valor));

                                _xmlRetEvento.writeTextElement("tpAmb", _valor);
                            }
                            if(_xml.name() == QStringLiteral("verAplic"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_verAplic(_valor);

                                _xmlRetEvento.writeTextElement("verAplic", _valor);
                            }
                            if(_xml.name() == QStringLiteral("cOrgao"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                   this->retorno->retEvento->obj->infEvento->set_cOrgao(_valor.toInt());

                                _xmlRetEvento.writeTextElement("cOrgao", _valor);
                            }
                            if(_xml.name() == QStringLiteral("cStat"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_cStat(_valor.toInt());

                                _xmlRetEvento.writeTextElement("cStat", _valor);
                            }
                            if(_xml.name() == QStringLiteral("xMotivo"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_xMotivo(_valor);

                                _xmlRetEvento.writeTextElement("xMotivo", _valor);
                            }
                            if(_xml.name() == QStringLiteral("chNFe"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_chNFe(_valor);

                                _xmlRetEvento.writeTextElement("chNFe", _valor);
                            }
                            if(_xml.name() == QStringLiteral("tpEvento"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_tpEvento(ConvNF::strToTpEvento(_valor));

                                _xmlRetEvento.writeTextElement("tpEvento", _valor);
                            }
                            if(_xml.name() == QStringLiteral("xEvento"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_xEvento(_valor);

                                _xmlRetEvento.writeTextElement("xEvento", _valor);
                            }
                            if(_xml.name() == QStringLiteral("nSeqEvento"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_nSeqEvento(_valor.toInt());

                                _xmlRetEvento.writeTextElement("nSeqEvento", _valor);
                            }
                            if(_xml.name() == QStringLiteral("cOrgaoAutor"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_cOrgaoAutor(_valor.toInt());

                                _xmlRetEvento.writeTextElement("cOrgaoAutor", _valor);
                            }
                            if(_xml.name() == QStringLiteral("CNPJDest"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_CNPJDest(_valor);

                                _xmlRetEvento.writeTextElement("CNPJDest", _valor);
                            }
                            if(_xml.name() == QStringLiteral("CPFDest"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_CPFDest(_valor);

                                _xmlRetEvento.writeTextElement("CPFDest", _valor);
                            }
                            if(_xml.name() == QStringLiteral("emailDest"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_emailDest(_valor);

                                _xmlRetEvento.writeTextElement("emailDest", _valor);
                            }
                            if(_xml.name() == QStringLiteral("dhRegEvento"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_dhRegEvento(QDateTime::fromString(_valor, Qt::ISODate));

                                _xmlRetEvento.writeTextElement("dhRegEvento", _valor);
                            }
                            if(_xml.name() == QStringLiteral("nProt"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_nProt(_valor);

                                _xmlRetEvento.writeTextElement("nProt", _valor);
                            }
                            if(_xml.name() == QStringLiteral("chNFePend"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retEvento->obj->infEvento->set_chNFePend(_valor);

                                _xmlRetEvento.writeTextElement("chNFePend", _valor);
                            }

                            _xml.readNext();
                        }

                        _xmlRetEvento.writeEndElement(); //fechamento infEvento
                    }

                    _xml.readNext();
                }
                _xmlRetEvento.writeEndElement(); //fechamento retEvento
                this->retorno->retEvento->obj->set_XML(_strXMLRetEvento);
                this->retorno->retEvento->add();
            }

            _xml.readNext();
        }
    }
    else
    {
        set_error("Erro: O XML retornado pelo web services não é válido.");
        //caso errado, salvar log do retorno em formato html
        salvarLogs(TipoArquivo::HTML,TipoMsgLog::MsgErro ,"",
                   WebServicesNF::NFeRecepcaoEvento, this->m_msgRetorno);
    }

    return _ret;
}

//WSConsultaProtocolo---------------------------------------------------

WSConsultaProtocolo::WSConsultaProtocolo(ConfigNFe *confgNFe, CppCrypto *crypto, RetConsSitNFe *retorno) :
   WSNFeBase(confgNFe, crypto), retorno(retorno)
{

}

bool WSConsultaProtocolo::send(const QByteArray &xml, const QString &verLayout)
{
    set_verLayout(verLayout);
    StatusRetorno _ret = StatusRetorno::EmProcessamento;
    QString _url = get_urlServidorWS(WebServicesNF::NFeConsultaProtocolo);
    //gera xml do lote na var m_msgEnvio
    if (gerarXML(xml))
    {
        //enviando mensagem
        emit wsChange(WebServicesNF::NFeConsultaProtocolo);
        this->m_msgRetorno.clear();
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio);
        //se não houver erro
        if (this->get_error().isEmpty())
        {
            //envio de consulta é síncrono
            _ret = tratarRetorno();
        }
        else
        {
            _ret = StatusRetorno::Erro;
            set_error("Erro ao enviar evento.");
        }
    }
    emit wsChange(WebServicesNF::None);
    return (_ret == StatusRetorno::Erro) ? false : true;

}

bool WSConsultaProtocolo::gerarXML(const QByteArray &xml)
{
    //só será necessário gerar o envelop soap, já que o xml foi assinado e validado na sua criação
    bool _ret = true;
    //envelop soap
    QString _urlBodyElement = get_urlNamespaceWS(WebServicesNF::NFeConsultaProtocolo);

    this->m_msgEnvio.clear();
    _ret = get_soapEnvelop(xml, get_namespaceBase().toLocal8Bit(),
                                _urlBodyElement.toLocal8Bit(), this->m_msgEnvio);
    //salva log
    salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,"consSitNFe",
               WebServicesNF::NFeConsultaProtocolo, this->m_msgEnvio);

    return _ret;

}

WSNFeBase::StatusRetorno WSConsultaProtocolo::tratarRetorno()
{
    //consultar schema e analizar o resultado
    QString _schemaName = "retConsSitNFe_v" + get_verLayout() + ".xsd";
    StatusRetorno _ret = (validarXML(_schemaName, "retConsSitNFe", this->m_msgRetorno) ? StatusRetorno::Processado : StatusRetorno::Erro );

    if (_ret == StatusRetorno::Processado)
    {
        QXmlStreamReader _xml(this->m_msgRetorno);
        QString _valor;
        this->retorno->set_xml(CppUtility::extractStr(this->m_msgRetorno, "<retConsSitNFe", "retConsSitNFe>"));
        //salvar log do retorno
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgRetorno ,"retConsSitNFe",
                   WebServicesNF::NFeConsultaProtocolo, this->retorno->get_xml().toLocal8Bit());

        QXmlStreamAttributes attributes;
        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retConsSitNFe")))
        {
            attributes = _xml.attributes();
            if(attributes.hasAttribute("versao"))
            {
                _valor = attributes.value("versao").toString();
                if (!_valor.isEmpty())
                    this->retorno->set_versao(_valor);
            }
            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_cStat(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("cUF"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_cUF(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRegEvento"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("chNFe"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->retorno->set_chNFe(_valor);
            }

            if(_xml.name() == QStringLiteral("protNFe"))
            {
                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("protNFe")))
                {
                    if(attributes.hasAttribute("versao"))
                    {
                        _valor = attributes.value("versao").toString();
                        if (!_valor.isEmpty())
                            this->retorno->protNFe->set_versao(_valor);
                    }
                    if(_xml.name() == QStringLiteral("infProt"))
                    {
                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infProt")))
                        {
                            if(attributes.hasAttribute("Id"))
                            {
                                _valor = attributes.value("Id").toString();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_Id(_valor);
                            }

                            if(_xml.name() == QStringLiteral("tpAmb"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_tpAmb(ConvNF::strToTpAmb(_valor));
                            }
                            if(_xml.name() == QStringLiteral("verAplic"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_verAplic(_valor);
                            }
                            if(_xml.name() == QStringLiteral("chNFe"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_chNFe(_valor);
                            }
                            if(_xml.name() == QStringLiteral("dhRecbto"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                            }
                            if(_xml.name() == QStringLiteral("nProt"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_nProt(_valor);
                            }
                            if(_xml.name() == QStringLiteral("digVal"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_digVal(_valor);
                            }
                            if(_xml.name() == QStringLiteral("cStat"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_cStat(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("xMotivo"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_xMotivo(_valor);
                            }
                            //Sequência XML - PR13 - dentro de PR03
                            if(_xml.name() == QStringLiteral("cMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_cMsg(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("xMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->protNFe->set_xMsg(_valor);
                            }
                            _xml.readNext();
                            //while infProt
                        }
                    }
                    _xml.readNext();
                    //while protNFe
                }
            }

            if(_xml.name() == QStringLiteral("retCancNFe"))
            {
                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retCancNFe")))
                {
                    if(attributes.hasAttribute("versao"))
                    {
                        _valor = attributes.value("versao").toString();
                        if (!_valor.isEmpty())
                            this->retorno->retCancNFe->set_versao(_valor);
                    }
                    if(_xml.name() == QStringLiteral("infCanc"))
                    {
                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infCanc")))
                        {
                            if(attributes.hasAttribute("Id"))
                            {
                                _valor = attributes.value("Id").toString();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_Id(_valor);
                            }
                            if(_xml.name() == QStringLiteral("tpAmb"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_tpAmb(ConvNF::strToTpAmb(_valor));
                            }
                            if(_xml.name() == QStringLiteral("verAplic"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_verAplic(_valor);
                            }
                            if(_xml.name() == QStringLiteral("cStat"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_cStat(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("xMotivo"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_xMotivo(_valor);
                            }
                            if(_xml.name() == QStringLiteral("cUF"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_cUF(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("chNFe"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_chNFe(_valor);
                            }
                            if(_xml.name() == QStringLiteral("dhRecbto"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                            }
                            if(_xml.name() == QStringLiteral("nProt"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->retorno->retCancNFe->infCanc->set_nProt(_valor);
                            }
                            _xml.readNext();
                            //while infCanc
                        }
                    }
                    _xml.readNext();
                    //while retCancNFe
                }
            }
            if(_xml.name() == QStringLiteral("procEventoNFe"))
            {
                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("procEventoNFe")))
                {
                    if(attributes.hasAttribute("versao"))
                    {
                        _valor = attributes.value("versao").toString();
                        if (!_valor.isEmpty())
                            this->retorno->procEventoNFe->obj->set_versao(_valor);
                    }

                    if(_xml.name() == QStringLiteral("evento"))
                    {
                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("evento")))
                        {
                            if(attributes.hasAttribute("versao"))
                            {
                                _valor = attributes.value("versao").toString();
                                if (!_valor.isEmpty())
                                    this->retorno->procEventoNFe->obj->evento->set_versao(_valor);
                            }
                            if(_xml.name() == QStringLiteral("infEvento"))
                            {
                                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infEvento")))
                                {
                                    if(attributes.hasAttribute("Id"))
                                    {
                                        _valor = attributes.value("Id").toString();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_Id(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("cOrgao"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_cOrgao(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("tpAmb"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_tpAmb(ConvNF::strToTpAmb(_valor));
                                    }
                                    if(_xml.name() == QStringLiteral("CNPJ"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_CNPJ(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("CPF"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_CPF(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("chNFe"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_chNFe(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("dhEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_dhEvento(QDateTime::fromString(_valor, Qt::ISODate));
                                    }
                                    if(_xml.name() == QStringLiteral("tpEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_tpEvento(ConvNF::strToTpEvento(_valor));
                                    }
                                    if(_xml.name() == QStringLiteral("nSeqEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_nSeqEvento(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("verEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->evento->infEvento->set_verEvento(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("detEvento"))
                                    {
                                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("detEvento")))
                                        {
                                            if(_xml.name() == QStringLiteral("descEvento"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_descEvento(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("cOrgaoAutor"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_cOrgaoAutor(_valor.toInt());
                                            }
                                            if(_xml.name() == QStringLiteral("tpAutor"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_tpAutor(ConvNF::strToTpAutor(_valor));
                                            }
                                            if(_xml.name() == QStringLiteral("verAplic"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_verAplic(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("nProt"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_nProt(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("xJust"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_xJust(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("chNFeRef"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_chNFeRef(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("xCorrecao"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_xCorrecao(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("xCondUso"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_xCondUso(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("xCondUso"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_xCondUso(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("dhEmi"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_dhEmi(QDateTime::fromString(_valor, Qt::ISODate));
                                            }
                                            if(_xml.name() == QStringLiteral("tpNF"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_tpNF(ConvNF::strToTpNF(_valor));
                                            }
                                            if(_xml.name() == QStringLiteral("IE"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_IE(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("vNF"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_vNF(_valor.toDouble());
                                            }
                                            if(_xml.name() == QStringLiteral("vICMS"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_vICMS(_valor.toDouble());
                                            }
                                            if(_xml.name() == QStringLiteral("vST"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_vST(_valor.toDouble());
                                            }
                                            if(_xml.name() == QStringLiteral("tpAutorizacao"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_tpAutorizacao(ConvNF::strToTpAutorizacao(_valor));
                                            }
                                            if(_xml.name() == QStringLiteral("nDoc"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_nDoc(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("xNome"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_xNome(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("latGPS"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_latGPS(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("longGPS"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_longGPS(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("hashComprovante"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_hashComprovante(_valor);
                                            }
                                            if(_xml.name() == QStringLiteral("dhHashComprovante"))
                                            {
                                                _valor = _xml.readElementText();
                                                if (!_valor.isEmpty())
                                                    this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->set_dhHashComprovante(QDateTime::fromString(_valor, Qt::ISODate));
                                            }
                                            if(_xml.name() == QStringLiteral("dest"))
                                            {
                                                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("dest")))
                                                {
                                                    if(_xml.name() == QStringLiteral("UF"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->dest->set_UF(_valor.toInt());
                                                    }
                                                    if(_xml.name() == QStringLiteral("CNPJ"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->dest->set_CNPJ(_valor);
                                                    }
                                                    if(_xml.name() == QStringLiteral("CPF"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->dest->set_CPF(_valor);
                                                    }
                                                    if(_xml.name() == QStringLiteral("idEstrangeiro"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->dest->set_idEstrangeiro(_valor);
                                                    }
                                                    if(_xml.name() == QStringLiteral("IE"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->dest->set_IE(_valor);
                                                    }
                                                    _xml.readNext();
                                                    //while dest
                                                }
                                            }

                                            if(_xml.name() == QStringLiteral("itemPedido"))
                                            {
                                                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("itemPedido")))
                                                {
                                                    if(_xml.name() == QStringLiteral("numItem"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->itemPedido->obj->set_numItem(_valor.toInt());
                                                    }
                                                    if(_xml.name() == QStringLiteral("qtdeItem"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->itemPedido->obj->set_qtdeItem(_valor.toInt());
                                                    }

                                                    _xml.readNext();
                                                    //while itemPedido
                                                }
                                                this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->itemPedido->add();
                                            }

                                            if(_xml.name() == QStringLiteral("autXML"))
                                            {
                                                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("autXML")))
                                                {
                                                    if(_xml.name() == QStringLiteral("CNPJ"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->autXML->set_CNPJ(_valor);
                                                    }
                                                    if(_xml.name() == QStringLiteral("CPF"))
                                                    {
                                                        _valor = _xml.readElementText();
                                                        if (!_valor.isEmpty())
                                                            this->retorno->procEventoNFe->obj->evento->infEvento->detEvento->autXML->set_CPF(_valor);
                                                    }
                                                    _xml.readNext();
                                                    //while autXML
                                                }
                                            }
                                            _xml.readNext();
                                            //while detEvento
                                        }
                                    }
                                    _xml.readNext();
                                    //while infEvento
                                }
                            }
                            _xml.readNext();
                            //while evento
                        }
                    }
                    if(_xml.name() == QStringLiteral("retEvento"))
                    {
                        while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("retEvento")))
                        {
                            if(attributes.hasAttribute("versao"))
                            {
                                _valor = attributes.value("versao").toString();
                                if (!_valor.isEmpty())
                                    this->retorno->procEventoNFe->obj->retEvento->set_versao(_valor);
                            }

                            if(_xml.name() == QStringLiteral("infEvento"))
                            {
                                while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("infEvento")))
                                {
                                    if(_xml.name() == QStringLiteral("Id"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_Id(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("tpAmb"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_tpAmb(ConvNF::strToTpAmb(_valor));
                                    }
                                    if(_xml.name() == QStringLiteral("verAplic"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_verAplic(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("verAplic"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_verAplic(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("cOrgao"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_cOrgao(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("cStat"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_cStat(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("xMotivo"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_xMotivo(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("chNFe"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_chNFe(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("tpEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_tpEvento(ConvNF::strToTpEvento(_valor));
                                    }
                                    if(_xml.name() == QStringLiteral("xEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_xEvento(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("nSeqEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_nSeqEvento(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("cOrgaoAutor"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_cOrgaoAutor(_valor.toInt());
                                    }
                                    if(_xml.name() == QStringLiteral("CNPJDest"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_CNPJDest(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("CPFDest"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_CPFDest(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("emailDest"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_emailDest(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("dhRegEvento"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_dhRegEvento(QDateTime::fromString(_valor, Qt::ISODate));
                                    }
                                    if(_xml.name() == QStringLiteral("nProt"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_nProt(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("chNFePend"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_chNFePend(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("dhRecbto"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                                    }
                                    if(_xml.name() == QStringLiteral("chNFePend"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_chNFePend(_valor);
                                    }
                                    if(_xml.name() == QStringLiteral("cUF"))
                                    {
                                        _valor = _xml.readElementText();
                                        if (!_valor.isEmpty())
                                            this->retorno->procEventoNFe->obj->retEvento->infEvento->set_cUF(_valor.toInt());
                                    }
                                    _xml.readNext();
                                    //while infEvento
                                }
                            }
                            _xml.readNext();
                            //while retEvento
                        }
                    }
                    _xml.readNext();
                    //while procEventoNFe
                }
                this->retorno->procEventoNFe->add();
            }
            _xml.readNext();
            //while retConsSitNFe
        }
    }
    else
    {
        set_error("Erro: O XML retornado pelo web services não é válido.");
        //caso errado, salvar log do retorno em formato html
        salvarLogs(TipoArquivo::HTML,TipoMsgLog::MsgErro ,"",
                   WebServicesNF::NFeConsultaProtocolo, this->m_msgRetorno);
    }

    return _ret;
}
