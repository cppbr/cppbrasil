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


#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QSslKey>
#include <QEventLoop>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppcrypto.h>

enum class HttpType{None = -1, HttpQt, HttpOpenSSL};

class CPPNFE_EXPORT HttpClient : public QObject

{
    Q_OBJECT

public:
    HttpClient();
    virtual ~HttpClient();

    QByteArray get_lastData() const;
    virtual QByteArray send(const QByteArray &host, const QByteArray &data);

    void set_crypto(const CppCrypto* crypto);
    void set_proxy(const QNetworkProxy &proxy);
    void set_protocolo(const QSsl::SslProtocol &protocol);
    void set_verificarSslSocket(const bool &verifyMode);

signals:
    void errorOccurred(const QString &error);

protected:
    HttpClient(const CppCrypto* crypto);
    HttpClient(const CppCrypto* crypto, const QNetworkProxy &proxy,
               const QSsl::SslProtocol &protocol, const int &timeout = 0,
               const bool &verifyMode = true);
    QByteArray m_data;
    const CppCrypto *m_crypto;
    QNetworkProxy m_proxy;
    QSsl::SslProtocol m_protocol;
    int m_timeout;
    bool m_verifyMode;

};

#endif // HTTPCLIENT_H
