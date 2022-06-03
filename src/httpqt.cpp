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


#include "httpqt.h"

HttpQt::HttpQt(const CppCrypto* crypto, const QNetworkProxy &proxy,
               const QSsl::SslProtocol &protocol, const int &timeout,
               const bool &verifyMode) :
  HttpClient(crypto, proxy, protocol, timeout, verifyMode)
{
}

HttpQt::~HttpQt()
{
}

QByteArray HttpQt::send(const QByteArray &host, const QByteArray &data,
                        const int &lenReturn)
{
    Q_UNUSED(lenReturn);
    //default 15 segundos
    int _timeout = (this->m_timeout > 0) ? this->m_timeout : 15000;
    QNetworkRequest *_request = new QNetworkRequest(QUrl(host));
    configSSL(_request);
    _request->setTransferTimeout(_timeout);
    _request->setHeader(QNetworkRequest::ContentTypeHeader, "text/xml;charst=utf-8");

    QNetworkAccessManager *_manager = new QNetworkAccessManager(this);
    configProxy(_manager);
    _manager->setTransferTimeout(_timeout);
    connect(_manager, &QNetworkAccessManager::sslErrors, this, &HttpQt::sslErrors);
    connect(_manager, &QNetworkAccessManager::finished, this, &HttpQt::finished);

    QEventLoop _loop;
    QNetworkReply* _reply = _manager->post(*_request,data);
    connect(_reply, &QNetworkReply::finished, this, &HttpQt::replyfinished);
    connect(_reply, &QNetworkReply::finished, &_loop, &QEventLoop::quit);
    _loop.exec();

    _manager->clearAccessCache();
    _manager->clearConnectionCache();

    delete _request;
    delete _manager;
    return this->m_data;
}

void HttpQt::configSSL(QNetworkRequest *request)
{
    QSslKey _qKey = QSslKey(this->m_crypto->get_pkeyDER(), QSsl::Rsa, QSsl::Der);

    QSslCertificate _qCert;
    _qCert = QSslCertificate(this->m_crypto->get_certDER(), QSsl::Der);

    QList<QSslCertificate> _qCaCerts;
    //QList<QByteArray> caList(m_crypto->get_caDER()); cria uma cópia, optei por pegar o endereço
    foreach (const QByteArray &item, this->m_crypto->get_caDER()) {
        _qCaCerts << QSslCertificate(item, QSsl::Der);
    }
    QSslConfiguration _sslConfig = QSslConfiguration::defaultConfiguration();
    _sslConfig.setLocalCertificate(_qCert);
    _sslConfig.setPrivateKey(_qKey);
    _sslConfig.setCaCertificates(_qCaCerts);
    //pegar configuração do protocolo
    _sslConfig.setProtocol(this->m_protocol);
    //verifica todos os certificados do servidor. Por padrão está QSslSocket::AutoVerifyPeer,
    //caso deseje que não verifique nada será QSslSocket::VerifyNone
    if (!this->m_verifyMode)
        _sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    //configura request
    request->setSslConfiguration(_sslConfig);
}

void HttpQt::configProxy(QNetworkAccessManager *manager)
{
    //config proxy
    if ((!this->m_proxy.hostName().isEmpty()) ||
        (this->m_proxy.port() != 0) ||
        (!this->m_proxy.user().isEmpty()) ||
        (!this->m_proxy.password().isEmpty()) )
    {
        manager->setProxy(this->m_proxy);
    }
}

void HttpQt::replyfinished()
{
    this->m_data.clear();
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply)
       this->m_data.append(reply->readAll());
}


void HttpQt::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString _err;
    foreach ( QSslError e , errors )
    {
        _err.append(e.errorString());
    }
    qWarning() << "Error: "<< _err;

    emit errorOccurred(_err);

    reply->ignoreSslErrors();
}

void HttpQt::finished(QNetworkReply *reply)
{
    // after all of your processing
    reply->deleteLater();
}
