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


#include "entrega.h"

Entrega::Entrega(): m_nro(0), m_cMun(0), m_CEP(0), m_cPais(0)
{
}

Entrega::~Entrega()
{
}

void Entrega::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_xNome.clear();
    this->m_xLgr.clear();
    this->m_nro = 0;
    this->m_xCpl.clear();
    this->m_xBairro.clear();
    this->m_cMun = 0;
    this->m_xMun.clear();
    this->m_UF.clear();
    this->m_CEP = 0;
    this->m_cPais = 0;
    this->m_xPais.clear();
    this->m_fone.clear();
    this->m_email.clear();
    this->m_IE.clear();
}


QString Entrega::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Entrega::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Entrega::get_CPF() const
{
    return this->m_CPF;
}

void Entrega::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString Entrega::get_xNome() const
{
    return this->m_xNome;
}

void Entrega::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

QString Entrega::get_xLgr() const
{
    return this->m_xLgr;
}

void Entrega::set_xLgr(const QString &xLgr)
{
    this->m_xLgr = xLgr;
}

int Entrega::get_nro() const
{
    return this->m_nro;
}

void Entrega::set_nro(const int &nro)
{
    this->m_nro = nro;
}

QString Entrega::get_xCpl() const
{
    return this->m_xCpl;
}

void Entrega::set_xCpl(const QString &xCpl)
{
    m_xCpl = xCpl;
}

QString Entrega::get_xBairro() const
{
    return this->m_xBairro;
}

void Entrega::set_xBairro(const QString &xBairro)
{
    this->m_xBairro = xBairro;
}

int Entrega::get_cMun() const
{
    return this->m_cMun;
}

void Entrega::set_cMun(const int &cMun)
{
    this->m_cMun = cMun;
}

QString Entrega::get_xMun() const
{
    return this->m_xMun;
}

void Entrega::set_xMun(const QString &xMun)
{
    this->m_xMun = xMun;
}

QString Entrega::get_UF() const
{
    return this->m_UF;
}

void Entrega::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

int Entrega::get_CEP() const
{
    return this->m_CEP;
}

void Entrega::set_CEP(const int &CEP)
{
    this->m_CEP = CEP;
}

int Entrega::get_cPais() const
{
    return this->m_cPais;
}

void Entrega::set_cPais(const int &cPais)
{
    this->m_cPais = cPais;
}

QString Entrega::get_xPais() const
{
    return this->m_xPais;
}

void Entrega::set_xPais(const QString &xPais)
{
    this->m_xPais = xPais;
}

QString Entrega::get_fone() const
{
    return this->m_fone;
}

void Entrega::set_fone(const QString &fone)
{
    this->m_fone = fone;
}

QString Entrega::get_email() const
{
    return this->m_email;
}

void Entrega::set_email(const QString &email)
{
    this->m_email = email;
}

QString Entrega::get_IE() const
{
    return this->m_IE;
}

void Entrega::set_IE(const QString &IE)
{
    this->m_IE = IE;
}
