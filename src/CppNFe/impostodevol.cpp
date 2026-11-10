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


#include "impostodevol.h"

ImpostoDevol::ImpostoDevol(): m_pDevol(0.0), m_vIPIDevol(0.0)
{
}

ImpostoDevol::~ImpostoDevol()
{
}

void ImpostoDevol::clear()
{
    this->m_pDevol = 0.0;
    this->m_vIPIDevol = 0.0;
}

double ImpostoDevol::get_pDevol() const
{
    return this->m_pDevol;
}

void ImpostoDevol::set_pDevol(const double &pDevol)
{
    this->m_pDevol = pDevol;
}

double ImpostoDevol::get_vIPIDevol() const
{
    return this->m_vIPIDevol;
}

void ImpostoDevol::set_vIPIDevol(const double &vIPIDevol)
{
    this->m_vIPIDevol = vIPIDevol;
}
