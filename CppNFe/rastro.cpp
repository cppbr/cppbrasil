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


#include "rastro.h"

Rastro::Rastro(): m_qLote(0.0)
{
}

Rastro::~Rastro()
{
}

void Rastro::clear()
{
    this->m_nLote.clear();
    this->m_qLote = 0.0;
    CppUtility::clearDateTime(this->m_dFab);
    CppUtility::clearDateTime(this->m_dVal);
    this->m_cAgreg.clear();
}

QString Rastro::get_nLote() const
{
    return this->m_nLote;
}

void Rastro::set_nLote(const QString &nLote)
{
    this->m_nLote = nLote;
}

double Rastro::get_qLote() const
{
    return this->m_qLote;
}

void Rastro::set_qLote(const double &qLote)
{
    this->m_qLote = qLote;
}

QDateTime Rastro::get_dFab() const
{
    return this->m_dFab;
}

void Rastro::set_dFab(const QDateTime &dFab)
{
    this->m_dFab = dFab;
}

QDateTime Rastro::get_dVal() const
{
    return this->m_dVal;
}

void Rastro::set_dVal(const QDateTime &dVal)
{
    this->m_dVal = dVal;
}

QString Rastro::get_cAgreg() const
{
    return this->m_cAgreg;
}

void Rastro::set_cAgreg(const QString &cAgreg)
{
    this->m_cAgreg = cAgreg;
}
