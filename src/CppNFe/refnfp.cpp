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


#include "refnfp.h"

RefNFP::RefNFP(): m_cUF(0), m_serie(0), m_nNF(0)
{
}

RefNFP::~RefNFP()
{
}

void RefNFP::clear()
{
    this->m_cUF = 0;
    this->m_AAMM.clear();
    this->m_CNPJ.clear();
    this->m_CPF.clear();
    this->m_IE.clear();
    this->m_mod.clear();
    this->m_serie = 0;
    this->m_nNF = 0;
}

int RefNFP::get_cUF() const
{
    return this->m_cUF;
}
void RefNFP::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}
QString RefNFP::get_AAMM() const
{
    return  this->m_AAMM;
}
void RefNFP::set_AAMM(const QString &AAMM)
{
    this->m_AAMM = AAMM;
}
QString RefNFP::get_CNPJ() const
{
    return this->m_CNPJ;
}
void RefNFP::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString RefNFP::get_CPF() const
{
    return this->m_CPF;
}

void RefNFP::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}

QString RefNFP::get_IE() const
{
     return this->m_IE;
}

void RefNFP::set_IE(const QString &IE)
{
    this->m_IE = IE;
}
QString RefNFP::get_mod() const
{
    return this->m_mod;
}
void RefNFP::set_mod(const QString &mod)
{
    this->m_mod = mod;
}
int RefNFP::get_serie() const
{
    return this->m_serie;
}
void RefNFP::set_serie(const int &serie)
{
    this->m_serie = serie;
}
int RefNFP::get_nNF() const
{
    return this->m_nNF;
}
void RefNFP::set_nNF(const int &nNF)
{
    this->m_nNF = nNF;
}

