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


#include "reboque.h"

Reboque::Reboque()
{
}

Reboque::~Reboque()
{
}

void Reboque::clear()
{
    this->m_placa.clear();
    this->m_UF.clear();
    this->m_RNTC.clear();
}

QString Reboque::get_placa() const
{
    return this->m_placa;
}

void Reboque::set_placa(const QString &placa)
{
    this->m_placa = placa;
}

QString Reboque::get_UF() const
{
    return this->m_UF;
}

void Reboque::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

QString Reboque::get_RNTC() const
{
    return this->m_RNTC;
}

void Reboque::set_RNTC(const QString &RNTC)
{
    this->m_RNTC = RNTC;
}

