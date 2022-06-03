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


#ifndef PISST_H
#define PISST_H

#include "convnf.h"


#ifdef CPPBRASIL_STATIC
    class PisST
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT PisST
#endif
{
public:
    PisST();
    ~PisST();
    void clear();
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_pPIS() const;
    void set_pPIS(const double &pPIS);
    double get_qBCProd() const;
    void set_qBCProd(const double &qBCProd);
    double get_vAliqProd() const;
    void set_vAliqProd(const double &vAliqProd);
    double get_vPIS() const;
    void set_vPIS(const double &vPIS);
    IndTot get_indSomaPISST() const;
    void set_indSomaPISST(const IndTot &indSomaPISST);
private:
    double m_vBC;
    double m_pPIS;
    double m_qBCProd;
    double m_vAliqProd;
    double m_vPIS;
    IndTot m_indSomaPISST;
};

#endif // PISST_H
