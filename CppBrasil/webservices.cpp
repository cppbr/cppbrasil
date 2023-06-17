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


#include "webservices.h"

WebServices::WebServices(const CppCrypto* crypto, const QNetworkProxy &proxy,
                        const QSsl::SslProtocol &protocol, const HttpType &httpType,
                        const int &timeout, const bool &verifyMode):
    m_crypto(crypto), m_proxy(proxy), m_protocol(protocol), m_httpType(httpType),
    m_timeout(timeout), m_verifyMode(verifyMode)
{
}

WebServices::~WebServices()
{
}

QString WebServices::get_error() const
{
    return this->m_error;
}

bool WebServices::timeout(const int &time)
{
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(time);
    loop.exec();

    if(timer.isActive())
    {
        timer.stop();
    }

    return true;
}

void WebServices::set_error(const QString &error)
{
    this->m_error = error;
    emit errorOccurred(error);
}

bool WebServices::get_soapEnvelop(const QByteArray &dados, const QByteArray &NameSpace,
                                  const QByteArray &urlNamespace, QByteArray &output)
{
    QString _env;
    try {
        _env += QStringLiteral("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
        _env += QStringLiteral("<soap12:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");
        _env += QStringLiteral(" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"");
        _env += QStringLiteral(" xmlns:soap12=\"http://www.w3.org/2003/05/soap-envelope\">");
        _env += QStringLiteral("<soap12:Body>");
        _env += QStringLiteral("<") + QString(NameSpace) + QStringLiteral(" xmlns=\"") + QString(urlNamespace) + QStringLiteral("\">");
        _env += QString(dados);
        _env += QStringLiteral("</") + QString(NameSpace) + QStringLiteral(">");
        _env += QStringLiteral("</soap12:Body>");
        _env += QStringLiteral("</soap12:Envelope>");

        output.append(_env.toLocal8Bit());
        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        output.clear();
        return false;
    }
}

QByteArray WebServices::sendMsg(const QByteArray &host, const QByteArray &data)
{
    this->m_error.clear();
    QByteArray _output;
    HttpClient *_http;
    if ((this->m_httpType == HttpType::None) || (this->m_httpType == HttpType::HttpQt))
    { //qt
        _http = new HttpQt(m_crypto, m_proxy, m_protocol, m_timeout, m_verifyMode);
    }
    else
    {// openssl
        _http = new HttpOpenSSL(m_crypto);
    }

    connect(_http, &HttpClient::errorOccurred, this, &WebServices::onError);
    _output.append(_http->send(host, data));

    delete _http;
    return _output;
}

void WebServices::onError(const QString &error)
{
    this->m_error = error;
    emit errorOccurred(error);
}
