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


#ifndef ICMSUFDEST_H
#define ICMSUFDEST_H

#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT IcmsUFDest
{
public:
    IcmsUFDest();
    ~IcmsUFDest();
    void clear();
    double get_vBCUFDest() const;
    void set_vBCUFDest(const double &vBCUFDest);
    double get_vBCFCPUFDest() const;
    void set_vBCFCPUFDest(const double &vBCFCPUFDest);
    double get_pFCPUFDest() const;
    void set_pFCPUFDest(const double &pFCPUFDest);
    double get_pICMSUFDest() const;
    void set_pICMSUFDest(const double &pICMSUFDest);
    double get_pICMSInter() const;
    void set_pICMSInter(const double &pICMSInter);
    double get_pICMSInterPart() const;
    void set_pICMSInterPart(const double &pICMSInterPart);
    double get_vFCPUFDest() const;
    void set_vFCPUFDest(const double &vFCPUFDest);
    double get_vICMSUFDest() const;
    void set_vICMSUFDest(const double &vICMSUFDest);
    double get_vICMSUFRemet() const;
    void set_vICMSUFRemet(const double &vICMSUFRemet);

private:
    double m_vBCUFDest;
    double m_vBCFCPUFDest;
    double m_pFCPUFDest;
    double m_pICMSUFDest;
    double m_pICMSInter;
    double m_pICMSInterPart;
    double m_vFCPUFDest;
    double m_vICMSUFDest;
    double m_vICMSUFRemet;

};

#endif // ICMSUFDEST_H
