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


#ifndef CPPOPENSSL_H
#define CPPOPENSSL_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
 #define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <stdio.h>
#include <windows.h>
#endif
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>

#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppcrypto.h>

class CPPNFE_EXPORT CppOpenSSL: public CppCrypto
{
public:
    CppOpenSSL(const QByteArray &p12path, const QByteArray &pass);
    ~CppOpenSSL();
    //same digest function but static
    static bool digest_s(const HashType &hashtype, const QByteArray &input,
                         const bool returnBase64, QByteArray &output);

    //CppCrypto interface
    virtual bool insertCAcert(const QString &certPath);
    virtual bool digest(const HashType &hashtype, const QByteArray &input,
                        const bool returnBase64, QByteArray &output);
    virtual bool signature(const HashType &hashtype, const QByteArray &input,
                           const bool returnBase64, QByteArray &output);
    virtual bool load();
    virtual bool unload();

private:
    QByteArray m_p12path;
    QByteArray m_pass;
    EVP_PKEY *m_pkey = NULL;
    X509 *m_cert = NULL;
    STACK_OF(X509) *m_ca = NULL;

    bool insertCA(const char* certPath);
    QDateTime asn1_to_QDateTime(const ASN1_TIME *aTime);
    bool certToBase64(X509 *cert);
    bool issuerName(X509 *cert);
    bool serialNumber(X509 *cert);
    bool subjectName(X509 *cert);
    bool dataEmissao(X509 *cert);
    bool dataVencimento(X509 *cert);
    bool certificadora();
    bool razaoSocial();
    bool cnpj();
    static bool hash(const HashType &hashtype, const QByteArray &input, EVP_PKEY **pKey,
                     const bool tosign, const bool base64, QByteArray &output);
    QByteArray x509ToQByteArray(X509 *cert);
    bool certToDER();

    bool loadOpenssl();
    bool unloadOpenssl();
};


#endif // CPPOPENSSL_H
