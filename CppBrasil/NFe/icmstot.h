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


#ifndef ICMSTOT_H
#define ICMSTOT_H
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT IcmsTot
{
public:
    IcmsTot();
    ~IcmsTot();
    void clear();
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_vICMS() const;
    void set_vICMS(const double &vICMS);
    double get_vICMSDeson() const;
    void set_vICMSDeson(const double &vICMSDeson);
    double get_vFCPUFDest() const;
    void set_vFCPUFDest(const double &vFCPUFDest);
    double get_vICMSUFDest() const;
    void set_vICMSUFDest(const double &vICMSUFDest);
    double get_vICMSUFRemet() const;
    void set_vICMSUFRemet(const double &vICMSUFRemet);
    double get_vFCP() const;
    void set_vFCP(const double &vFCP);
    double get_vBCST() const;
    void set_vBCST(const double &vBCST);
    double get_vST() const;
    void set_vST(const double &vST);
    double get_vFCPST() const;
    void set_vFCPST(const double &vFCPST);
    double get_vFCPSTRet() const;
    void set_vFCPSTRet(const double &vFCPSTRet);
    double get_vProd() const;
    void set_vProd(const double &vProd);
    double get_vFrete() const;
    void set_vFrete(const double &vFrete);
    double get_vSeg() const;
    void set_vSeg(const double &vSeg);
    double get_vDesc() const;
    void set_vDesc(const double &vDesc);
    double get_vII() const;
    void set_vII(const double &vII);
    double get_vIPI() const;
    void set_vIPI(const double &vIPI);
    double get_vIPIDevol() const;
    void set_vIPIDevol(const double &vIPIDevol);
    double get_vPIS() const;
    void set_vPIS(const double &vPIS);
    double get_vCOFINS() const;
    void set_vCOFINS(const double &vCOFINS);
    double get_vOutro() const;
    void set_vOutro(const double &vOutro);
    double get_vNF() const;
    void set_vNF(const double &vNF);
    double get_vTotTrib() const;
    void set_vTotTrib(const double &vTotTrib);

private:
    double m_vBC;
    double m_vICMS;
    double m_vICMSDeson;
    double m_vFCPUFDest;
    double m_vICMSUFDest;
    double m_vICMSUFRemet;
    double m_vFCP;
    double m_vBCST;
    double m_vST;
    double m_vFCPST;
    double m_vFCPSTRet;
    double m_vProd;
    double m_vFrete;
    double m_vSeg;
    double m_vDesc;
    double m_vII;
    double m_vIPI;
    double m_vIPIDevol;
    double m_vPIS;
    double m_vCOFINS;
    double m_vOutro;
    double m_vNF;
    double m_vTotTrib;
};

#endif // ICMSTOT_H
