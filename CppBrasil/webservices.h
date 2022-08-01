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

#ifndef WEBSERVICES_H
#define WEBSERVICES_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/httpclient.h>
#include <CppBrasil/httpqt.h>
#include <CppBrasil/httpopenssl.h>
#include <CppBrasil/cppcrypto.h>

class CPPNFE_EXPORT WebServices : public QObject
{
    Q_OBJECT

public:
    WebServices(const CppCrypto* crypto, const QNetworkProxy &proxy,
                const QSsl::SslProtocol &protocol, const HttpType &httpType,
                const int &timeout,const bool &verifyMode);
    ~WebServices();
    QString get_error() const;

protected:
    bool timeout(const int &time);
    void set_error(const QString &error);
    bool get_soapEnvelop(const QByteArray &data, const QByteArray &NameSpace,
                               const QByteArray &urlNamespace, QByteArray &output);

    QByteArray sendMsg(const QByteArray &host, const QByteArray &data, const int &lenReturn = 0);

signals:
    void errorOccurred(const QString &error);

private slots:
    void onError(const QString &error);

private:
    QString m_error;

    const CppCrypto *m_crypto;
    QNetworkProxy m_proxy;
    QSsl::SslProtocol m_protocol;
    HttpType m_httpType;
    int m_timeout;
    bool m_verifyMode;

};

#endif // WEBSERVICES_H
