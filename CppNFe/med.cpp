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


#include "med.h"

Med::Med(): m_vPMC(0.0)
{
}

Med::~Med()
{
}

void Med::clear()
{
    this->m_cProdANVISA.clear();
    this->m_xMotivoIsencao.clear();
    this->m_vPMC = 0.0;
}

QString Med::get_cProdANVISA() const
{
    return this->m_cProdANVISA;
}

void Med::set_cProdANVISA(const QString &cProdANVISA)
{
    this->m_cProdANVISA = cProdANVISA;
}

QString Med::get_xMotivoIsencao() const
{
    return this->m_xMotivoIsencao;
}

void Med::set_xMotivoIsencao(const QString &xMotivoIsencao)
{
    this->m_xMotivoIsencao = xMotivoIsencao;
}

double Med::get_vPMC() const
{
    return this->m_vPMC;
}

void Med::set_vPMC(const double &vPMC)
{
    this->m_vPMC = vPMC;
}
