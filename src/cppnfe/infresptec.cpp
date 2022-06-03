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


#include "infresptec.h"

InfRespTec::InfRespTec(): m_idCSRT(0)
{
}

InfRespTec::~InfRespTec()
{
}

void InfRespTec::clear()
{
    this->m_CNPJ.clear();
    this->m_xContato.clear();
    this->m_email.clear();
    this->m_fone.clear();
    this->m_idCSRT = 0;
    this->m_hashCSRT.clear();
}

QString InfRespTec::get_CNPJ() const
{
    return this->m_CNPJ;
}

void InfRespTec::set_CNPJ(const QString &CNPJ)
{
    this->m_CNPJ = CNPJ;
}

QString InfRespTec::get_xContato() const
{
    return this->m_xContato;
}

void InfRespTec::set_xContato(const QString &xContato)
{
    this->m_xContato = xContato;
}

QString InfRespTec::get_email() const
{
    return this->m_email;
}

void InfRespTec::set_email(const QString &email)
{
    this->m_email = email;
}

QString InfRespTec::get_fone() const
{
    return this->m_fone;
}

void InfRespTec::set_fone(const QString &fone)
{
    this->m_fone = fone;
}

int InfRespTec::get_idCSRT() const
{
    return this->m_idCSRT;
}

void InfRespTec::set_idCSRT(const int &idCSRT)
{
    this->m_idCSRT = idCSRT;
}

QString InfRespTec::get_hashCSRT() const
{
    return this->m_hashCSRT;
}

void InfRespTec::set_hashCSRT(const QString &hashCSRT)
{
    this->m_hashCSRT = hashCSRT;
}
