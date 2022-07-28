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


#include "pis.h"

Pis::Pis() : m_CST(CstPIS::None), m_vBC(0.0), m_pPIS(0.0), m_vPIS(0.0), m_qBCProd(0.0),
    m_vAliqProd(0.0)
{
}

Pis::~Pis()
{
}

void Pis::clear()
{
   this->m_CST = CstPIS::None;
   this->m_vBC = 0.0;
   this->m_pPIS = 0.0;
   this->m_vPIS = 0.0;
   this->m_qBCProd = 0.0;
   this->m_vAliqProd = 0.0;
}

CstPIS Pis::get_CST() const
{
    return this->m_CST;
}

void Pis::set_CST(const CstPIS &CST)
{
    this->m_CST = CST;
}

double Pis::get_vBC() const
{
    return this->m_vBC;
}

void Pis::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double Pis::get_pPIS() const
{
    return this->m_pPIS;
}

void Pis::set_pPIS(const double &pPIS)
{
    this->m_pPIS = pPIS;
}

double Pis::get_vPIS() const
{
    return this->m_vPIS;
}

void Pis::set_vPIS(const double &vPIS)
{
    this->m_vPIS = vPIS;
}

double Pis::get_qBCProd() const
{
    return this->m_qBCProd;
}

void Pis::set_qBCProd(const double &qBCProd)
{
    this->m_qBCProd = qBCProd;
}

double Pis::get_vAliqProd() const
{
    return this->m_vAliqProd;
}

void Pis::set_vAliqProd(const double &vAliqProd)
{
    this->m_vAliqProd = vAliqProd;
}
