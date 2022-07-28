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
    m_confgNFe(confgNFe), m_crypto(crypto)
{
}

WSNFeBase::~WSNFeBase()
{
}

QByteArray WSNFeBase::get_mensagemRetorno() const
{
    return this->m_msgRetorno;

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
    QString _versao = ConvNF::versaoNFToStr(m_confgNFe->get_VersaoNF());
    _versao = (_versao.isEmpty() ? "4" : QString(_versao.at(0)));
    QString _res = get_urlNamespaceBase() +
                   QStringLiteral("/wsdl/") +
                   ConvNF::webServicesNFToMetodo(webServicesNF) + _versao;
    return _res;
}

QString WSNFeBase::get_urlServidorWS(const WebServicesNF &webServicesNF) const
{
    QString _url = UrlNFe::get_urlServidor(m_confgNFe->webservices->get_uf(),
                                           m_confgNFe->webservices->get_tpAmb(),
                                           m_confgNFe->get_TipoEmissao(),
                                           m_confgNFe->get_ModeloDF(),
                                           m_confgNFe->get_VersaoNF(),
                                           webServicesNF);
    return _url;

}

QString WSNFeBase::get_versaoSchema(const QString &schemaName)
{
    QString _versao = ConvNF::versaoNFToStr(this->m_confgNFe->get_VersaoNF());
    QString _schema = schemaName + "_v" + _versao + ".xsd";
    return _schema;
}

bool WSNFeBase::validarXML(const QString &schemaName, const QString &nomeGrupo, const QByteArray &xml)
{
    QByteArray _schemaPath(this->m_confgNFe->arquivos->get_caminhoSchema().toLocal8Bit());
    bool _ret = true;
    CppXML * _libxml = new CppLibXml2(this->m_crypto, _schemaPath);

    if (nomeGrupo.isEmpty())
    {
         _ret = _libxml->parseXML(xml, schemaName.toLocal8Bit());
    } else
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

    if (m_confgNFe->arquivos->get_salvarLogs())
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
            CppUtility::saveFile(m_confgNFe->get_caminhoLogs(), _file, tipoArquivo, dadosLog);
        } else
        {
            QString _xml;
            _xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            _xml.append(CppUtility::extractStr(dadosLog, "<" + nomeGrupo, nomeGrupo + ">"));
            CppUtility::saveFile(m_confgNFe->get_caminhoLogs(), _file, tipoArquivo, _xml.toLocal8Bit());
        }
    }
}

//WSNFe---------------------------------------------------

WSNFe::WSNFe(ConfigNFe* confgNFe, CppCrypto* crypto, RetConsReciNFe* retorno):
    WSNFeBase(confgNFe, crypto), m_retorno(retorno)
{
}

WSNFe::~WSNFe()
{
}

bool WSNFe::send(const int &numLote, const QByteArray &xml, const int &totDoc)
{
    //tamanho do BUF para armazenamento de retorno. somente para OpenSSL
    int _lenRet = 0, _retEnviNFe = 570, _retConsReciNFe = 780, _protNFe = 850;
    StatusRetorno _ret = StatusRetorno::EmProcessamento;
    QString _url = get_urlServidorWS(WebServicesNF::NFeAutorizacao);
    //se não for informado quantas tentativas, o padrão será ao menos 2
    int _tentativas = this->m_confgNFe->webservices->get_tentativas();
    if (_tentativas <= 0)
      _tentativas = 2;
    //se não for informado o timeout, o padrão será 8000 milisegundos (8 segundos)
    int _timeout = this->m_confgNFe->webservices->get_timeoutAssincrono();
    if (_timeout <= 0)
      _timeout = 8000;

    //gera xml do lote na var m_msgEnvio
    if (gerarXMLlote(numLote, xml))
    {
        //enviando mensagem
        _lenRet = _retEnviNFe + _protNFe; //_protNFe caso sincrono
        emit wsChange(WebServicesNF::NFeAutorizacao);
        this->m_msgRetorno.clear();
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio, _lenRet);
        //se não houver erro
        if (this->get_error().isEmpty())
        {
            //caso seja sincrono, apenas trata o retorno já que o lote vem junto
            if (this->m_confgNFe->get_indicadorSincrono() == IndSinc::Sim)
            {
                _ret = tratarRetorno(TipoRetorno::retEnviNFe);
            } else
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
                                _lenRet = _retConsReciNFe + (_protNFe * totDoc);
                                emit wsChange(WebServicesNF::NFeRetAutorizacao);
                                this->m_msgRetorno.clear();
                                this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio, _lenRet);
                                //se não houver erro
                                if (this->get_error().isEmpty())
                                {
                                    _ret = tratarRetorno(TipoRetorno::retConsReciNFe);
                                    //se retornar algo diferente de EmProcessamento, deverá parar, já que foi processado
                                    if (_ret != StatusRetorno::EmProcessamento)
                                      break;
                                }else
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
        } else
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
    QString _versao = ConvNF::versaoNFToStr(this->m_confgNFe->get_VersaoNF());
    QString _indSinc = ConvNF::indSincToStr(this->m_confgNFe->get_indicadorSincrono());
    QString _env;
    _env += QStringLiteral("<enviNFe xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _env += QStringLiteral("<idLote>") + QString::number(numLote) +  QStringLiteral("</idLote>");
    _env += QStringLiteral("<indSinc>") + _indSinc +  QStringLiteral("</indSinc>");
    _env += QString(xml);
    _env += QStringLiteral("</enviNFe>");

    //nome do schema
    QString _schemaName = get_versaoSchema("enviNFe");
    //validar xml com schema
    bool _ret = validarXML(_schemaName, "", _env.toLocal8Bit());
    if (_ret)
    {
        //se deseja compactar
        if (this->m_confgNFe->webservices->get_compactar())
        {
            int _size = this->m_confgNFe->webservices->get_compactarAcimaDe();
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
    } else
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
    QString _versao = ConvNF::versaoNFToStr(this->m_confgNFe->get_VersaoNF());
    QString _tpAmb = ConvNF::tpAmbToStr(this->m_confgNFe->webservices->get_tpAmb());
    QString _nRec = this->m_retorno->get_nRec();
    QString _rec;
    _rec += QStringLiteral("<consReciNFe xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _rec += QStringLiteral("<tpAmb>") + _tpAmb +  QStringLiteral("</tpAmb>");
    _rec += QStringLiteral("<nRec>") + _nRec +  QStringLiteral("</nRec>");
    _rec += QStringLiteral("</consReciNFe>");

    //nome do schema
    QString _schemaName = get_versaoSchema("consReciNFe");
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
    } else
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
    QString _schemaName = get_versaoSchema(_parent);
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
                    this->m_retorno->set_versao(_valor);
            }

            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("nRec"))            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_nRec(_valor);
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_cStat(_valor.toInt());

                if (_valor.toInt() == 105) //Lote em processamento
                    _ret = StatusRetorno::EmProcessamento;
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("cUF"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_cUF(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRecbto"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->m_retorno->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("cMsg"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                    this->m_retorno->set_cMsg(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMsg"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->m_retorno->set_xMsg(_valor);
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
                          this->m_retorno->set_nRec(_valor);
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
                            this->m_retorno->protNFe->obj->set_versao(_valor);
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
                                    this->m_retorno->protNFe->obj->set_Id(_valor);
                            }

                            if(_xml.name() == QStringLiteral("tpAmb"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_tpAmb(ConvNF::strToTpAmb(_valor));
                            }
                            if(_xml.name() == QStringLiteral("verAplic"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_verAplic(_valor);
                            }
                            if(_xml.name() == QStringLiteral("chNFe"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_chNFe(_valor);
                            }
                            if(_xml.name() == QStringLiteral("dhRecbto"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                   this->m_retorno->protNFe->obj->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                            }
                            if(_xml.name() == QStringLiteral("nProt"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                  this->m_retorno->protNFe->obj->set_nProt(_valor);
                            }
                            if(_xml.name() == QStringLiteral("digVal"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_digVal(_valor);
                            }
                            if(_xml.name() == QStringLiteral("cStat"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_cStat(_valor.toInt());
                            }
                            if(_xml.name() == QStringLiteral("xMotivo"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_xMotivo(_valor);
                            }
                            //Sequência XML - PR13 - dentro de PR03
                            if(_xml.name() == QStringLiteral("cMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_cMsg(_valor.toInt());

                            }
                            if(_xml.name() == QStringLiteral("xMsg"))
                            {
                                _valor = _xml.readElementText();
                                if (!_valor.isEmpty())
                                    this->m_retorno->protNFe->obj->set_xMsg(_valor);
                            }

                            _xml.readNext();
                        }
                    }

                    _xml.readNext();
                }

                this->m_retorno->protNFe->add();
            }

            _xml.readNext();
        }


    } else
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
  WSNFeBase(confgNFe, crypto), m_retorno(retorno)
{
}

WSStatus::~WSStatus()
{
}

bool WSStatus::send()
{
    int _lenRet = 651;
    StatusRetorno _ret;
    if (gerarXML())
    {
        QString _url = get_urlServidorWS(WebServicesNF::NFeStatusServico);
        emit wsChange(WebServicesNF::NFeStatusServico);
        this->m_msgRetorno = sendMsg(_url.toLocal8Bit(), this->m_msgEnvio, _lenRet);

        //se não houve erro no envio, trata o retorno
        if (this->get_error().isEmpty())
            _ret = tratarRetorno();
        else
            _ret = StatusRetorno::Erro;

    }else //se houve erro ao gerar xml
       _ret = StatusRetorno::Erro;

    emit wsChange(WebServicesNF::None);
    return (_ret == StatusRetorno::Erro) ? false : true;
}

bool WSStatus::gerarXML()
{
    //envelop soap
    QString _urlBodyElement = get_urlNamespaceWS(WebServicesNF::NFeStatusServico);
    QString _versao = ConvNF::versaoNFToStr(this->m_confgNFe->get_VersaoNF());
    QString _tpAmb = ConvNF::tpAmbToStr(this->m_confgNFe->webservices->get_tpAmb());
    int _cUF = ConvNF::ufToInt(this->m_confgNFe->webservices->get_uf());
    QString _env;
    _env += QStringLiteral("<consStatServ xmlns=\"") + get_urlNamespaceBase() + QStringLiteral("\" versao=\"") + _versao + QStringLiteral("\">");
    _env += QStringLiteral("<tpAmb>") + _tpAmb +  QStringLiteral("</tpAmb>");
    _env += QStringLiteral("<cUF>") + QString::number(_cUF) +  QStringLiteral("</cUF>");
    _env += QStringLiteral("<xServ>STATUS</xServ>");
    _env += QStringLiteral("</consStatServ>");

    //validar o _env
    QString _schemaName = get_versaoSchema("consStatServ");
    bool _ret = validarXML(_schemaName, "", _env.toLocal8Bit());
    if (_ret)
    {
        this->m_msgEnvio.clear();
        _ret = get_soapEnvelop(_env.toLocal8Bit(), get_namespaceBase().toLocal8Bit(),
                                    _urlBodyElement.toLocal8Bit(), this->m_msgEnvio);
        //salva log
        salvarLogs(TipoArquivo::XML,TipoMsgLog::MsgEnvio ,"consStatServ",
                   WebServicesNF::NFeStatusServico, this->m_msgEnvio);
    } else
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
    QString _schemaName = get_versaoSchema("retConsStatServ");
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
                    this->m_retorno->set_versao(_valor);
            }

            if(_xml.name() == QStringLiteral("tpAmb"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_tpAmb(ConvNF::strToTpAmb(_valor));
            }
            if(_xml.name() == QStringLiteral("verAplic"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_verAplic(_valor);
            }
            if(_xml.name() == QStringLiteral("cStat"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_cStat(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("xMotivo"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_xMotivo(_valor);
            }
            if(_xml.name() == QStringLiteral("cUF"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_cUF(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRecbto"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->m_retorno->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("tMed"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_tMed(_valor.toInt());
            }
            if(_xml.name() == QStringLiteral("dhRetorno"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                  this->m_retorno->set_dhRetorno(QDateTime::fromString(_valor, Qt::ISODate));
            }
            if(_xml.name() == QStringLiteral("xObs"))
            {
                _valor = _xml.readElementText();
                if (!_valor.isEmpty())
                   this->m_retorno->set_xObs(_valor);
            }

            _xml.readNext();
        }


    } else
    {
        set_error("Erro: O XML retornado pelo web services não é válido.");
        //caso errado, salvar log do retorno em formato html
        salvarLogs(TipoArquivo::HTML,TipoMsgLog::MsgErro ,"",
                   WebServicesNF::NFeStatusServico, this->m_msgRetorno);

    }

    return _ret;
}

