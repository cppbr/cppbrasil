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

#include "gcred.h"

void GCred::clear()
{
    this->m_cCredPresumido.clear();
    this->m_pCredPresumido = 0.0;
    this->m_vCredPresumido = 0.0;
}

QString GCred::get_cCredPresumido() const
{
    return this->m_cCredPresumido;
}

void GCred::set_cCredPresumido(const QString &cCredPresumido)
{
    this->m_cCredPresumido = cCredPresumido;
}

double GCred::get_pCredPresumido() const
{
    return this->m_pCredPresumido;
}

void GCred::set_pCredPresumido(const double &pCredPresumido)
{
    this->m_pCredPresumido = pCredPresumido;
}

double GCred::get_vCredPresumido() const
{
    return this->m_vCredPresumido;
}

void GCred::set_vCredPresumido(const double &vCredPresumido)
{
    this->m_vCredPresumido = vCredPresumido;
}
