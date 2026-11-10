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


#include "issqntot.h"

IssQNtot::IssQNtot(): m_vServ(0.0), m_vBC(0.0), m_vISS(0.0), m_vPIS(0.0), m_vCOFINS(0.0),
    m_vDeducao(0.0), m_vOutro(0.0), m_vDescIncond(0.0), m_vDescCond(0.0), m_vISSRet(0.0),
    m_cRegTrib(CRegTrib::None)
{
}

IssQNtot::~IssQNtot()
{
}

void IssQNtot::clear()
{
    this->m_vServ = 0.0;
    this->m_vBC = 0.0;
    this->m_vISS = 0.0;
    this->m_vPIS = 0.0;
    this->m_vCOFINS = 0.0;
    CppUtility::clearDateTime(this->m_dCompet);
    this->m_vDeducao = 0.0;
    this->m_vOutro = 0.0;
    this->m_vDescIncond = 0.0;
    this->m_vDescCond = 0.0;
    this->m_vISSRet = 0.0;
    this->m_cRegTrib = CRegTrib::None;
}

double IssQNtot::get_vServ() const
{
    return this->m_vServ;
}

void IssQNtot::set_vServ(const double &vServ)
{
    this->m_vServ = vServ;
}

double IssQNtot::get_vBC() const
{
    return this->m_vBC;
}

void IssQNtot::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double IssQNtot::get_vISS() const
{
    return this->m_vISS;
}

void IssQNtot::set_vISS(const double &vISS)
{
    this->m_vISS = vISS;
}

double IssQNtot::get_vPIS() const
{
    return this->m_vPIS;
}

void IssQNtot::set_vPIS(const double &vPIS)
{
    this->m_vPIS = vPIS;
}

double IssQNtot::get_vCOFINS() const
{
    return this->m_vCOFINS;
}

void IssQNtot::set_vCOFINS(const double &vCOFINS)
{
    this->m_vCOFINS = vCOFINS;
}

QDateTime IssQNtot::get_dCompet() const
{
    return this->m_dCompet;
}

void IssQNtot::set_dCompet(const QDateTime &dCompet)
{
    this->m_dCompet = dCompet;
}

double IssQNtot::get_vDeducao() const
{
    return this->m_vDeducao;
}

void IssQNtot::set_vDeducao(const double &vDeducao)
{
    this->m_vDeducao = vDeducao;
}

double IssQNtot::get_vOutro() const
{
    return this->m_vOutro;
}

void IssQNtot::set_vOutro(const double &vOutro)
{
    this->m_vOutro = vOutro;
}

double IssQNtot::get_vDescIncond() const
{
    return this->m_vDescIncond;
}

void IssQNtot::set_vDescIncond(const double &vDescIncond)
{
   this->m_vDescIncond = vDescIncond;
}

double IssQNtot::get_vDescCond() const
{
    return this->m_vDescCond;
}

void IssQNtot::set_vDescCond(const double &vDescCond)
{
    this->m_vDescCond = vDescCond;
}

double IssQNtot::get_vISSRet() const
{
    return this->m_vISSRet;
}

void IssQNtot::set_vISSRet(const double &vISSRet)
{
    this->m_vISSRet = vISSRet;
}

CRegTrib IssQNtot::get_cRegTrib() const
{
    return this->m_cRegTrib;
}

void IssQNtot::set_cRegTrib(const CRegTrib &cRegTrib)
{
    this->m_cRegTrib = cRegTrib;
}
