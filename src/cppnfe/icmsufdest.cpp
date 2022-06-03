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


#include "icmsufdest.h"

IcmsUFDest::IcmsUFDest(): m_vBCUFDest(0.0), m_vBCFCPUFDest(0.0), m_pFCPUFDest(0.0),
    m_pICMSUFDest(0.0), m_pICMSInter(0.0), m_pICMSInterPart(0.0), m_vFCPUFDest(0.0),
    m_vICMSUFDest(0.0), m_vICMSUFRemet(0.0)
{
}

IcmsUFDest::~IcmsUFDest()
{
}

void IcmsUFDest::clear()
{
    this->m_vBCUFDest = 0.0;
    this->m_vBCFCPUFDest = 0.0;
    this->m_pFCPUFDest = 0.0;
    this->m_pICMSUFDest = 0.0;
    this->m_pICMSInter = 0.0;
    this->m_pICMSInterPart = 0.0;
    this->m_vFCPUFDest = 0.0;
    this->m_vICMSUFDest = 0.0;
    this->m_vICMSUFRemet = 0.0;
}

double IcmsUFDest::get_vBCUFDest() const
{
    return this->m_vBCUFDest;
}

void IcmsUFDest::set_vBCUFDest(const double &vBCUFDest)
{
    this->m_vBCUFDest = vBCUFDest;
}

double IcmsUFDest::get_vBCFCPUFDest() const
{
    return this->m_vBCFCPUFDest;
}

void IcmsUFDest::set_vBCFCPUFDest(const double &vBCFCPUFDest)
{
    this->m_vBCFCPUFDest = vBCFCPUFDest;
}

double IcmsUFDest::get_pFCPUFDest() const
{
    return this->m_pFCPUFDest;
}

void IcmsUFDest::set_pFCPUFDest(const double &pFCPUFDest)
{
    this->m_pFCPUFDest = pFCPUFDest;
}

double IcmsUFDest::get_pICMSUFDest() const
{
    return this->m_pICMSUFDest;
}

void IcmsUFDest::set_pICMSUFDest(const double &pICMSUFDest)
{
    this->m_pICMSUFDest = pICMSUFDest;
}

double IcmsUFDest::get_pICMSInter() const
{
    return this->m_pICMSInter;
}

void IcmsUFDest::set_pICMSInter(const double &pICMSInter)
{
    this->m_pICMSInter = pICMSInter;
}

double IcmsUFDest::get_pICMSInterPart() const
{
    return this->m_pICMSInterPart;
}

void IcmsUFDest::set_pICMSInterPart(const double &pICMSInterPart)
{
    this->m_pICMSInterPart = pICMSInterPart;
}

double IcmsUFDest::get_vFCPUFDest() const
{
    return this->m_vFCPUFDest;
}

void IcmsUFDest::set_vFCPUFDest(const double &vFCPUFDest)
{
    this->m_vFCPUFDest = vFCPUFDest;
}

double IcmsUFDest::get_vICMSUFDest() const
{
    return this->m_vICMSUFDest;
}

void IcmsUFDest::set_vICMSUFDest(const double &vICMSUFDest)
{
    this->m_vICMSUFDest = vICMSUFDest;
}

double IcmsUFDest::get_vICMSUFRemet() const
{
    return this->m_vICMSUFRemet;
}

void IcmsUFDest::set_vICMSUFRemet(const double &vICMSUFRemet)
{
    this->m_vICMSUFRemet = vICMSUFRemet;
}
