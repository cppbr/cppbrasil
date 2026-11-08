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

#include "compragov.h"

void GCompraGov::clear()
{
    m_tpEnteGov = TpEnteGov::None;
    m_pRedutor = 0.0;
    m_tpOperGov = TpOperGov::None;
    m_refDFeAnt.clear();
}

bool GCompraGov::isEmpty() const
{
    return m_tpEnteGov == TpEnteGov::None &&
           qFuzzyIsNull(m_pRedutor) &&
           m_tpOperGov == TpOperGov::None &&
           m_refDFeAnt.isEmpty();
}

TpEnteGov GCompraGov::get_tpEnteGov() const
{
    return m_tpEnteGov;
}

void GCompraGov::set_tpEnteGov(const TpEnteGov &tpEnteGov)
{
    m_tpEnteGov = tpEnteGov;
}

double GCompraGov::get_pRedutor() const
{
    return m_pRedutor;
}

void GCompraGov::set_pRedutor(const double &pRedutor)
{
    m_pRedutor = pRedutor;
}

TpOperGov GCompraGov::get_tpOperGov() const
{
    return m_tpOperGov;
}

void GCompraGov::set_tpOperGov(const TpOperGov &tpOperGov)
{
    m_tpOperGov = tpOperGov;
}

QStringList GCompraGov::get_refDFeAnt() const
{
    return m_refDFeAnt;
}

void GCompraGov::set_refDFeAnt(const QStringList &refDFeAnt)
{
    m_refDFeAnt = refDFeAnt;
}

void GCompraGov::add_refDFeAnt(const QString &refDFeAnt)
{
    m_refDFeAnt.append(refDFeAnt);
}
