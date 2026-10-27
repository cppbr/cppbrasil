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

#include "origcomb.h"

OrigComb::OrigComb(): m_indImport(IndImport::None), m_cUFOrig(0), m_pOrig(0.0)
{
}

OrigComb::~OrigComb()
{
}

void OrigComb::clear()
{
   this->m_indImport = IndImport::None;
   this->m_cUFOrig = 0;
   this->m_pOrig = 0.0;
}

IndImport OrigComb::get_indImport() const
{
    return this->m_indImport;
}

void OrigComb::set_indImport(const IndImport &indImport)
{
    this->m_indImport = indImport;
}

int OrigComb::get_cUFOrig() const
{
    return this->m_cUFOrig;
}

void OrigComb::set_cUFOrig(const int &cUFOrig)
{
    this->m_cUFOrig = cUFOrig;
}

double OrigComb::get_pOrig() const
{
    return this->m_pOrig;
}

void OrigComb::set_pOrig(const double &pOrig)
{
    this->m_pOrig = pOrig;
}
