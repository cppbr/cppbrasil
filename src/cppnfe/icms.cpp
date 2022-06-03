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


#include "icms.h"

Icms::Icms(): m_orig(Orig::None), m_CST(CstICMS::None), m_CSOSN(CsosnICMS::None),
    m_modBC(ModBC::None), m_vBC(0.0), m_pICMS(0.0), m_vICMS(0.0), m_pFCP(0.0),
    m_vFCP(0.0), m_vBCFCP(0.0), m_modBCST(ModBCST::None), m_pMVAST(0.0),
    m_pRedBCST(0.0), m_vBCST(0.0), m_pICMSST(0.0), m_vICMSST(0.0), m_vBCFCPST(0.0), m_pFCPST(0.0),
    m_vFCPST(0.0), m_vICMSDeson(0.0), m_motDesICMS(MotDesICMS::None), m_vICMSOp(0.0),
    m_pDif(0.0), m_vICMSDif(0.0), m_vBCSTRet(0.0), m_pST(0.0), m_vICMSSubstituto(0.0),
    m_vICMSSTRet(0.0), m_vBCFCPSTRet(0.0), m_pFCPSTRet(0.0), m_vFCPSTRet(0.0), m_vBCSTDest(0.0),
    m_vICMSSTDest(0.0), m_pRedBCEfet(0.0), m_vBCEfet(0.0), m_pICMSEfet(0.0), m_vICMSEfet(0.0),
    m_pBCOp(0.0), m_pRedBC(0.0), m_pCredSN(0.0), m_vCredICMSSN(0.0), m_vICMSSTDeson(0.0),
    m_motDesICMSST(MotDesICMS::None), m_pFCPDif(0.0), m_vFCPDif(0.0), m_vFCPEfet(0.0)
{
}

Icms::~Icms()
{
}

void Icms::clear()
{
    this->m_orig = Orig::None;
    this->m_CST = CstICMS::None;
    this->m_CSOSN = CsosnICMS::None;
    this->m_modBC = ModBC::None;
    this->m_vBC = 0.0;
    this->m_pICMS = 0.0;
    this->m_vICMS = 0.0;
    this->m_pFCP = 0.0;
    this->m_vFCP = 0.0;
    this->m_vBCFCP = 0.0;
    this->m_modBCST = ModBCST::None;
    this->m_pMVAST = 0.0;
    this->m_pRedBCST = 0.0;
    this->m_vBCST = 0.0;
    this->m_pICMSST = 0.0;
    this->m_vICMSST = 0.0;
    this->m_vBCFCPST = 0.0;
    this->m_pFCPST = 0.0;
    this->m_vFCPST = 0.0;
    this->m_vICMSDeson = 0.0;
    this->m_motDesICMS = MotDesICMS::None;
    this->m_vICMSOp = 0.0;
    this->m_pDif = 0.0;
    this->m_vICMSDif = 0.0;
    this->m_vBCSTRet = 0.0;
    this->m_pST = 0.0;
    this->m_vICMSSubstituto = 0.0;
    this->m_vICMSSTRet = 0.0;
    this->m_vBCFCPSTRet = 0.0;
    this->m_pFCPSTRet = 0.0;
    this->m_vFCPSTRet = 0.0;
    this->m_vBCSTDest = 0.0;
    this->m_vICMSSTDest = 0.0;
    this->m_pRedBCEfet = 0.0;
    this->m_vBCEfet = 0.0;
    this->m_pICMSEfet = 0.0;
    this->m_vICMSEfet = 0.0;
    this->m_pBCOp = 0.0;
    this->m_UFST.clear();
    this->m_pRedBC = 0.0;
    this->m_pCredSN = 0.0;
    this->m_vCredICMSSN = 0.0;
    this->m_vICMSSTDeson = 0.0;
    this->m_motDesICMSST = MotDesICMS::None;
    this->m_pFCPDif = 0.0;
    this->m_vFCPDif = 0.0;
    this->m_vFCPEfet = 0.0;

}

Orig Icms::get_orig() const
{
    return this->m_orig;
}

void Icms::set_orig(const Orig &orig)
{
    this->m_orig = orig;
}

CstICMS Icms::get_CST() const
{
    return this->m_CST;
}

void Icms::set_CST(const CstICMS &CST)
{
    this->m_CST = CST;
}

CsosnICMS Icms::get_CSOSN() const
{
    return this->m_CSOSN;
}

void Icms::set_CSOSN(const CsosnICMS &CSOSN)
{
    this->m_CSOSN = CSOSN;
}

ModBC Icms::get_modBC() const
{
    return this->m_modBC;
}

void Icms::set_modBC(const ModBC &modBC)
{
    this->m_modBC = modBC;
}

double Icms::get_vBC() const
{
    return this->m_vBC;
}

void Icms::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double Icms::get_pICMS() const
{
    return this->m_pICMS;
}

void Icms::set_pICMS(const double &pICMS)
{
    this->m_pICMS = pICMS;
}

double Icms::get_vICMS() const
{
    return this->m_vICMS;
}

void Icms::set_vICMS(const double &vICMS)
{
    this->m_vICMS = vICMS;
}

double Icms::get_pFCP() const
{
    return this->m_pFCP;
}

void Icms::set_pFCP(const double &pFCP)
{
    this->m_pFCP = pFCP;
}

double Icms::get_vFCP() const
{
    return this->m_vFCP;
}

void Icms::set_vFCP(const double &vFCP)
{
    this->m_vFCP = vFCP;
}

double Icms::get_vBCFCP() const
{
    return this->m_vBCFCP;
}

void Icms::set_vBCFCP(const double &vBCFCP)
{
    this->m_vBCFCP = vBCFCP;
}

ModBCST Icms::get_modBCST() const
{
    return this->m_modBCST;
}

void Icms::set_modBCST(const ModBCST &modBCST)
{
    this->m_modBCST = modBCST;
}

double Icms::get_pMVAST() const
{
    return this->m_pMVAST;
}

void Icms::set_pMVAST(const double &pMVAST)
{
    this->m_pMVAST = pMVAST;
}

double Icms::get_pRedBCST() const
{
    return this->m_pRedBCST;
}

void Icms::set_pRedBCST(const double &pRedBCST)
{
    this->m_pRedBCST = pRedBCST;
}

double Icms::get_vBCST() const
{
    return this->m_vBCST;
}

void Icms::set_vBCST(const double &vBCST)
{
    this->m_vBCST = vBCST;
}

double Icms::get_pICMSST() const
{
    return this->m_pICMSST;
}

void Icms::set_pICMSST(const double &pICMSST)
{
    this->m_pICMSST = pICMSST;
}

double Icms::get_vICMSST() const
{
    return this->m_vICMSST;
}

void Icms::set_vICMSST(const double &vICMSST)
{
    this->m_vICMSST = vICMSST;
}

double Icms::get_vBCFCPST() const
{
    return this->m_vBCFCPST;
}

void Icms::set_vBCFCPST(const double &vBCFCPST)
{
    this->m_vBCFCPST = vBCFCPST;
}

double Icms::get_pFCPST() const
{
    return this->m_pFCPST;
}

void Icms::set_pFCPST(const double &pFCPST)
{
    this->m_pFCPST = pFCPST;
}

double Icms::get_vFCPST() const
{
    return this->m_vFCPST;
}

void Icms::set_vFCPST(const double &vFCPST)
{
    this->m_vFCPST = vFCPST;
}

double Icms::get_vICMSDeson() const
{
    return this->m_vICMSDeson;
}

void Icms::set_vICMSDeson(const double &vICMSDeson)
{
    this->m_vICMSDeson = vICMSDeson;
}

MotDesICMS Icms::get_motDesICMS() const
{
    return this->m_motDesICMS;
}

void Icms::set_motDesICMS(const MotDesICMS &motDesICMS)
{
    this->m_motDesICMS = motDesICMS;
}

double Icms::get_vICMSOp() const
{
    return this->m_vICMSOp;
}

void Icms::set_vICMSOp(const double &vICMSOp)
{
    this->m_vICMSOp = vICMSOp;
}

double Icms::get_pDif() const
{
    return this->m_pDif;
}

void Icms::set_pDif(const double &pDif)
{
    this->m_pDif = pDif;
}

double Icms::get_vICMSDif() const
{
    return this->m_vICMSDif;
}

void Icms::set_vICMSDif(const double &vICMSDif)
{
    this->m_vICMSDif = vICMSDif;
}

double Icms::get_vBCSTRet() const
{
    return this->m_vBCSTRet;
}

void Icms::set_vBCSTRet(const double &vBCSTRet)
{
    this->m_vBCSTRet = vBCSTRet;
}

double Icms::get_pST() const
{
    return this->m_pST;
}

void Icms::set_pST(const double &pST)
{
    this->m_pST = pST;
}

double Icms::get_vICMSSubstituto() const
{
    return this->m_vICMSSubstituto;
}

void Icms::set_vICMSSubstituto(const double &vICMSSubstituto)
{
    this->m_vICMSSubstituto = vICMSSubstituto;
}

double Icms::get_vICMSSTRet() const
{
    return this->m_vICMSSTRet;
}

void Icms::set_vICMSSTRet(const double &vICMSSTRet)
{
    this->m_vICMSSTRet = vICMSSTRet;
}

double Icms::get_vBCFCPSTRet() const
{
    return this->m_vBCFCPSTRet;
}

void Icms::set_vBCFCPSTRet(const double &vBCFCPSTRet)
{
    this->m_vBCFCPSTRet = vBCFCPSTRet;
}

double Icms::get_pFCPSTRet() const
{
    return this->m_pFCPSTRet;
}

void Icms::set_pFCPSTRet(const double &pFCPSTRet)
{
    this->m_pFCPSTRet = pFCPSTRet;
}

double Icms::get_vFCPSTRet() const
{
    return this->m_vFCPSTRet;
}

void Icms::set_vFCPSTRet(const double &vFCPSTRet)
{
    this->m_vFCPSTRet = vFCPSTRet;
}

double Icms::get_vBCSTDest() const
{
    return this->m_vBCSTDest;
}

void Icms::set_vBCSTDest(const double &vBCSTDest)
{
    this->m_vBCSTDest = vBCSTDest;
}

double Icms::get_vICMSSTDest() const
{
    return this->m_vICMSSTDest;
}

void Icms::set_vICMSSTDest(const double &vICMSSTDest)
{
    this->m_vICMSSTDest = vICMSSTDest;
}

double Icms::get_pRedBCEfet() const
{
    return this->m_pRedBCEfet;
}

void Icms::set_pRedBCEfet(const double &pRedBCEfet)
{
    this->m_pRedBCEfet = pRedBCEfet;
}

double Icms::get_vBCEfet() const
{
    return this->m_vBCEfet;
}

void Icms::set_vBCEfet(const double &vBCEfet)
{
    this->m_vBCEfet = vBCEfet;
}

double Icms::get_pICMSEfet() const
{
    return this->m_pICMSEfet;
}

void Icms::set_pICMSEfet(const double &pICMSEfet)
{
    this->m_pICMSEfet = pICMSEfet;
}

double Icms::get_vICMSEfet() const
{
    return this->m_vICMSEfet;
}

void Icms::set_vICMSEfet(const double &vICMSEfet)
{
    this->m_vICMSEfet = vICMSEfet;
}

double Icms::get_pBCOp() const
{
    return this->m_pBCOp;
}

void Icms::set_pBCOp(const double &pBCOp)
{
    this->m_pBCOp = pBCOp;
}

QString Icms::get_UFST() const
{
    return this->m_UFST;
}

void Icms::set_UFST(const QString &UFST)
{
    this->m_UFST = UFST;
}

double Icms::get_pRedBC() const
{
    return this->m_pRedBC;
}

void Icms::set_pRedBC(const double &pRedBC)
{
    this->m_pRedBC = pRedBC;
}

double Icms::get_pCredSN() const
{
    return this->m_pCredSN;
}

void Icms::set_pCredSN(const double &pCredSN)
{
    this->m_pCredSN = pCredSN;
}

double Icms::get_vCredICMSSN() const
{
    return this->m_vCredICMSSN;
}

void Icms::set_vCredICMSSN(const double &vCredICMSSN)
{
    this->m_vCredICMSSN = vCredICMSSN;
}

double Icms::get_vICMSSTDeson() const
{
    return this->m_vICMSSTDeson;
}

void Icms::set_vICMSSTDeson(const double &vICMSSTDeson)
{
    this->m_vICMSSTDeson = vICMSSTDeson;
}

MotDesICMS Icms::get_motDesICMSST() const
{
    return this->m_motDesICMSST;
}

void Icms::set_motDesICMSST(const MotDesICMS &motDesICMSST)
{
    this->m_motDesICMSST = motDesICMSST;
}

double Icms::get_pFCPDif() const
{
    return this->m_pFCPDif;
}

void Icms::set_pFCPDif(const double &pFCPDif)
{
    this->m_pFCPDif = pFCPDif;
}

double Icms::get_vFCPDif() const
{
    return this->m_vFCPDif;
}

void Icms::set_vFCPDif(const double &vFCPDif)
{
    this->m_vFCPDif = vFCPDif;
}

double Icms::get_vFCPEfet() const
{
    return this->m_vFCPEfet;
}

void Icms::set_vFCPEfet(const double &vFCPEfet)
{
    this->m_vFCPEfet = vFCPEfet;
}
