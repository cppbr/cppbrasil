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


#include "autxml.h"

AutXML::AutXML()
{
}

AutXML::~AutXML()
{
}

void AutXML::clear()
{
    this->m_CNPJ.clear();
    this->m_CPF.clear();
}

QString AutXML::get_CNPJ() const
{
    return this->m_CNPJ;
}

void AutXML::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString AutXML::get_CPF() const
{
    return this->m_CPF;
}

void AutXML::set_CPF(const QString &CPF)
{
    this->m_CPF = CPF;
}
