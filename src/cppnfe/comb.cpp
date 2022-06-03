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


#include "comb.h"

Comb::Comb() :
    CIDE(new Cide), encerrante(new Encerrante),
    m_cProdANP(0), m_pGLP(0.0), m_pGNn(0.0), m_pGNi(0.0), m_vPart(0.0), m_qTemp(0.0)
{

}

Comb::~Comb()
{
}

void Comb::clear()
{
    this->m_cProdANP = 0;
    this->m_descANP.clear();
    this->m_pGLP = 0.0;
    this->m_pGNn = 0.0;
    this->m_pGNi = 0.0;
    this->m_vPart = 0.0;
    this->m_CODIF.clear();
    this->m_qTemp = 0.0;
    this->m_UFCons.clear();

    this->CIDE->clear();
    this->encerrante->clear();
}

int Comb::get_cProdANP() const
{
    return this->m_cProdANP;
}

void Comb::set_cProdANP(const int &cProdANP)
{
    this->m_cProdANP = cProdANP;
}

QString Comb::get_descANP() const
{
    return this->m_descANP;
}

void Comb::set_descANP(const QString &descANP)
{
    this->m_descANP = descANP;
}

double Comb::get_pGLP() const
{
    return this->m_pGLP;
}

void Comb::set_pGLP(const double &pGLP)
{
    this->m_pGLP = pGLP;
}

double Comb::get_pGNn() const
{
    return this->m_pGNn;
}

void Comb::set_pGNn(const double &pGNn)
{
    this->m_pGNn = pGNn;
}

double Comb::get_pGNi() const
{
    return this->m_pGNi;
}

void Comb::set_pGNi(const double &pGNi)
{
    this->m_pGNi = pGNi;
}

double Comb::get_vPart() const
{
    return this->m_vPart;
}

void Comb::set_vPart(const double &vPart)
{
    this->m_vPart = vPart;
}

QString Comb::get_CODIF() const
{
    return this->m_CODIF;
}

void Comb::set_CODIF(const QString &CODIF)
{
    this->m_CODIF = CODIF;
}

double Comb::get_qTemp() const
{
    return this->m_qTemp;
}

void Comb::set_qTemp(const double &qTemp)
{
    this->m_qTemp = qTemp;
}

QString Comb::get_UFCons() const
{
    return this->m_UFCons;
}

void Comb::set_UFCons(const QString &UFCons)
{
    this->m_UFCons = UFCons;
}
