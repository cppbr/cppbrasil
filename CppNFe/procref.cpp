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


#include "procref.h"

ProcRef::ProcRef(): m_indProc(IndProc::None), m_tpAto(TpAto::None)
{
}

ProcRef::~ProcRef()
{
}

void ProcRef::clear()
{
    this->m_nProc.clear();
    this->m_indProc = IndProc::None;
    this->m_tpAto = TpAto::None;
}

QString ProcRef::get_nProc() const
{
    return this->m_nProc;
}

void ProcRef::set_nProc(const QString &nProc)
{
    this->m_nProc = nProc;
}

IndProc ProcRef::get_indProc() const
{
    return this->m_indProc;
}

void ProcRef::set_indProc(const IndProc &indProc)
{
    this->m_indProc = indProc;
}

TpAto ProcRef::get_tpAto() const
{
    return this->m_tpAto;
}

void ProcRef::set_tpAto(const TpAto &tpAto)
{
    this->m_tpAto = tpAto;
}
