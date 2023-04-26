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


#ifndef HTTPOPENSSL_H
#define HTTPOPENSSL_H

#include <QObject>
#include <QDebug>
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#elif _WIN32
#ifndef WIN32_LEAN_AND_MEAN
 #define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif
#include <openssl/bio.h>
#include <openssl/asn1.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/httpclient.h>
#include <CppBrasil/cppcrypto.h>
#include <CppBrasil/cpputility.h>

class CPPNFE_EXPORT HttpOpenSSL : public HttpClient
{
    Q_OBJECT
public:
    HttpOpenSSL(const CppCrypto* crypto);
    ~HttpOpenSSL();

    virtual QByteArray send(const QByteArray &host, const QByteArray &data);

private:
    enum THost {host = 0, server = 1};
    EVP_PKEY *m_pkey = NULL;
    X509 *m_cert = NULL;
    STACK_OF(X509) *m_ca = NULL;
    void sendPacket(SSL *ssl, const char *buf);
    void receivePacket(SSL *ssl);
    QByteArray getCommand(const QByteArray &host, const QByteArray &data);
    QByteArray getHostName(const QByteArray &host, const THost &tipo);
    QString getErrorName(const int &err);
};

#endif // HTTPOPENSSL_H
