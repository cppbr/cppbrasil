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


#include "transporta.h"

Transporta::Transporta()
{
}

Transporta::~Transporta()
{
}

void Transporta::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_xNome.clear();
    this->m_IE.clear();
    this->m_xEnder.clear();
    this->m_xMun.clear();
    this->m_UF.clear();
}

QString Transporta::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Transporta::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Transporta::get_CPF() const
{
    return this->m_CPF;
}

void Transporta::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString Transporta::get_xNome() const
{
    return this->m_xNome;
}

void Transporta::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

QString Transporta::get_IE() const
{
    return this->m_IE;
}

void Transporta::set_IE(const QString &IE)
{
    this->m_IE = IE;
}

QString Transporta::get_xEnder() const
{
    return this->m_xEnder;
}

void Transporta::set_xEnder(const QString &xEnder)
{
    this->m_xEnder = xEnder;
}

QString Transporta::get_xMun() const
{
    return this->m_xMun;
}

void Transporta::set_xMun(const QString &xMun)
{
    this->m_xMun = xMun;
}

QString Transporta::get_UF() const
{
    return this->m_UF;
}

void Transporta::set_UF(const QString &UF)
{
    this->m_UF = UF;
}
