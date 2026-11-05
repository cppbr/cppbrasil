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


#include "adi.h"

Adi::Adi(): m_nAdicao(0), m_nSeqAdic(0), m_vDescDI(0.0)
{
}

Adi::~Adi()
{
}

void Adi::clear()
{
    this->m_nAdicao = 0;
    this->m_nSeqAdic = 0;
    this->m_cFabricante.clear();
    this->m_vDescDI = 0.0;
    this->m_nDraw.clear();
}

int Adi::get_nAdicao() const
{
    return this->m_nAdicao;
}

void Adi::set_nAdicao(const int &nAdicao)
{
    this->m_nAdicao = nAdicao;
}

int Adi::get_nSeqAdic() const
{
    return this->m_nSeqAdic;
}

void Adi::set_nSeqAdic(const int &nSeqAdic)
{
    this->m_nSeqAdic = nSeqAdic;
}

QString Adi::get_cFabricante() const
{
    return this->m_cFabricante;
}

void Adi::set_cFabricante(const QString &cFabricante)
{
    this->m_cFabricante = cFabricante;
}

double Adi::get_vDescDI() const
{
    return this->m_vDescDI;
}

void Adi::set_vDescDI(const double &vDescDI)
{
    this->m_vDescDI = vDescDI;
}

QString Adi::get_nDraw() const
{
    return this->m_nDraw;
}

void Adi::set_nDraw(const QString &nDraw)
{
    this->m_nDraw = nDraw;
}
