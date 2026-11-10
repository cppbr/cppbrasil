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


#include "deduc.h"

Deduc::Deduc() :m_vDed(0.0)
{
}

Deduc::~Deduc()
{
}

void Deduc::clear()
{
    this->m_xDed.clear();
    this->m_vDed = 0.0;
}

QString Deduc::get_xDed() const
{
    return this->m_xDed;
}

void Deduc::set_xDed(const QString &xDed)
{
    this->m_xDed = xDed;
}

double Deduc::get_vDed() const
{
    return this->m_vDed;
}

void Deduc::set_vDed(const double &vDed)
{
    this->m_vDed = vDed;
}

