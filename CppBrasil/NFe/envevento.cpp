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

bool EnvEvento::enviarEvento(const int &idLote, const bool &salvarNotaVinculada)
{
//salvarNotaVinculada serve para salvar notas onde a nota em questão foi emitida pelo próprio
//transmissor do evento.
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

            connect(_ws, &WSEvento::wsChange, this, &EnvEvento::wsChange);
            connect(_ws, &WSEvento::errorOccurred, this,
                    [=] (const QString &error)  {
                this->m_error = error;
            });


            bool _ret = _ws->send(this->m_xml.toLocal8Bit(),
                                  ConvNF::versaoLayout(WebServicesNF::NFeRecepcaoEvento));

            if (_ret)
              tratarRetorno(salvarNotaVinculada);

            delete _ws;
        }
        else
        {
            emit errorOccurred(this->m_error);
            _ret = false;
        }
    }
    else
    {
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

QString EnvEvento::get_error() const
{
    return this->m_error;
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
    }
    else
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

    //versão - Caso não seja informado, será 1.00 que é a versão do momento.
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
        }
        else
        {
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


void EnvEvento::updateEvento(int &index)
{
    //versão
    evento->items->value(index)->set_versao(this->m_versao);
    //nSeqEvento deve ser tratado pelo software do usuário e não pela lib, portanto,
    //se não for informado ocorrerá erro na valização.
    //caso seja maior que 0 e menor que 10, será acrescido zero a esquerda.
    //nSeqEvento deve ter dois algarismos, para não ocorrer erro no ID.
    QString _strSeq;
    if ((evento->items->value(index)->infEvento->get_nSeqEvento() > 0) &&
        (evento->items->value(index)->infEvento->get_nSeqEvento() < 10))
    {
        _strSeq = CppUtil::insZeroLeft(QString::number(evento->items->value(index)->infEvento->get_nSeqEvento()), 2);
    }
    else
    {
        _strSeq = QString::number(evento->items->value(index)->infEvento->get_nSeqEvento());
    }
    QString _id = "ID" + ConvNF::tpEventoToStr(evento->items->value(index)->infEvento->get_tpEvento())
                 + evento->items->value(index)->infEvento->get_chNFe()
                 + _strSeq;

    evento->items->value(index)->infEvento->set_Id(_id);

    //se não informou o ambiente, será o padrão de configurações
    if (evento->items->value(index)->infEvento->get_tpAmb() == TpAmb::None)
        evento->items->value(index)->infEvento->set_tpAmb(config->webservices->get_tpAmb());
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

void EnvEvento::tratarRetorno(const bool &salvarNotaVinculada)
{
    //atualiza e salva o xml do evento
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
                        _xmlOriginal.append(CppUtility::extractStr(this->evento->items->value(j)->get_XMLAssinado().toLocal8Bit(), "<evento", "/evento>"));
                        _xmlOriginal.append("</a>");

                        //xml do evento é inserido ao retornar do webservices.
                        _retEvento.append("<a>");
                        _retEvento.append(CppUtility::extractStr(this->retorno->retEvento->items->value(i)->get_XML().toLocal8Bit(), "<retEvento", "/retEvento>"));
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
                                    QString _nomeArqEvento =  evento->items->value(j)->infEvento->get_Id().mid(3, evento->items->value(j)->infEvento->get_Id().length() - 3) +
                                                             "-procEventoNFe";
                                    this->retorno->retEvento->items->value(i)->salvarXML(this->config->arquivos->get_caminhoEvento(), _nomeArqEvento);
                                }
                            }
                        }

                        //cria novo xml da nota com o evento enviado
                        if (salvarNotaVinculada && this->notafiscal->NFe->items->count() > 0)
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
                                        QByteArray _xmlNota = "<DFe></DFe>";
                                        QByteArray _nfeProc;
                                        _nfeProc.append("<a>");
                                        _nfeProc.append(CppUtility::extractStr(this->notafiscal->NFe->items->value(m)->get_XMLAutorizado().toLocal8Bit(),
                                                                               "<nfeProc", "/nfeProc>"));
                                        _nfeProc.append("</a>");

                                        if (_libxml->addChildXML(_xmlNota, _nfeProc, QByteArray("DFe")))
                                        {
                                            QByteArray _evento;
                                            _evento.append("<a>");
                                            _evento.append("<procEventoNFe>"); //criando mais um grupo para evitar o erro no procEcentoNFe
                                            _evento.append(CppUtility::extractStr(_procEventoNFe, "<procEventoNFe", "procEventoNFe>"));
                                            _evento.append("</procEventoNFe>");
                                            _evento.append("</a>");

                                            if (_libxml->addChildXML(_xmlNota, _evento, QByteArray("DFe")))
                                            {
                                                this->notafiscal->NFe->items->value(m)->set_XMLAutorizado(_xmlNota);
                                                QString _nomeArqNota = this->notafiscal->NFe->items->value(m)->get_chNFe() + "-nfe-" +
                                                                       nomeEvento(evento->items->value(j)->infEvento->get_tpEvento()) + "-" +
                                                                       CppUtil::insZeroLeft(QString::number(evento->items->value(j)->infEvento->get_nSeqEvento()), 2);

                                                CppUtility::saveFile(this->config->arquivos->get_caminhoNF(),
                                                                     _nomeArqNota,
                                                                     TipoArquivo::XML,
                                                                     _xmlNota);
                                            }
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

QString EnvEvento::nomeEvento(const TpEvento &evento)
{
//retorna o nome do evento de forma abreviada, diferente da função ConvNF::tpEventoToStr que retorna o código do evento em string
    QString _strEvento;
    switch (evento) {
    case TpEvento::AtorInteressadoNFeTransp :
        _strEvento = "AtorInter";
        break;
    case TpEvento::Cancelamento :
        _strEvento =  "CancNFe";
        break;
    case TpEvento::CancelamentoSubstituicao :
        _strEvento = "CancSubst";
        break;
    case TpEvento::CancelPedidoProrrogacao1 :
        _strEvento = "CancPedProrroga";
        break;
    case TpEvento::CancelPedidoProrrogacao2 :
        _strEvento = "CancPedProrroga";
        break;
    case TpEvento::CartaCorrecao :
        _strEvento = "CCe";
        break;
    case TpEvento::CienciaOperacaoDest :
        _strEvento = "CienciaOpera";
        break;
    case TpEvento::ConfirmacaoOperacaoDest :
        _strEvento = "ConfirmaOpera";
        break;
    case TpEvento::DesconhecimentoOperacaoDest :
        _strEvento = "DesconheciOp";
        break;
    case TpEvento::EPEC :
        _strEvento = "EPEC";
        break;
    case TpEvento::OperacaoNaoRealizadaDest :
        _strEvento = "OperaNaoRealizada";
        break;
    case TpEvento::PedidoProrrogacao1 :
        _strEvento = "PedidoPro";
        break;
    case TpEvento::PedidoProrrogacao2 :
        _strEvento = "PedidoPro";
        break;
    case TpEvento::ComprovanteEntregaNFe :
        _strEvento = "EntregaNFe";
        break;
    case TpEvento::CancelComprovanteEntregaNFe :
        _strEvento = "CancEntregaNFe";
        break;
    default: _strEvento = "";
    }

    return _strEvento;
}

