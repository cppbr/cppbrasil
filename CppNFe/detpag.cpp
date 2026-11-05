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


#include "detpag.h"

DetPag::DetPag() :
    card(new Card), m_indPag(IndPag::None), m_tPag(TPag::None), m_vPag(0.0),
    m_UFPag(UF::None), m_UFReceb(UF::None)
{
}

DetPag::~DetPag()
{
}

void DetPag::clear()
{
    this->m_indPag = IndPag::None;
    this->m_tPag = TPag::None;
    this->m_vPag = 0.0;
    this->m_xPag.clear();
    this->m_dPag = QDate();
    this->m_CNPJPag = QString();
    this->m_UFPag = UF::None;
    this->m_CNPJReceb = QString();
    this->m_UFReceb = UF::None;

    this->card->clear();
}

IndPag DetPag::get_indPag() const
{
    return this->m_indPag;
}

void DetPag::set_indPag(const IndPag &indPag)
{
    this->m_indPag = indPag;
}

TPag DetPag::get_tPag() const
{
    return this->m_tPag;
}

void DetPag::set_tPag(const TPag &tPag)
{
    this->m_tPag = tPag;
}

double DetPag::get_vPag() const
{
    return this->m_vPag;
}

void DetPag::set_vPag(const double &vPag)
{
    this->m_vPag = vPag;
}

QString DetPag::get_xPag() const
{
    return this->m_xPag;
}

void DetPag::set_xPag(const QString &xPag)
{
    this->m_xPag = xPag;
}

QDate DetPag::get_dPag() const
{
    return this->m_dPag;
}

void DetPag::set_dPag(const QDate &dPag)
{
    this->m_dPag = dPag;
}

QString DetPag::get_CNPJPag() const
{
    return this->m_CNPJPag;
}

void DetPag::set_CNPJPag(const QString &CNPJPag)
{
    this->m_CNPJPag = CNPJPag;
}

UF DetPag::get_UFPag() const
{
    return this->m_UFPag;
}

void DetPag::set_UFPag(const UF &UFPag)
{
    this->m_UFPag = UFPag;
}

QString DetPag::get_CNPJReceb() const
{
    return this->m_CNPJReceb;
}

void DetPag::set_CNPJReceb(const QString &CNPJReceb)
{
    this->m_CNPJReceb = CNPJReceb;
}

UF DetPag::get_UFReceb() const
{
    return this->m_UFReceb;
}

void DetPag::set_UFReceb(const UF &UFReceb)
{
    this->m_UFReceb = UFReceb;
}
