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


#include "emit.h"

Emit::Emit(): enderEmit(new EnderEmit), m_CRT(Crt::None)
{
}

Emit::~Emit()
{
}


void Emit::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_xNome.clear();
    this->m_xFant.clear();
    this->m_IE.clear();
    this->m_IEST.clear();
    this->m_IM.clear();
    this->m_CNAE.clear();
    this->m_CRT = Crt::None;

    this->enderEmit->clear();
}

QString Emit::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Emit::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Emit::get_CPF() const
{
    return this->m_CPF;
}

void Emit::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString Emit::get_xNome() const
{
    return this->m_xNome;
}

void Emit::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

QString Emit::get_xFant() const
{
    return this->m_xFant;
}

void Emit::set_xFant(const QString &xFant)
{
    this->m_xFant = xFant;
}

QString Emit::get_IE() const
{
    return this->m_IE;
}

void Emit::set_IE(const QString &IE)
{
    this->m_IE = IE;
}

QString Emit::get_IEST() const
{
    return this->m_IEST;
}

void Emit::set_IEST(const QString &IEST)
{
    this->m_IEST = IEST;
}

QString Emit::get_IM() const
{
    return this->m_IM;
}

void Emit::set_IM(const QString &IM)
{
    this->m_IM = IM;
}

QString Emit::get_CNAE() const
{
    return this->m_CNAE;
}

void Emit::set_CNAE(const QString &CNAE)
{
    this->m_CNAE = CNAE;
}

Crt Emit::get_CRT() const
{
    return this->m_CRT;
}

void Emit::set_CRT(const Crt &CRT)
{
    this->m_CRT = CRT;
}
