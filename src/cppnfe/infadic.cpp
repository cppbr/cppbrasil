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


#include "infadic.h"

InfAdic::InfAdic(): obsCont(new Container<ObsCont>), obsFisco(new Container<ObsFisco>),
    procRef(new Container<ProcRef>)
{
}

InfAdic::~InfAdic()
{
}

void InfAdic::clear()
{
    this->m_infAdFisco.clear();
    this->m_infCpl.clear();

    this->obsCont->clear();
    this->obsFisco->clear();
    this->procRef->clear();
}

QString InfAdic::get_infAdFisco() const
{
    return this->m_infAdFisco;
}

void InfAdic::set_infAdFisco(const QString &infAdFisco)
{
    this->m_infAdFisco = infAdFisco;
}

QString InfAdic::get_infCpl() const
{
    return this->m_infCpl;
}

void InfAdic::set_infCpl(const QString &infCpl)
{
    this->m_infCpl = infCpl;
}
