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


#include "fat.h"

Fat::Fat(): m_vOrig(0.0), m_vDesc(0.0), m_vLiq(0.0)
{
}

Fat::~Fat()
{
}

void Fat::clear()
{
    this->m_nFat.clear();
    this->m_vOrig = 0.0;
    this->m_vDesc = 0.0;
    this->m_vLiq = 0.0;
}

QString Fat::get_nFat() const
{
    return this->m_nFat;
}

void Fat::set_nFat(const QString &nFat)
{
    this->m_nFat = nFat;
}

double Fat::get_vOrig() const
{
    return this->m_vOrig;
}

void Fat::set_vOrig(const double &vOrig)
{
    this->m_vOrig = vOrig;
}

double Fat::get_vDesc() const
{
    return this->m_vDesc;
}

void Fat::set_vDesc(const double &vDesc)
{
    this->m_vDesc = vDesc;
}

double Fat::get_vLiq() const
{
    return this->m_vLiq;
}

void Fat::set_vLiq(const double &vLiq)
{
    this->m_vLiq = vLiq;
}
