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


#include "rettrib.h"

RetTrib::RetTrib(): m_vRetPIS(0.0), m_vRetCOFINS(0.0), m_vRetCSLL(0.0), m_vBCIRRF(0.0),
    m_vIRRF(0.0), m_vBCRetPrev(0.0), m_vRetPrev(0.0)
{
}

RetTrib::~RetTrib()
{
}

void RetTrib::clear()
{
    this->m_vRetPIS = 0.0;
    this->m_vRetCOFINS = 0.0;
    this->m_vRetCSLL = 0.0;
    this->m_vBCIRRF = 0.0;
    this->m_vIRRF = 0.0;
    this->m_vBCRetPrev = 0.0;
    this->m_vRetPrev = 0.0;
}

double RetTrib::get_vRetPIS() const
{
    return this->m_vRetPIS;
}

void RetTrib::set_vRetPIS(const double &vRetPIS)
{
    this->m_vRetPIS = vRetPIS;
}

double RetTrib::get_vRetCOFINS() const
{
    return this->m_vRetCOFINS;
}

void RetTrib::set_vRetCOFINS(const double &vRetCOFINS)
{
    this->m_vRetCOFINS = vRetCOFINS;
}

double RetTrib::get_vRetCSLL() const
{
    return this->m_vRetCSLL;
}

void RetTrib::set_vRetCSLL(const double &vRetCSLL)
{
    this->m_vRetCSLL = vRetCSLL;
}

double RetTrib::get_vBCIRRF() const
{
    return this->m_vBCIRRF;
}

void RetTrib::set_vBCIRRF(const double &vBCIRRF)
{
    this->m_vBCIRRF = vBCIRRF;
}

double RetTrib::get_vIRRF() const
{
    return this->m_vIRRF;
}

void RetTrib::set_vIRRF(const double &vIRRF)
{
    this->m_vIRRF = vIRRF;
}

double RetTrib::get_vBCRetPrev() const
{
    return this->m_vBCRetPrev;
}

void RetTrib::set_vBCRetPrev(const double &vBCRetPrev)
{
    this->m_vBCRetPrev = vBCRetPrev;
}

double RetTrib::get_vRetPrev() const
{
    return this->m_vRetPrev;
}

void RetTrib::set_vRetPrev(const double &vRetPrev)
{
    this->m_vRetPrev = vRetPrev;
}
