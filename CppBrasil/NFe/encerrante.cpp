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


#include "encerrante.h"

Encerrante::Encerrante():m_nBico(0), m_nBomba(0), m_nTanque(0), m_vEncIni(0.0), m_vEncFin(0.0)
{
}

Encerrante::~Encerrante()
{
}

void Encerrante::clear()
{
    this->m_nBico = 0;
    this->m_nBomba = 0;
    this->m_nTanque = 0;
    this->m_vEncIni = 0.0;
    this->m_vEncFin = 0.0;
}

int Encerrante::get_nBico() const
{
    return this->m_nBico;
}

void Encerrante::set_nBico(const int &nBico)
{
    this->m_nBico = nBico;
}

int Encerrante::get_nBomba() const
{
    return this->m_nBomba;
}

void Encerrante::set_nBomba(const int &nBomba)
{
    this->m_nBomba = nBomba;
}

int Encerrante::get_nTanque() const
{
    return this->m_nTanque;
}

void Encerrante::set_nTanque(const int &nTanque)
{
    this->m_nTanque = nTanque;
}

double Encerrante::get_vEncIni() const
{
    return this->m_vEncIni;
}

void Encerrante::set_vEncIni(const double &vEncIni)
{
    this->m_vEncIni = vEncIni;
}

double Encerrante::get_vEncFin() const
{
    return this->m_vEncFin;
}

void Encerrante::set_vEncFin(const double &vEncFin)
{
    this->m_vEncFin = vEncFin;
}
