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


#include "icmstot.h"

IcmsTot::IcmsTot(): m_vBC(0.0), m_vICMS(0.0), m_vICMSDeson(0.0), m_vFCPUFDest(0.0), m_vICMSUFDest(0.0),
    m_vICMSUFRemet(0.0), m_vFCP(0.0), m_vBCST(0.0), m_vST(0.0), m_vFCPST(0.0), m_vFCPSTRet(0.0),
    m_vProd(0.0), m_vFrete(0.0), m_vSeg(0.0), m_vDesc(0.0), m_vII(0.0), m_vIPI(0.0), m_vIPIDevol(0.0),
    m_vPIS(0.0), m_vCOFINS(0.0), m_vOutro(0.0), m_vNF(0.0), m_vTotTrib(0.0)
{
}

IcmsTot::~IcmsTot()
{
}

void IcmsTot::clear()
{
    this->m_vBC = 0.0;
    this->m_vICMS = 0.0;
    this->m_vICMSDeson = 0.0;
    this->m_vFCPUFDest = 0.0;
    this->m_vICMSUFDest = 0.0;
    this->m_vICMSUFRemet = 0.0;
    this->m_vFCP = 0.0;
    this->m_vBCST = 0.0;
    this->m_vST = 0.0;
    this->m_vFCPST = 0.0;
    this->m_vFCPSTRet = 0.0;
    this->m_vProd = 0.0;
    this->m_vFrete = 0.0;
    this->m_vSeg = 0.0;
    this->m_vDesc = 0.0;
    this->m_vII = 0.0;
    this->m_vIPI = 0.0;
    this->m_vIPIDevol = 0.0;
    this->m_vPIS = 0.0;
    this->m_vCOFINS = 0.0;
    this->m_vOutro = 0.0;
    this->m_vNF = 0.0;
    this->m_vTotTrib = 0.0;
}

double IcmsTot::get_vBC() const
{
    return this->m_vBC;
}

void IcmsTot::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double IcmsTot::get_vICMS() const
{
    return this->m_vICMS;
}

void IcmsTot::set_vICMS(const double &vICMS)
{
    this->m_vICMS = vICMS;
}

double IcmsTot::get_vICMSDeson() const
{
    return this->m_vICMSDeson;
}

void IcmsTot::set_vICMSDeson(const double &vICMSDeson)
{
    this->m_vICMSDeson = vICMSDeson;
}

double IcmsTot::get_vFCPUFDest() const
{
    return this->m_vFCPUFDest;
}

void IcmsTot::set_vFCPUFDest(const double &vFCPUFDest)
{
    this->m_vFCPUFDest = vFCPUFDest;
}

double IcmsTot::get_vICMSUFDest() const
{
    return this->m_vICMSUFDest;
}

void IcmsTot::set_vICMSUFDest(const double &vICMSUFDest)
{
    this->m_vICMSUFDest = vICMSUFDest;
}

double IcmsTot::get_vICMSUFRemet() const
{
    return this->m_vICMSUFRemet;
}

void IcmsTot::set_vICMSUFRemet(const double &vICMSUFRemet)
{
    this->m_vICMSUFRemet = vICMSUFRemet;
}

double IcmsTot::get_vFCP() const
{
    return this->m_vFCP;
}

void IcmsTot::set_vFCP(const double &vFCP)
{
    this->m_vFCP = vFCP;
}

double IcmsTot::get_vBCST() const
{
    return this->m_vBCST;
}

void IcmsTot::set_vBCST(const double &vBCST)
{
    this->m_vBCST = vBCST;
}

double IcmsTot::get_vST() const
{
    return this->m_vST;
}

void IcmsTot::set_vST(const double &vST)
{
    this->m_vST = vST;
}

double IcmsTot::get_vFCPST() const
{
    return this->m_vFCPST;
}

void IcmsTot::set_vFCPST(const double &vFCPST)
{
    this->m_vFCPST = vFCPST;
}

double IcmsTot::get_vFCPSTRet() const
{
    return this->m_vFCPSTRet;
}

void IcmsTot::set_vFCPSTRet(const double &vFCPSTRet)
{
    this->m_vFCPSTRet = vFCPSTRet;
}

double IcmsTot::get_vProd() const
{
    return this->m_vProd;
}

void IcmsTot::set_vProd(const double &vProd)
{
    this->m_vProd = vProd;
}

double IcmsTot::get_vFrete() const
{
    return this->m_vFrete;
}

void IcmsTot::set_vFrete(const double &vFrete)
{
    this->m_vFrete = vFrete;
}

double IcmsTot::get_vSeg() const
{
    return this->m_vSeg;
}

void IcmsTot::set_vSeg(const double &vSeg)
{
    this->m_vSeg = vSeg;
}

double IcmsTot::get_vDesc() const
{
    return this->m_vDesc;
}

void IcmsTot::set_vDesc(const double &vDesc)
{
    this->m_vDesc = vDesc;
}

double IcmsTot::get_vII() const
{
    return this->m_vII;
}

void IcmsTot::set_vII(const double &vII)
{
    this->m_vII = vII;
}

double IcmsTot::get_vIPI() const
{
    return this->m_vIPI;
}

void IcmsTot::set_vIPI(const double &vIPI)
{
    this->m_vIPI = vIPI;
}

double IcmsTot::get_vIPIDevol() const
{
    return this->m_vIPIDevol;
}

void IcmsTot::set_vIPIDevol(const double &vIPIDevol)
{
    this->m_vIPIDevol = vIPIDevol;
}

double IcmsTot::get_vPIS() const
{
    return this->m_vPIS;
}

void IcmsTot::set_vPIS(const double &vPIS)
{
    this->m_vPIS = vPIS;
}

double IcmsTot::get_vCOFINS() const
{
    return this->m_vCOFINS;
}

void IcmsTot::set_vCOFINS(const double &vCOFINS)
{
    this->m_vCOFINS = vCOFINS;
}

double IcmsTot::get_vOutro() const
{
    return this->m_vOutro;
}

void IcmsTot::set_vOutro(const double &vOutro)
{
    this->m_vOutro = vOutro;
}

double IcmsTot::get_vNF() const
{
    return this->m_vNF;
}

void IcmsTot::set_vNF(const double &vNF)
{
    this->m_vNF = vNF;
}

double IcmsTot::get_vTotTrib() const
{
    return this->m_vTotTrib;
}

void IcmsTot::set_vTotTrib(const double &vTotTrib)
{
    this->m_vTotTrib = vTotTrib;
}
