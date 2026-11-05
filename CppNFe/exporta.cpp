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


#include "exporta.h"

Exporta::Exporta()
{
}

Exporta::~Exporta()
{
}

void Exporta::clear()
{
    this->m_UFSaidaPais.clear();
    this->m_xLocExporta.clear();
    this->m_xLocDespacho.clear();
}

QString Exporta::get_UFSaidaPais() const
{
    return this->m_UFSaidaPais;
}

void Exporta::set_UFSaidaPais(const QString &UFSaidaPais)
{
    this->m_UFSaidaPais = UFSaidaPais;
}

QString Exporta::get_xLocExporta() const
{
    return this->m_xLocExporta;
}

void Exporta::set_xLocExporta(const QString &xLocExporta)
{
    this->m_xLocExporta = xLocExporta;
}

QString Exporta::get_xLocDespacho() const
{
    return this->m_xLocDespacho;
}

void Exporta::set_xLocDespacho(const QString &xLocDespacho)
{
    this->m_xLocDespacho = xLocDespacho;
}
