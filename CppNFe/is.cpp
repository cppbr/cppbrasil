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

#include "is.h"

void Is::clear()
{
    m_CSTIS.clear();
    m_cClassTribIS.clear();
    m_vBCIS = 0.0;
    m_pIS = 0.0;
    m_adRemIS = 0.0;
    m_uTrib.clear();
    m_qTrib = 0.0;
    m_vIS = 0.0;
}

bool Is::isEmpty() const
{
    return m_CSTIS.isEmpty() &&
           m_cClassTribIS.isEmpty() &&
           qFuzzyIsNull(m_vBCIS) &&
           qFuzzyIsNull(m_pIS) &&
           qFuzzyIsNull(m_adRemIS) &&
           m_uTrib.isEmpty() &&
           qFuzzyIsNull(m_qTrib) &&
           qFuzzyIsNull(m_vIS);
}

QString Is::get_CSTIS() const
{
    return m_CSTIS;
}

void Is::set_CSTIS(const QString &CSTIS)
{
    m_CSTIS = CSTIS;
}

QString Is::get_cClassTribIS() const
{
    return m_cClassTribIS;
}

void Is::set_cClassTribIS(const QString &cClassTribIS)
{
    m_cClassTribIS = cClassTribIS;
}

double Is::get_vBCIS() const
{
    return m_vBCIS;
}

void Is::set_vBCIS(const double &vBCIS)
{
    m_vBCIS = vBCIS;
}

double Is::get_pIS() const
{
    return m_pIS;
}

void Is::set_pIS(const double &pIS)
{
    m_pIS = pIS;
}

double Is::get_adRemIS() const
{
    return m_adRemIS;
}

void Is::set_adRemIS(const double &adRemIS)
{
    m_adRemIS = adRemIS;
}

QString Is::get_uTrib() const
{
    return m_uTrib;
}

void Is::set_uTrib(const QString &uTrib)
{
    m_uTrib = uTrib;
}

double Is::get_qTrib() const
{
    return m_qTrib;
}

void Is::set_qTrib(const double &qTrib)
{
    m_qTrib = qTrib;
}

double Is::get_vIS() const
{
    return m_vIS;
}

void Is::set_vIS(const double &vIS)
{
    m_vIS = vIS;
}
