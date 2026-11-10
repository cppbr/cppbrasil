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


#include "infresptec.h"
#include <QCryptographicHash>

InfRespTec::InfRespTec(): m_idCSRT(0)
{
}

InfRespTec::~InfRespTec()
{
}

void InfRespTec::clear()
{
    this->m_CNPJ.clear();
    this->m_xContato.clear();
    this->m_email.clear();
    this->m_fone.clear();
    this->m_idCSRT = 0;
    this->m_hashCSRT.clear();
    this->m_CSRT.clear();
}

QString InfRespTec::get_CNPJ() const
{
    return this->m_CNPJ;
}

void InfRespTec::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString InfRespTec::get_xContato() const
{
    return this->m_xContato;
}

void InfRespTec::set_xContato(const QString &xContato)
{
    this->m_xContato = xContato;
}

QString InfRespTec::get_email() const
{
    return this->m_email;
}

void InfRespTec::set_email(const QString &email)
{
    this->m_email = email;
}

QString InfRespTec::get_fone() const
{
    return this->m_fone;
}

void InfRespTec::set_fone(const QString &fone)
{
    this->m_fone = fone;
}

int InfRespTec::get_idCSRT() const
{
    return this->m_idCSRT;
}

void InfRespTec::set_idCSRT(const int &idCSRT)
{
    this->m_idCSRT = idCSRT;
}

QString InfRespTec::get_hashCSRT() const
{
    return this->m_hashCSRT;
}

void InfRespTec::set_hashCSRT(const QString &hashCSRT)
{
    this->m_hashCSRT = hashCSRT;
    this->m_CSRT.clear();
}

void InfRespTec::set_CSRT(const QString &CSRT)
{
    this->m_CSRT = CSRT;
    this->m_hashCSRT.clear();
}

QString InfRespTec::hashCSRTParaChave(const QString &chNFe) const
{
    if (this->m_CSRT.isEmpty())
        return this->m_hashCSRT;

    const QByteArray dados = this->m_CSRT.toUtf8() + chNFe.toLatin1();
    return QString::fromLatin1(QCryptographicHash::hash(dados, QCryptographicHash::Sha1).toBase64());
}
