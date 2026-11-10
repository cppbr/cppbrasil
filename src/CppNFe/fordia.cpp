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


#include "fordia.h"

ForDia::ForDia() : m_dia(0), m_qtde(0.0)
{
}

ForDia::~ForDia()
{
}

void ForDia::clear()
{
    this->m_dia = 0;
    this->m_qtde = 0.0;
}

int ForDia::get_dia() const
{
    return this->m_dia;
}

void ForDia::set_dia(const int &dia)
{
    this->m_dia = dia;
}

double ForDia::get_qtde() const
{
    return this->m_qtde;
}

void ForDia::set_qtde(const double &qtde)
{
    this->m_qtde = qtde;
}

