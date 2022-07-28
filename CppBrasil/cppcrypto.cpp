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


#include "cppcrypto.h"

bool CppCrypto::load()
{
    this->set_error("Uninitialized");
    return false;
}

bool CppCrypto::unload()
{
    this->set_error("Uninitialized");
    return false;
}

CppCrypto::CppCrypto()
{
}

CppCrypto::~CppCrypto()
{
}

QString CppCrypto::get_error() const
{
    return this->m_error;
}

QString CppCrypto::get_certBase64() const
{
    return this->m_certB64;
}

QString CppCrypto::get_numeroSerie() const
{
    return this->m_numeroSerie;
}

QString CppCrypto::get_issuerName() const
{
    return this->m_issuerName;
}

QString CppCrypto::get_subjectName() const
{
    return this->m_subjectName;
}

QDateTime CppCrypto::get_dataEmissao() const
{
    return this->m_dataEmissao;
}

QDateTime CppCrypto::get_dataVencimento() const
{
    return this->m_dataVencimento;
}

QString CppCrypto::get_certificadora() const
{
    return this->m_certificadora;
}

QString CppCrypto::get_razaoSocial() const
{
    return this->m_razaoSocial;
}

QString CppCrypto::get_cnpj() const
{
    return this->m_cnpj;
}

QByteArray CppCrypto::get_pkeyDER() const
{
    return this->m_pkeyDER;
}

QByteArray CppCrypto::get_certDER() const
{
    return this->m_certDER;
}

const QList<QByteArray>& CppCrypto::get_caDER() const
{
    return this->m_caDER;
}

bool CppCrypto::insertCAcert(const QString &certPath)
{
    this->set_error("Uninitialized");
    Q_UNUSED(certPath);
    return false;
}

void CppCrypto::clear()
{
    this->m_numeroSerie.clear();
    this->m_issuerName.clear();
    this->m_certificadora.clear();
    CppUtility::clearDateTime(this->m_dataEmissao);
    CppUtility::clearDateTime(this->m_dataVencimento);
    this->m_subjectName.clear();
    this->m_razaoSocial.clear();
    this->m_cnpj.clear();
    this->m_certB64.clear();
    this->m_pkeyDER.clear();
    this->m_certDER.clear();
    this->m_caDER.clear();
}

void CppCrypto::set_error(const QString &error)
{
    this->m_error = error;
}

void CppCrypto::set_certBase64(const QString &certB64)
{
    this->m_certB64 = certB64;
}

void CppCrypto::set_numeroSerie(const QString &numeroSerie)
{
    this->m_numeroSerie = numeroSerie;
}

void CppCrypto::set_issuerName(const QString &issuerName)
{
    this->m_issuerName = issuerName;
}

void CppCrypto::set_subjectName(const QString &subjectName)
{
    this->m_subjectName = subjectName;
}

void CppCrypto::set_dataEmissao(const QDateTime &dataEmissao)
{
    this->m_dataEmissao = dataEmissao;
}

void CppCrypto::set_dataVencimento(const QDateTime &dataVencimento)
{
    this->m_dataVencimento = dataVencimento;
}

void CppCrypto::set_certificadora(const QString &certificadora)
{
    this->m_certificadora = certificadora;
}

void CppCrypto::set_razaoSocial(const QString &razaoSocial)
{
    this->m_razaoSocial = razaoSocial;
}

void CppCrypto::set_cnpj(const QString &cnpj)
{
    this->m_cnpj = cnpj;
}

void CppCrypto::set_pkeyDER(const QByteArray &pkey)
{
    this->m_pkeyDER = pkey;
}

void CppCrypto::set_certDER(const QByteArray &cert)
{
    this->m_certDER = cert;
}

void CppCrypto::set_caDER(const QList<QByteArray> &ca)
{
    this->m_caDER.clear();
    this->m_caDER = ca;
}

QString CppCrypto::hashTypeToStr(const HashType &rsa)
{
    QList<QString> _r = {"sha", "sha1", "mdc2", "ripemd160", "sha224",
                         "sha256", "sha384", "sha512", "md2", "md4", "md5"};
    return _r[static_cast<int>(rsa)];
}

bool CppCrypto::digest(const HashType &hashtype, const QByteArray &input,
                       const bool returnBase64, QByteArray &output)
{
    this->set_error("Uninitialized");
    Q_UNUSED(hashtype);
    Q_UNUSED(input);
    Q_UNUSED(returnBase64);
    Q_UNUSED(output);
    return false;
}

bool CppCrypto::signature(const HashType &hashtype, const QByteArray &input,
                          const bool returnBase64, QByteArray &output)
{
    this->set_error("Uninitialized");
    Q_UNUSED(hashtype);
    Q_UNUSED(input);
    Q_UNUSED(returnBase64);
    Q_UNUSED(output);
    return false;
}

