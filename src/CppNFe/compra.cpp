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


#include "compra.h"

Compra::Compra()
{
}

Compra::~Compra()
{
}

void Compra::clear()
{
    this->m_xNEmp.clear();
    this->m_xPed.clear();
    this->m_xCont.clear();
}

QString Compra::get_xNEmp() const
{
    return this->m_xNEmp;
}

void Compra::set_xNEmp(const QString &xNEmp)
{
    this->m_xNEmp = xNEmp;
}

QString Compra::get_xPed() const
{
    return this->m_xPed;
}

void Compra::set_xPed(const QString &xPed)
{
    this->m_xPed = xPed;
}

QString Compra::get_xCont() const
{
    return this->m_xCont;
}

void Compra::set_xCont(const QString &xCont)
{
    this->m_xCont = xCont;
}
