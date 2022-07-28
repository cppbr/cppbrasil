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


#include "dest.h"

Dest::Dest(): enderDest(new EnderDest), m_indIEDest(IndIEDest::None)
{
}

Dest::~Dest()
{
}

void Dest::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_idEstrangeiro.clear();
    this->m_xNome.clear();
    this->m_indIEDest = IndIEDest::None;
    this->m_IE.clear();
    this->m_ISUF.clear();
    this->m_IM.clear();
    this->m_email.clear();

    this->enderDest->clear();
}

QString Dest::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Dest::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Dest::get_CPF() const
{
    return this->m_CPF;
}

void Dest::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString Dest::get_idEstrangeiro() const
{
    return this->m_idEstrangeiro;
}

void Dest::set_idEstrangeiro(const QString &idEstrangeiro)
{
    this->m_idEstrangeiro = idEstrangeiro;
}

QString Dest::get_xNome() const
{
    return this->m_xNome;
}

void Dest::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

IndIEDest Dest::get_indIEDest() const
{
    return this->m_indIEDest;
}

void Dest::set_indIEDest(const IndIEDest &indIEDest)
{
    this->m_indIEDest = indIEDest;
}

QString Dest::get_IE() const
{
    return this->m_IE;
}

void Dest::set_IE(const QString &IE)
{
    this->m_IE = IE;
}

QString Dest::get_ISUF() const
{
    return this->m_ISUF;
}

void Dest::set_ISUF(const QString &ISUF)
{
    this->m_ISUF = ISUF;
}

QString Dest::get_IM() const
{
    return this->m_IM;
}

void Dest::set_IM(const QString &IM)
{
    this->m_IM = IM;
}

QString Dest::get_email() const
{
    return this->m_email;
}

void Dest::set_email(const QString &email)
{
    this->m_email = email;
}
