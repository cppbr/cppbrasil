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


#ifndef CPPCRYPTO_H
#define CPPCRYPTO_H

#include <QString>
#include <QByteArray>
#include <QDateTime>
#include "cpputility.h"

enum class CertType{None = -1, A1, A3};
enum class CryptoType{None = -1, OpenSSL};
enum class HashType{sha = 0, sha1, mdc2, ripemd160, sha224, sha256, sha384,
                    sha512, md2, md4, md5};


#ifdef CPPBRASIL_STATIC
    class CppCrypto
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT CppCrypto
#endif
{
public:
    CppCrypto();
    virtual ~CppCrypto();
    QString get_error() const;
    QString get_certBase64() const;
    QString get_numeroSerie() const;
    QString get_issuerName() const;
    QString get_subjectName() const;
    QDateTime get_dataEmissao() const;
    QDateTime get_dataVencimento() const;
    QString get_certificadora() const;
    QString get_razaoSocial() const;
    QString get_cnpj() const;
    QByteArray get_pkeyDER() const;
    QByteArray get_certDER() const;
    const QList<QByteArray>& get_caDER() const;
    virtual bool insertCAcert(const QString &certPath);

    static QString hashTypeToStr(const HashType &rsa);

    virtual bool digest(const HashType &hashtype, const QByteArray &input,
                        const bool returnBase64, QByteArray &output);
    virtual bool signature(const HashType &hashtype, const QByteArray &input,
                           const bool returnBase64, QByteArray &output);
    virtual bool load();
    virtual bool unload();
protected:
    void clear();
    void set_error(const QString &error);
    void set_certBase64(const QString &certB64);
    void set_numeroSerie(const QString &numeroSerie);
    void set_issuerName(const QString &issuerName);
    void set_subjectName(const QString &subjectName);
    void set_dataEmissao(const QDateTime &dataEmissao);
    void set_dataVencimento(const QDateTime &dataVencimento);
    void set_certificadora(const QString &certificadora);
    void set_razaoSocial(const QString &razaoSocial);
    void set_cnpj(const QString &cnpj);
    void set_pkeyDER(const QByteArray &pkey);
    void set_certDER(const QByteArray &cert);
    void set_caDER(const QList<QByteArray> &ca);
private:
    QString m_numeroSerie;
    QString m_issuerName;
    QString m_subjectName;
    QString m_certificadora;
    QDateTime m_dataEmissao;
    QDateTime m_dataVencimento;
    QString m_razaoSocial;
    QString m_cnpj;
    QString m_certB64;
    QString m_error;
    QByteArray m_pkeyDER;
    QByteArray m_certDER;
    QList<QByteArray> m_caDER;
};

#endif // CPPCRYPTO_H
