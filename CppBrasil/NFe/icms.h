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


#ifndef ICMS_H
#define ICMS_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPBRASIL_EXPORT Icms
{
public:
    Icms();
    ~Icms();
    void clear();
    Orig get_orig() const;
    void set_orig(const Orig &orig);
    CstICMS get_CST() const;
    void set_CST(const CstICMS &CST);
    CsosnICMS get_CSOSN() const;
    void set_CSOSN(const CsosnICMS &CSOSN);
    ModBC get_modBC() const;
    void set_modBC(const ModBC &modBC);
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_pICMS() const;
    void set_pICMS(const double &pICMS);
    double get_vICMS() const;
    void set_vICMS(const double &vICMS);
    double get_pFCP() const;
    void set_pFCP(const double &pFCP);
    double get_vFCP() const;
    void set_vFCP(const double &vFCP);
    double get_vBCFCP() const;
    void set_vBCFCP(const double &vBCFCP);
    ModBCST get_modBCST() const;
    void set_modBCST(const ModBCST &modBCST);
    double get_pMVAST() const;
    void set_pMVAST(const double &pMVAST);
    double get_pRedBCST() const;
    void set_pRedBCST(const double &pRedBCST);
    double get_vBCST() const;
    void set_vBCST(const double &vBCST);
    double get_pICMSST() const;
    void set_pICMSST(const double &pICMSST);
    double get_vICMSST() const;
    void set_vICMSST(const double &vICMSST);
    double get_vBCFCPST() const;
    void set_vBCFCPST(const double &vBCFCPST);
    double get_pFCPST() const;
    void set_pFCPST(const double &pFCPST);
    double get_vFCPST() const;
    void set_vFCPST(const double &vFCPST);
    double get_vICMSDeson() const;
    void set_vICMSDeson(const double &vICMSDeson);
    MotDesICMS get_motDesICMS() const;
    void set_motDesICMS(const MotDesICMS &motDesICMS);
    double get_vICMSOp() const;
    void set_vICMSOp(const double &vICMSOp);
    double get_pDif() const;
    void set_pDif(const double &pDif);
    double get_vICMSDif() const;
    void set_vICMSDif(const double &vICMSDif);
    double get_vBCSTRet() const;
    void set_vBCSTRet(const double &vBCSTRet);
    double get_pST() const;
    void set_pST(const double &pST);
    double get_vICMSSubstituto() const;
    void set_vICMSSubstituto(const double &vICMSSubstituto);
    double get_vICMSSTRet() const;
    void set_vICMSSTRet(const double &vICMSSTRet);
    double get_vBCFCPSTRet() const;
    void set_vBCFCPSTRet(const double &vBCFCPSTRet);
    double get_pFCPSTRet() const;
    void set_pFCPSTRet(const double &pFCPSTRet);
    double get_vFCPSTRet() const;
    void set_vFCPSTRet(const double &vFCPSTRet);
    double get_vBCSTDest() const;
    void set_vBCSTDest(const double &vBCSTDest);
    double get_vICMSSTDest() const;
    void set_vICMSSTDest(const double &vICMSSTDest);
    double get_pRedBCEfet() const;
    void set_pRedBCEfet(const double &pRedBCEfet);
    double get_vBCEfet() const;
    void set_vBCEfet(const double &vBCEfet);
    double get_pICMSEfet() const;
    void set_pICMSEfet(const double &pICMSEfet);
    double get_vICMSEfet() const;
    void set_vICMSEfet(const double &vICMSEfet);
    double get_pBCOp() const;
    void set_pBCOp(const double &pBCOp);
    QString get_UFST() const;
    void set_UFST(const QString &UFST);
    double get_pRedBC() const;
    void set_pRedBC(const double &pRedBC);
    double get_pCredSN() const;
    void set_pCredSN(const double &pCredSN);
    double get_vCredICMSSN() const;
    void set_vCredICMSSN(const double &vCredICMSSN);
    double get_vICMSSTDeson() const;
    void set_vICMSSTDeson(const double &vICMSSTDeson);
    MotDesICMS get_motDesICMSST() const;
    void set_motDesICMSST(const MotDesICMS &motDesICMSST);
    double get_pFCPDif() const;
    void set_pFCPDif(const double &pFCPDif);
    double get_vFCPDif() const;
    void set_vFCPDif(const double &vFCPDif);
    double get_vFCPEfet() const;
    void set_vFCPEfet(const double &vFCPEfet);

private:
    Orig m_orig;
    CstICMS m_CST;
    CsosnICMS m_CSOSN;
    ModBC m_modBC;
    double m_vBC;
    double m_pICMS;
    double m_vICMS;
    double m_pFCP;
    double m_vFCP;
    double m_vBCFCP;
    ModBCST m_modBCST;
    double m_pMVAST;
    double m_pRedBCST;
    double m_vBCST;
    double m_pICMSST;
    double m_vICMSST;
    double m_vBCFCPST;
    double m_pFCPST;
    double m_vFCPST;
    double m_vICMSDeson;
    MotDesICMS m_motDesICMS;
    double m_vICMSOp;
    double m_pDif;
    double m_vICMSDif;
    double m_vBCSTRet;
    double m_pST;
    double m_vICMSSubstituto;
    double m_vICMSSTRet;
    double m_vBCFCPSTRet;
    double m_pFCPSTRet;
    double m_vFCPSTRet;
    double m_vBCSTDest;
    double m_vICMSSTDest;
    double m_pRedBCEfet;
    double m_vBCEfet;
    double m_pICMSEfet;
    double m_vICMSEfet;
    double m_pBCOp;
    QString m_UFST;
    double m_pRedBC;
    double m_pCredSN;
    double m_vCredICMSSN;
    double m_vICMSSTDeson;
    MotDesICMS m_motDesICMSST;
    double m_pFCPDif;
    double m_vFCPDif;
    double m_vFCPEfet;
};

#endif // ICMS_H
