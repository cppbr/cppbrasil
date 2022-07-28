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


#include "card.h"

Card::Card(): m_tpIntegra(TpIntegra::None), m_tBand(TBand::None)
{
}

Card::~Card()
{
}

void Card::clear()
{
    this->m_tpIntegra = TpIntegra::None ;
    this->m_CNPJ.clear();
    this->m_tBand = TBand::None;
    this->m_cAut.clear();
}

TpIntegra Card::get_tpIntegra() const
{
    return this->m_tpIntegra;
}

void Card::set_tpIntegra(const TpIntegra &tpIntegra)
{
    this->m_tpIntegra = tpIntegra;
}

QString Card::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Card::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

TBand Card::get_tBand() const
{
    return this->m_tBand;
}

void Card::set_tBand(const TBand &tBand)
{
    this->m_tBand = tBand;
}

QString Card::get_cAut() const
{
    return this->m_cAut;
}

void Card::set_cAut(const QString &cAut)
{
    this->m_cAut = cAut;
}
