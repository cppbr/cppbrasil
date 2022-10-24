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


#include "notafiscal.h"

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)

NotaFiscal::NotaFiscal(ConfigNFe *config): NFe(new Container<Nfe>),
    retorno(new RetConsReciNFe), m_tError(0), config(config)
{
}

NotaFiscal::~NotaFiscal()
{
}

void NotaFiscal::clear()
{
    this->m_tError = 0;
    this->NFe->clear();
    this->retorno->clear();
}

bool NotaFiscal::assinar()
{
    int _err = 0;
    for(int i = 0; i < this->NFe->items->count(); ++i)
    {
       this->NFe->items->value(i)->assinarXML(this->config);
       if (!this->NFe->items->value(i)->get_error().isEmpty())
            _err += 1;
       //caso esteja configurado para salvar, será salvo automático após assinar
       if (this->config->arquivos->get_salvar())
       {
            CppUtility::saveFile(this->config->get_caminhoNF(),
                                 QString(this->NFe->items->value(i)->get_chNFe() + "-nfe"),
                                 TipoArquivo::XML,
                                 this->NFe->items->value(i)->get_XMLAssinado().toLocal8Bit());
       }
    }

    if(_err > 0)
      set_error("Erro ao assinar " + QString::number(this->m_tError) + " nota(s).", _err);

    return (_err == 0);
}

bool NotaFiscal::gerar()
{
    int _err = 0;
    for(int i = 0; i < this->NFe->items->count(); ++i)
    {
       this->NFe->items->value(i)->gerarXML(this->config);
       if (!this->NFe->items->value(i)->get_error().isEmpty())
          _err += 1;
    }

    if(_err > 0)
      set_error("Erro ao gerar " + QString::number(this->m_tError) + " nota(s).", _err);

    return (_err == 0);
}

bool NotaFiscal::validar()
{
    int _err = 0;
    for(int i = 0; i < this->NFe->items->count(); ++i)
    {
       this->NFe->items->value(i)->validarXML(this->config);
       if (!this->NFe->items->value(i)->get_error().isEmpty())
          _err += 1;
    }

    if(_err > 0)
      set_error("Erro ao validar " + QString::number(this->m_tError) + " nota(s).", _err);

    return (_err == 0);
}

bool NotaFiscal::enviar(const int &numLote)
{
    QString _notas;

    for(int i = 0; i < this->NFe->items->count(); ++i)
    {
        _notas.append(CppUtility::extractStr(this->NFe->items->value(i)->get_XMLAssinado(),
                                             "<NFe xmlns", "</NFe>"));

    }

    WSNFe* _ws = new WSNFe(this->config,
                           this->config->certificado->get_cryptoLib(),
                           this->retorno.get());

    connect(_ws, SIGNAL(wsChange(WebServicesNF)), this, SIGNAL(wsChange(WebServicesNF)));
    connect(_ws, &WSNFe::errorOccurred, [=]( const QString &error ) {
        set_error(error, 1);
    } );

    bool _ret = _ws->send(numLote, _notas.toLocal8Bit(),
                          this->NFe->items->count(),
                          ConvNF::versaoNFToStr(this->config->get_VersaoNF()));

    if (_ret)
      tratarRetorno();

    delete _ws;
    return _ret;
}

bool NotaFiscal::gerarEnviar(const int &numLote)
{
    bool _ret = true;
    _ret = gerar();

    if (_ret)
      _ret = assinar();

    if (_ret)
      _ret = validar();

    if (_ret)
      _ret = enviar(numLote);

    return _ret;
}

bool NotaFiscal::loadFromFile(const QString &caminhoArquivo)
{
    QByteArray _xml;

    QFile _file(caminhoArquivo);
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

    while (!_file.atEnd())
    {
        _xml.append(_file.readLine());
    }
    _file.close();

    if (_xml.isEmpty())
        return false;
    else
        return importXML(_xml);
}

bool NotaFiscal::loadFromString(const QString &xmlString)
{
    QByteArray _xml(xmlString.toLocal8Bit());
    if (xmlString.isEmpty())
        return false;
    else
        return importXML(_xml);
}

int NotaFiscal::get_totalErros() const
{
    return this->m_tError;
}

QString NotaFiscal::get_error() const
{
    return this->m_error;
}

void NotaFiscal::tratarRetorno()
{
    if (retorno->protNFe->items->count() > 0)
    {
        QString _chNFe;
        QByteArray _nfeProc, _protNFe, _xmlOriginal, _schema;
        _schema.append(this->config->arquivos->get_caminhoSchema().toLocal8Bit());
        CppXML * _libxml = new CppLibXml2(this->config->certificado->get_cryptoLib(), _schema);

        for(int i = 0; i < this->retorno->protNFe->items->count(); ++i)
        {
            _xmlOriginal.clear();
            _chNFe.clear();
            _protNFe.clear();
            int _cStat = this->retorno->protNFe->items->value(i)->get_cStat();
            _chNFe = this->retorno->protNFe->items->value(i)->get_chNFe();

            for(int j = 0; j < this->NFe->items->count(); ++j)
            {
                if (this->NFe->items->value(j)->get_chNFe() == _chNFe)
                {
                    //montando xml com retorno
                    //100-uso autorizado, 150-uso autorizado fora de prazo
                    if (_cStat == 100 || _cStat == 150)
                    {
                        _xmlOriginal.append("<a>");
                        _xmlOriginal.append(CppUtility::extractStr(this->NFe->items->value(j)->get_XMLAssinado(),
                                                                   "<NFe xmlns", "</NFe>").toLocal8Bit());
                        _xmlOriginal.append("</a>");

                        //montando xml
                        _nfeProc = XmlWrite::get_nfeProc(this->retorno->protNFe->items->value(i)->get_versao());

                        _protNFe = XmlWrite::get_protNFe(this->retorno->protNFe->items->value(i)->get_versao(),
                                                         this->retorno->protNFe->items->value(i)->get_Id(),
                                                         this->retorno->protNFe->items->value(i)->get_tpAmb(),
                                                         this->retorno->protNFe->items->value(i)->get_verAplic(),
                                                         this->retorno->protNFe->items->value(i)->get_chNFe(),
                                                         this->retorno->protNFe->items->value(i)->get_dhRecbto(),
                                                         this->retorno->protNFe->items->value(i)->get_nProt(),
                                                         this->retorno->protNFe->items->value(i)->get_digVal(),
                                                         this->retorno->protNFe->items->value(i)->get_cStat(),
                                                         this->retorno->protNFe->items->value(i)->get_xMotivo(),
                                                         this->retorno->protNFe->items->value(i)->get_cMsg(),
                                                         this->retorno->protNFe->items->value(i)->get_xMsg());

                        //primeiro o xml original dentro de nfeProc
                        if (_libxml->addChildXML(_nfeProc, _xmlOriginal, QByteArray("nfeProc")))
                        {
                            //agora o protNFe abaixo do xml original
                            if (_libxml->addChildXML(_nfeProc, _protNFe, QByteArray("nfeProc")))
                            {
                                //atualiza o xml de protNFe da classe Retorno
                                this->retorno->protNFe->items->value(i)->set_xml(_nfeProc);
                                //atualiza xml de NFe
                                this->NFe->items->value(j)->set_XMLAutorizado(_nfeProc);
                                //caso esteja configurado para salvar
                                if (this->config->arquivos->get_salvar())
                                     this->retorno->protNFe->items->value(i)->salvarXML(this->config->get_caminhoNF(), "" );
                            }
                        }
                    }

                    //atualizando protNFe da classe NFe
                    this->NFe->items->value(j)->protNFe->set_versao(this->retorno->protNFe->items->value(i)->get_versao());
                    this->NFe->items->value(j)->protNFe->set_Id(this->retorno->protNFe->items->value(i)->get_Id());
                    this->NFe->items->value(j)->protNFe->set_tpAmb(this->retorno->protNFe->items->value(i)->get_tpAmb());
                    this->NFe->items->value(j)->protNFe->set_verAplic(this->retorno->protNFe->items->value(i)->get_verAplic());
                    this->NFe->items->value(j)->protNFe->set_chNFe(this->retorno->protNFe->items->value(i)->get_chNFe());
                    this->NFe->items->value(j)->protNFe->set_dhRecbto(this->retorno->protNFe->items->value(i)->get_dhRecbto());
                    this->NFe->items->value(j)->protNFe->set_nProt(this->retorno->protNFe->items->value(i)->get_nProt());
                    this->NFe->items->value(j)->protNFe->set_digVal(this->retorno->protNFe->items->value(i)->get_digVal());
                    this->NFe->items->value(j)->protNFe->set_cStat(this->retorno->protNFe->items->value(i)->get_cStat());
                    this->NFe->items->value(j)->protNFe->set_xMotivo(this->retorno->protNFe->items->value(i)->get_xMotivo());
                    this->NFe->items->value(j)->protNFe->set_cMsg(this->retorno->protNFe->items->value(i)->get_cMsg());
                    this->NFe->items->value(j)->protNFe->set_xMsg(this->retorno->protNFe->items->value(i)->get_xMsg());
                    this->NFe->items->value(j)->protNFe->set_xml(_nfeProc);

                    break;
                }
            }
        }

        delete _libxml;
    }
}

bool NotaFiscal::importXML(const QByteArray &xml)
{
    Nfe *_nfe = new Nfe;
    XmlRead *_xRead = new XmlRead(_nfe, xml);
    bool _ret = _xRead->import();

    if (_ret)
        NFe->add(_nfe);
    else
        delete _nfe; //caso a importação falhe, deve ser excluido o ponteiro _nfe já que não será inserido no container.

    delete _xRead;
    return _ret;
}

void NotaFiscal::set_error(const QString &error, const int &tError)
{
    this->m_error = error;

    if (tError > 0)
      this->m_tError = tError;

    emit errorOccurred(error);
}
