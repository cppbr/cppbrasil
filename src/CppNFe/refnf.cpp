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


#include "refnf.h"

RefNF::RefNF(): m_cUF(0), m_serie(0), m_nNF(0)
{
}

RefNF::~RefNF()
{
}

void RefNF::clear()
{
    this->m_cUF = 0;
    this->m_AAMM.clear();
    this->m_CNPJ.clear();
    this->m_mod.clear();
    this->m_serie = 0;
    this->m_nNF = 0;
}

int RefNF::get_cUF() const
{
    return this->m_cUF;
}
void RefNF::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}
QString RefNF::get_AAMM() const
{
    return  this->m_AAMM;
}
void RefNF::set_AAMM(const QString &AAMM)
{
    this->m_AAMM = AAMM;
}
QString RefNF::get_CNPJ() const
{
    return this->m_CNPJ;
}
void RefNF::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}
QString RefNF::get_mod() const
{
    return this->m_mod;
}
void RefNF::set_mod(const QString &mod)
{
    this->m_mod = mod;
}
int RefNF::get_serie() const
{
    return this->m_serie;
}
void RefNF::set_serie(const int &serie)
{
    this->m_serie = serie;
}
int RefNF::get_nNF() const
{
    return this->m_nNF;
}
void RefNF::set_nNF(const int &nNF)
{
    this->m_nNF = nNF;
}

