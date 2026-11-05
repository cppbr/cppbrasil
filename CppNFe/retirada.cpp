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


#include "retirada.h"

Retirada::Retirada(): m_nro(0), m_cMun(0), m_CEP(0), m_cPais(0)
{
}

Retirada::~Retirada()
{
}

void Retirada::clear()
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

QString Retirada::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Retirada::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Retirada::get_CPF() const
{
    return this->m_CPF;
}

void Retirada::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString Retirada::get_xNome() const
{
    return this->m_xNome;
}

void Retirada::set_xNome(const QString &xNome)
{
    this->m_xNome = xNome;
}

QString Retirada::get_xLgr() const
{
    return this->m_xLgr;
}

void Retirada::set_xLgr(const QString &xLgr)
{
    this->m_xLgr = xLgr;
}

int Retirada::get_nro() const
{
    return this->m_nro;
}

void Retirada::set_nro(const int &nro)
{
    this->m_nro = nro;
}

QString Retirada::get_xCpl() const
{
    return this->m_xCpl;
}

void Retirada::set_xCpl(const QString &xCpl)
{
    m_xCpl = xCpl;
}

QString Retirada::get_xBairro() const
{
    return this->m_xBairro;
}

void Retirada::set_xBairro(const QString &xBairro)
{
    this->m_xBairro = xBairro;
}

int Retirada::get_cMun() const
{
    return this->m_cMun;
}

void Retirada::set_cMun(const int &cMun)
{
    this->m_cMun = cMun;
}

QString Retirada::get_xMun() const
{
    return this->m_xMun;
}

void Retirada::set_xMun(const QString &xMun)
{
    this->m_xMun = xMun;
}

QString Retirada::get_UF() const
{
    return this->m_UF;
}

void Retirada::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

int Retirada::get_CEP() const
{
    return this->m_CEP;
}

void Retirada::set_CEP(const int &CEP)
{
    this->m_CEP = CEP;
}

int Retirada::get_cPais() const
{
    return this->m_cPais;
}

void Retirada::set_cPais(const int &cPais)
{
    this->m_cPais = cPais;
}

QString Retirada::get_xPais() const
{
    return this->m_xPais;
}

void Retirada::set_xPais(const QString &xPais)
{
    this->m_xPais = xPais;
}

QString Retirada::get_fone() const
{
    return this->m_fone;
}

void Retirada::set_fone(const QString &fone)
{
    this->m_fone = fone;
}

QString Retirada::get_email() const
{
    return this->m_email;
}

void Retirada::set_email(const QString &email)
{
    this->m_email = email;
}

QString Retirada::get_IE() const
{
    return this->m_IE;
}

void Retirada::set_IE(const QString &IE)
{
    this->m_IE = IE;
}
