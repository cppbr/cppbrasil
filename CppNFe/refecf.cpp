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


#include "refecf.h"

RefECF::RefECF()
{
}

RefECF::~RefECF()
{
}

void RefECF::clear()
{
    this->m_mod.clear();
    this->m_nECF.clear();
    this->m_nCOO.clear();
}

QString RefECF::get_mod() const
{
    return this->m_mod;
}

void RefECF::set_mod(const QString &mod)
{
    this->m_mod = mod;
}

QString RefECF::get_nECF() const
{
    return this->m_nECF;
}

void RefECF::set_nECF(const QString &nECF)
{
    this->m_nECF = nECF;
}

QString RefECF::get_nCOO() const
{
    return this->m_nCOO;
}

void RefECF::set_nCOO(const QString &nCOO)
{
    this->m_nCOO = nCOO;
}

