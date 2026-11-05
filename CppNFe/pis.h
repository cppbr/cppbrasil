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


#ifndef PIS_H
#define PIS_H

#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT Pis
{
public:
    Pis();
    ~Pis();
    void clear();
    CstPIS get_CST() const;
    void set_CST(const CstPIS &CST);
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_pPIS() const;
    void set_pPIS(const double &pPIS);
    double get_vPIS() const;
    void set_vPIS(const double &vPIS);
    double get_qBCProd() const;
    void set_qBCProd(const double &qBCProd);
    double get_vAliqProd() const;
    void set_vAliqProd(const double &vAliqProd);
private:
    CstPIS m_CST;
    double m_vBC;
    double m_pPIS;
    double m_vPIS;
    double m_qBCProd;
    double m_vAliqProd;

};

#endif // PIS_H
