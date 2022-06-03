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
    card(new Card), m_indPag(IndPag::None), m_tPag(TPag::None), m_vPag(0.0)
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
