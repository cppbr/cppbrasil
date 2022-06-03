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


#include "enderdest.h"


EnderDest::EnderDest(): m_nro(0), m_cMun(0), m_CEP(0), m_cPais(0)
{
}

EnderDest::~EnderDest()
{
}


void EnderDest::clear()
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

QString EnderDest::get_xLgr() const
{
    return this->m_xLgr;
}

void EnderDest::set_xLgr(const QString &xLgr)
{
    this->m_xLgr = xLgr;
}

int EnderDest::get_nro() const
{
    return this->m_nro;
}

void EnderDest::set_nro(const int &nro)
{
    this->m_nro = nro;
}

QString EnderDest::get_xCpl() const
{
    return this->m_xCpl;
}

void EnderDest::set_xCpl(const QString &xCpl)
{
    m_xCpl = xCpl;
}

QString EnderDest::get_xBairro() const
{
    return this->m_xBairro;
}

void EnderDest::set_xBairro(const QString &xBairro)
{
    this->m_xBairro = xBairro;
}

int EnderDest::get_cMun() const
{
    return this->m_cMun;
}

void EnderDest::set_cMun(const int &cMun)
{
    this->m_cMun = cMun;
}

QString EnderDest::get_xMun() const
{
    return this->m_xMun;
}

void EnderDest::set_xMun(const QString &xMun)
{
    this->m_xMun = xMun;
}

QString EnderDest::get_UF() const
{
    return this->m_UF;
}

void EnderDest::set_UF(const QString &UF)
{
    this->m_UF = UF;
}

int EnderDest::get_CEP() const
{
    return this->m_CEP;
}

void EnderDest::set_CEP(const int &CEP)
{
    this->m_CEP = CEP;
}

int EnderDest::get_cPais() const
{
    return this->m_cPais;
}

void EnderDest::set_cPais(const int &cPais)
{
    this->m_cPais = cPais;
}

QString EnderDest::get_xPais() const
{
    return this->m_xPais;
}

void EnderDest::set_xPais(const QString &xPais)
{
    this->m_xPais = xPais;
}

QString EnderDest::get_fone() const
{
    return this->m_fone;
}

void EnderDest::set_fone(const QString &fone)
{
    this->m_fone = fone;
}
