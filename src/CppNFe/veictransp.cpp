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


#include "veictransp.h"

VeicTransp::VeicTransp()
{
}

VeicTransp::~VeicTransp()
{
}

void VeicTransp::clear()
{
    this->m_placa.clear();
    this->m_UF.clear();
    this->m_RNTC.clear();
}

QString VeicTransp::get_placa() const
{
    return this->m_placa;
}

void VeicTransp::set_placa(const QString &placa)
{
    this->m_placa = placa;
}

QString VeicTransp::get_UF() const
{
    return this->m_UF;
}

void VeicTransp::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

QString VeicTransp::get_RNTC() const
{
    return this->m_RNTC;
}

void VeicTransp::set_RNTC(const QString &RNTC)
{
    this->m_RNTC = RNTC;
}
