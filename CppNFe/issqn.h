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


#ifndef ISSQN_H
#define ISSQN_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT IssQN
{
public:
    IssQN();
    ~IssQN();
    void clear();
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_vAliq() const;
    void set_vAliq(const double &vAliq);
    double get_vISSQN() const;
    void set_vISSQN(const double &vISSQN);
    int get_cMunFG() const;
    void set_cMunFG(const int &cMunFG);
    QString get_cListServ() const;
    void set_cListServ(const QString &cListServ);
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
    IndISS get_indISS() const;
    void set_indISS(const IndISS &indISS);
    QString get_cServico() const;
    void set_cServico(const QString &cServico);
    int get_cMun() const;
    void set_cMun(const int &cMun);
    int get_cPais() const;
    void set_cPais(const int &cPais);
    QString get_nProcesso() const;
    void set_nProcesso(const QString &nProcesso);
    IndIncentivo get_indIncentivo() const;
    void set_indIncentivo(const IndIncentivo &indIncentivo);

private:
    double m_vBC;
    double m_vAliq;
    double m_vISSQN;
    int m_cMunFG;
    QString m_cListServ;
    double m_vDeducao;
    double m_vOutro;
    double m_vDescIncond;
    double m_vDescCond;
    double m_vISSRet;
    IndISS m_indISS;
    QString m_cServico;
    int m_cMun;
    int m_cPais;
    QString m_nProcesso;
    IndIncentivo m_indIncentivo;

};

#endif // ISSQN_H
