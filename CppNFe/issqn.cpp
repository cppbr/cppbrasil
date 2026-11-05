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


#include "issqn.h"

IssQN::IssQN() :m_vBC(0.0), m_vAliq(0.0), m_vISSQN(0.0), m_cMunFG(0), m_vDeducao(0.0),
    m_vOutro(0.0), m_vDescIncond(0.0), m_vDescCond(0.0), m_vISSRet(0.0), m_indISS(IndISS::None),
    m_cMun(0), m_cPais(0), m_indIncentivo(IndIncentivo::None)
{
}

IssQN::~IssQN()
{
}

void IssQN::clear()
{
    this->m_vBC = 0.0;
    this->m_vAliq = 0.0;
    this->m_vISSQN = 0.0;
    this->m_cMunFG = 0;
    this->m_cListServ.clear();
    this->m_vDeducao = 0.0;
    this->m_vOutro = 0.0;
    this->m_vDescIncond = 0.0;
    this->m_vDescCond = 0.0;
    this->m_vISSRet = 0.0;
    this->m_indISS = IndISS::None;
    this->m_cServico.clear();
    this->m_cMun = 0;
    this->m_cPais = 0;
    this->m_nProcesso.clear();
    this->m_indIncentivo = IndIncentivo::None;
}

double IssQN::get_vBC() const
{
    return this->m_vBC;
}

void IssQN::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double IssQN::get_vAliq() const
{
    return this->m_vAliq;
}

void IssQN::set_vAliq(const double &vAliq)
{
    this->m_vAliq = vAliq;
}

double IssQN::get_vISSQN() const
{
    return this->m_vISSQN;
}

void IssQN::set_vISSQN(const double &vISSQN)
{
    this->m_vISSQN = vISSQN;
}

int IssQN::get_cMunFG() const
{
    return this->m_cMunFG;
}

void IssQN::set_cMunFG(const int &cMunFG)
{
    this->m_cMunFG = cMunFG;
}

QString IssQN::get_cListServ() const
{
    return this->m_cListServ;
}

void IssQN::set_cListServ(const QString &cListServ)
{
    this->m_cListServ = cListServ;
}

double IssQN::get_vDeducao() const
{
    return this->m_vDeducao;
}

void IssQN::set_vDeducao(const double &vDeducao)
{
    this->m_vDeducao = vDeducao;
}

double IssQN::get_vOutro() const
{
    return this->m_vOutro;
}

void IssQN::set_vOutro(const double &vOutro)
{
    this->m_vOutro = vOutro;
}

double IssQN::get_vDescIncond() const
{
    return this->m_vDescIncond;
}

void IssQN::set_vDescIncond(const double &vDescIncond)
{
    this->m_vDescIncond = vDescIncond;
}

double IssQN::get_vDescCond() const
{
    return this->m_vDescCond;
}

void IssQN::set_vDescCond(const double &vDescCond)
{
    this->m_vDescCond = vDescCond;
}

double IssQN::get_vISSRet() const
{
    return this->m_vISSRet;
}

void IssQN::set_vISSRet(const double &vISSRet)
{
    this->m_vISSRet = vISSRet;
}

IndISS IssQN::get_indISS() const
{
    return this->m_indISS;
}

void IssQN::set_indISS(const IndISS &indISS)
{
    this->m_indISS = indISS;
}

QString IssQN::get_cServico() const
{
    return this->m_cServico;
}

void IssQN::set_cServico(const QString &cServico)
{
    this->m_cServico = cServico;
}

int IssQN::get_cMun() const
{
    return this->m_cMun;
}

void IssQN::set_cMun(const int &cMun)
{
    this->m_cMun = cMun;
}

int IssQN::get_cPais() const
{
    return this->m_cPais;
}

void IssQN::set_cPais(const int &cPais)
{
    this->m_cPais = cPais;
}

QString IssQN::get_nProcesso() const
{
    return this->m_nProcesso;
}

void IssQN::set_nProcesso(const QString &nProcesso)
{
    this->m_nProcesso = nProcesso;
}

IndIncentivo IssQN::get_indIncentivo() const
{
    return this->m_indIncentivo;
}

void IssQN::set_indIncentivo(const IndIncentivo &indIncentivo)
{
    this->m_indIncentivo = indIncentivo;
}
