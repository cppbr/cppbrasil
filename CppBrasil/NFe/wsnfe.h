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


#ifndef WSNFE_H
#define WSNFE_H

#include <QObject>
#include <QXmlStreamReader>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/webservices.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/cppzip.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/retornonfe.h>
#include <CppBrasil/NFe/retconsstatserv.h>
#include <CppBrasil/NFe/urlnfe.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT WSNFeBase : public WebServices
{
    Q_OBJECT
public:
    WSNFeBase(ConfigNFe* confgNFe, CppCrypto* crypto);
    virtual ~WSNFeBase();
    QByteArray get_mensagemRetorno() const;

protected:
    QByteArray m_msgEnvio;
    QByteArray m_msgRetorno;
    ConfigNFe* m_confgNFe;
    CppCrypto* m_crypto;
    enum StatusRetorno{Erro = -1, EmProcessamento, Processado};
    QString get_namespaceBase() const;
    QString get_urlNamespaceBase() const;
    QString get_urlNamespaceWS(const WebServicesNF &webServicesNF) const;
    QString get_urlServidorWS(const WebServicesNF &webServicesNF) const;
    QString get_versaoSchema(const QString &schemaName);
    bool validarXML(const QString &schemaName, const QString &nomeGrupo, const QByteArray &xml);
    void salvarLogs(const TipoArquivo &tipolog, const TipoMsgLog &msglog,
                    const QString &nomeGrupo, const WebServicesNF &webServicesNF,
                    const QByteArray &dadosLog);
signals:
    void wsChange(const WebServicesNF &webServicesNF);

private:
    QString m_nameSpaceBase = "nfeDadosMsg";
    QString m_urlNameSpaceBase = "http://www.portalfiscal.inf.br/nfe";
};


class CPPNFE_EXPORT WSNFe : public WSNFeBase
{
public:
    WSNFe(ConfigNFe* confgNFe, CppCrypto* crypto, RetConsReciNFe* retorno);
    ~WSNFe();

    bool send(const int &numLote, const QByteArray &xml, const int &totDoc);

private:
    enum TipoRetorno {retEnviNFe, retConsReciNFe};
    RetConsReciNFe* m_retorno;
    bool gerarXMLlote(const int &numLote, const QByteArray &xml);
    bool gerarXMLrecibo();
    StatusRetorno tratarRetorno(const TipoRetorno &tipo);
    //função para testes, pode ser excluida
    /*
    void openFile(const QString &path){
        this->m_msgRetorno.clear();
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

        while (!file.atEnd())
        {
            this->m_msgRetorno.append(file.readLine());
        }
        qInfo() << m_msgRetorno;
        file.close();
    }
    */
};


class CPPNFE_EXPORT WSStatus : public WSNFeBase
{
public:
    WSStatus(ConfigNFe* confgNFe, CppCrypto* crypto, RetConsStatServ* retorno);
    ~WSStatus();

   bool send();

private:
    RetConsStatServ* m_retorno;
    bool gerarXML();
    StatusRetorno tratarRetorno();
};

#endif // WSNFE_H
