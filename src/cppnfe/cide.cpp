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


#include "cide.h"

Cide::Cide(): m_qBCProd(0.0), m_vAliqProd(0.0), m_vCIDE(0.0)
{
}

Cide::~Cide()
{
}

void Cide::clear()
{
    this->m_qBCProd = 0.0;
    this->m_vAliqProd = 0.0;
    this->m_vCIDE = 0.0;
}

double Cide::get_qBCProd() const
{
    return this->m_qBCProd;
}

void Cide::set_qBCProd(const double &qBCProd)
{
    this->m_qBCProd = qBCProd;
}

double Cide::get_vAliqProd() const
{
    return this->m_vAliqProd;
}

void Cide::set_vAliqProd(const double &vAliqProd)
{
    this->m_vAliqProd = vAliqProd;
}

double Cide::get_vCIDE() const
{
    return this->m_vCIDE;
}

void Cide::set_vCIDE(const double &vCIDE)
{
    this->m_vCIDE = vCIDE;
}
