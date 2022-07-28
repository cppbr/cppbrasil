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


#ifndef HTTPQT_H
#define HTTPQT_H

#include <QObject>
#include <QDebug>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/httpclient.h>

class CPPBRASIL_EXPORT HttpQt : public HttpClient
{
    Q_OBJECT
public:
    HttpQt(const CppCrypto* crypto, const QNetworkProxy &proxy,
           const QSsl::SslProtocol &protocol, const int &timeout = 0,
           const bool &verifyMode = true);
    ~HttpQt();

    virtual QByteArray send(const QByteArray &host, const QByteArray &data,
                            const int &lenReturn = 0);

private:
    void configSSL(QNetworkRequest *request);
    void configProxy(QNetworkAccessManager *m_manager);

private slots:
    void replyfinished();
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void finished(QNetworkReply *reply);
};

#endif // HTTPQT_H
