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


#include "ipi.h"

Ipi::Ipi(): m_qSelo(0), m_CST(CstIPI::None), m_vBC(0.0), m_pIPI(0.0), m_qUnid(0.0),
    m_vUnid(0.0), m_vIPI(0.0)
{
}

Ipi::~Ipi()
{
}

void Ipi::clear()
{
    this->m_CNPJProd.clear();
    this->m_cSelo.clear();
    this->m_qSelo = 0;
    this->m_cEnq.clear();
    this->m_CST = CstIPI::None;
    this->m_vBC = 0.0;
    this->m_pIPI = 0.0;
    this->m_qUnid = 0.0;
    this->m_vUnid = 0.0;
    this->m_vIPI = 0.0;
}

QString Ipi::get_CNPJProd() const
{
    return this->m_CNPJProd;
}

void Ipi::set_CNPJProd(const QString &CNPJProd)
{
    this->m_CNPJProd = CNPJProd;
}

QString Ipi::get_cSelo() const
{
    return this->m_cSelo;
}

void Ipi::set_cSelo(const QString &cSelo)
{
    this->m_cSelo = cSelo;
}

int Ipi::get_qSelo() const
{
    return this->m_qSelo;
}

void Ipi::set_qSelo(const int &qSelo)
{
    this->m_qSelo = qSelo;
}

QString Ipi::get_cEnq() const
{
    return this->m_cEnq;
}

void Ipi::set_cEnq(const QString &cEnq)
{
    this->m_cEnq = cEnq;
}

CstIPI Ipi::get_CST() const
{
    return this->m_CST;
}

void Ipi::set_CST(const CstIPI &CST)
{
    this->m_CST = CST;
}

double Ipi::get_vBC() const
{
    return this->m_vBC;
}

void Ipi::set_vBC(const double &vBC)
{
    this->m_vBC = vBC;
}

double Ipi::get_pIPI() const
{
    return this->m_pIPI;
}

void Ipi::set_pIPI(const double &pIPI)
{
    this->m_pIPI = pIPI;
}

double Ipi::get_qUnid() const
{
    return this->m_qUnid;
}

void Ipi::set_qUnid(const double &qUnid)
{
    this->m_qUnid = qUnid;
}

double Ipi::get_vUnid() const
{
    return this->m_vUnid;
}

void Ipi::set_vUnid(const double &vUnid)
{
    this->m_vUnid = vUnid;
}

double Ipi::get_vIPI() const
{
    return this->m_vIPI;
}

void Ipi::set_vIPI(const double &vIPI)
{
   this->m_vIPI = vIPI;
}
