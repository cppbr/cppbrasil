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


#ifndef RETTRIB_H
#define RETTRIB_H

#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT RetTrib
{
public:
    RetTrib();
    ~RetTrib();
    void clear();
    double get_vRetPIS() const;
    void set_vRetPIS(const double &vRetPIS);
    double get_vRetCOFINS() const;
    void set_vRetCOFINS(const double &vRetCOFINS);
    double get_vRetCSLL() const;
    void set_vRetCSLL(const double &vRetCSLL);
    double get_vBCIRRF() const;
    void set_vBCIRRF(const double &vBCIRRF);
    double get_vIRRF() const;
    void set_vIRRF(const double &vIRRF);
    double get_vBCRetPrev() const;
    void set_vBCRetPrev(const double &vBCRetPrev);
    double get_vRetPrev() const;
    void set_vRetPrev(const double &vRetPrev);

private:
    double m_vRetPIS;
    double m_vRetCOFINS;
    double m_vRetCSLL;
    double m_vBCIRRF;
    double m_vIRRF;
    double m_vBCRetPrev;
    double m_vRetPrev;
};

#endif // RETTRIB_H
