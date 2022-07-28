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


#include "cofins.h"

Cofins::Cofins() : m_CST(CstCOFINS::None), m_vBC(0.0), m_pCOFINS(0.0), m_vCOFINS(0.0),
    m_qBCProd(0.0), m_vAliqProd(0.0)
{
}

Cofins::~Cofins()
{
}

void Cofins::clear()
{
    this->m_CST = CstCOFINS::None;
    this->m_vBC = 0.0;
    this->m_pCOFINS = 0.0;
    this->m_vCOFINS = 0.0;
    this->m_qBCProd = 0.0;
    this->m_vAliqProd = 0.0;
}

CstCOFINS Cofins::get_CST() const
{
    return this->m_CST;
}

void Cofins::set_CST(const CstCOFINS &CST)
{
    this->m_CST = CST;
}

double Cofins::get_vBC() const
{
    return this->m_vBC;
}

void Cofins::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double Cofins::get_pCOFINS() const
{
    return this->m_pCOFINS;
}

void Cofins::set_pCOFINS(const double &pCOFINS)
{
    this->m_pCOFINS = pCOFINS;
}

double Cofins::get_vCOFINS() const
{
    return this->m_vCOFINS;
}

void Cofins::set_vCOFINS(const double &vCOFINS)
{
    this->m_vCOFINS = vCOFINS;
}

double Cofins::get_qBCProd() const
{
    return this->m_qBCProd;
}

void Cofins::set_qBCProd(const double &qBCProd)
{
    this->m_qBCProd = qBCProd;
}

double Cofins::get_vAliqProd() const
{
    return this->m_vAliqProd;
}

void Cofins::set_vAliqProd(const double &vAliqProd)
{
    this->m_vAliqProd = vAliqProd;
}
