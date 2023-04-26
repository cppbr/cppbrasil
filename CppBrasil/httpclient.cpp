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


#include "httpclient.h"

HttpClient::HttpClient(): QObject(nullptr)
{
}

HttpClient::~HttpClient()
{
}

QByteArray HttpClient::get_lastData() const
{
    return this->m_data;
}

QByteArray HttpClient::send(const QByteArray &host, const QByteArray &data)
{
    emit errorOccurred("Uninitialized");
    Q_UNUSED(host);
    Q_UNUSED(data);
    return QByteArray();
}

void HttpClient::set_crypto(const CppCrypto *crypto)
{
    this->m_crypto = crypto;
}

void HttpClient::set_proxy(const QNetworkProxy &proxy)
{
    this->m_proxy = proxy;
}

void HttpClient::set_protocolo(const QSsl::SslProtocol &protocol)
{
    this->m_protocol = protocol;
}

void HttpClient::set_verificarSslSocket(const bool &verifyMode)
{
    this->m_verifyMode = verifyMode;
}

HttpClient::HttpClient(const CppCrypto *crypto): m_crypto(crypto)
{

}

HttpClient::HttpClient(const CppCrypto *crypto, const QNetworkProxy &proxy,
                       const QSsl::SslProtocol &protocol, const int &timeout,
                       const bool &verifyMode):
    m_crypto(crypto), m_proxy(proxy), m_protocol(protocol), m_timeout(timeout), m_verifyMode(verifyMode)

{

}

