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


#include "rettransp.h"

RetTransp::RetTransp(): m_vServ(0.0), m_vBCRet(0.0), m_pICMSRet(0.0), m_vICMSRet(0.0),
    m_cMunFG(0)
{
}

RetTransp::~RetTransp()
{
}

void RetTransp::clear()
{
    this->m_vServ = 0.0;
    this->m_vBCRet = 0.0;
    this->m_pICMSRet = 0.0;
    this->m_vICMSRet = 0.0;
    this->m_CFOP.clear();
    this->m_cMunFG = 0;
}

double RetTransp::get_vServ() const
{
    return this->m_vServ;
}

void RetTransp::set_vServ(const double &vServ)
{
    this->m_vServ = vServ;
}

double RetTransp::get_vBCRet() const
{
    return this->m_vBCRet;
}

void RetTransp::set_vBCRet(const double &vBCRet)
{
    this->m_vBCRet = vBCRet;
}

double RetTransp::get_pICMSRet() const
{
    return this->m_pICMSRet;
}

void RetTransp::set_pICMSRet(const double &pICMSRet)
{
    this->m_pICMSRet = pICMSRet;
}

double RetTransp::get_vICMSRet() const
{
    return this->m_vICMSRet;
}

void RetTransp::set_vICMSRet(const double &vICMSRet)
{
    this->m_vICMSRet = vICMSRet;
}

QString RetTransp::get_CFOP() const
{
    return this->m_CFOP;
}

void RetTransp::set_CFOP(const QString &CFOP)
{
    this->m_CFOP = CFOP;
}

int RetTransp::get_cMunFG() const
{
    return this->m_cMunFG;
}

void RetTransp::set_cMunFG(const int &cMunFG)
{
    this->m_cMunFG = cMunFG;
}
