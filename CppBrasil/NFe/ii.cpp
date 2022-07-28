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


#include "ii.h"

Ii::Ii(): m_vBC(0.0), m_vDespAdu(0.0), m_vII(0.0), m_vIOF(0.0)
{
}

Ii::~Ii()
{
}

void Ii::clear()
{
    this->m_vBC = 0.0;
    this->m_vDespAdu = 0.0;
    this->m_vII = 0.0;
    this->m_vIOF = 0.0;
}

double Ii::get_vBC() const
{
    return this->m_vBC;
}

void Ii::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double Ii::get_vDespAdu() const
{
    return this->m_vDespAdu;
}

void Ii::set_vDespAdu(const double &vDespAdu)
{
    this->m_vDespAdu = vDespAdu;
}

double Ii::get_vII() const
{
    return this->m_vII;
}

void Ii::set_vII(const double &vII)
{
    this->m_vII = vII;
}

double Ii::get_vIOF() const
{
    return this->m_vIOF;
}

void Ii::set_vIOF(const double &vIOF)
{
    this->m_vIOF = vIOF;
}
