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


#ifndef RETTRANSP_H
#define RETTRANSP_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT RetTransp
{
public:
    RetTransp();
    ~RetTransp();
    void clear();
    double get_vServ() const;
    void set_vServ(const double &vServ);
    double get_vBCRet() const;
    void set_vBCRet(const double &vBCRet);
    double get_pICMSRet() const;
    void set_pICMSRet(const double &pICMSRet);
    double get_vICMSRet() const;
    void set_vICMSRet(const double &vICMSRet);
    QString get_CFOP() const;
    void set_CFOP(const QString &CFOP);
    int get_cMunFG() const;
    void set_cMunFG(const int &cMunFG);

private:
    double m_vServ;
    double m_vBCRet;
    double m_pICMSRet;
    double m_vICMSRet;
    QString m_CFOP;
    int m_cMunFG;

};

#endif // RETTRANSP_H
