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


#include "vol.h"

Vol::Vol(): lacres(new Container<Lacres>), m_qVol(0), m_pesoL(0), m_pesoB(0)
{
}

Vol::~Vol()
{
}

void Vol::clear()
{
    this->m_qVol = 0.0;
    this->m_esp.clear();
    this->m_marca.clear();
    this->m_nVol.clear();
    this->m_pesoL = 0.0;
    this->m_pesoB = 0.0;

    this->lacres->clear();
}

double Vol::get_qVol() const
{
    return this->m_qVol;
}

void Vol::set_qVol(const double &qVol)
{
   this->m_qVol = qVol;
}

QString Vol::get_esp() const
{
    return this->m_esp;
}

void Vol::set_esp(const QString &esp)
{
    this->m_esp = esp;
}

QString Vol::get_marca() const
{
    return this->m_marca;
}

void Vol::set_marca(const QString &marca)
{
    this->m_marca = marca;
}

QString Vol::get_nVol() const
{
    return this->m_nVol;
}

void Vol::set_nVol(const QString &nVol)
{
    this->m_nVol = nVol;
}

double Vol::get_pesoL() const
{
    return this->m_pesoL;
}

void Vol::set_pesoL(const double &pesoL)
{
    this->m_pesoL = pesoL;
}

double Vol::get_pesoB() const
{
    return this->m_pesoB;
}

void Vol::set_pesoB(const double &pesoB)
{
    this->m_pesoB = pesoB;
}
