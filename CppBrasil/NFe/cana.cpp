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


#include "cana.h"

Cana::Cana():forDia(new Container<ForDia>), deduc(new Container<Deduc>), m_qTotMes(0.0),
    m_qTotAnt(0.0), m_qTotGer(0.0), m_vFor(0.0), m_vTotDed(0.0), m_vLiqFor(0.0)
{
}

Cana::~Cana()
{
}

void Cana::clear()
{
    this->m_safra.clear();
    this->m_ref.clear();
    this->m_qTotMes = 0.0;
    this->m_qTotAnt = 0.0;
    this->m_qTotGer = 0.0;
    this->m_vFor = 0.0;
    this->m_vTotDed = 0.0;
    this->m_vLiqFor = 0.0;


    this->forDia->clear();
    this->deduc->clear();
}

QString Cana::get_safra() const
{
    return this->m_safra;
}

void Cana::set_safra(const QString &safra)
{
    this->m_safra = safra;
}

QString Cana::get_ref() const
{
    return this->m_ref;
}

void Cana::set_ref(const QString &ref)
{
    this->m_ref = ref;
}
double Cana::get_qTotMes() const
{
    return this->m_qTotMes;
}

void Cana::set_qTotMes(const double &qTotMes)
{
    this->m_qTotMes = qTotMes;
}

double Cana::get_qTotAnt() const
{
    return this->m_qTotAnt;
}

void Cana::set_qTotAnt(const double &qTotAnt)
{
    this->m_qTotAnt = qTotAnt;
}

double Cana::get_qTotGer() const
{
    return this->m_qTotGer;
}

void Cana::set_qTotGer(const double &qTotGer)
{
    this->m_qTotGer = qTotGer;
}

double Cana::get_vFor() const
{
    return this->m_vFor;
}

void Cana::set_vFor(const double &vFor)
{
    this->m_vFor = vFor;
}

double Cana::get_vTotDed() const
{
    return this->m_vTotDed;
}

void Cana::set_vTotDed(const double &vTotDed)
{
    this->m_vTotDed = vTotDed;
}

double Cana::get_vLiqFor() const
{
    return this->m_vLiqFor;
}

void Cana::set_vLiqFor(const double &vLiqFor)
{
    this->m_vLiqFor = vLiqFor;
}
