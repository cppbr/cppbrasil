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


#include "avulsa.h"

Avulsa::Avulsa(): m_vDAR(0.0)
{
}

Avulsa::~Avulsa()
{
}

void Avulsa::clear()
{
    this->m_CNPJ.clear();
    this->m_xOrgao.clear();
    this->m_matr.clear();
    this->m_xAgente.clear();
    this->m_fone.clear();
    this->m_UF.clear();
    this->m_nDAR.clear();
    CppUtility::clearDateTime(this->m_dEmi);
    this->m_vDAR = 0.0;
    this->m_repEmi.clear();
    CppUtility::clearDateTime(this->m_dPag);
}

QString Avulsa::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Avulsa::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString Avulsa::get_xOrgao() const
{
    return this->m_xOrgao;
}

void Avulsa::set_xOrgao(const QString &xOrgao)
{
    this->m_xOrgao = xOrgao;
}

QString Avulsa::get_matr() const
{
    return this->m_matr;
}

void Avulsa::set_matr(const QString &matr)
{
    this->m_matr = matr;
}

QString Avulsa::get_xAgente() const
{
    return this->m_xAgente;
}

void Avulsa::set_xAgente(const QString &xAgente)
{
    this->m_xAgente = xAgente;
}

QString Avulsa::get_fone() const
{
    return this->m_fone;
}

void Avulsa::set_fone(const QString &fone)
{
    this->m_fone = fone;
}

QString Avulsa::get_UF() const
{
    return this->m_UF;
}

void Avulsa::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

QString Avulsa::get_nDAR() const
{
    return this->m_nDAR;
}

void Avulsa::set_nDAR(const QString &nDAR)
{
    this->m_nDAR = nDAR;
}

QDateTime Avulsa::get_dEmi() const
{
    return this->m_dEmi;
}

void Avulsa::set_dEmi(const QDateTime &dEmi)
{
    this->m_dEmi = dEmi;
}

double Avulsa::get_vDAR() const
{
    return this->m_vDAR;
}

void Avulsa::set_vDAR(const double &vDAR)
{
    this->m_vDAR = vDAR;
}

QString Avulsa::get_repEmi() const
{
    return this->m_repEmi;
}

void Avulsa::set_repEmi(const QString &repEmi)
{
    this->m_repEmi = repEmi;
}

QDateTime Avulsa::get_dPag() const
{
    return this->m_dPag;
}

void Avulsa::set_dPag(const QDateTime &dPag)
{
    this->m_dPag = dPag ;
}
