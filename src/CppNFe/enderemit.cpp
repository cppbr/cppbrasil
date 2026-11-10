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


#include "enderemit.h"

EnderEmit::EnderEmit(): m_nro(0), m_cMun(0), m_CEP(0), m_cPais(0)
{
}

EnderEmit::~EnderEmit()
{
}

void EnderEmit::clear()
{
    this->m_xLgr.clear();
    this->m_nro = 0;
    this->m_xCpl.clear();
    this->m_xBairro.clear();
    this->m_cMun = 0;
    this->m_xMun.clear();
    this->m_UF.clear();
    this->m_CEP = 0;
    this->m_cPais = 0;
    this->m_xPais.clear();
    this->m_fone.clear();
}

QString EnderEmit::get_xLgr() const
{
    return this->m_xLgr;
}

void EnderEmit::set_xLgr(const QString &xLgr)
{
    this->m_xLgr = xLgr;
}

int EnderEmit::get_nro() const
{
    return this->m_nro;
}

void EnderEmit::set_nro(const int &nro)
{
    this->m_nro = nro;
}

QString EnderEmit::get_xCpl() const
{
    return this->m_xCpl;
}

void EnderEmit::set_xCpl(const QString &xCpl)
{
    m_xCpl = xCpl;
}

QString EnderEmit::get_xBairro() const
{
    return this->m_xBairro;
}

void EnderEmit::set_xBairro(const QString &xBairro)
{
    this->m_xBairro = xBairro;
}

int EnderEmit::get_cMun() const
{
    return this->m_cMun;
}

void EnderEmit::set_cMun(const int &cMun)
{
    this->m_cMun = cMun;
}

QString EnderEmit::get_xMun() const
{
    return this->m_xMun;
}

void EnderEmit::set_xMun(const QString &xMun)
{
    this->m_xMun = xMun;
}

QString EnderEmit::get_UF() const
{
    return this->m_UF;
}

void EnderEmit::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

int EnderEmit::get_CEP() const
{
    return this->m_CEP;
}

void EnderEmit::set_CEP(const int &CEP)
{
    this->m_CEP = CEP;
}

int EnderEmit::get_cPais() const
{
    return this->m_cPais;
}

void EnderEmit::set_cPais(const int &cPais)
{
    this->m_cPais = cPais;
}

QString EnderEmit::get_xPais() const
{
    return this->m_xPais;
}

void EnderEmit::set_xPais(const QString &xPais)
{
    this->m_xPais = xPais;
}

QString EnderEmit::get_fone() const
{
    return this->m_fone;
}

void EnderEmit::set_fone(const QString &fone)
{
    this->m_fone = fone;
}
