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


#include "imposto.h"

Imposto::Imposto(): ICMS(new Icms), ICMSUFDest(new IcmsUFDest), IPI(new Ipi),
    II(new Ii), PIS(new Pis), PISST(new PisST), COFINS(new Cofins), COFINSST(new CofinsST),
    ISSQN(new IssQN), m_vTotTrib(0.0)
{
}

Imposto::~Imposto()
{
}

void Imposto::clear()
{
    this->m_vTotTrib = 0.0;

    this->ICMS->clear();
    this->ICMSUFDest->clear();
    this->IPI->clear();
    this->II->clear();
    this->PIS->clear();
    this->PISST->clear();
    this->COFINS->clear();
    this->COFINSST->clear();
    this->ISSQN->clear();
}

double Imposto::get_vTotTrib() const
{
    return this->m_vTotTrib;
}

void Imposto::set_vTotTrib(const double &vTotTrib)
{
    this->m_vTotTrib = vTotTrib;
}
