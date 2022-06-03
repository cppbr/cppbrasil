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


#ifndef ISSQNTOT_H
#define ISSQNTOT_H
#include <QDateTime>
#include "convnf.h"
#include "cpputility.h"

#ifdef CPPBRASIL_STATIC
    class IssQNtot
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT IssQNtot
#endif
{
public:
    IssQNtot();
    ~IssQNtot();
    void clear();
    double get_vServ() const;
    void set_vServ(const double &vServ);
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_vISS() const;
    void set_vISS(const double &vISS);
    double get_vPIS() const;
    void set_vPIS(const double &vPIS);
    double get_vCOFINS() const;
    void set_vCOFINS(const double &vCOFINS);
    QDateTime get_dCompet() const;
    void set_dCompet(const QDateTime &dCompet);
    double get_vDeducao() const;
    void set_vDeducao(const double &vDeducao);
    double get_vOutro() const;
    void set_vOutro(const double &vOutro);
    double get_vDescIncond() const;
    void set_vDescIncond(const double &vDescIncond);
    double get_vDescCond() const;
    void set_vDescCond(const double &vDescCond);
    double get_vISSRet() const;
    void set_vISSRet(const double &vISSRet);
    CRegTrib get_cRegTrib() const;
    void set_cRegTrib(const CRegTrib &cRegTrib);

private:
    double m_vServ;
    double m_vBC;
    double m_vISS;
    double m_vPIS;
    double m_vCOFINS;
    QDateTime m_dCompet;
    double m_vDeducao;
    double m_vOutro;
    double m_vDescIncond;
    double m_vDescCond;
    double m_vISSRet;
    CRegTrib m_cRegTrib;

};

#endif // ISSQNTOT_H
