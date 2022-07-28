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


#include "di.h"

Di::Di():  adi(new Container<Adi>), m_tpViaTransp(TpViaTransp::None), m_vAFRMM(0.0),
    m_tpIntermedio(TpIntermedio::None)
{

}

Di::~Di()
{
}

void Di::clear()
{
    this->m_nDI.clear();
    CppUtility::clearDateTime(this->m_dDI);
    this->m_xLocDesemb.clear();
    this->m_UFDesemb.clear();
    CppUtility::clearDateTime(this->m_dDesemb);
    this->m_tpViaTransp = TpViaTransp::None;
    this->m_vAFRMM = 0.0;
    this->m_tpIntermedio = TpIntermedio::None;
    this->m_CNPJ.clear();
    this->m_UFTerceiro.clear();
    this->m_cExportador.clear();

    this->adi->clear();
}

QString Di::get_nDI() const
{
    return this->m_nDI;
}

void Di::set_nDI(const QString &nDI)
{
    this->m_nDI = nDI;
}

QDateTime Di::get_dDI() const
{
    return this->m_dDI;
}

void Di::set_dDI(const QDateTime &dDI)
{
    this->m_dDI = dDI;
}

QString Di::get_xLocDesemb() const
{
    return this->m_xLocDesemb;
}

void Di::set_xLocDesemb(const QString &xLocDesemb)
{
    this->m_xLocDesemb = xLocDesemb;
}

QString Di::get_UFDesemb() const
{
    return this->m_UFDesemb;
}

void Di::set_UFDesemb(const QString &UFDesemb)
{
    this->m_UFDesemb = UFDesemb;
}

QDateTime Di::get_dDesemb() const
{
    return this->m_dDesemb;
}

void Di::set_dDesemb(const QDateTime &dDesemb)
{
    this->m_dDesemb = dDesemb;
}

TpViaTransp Di::get_tpViaTransp() const
{
    return m_tpViaTransp;
}

void Di::set_tpViaTransp(const TpViaTransp &tpViaTransp)
{
    this->m_tpViaTransp = tpViaTransp;
}

double Di::get_vAFRMM() const
{
    return this->m_vAFRMM;
}

void Di::set_vAFRMM(const double &vAFRMM)
{
    this->m_vAFRMM = vAFRMM;
}

TpIntermedio Di::get_tpIntermedio() const
{
    return this->m_tpIntermedio;
}

void Di::set_tpIntermedio(const TpIntermedio &tpIntermedio)
{
    this->m_tpIntermedio = tpIntermedio;
}

QString Di::get_CNPJ() const
{
    return this->m_CNPJ;
}

void Di::set_CNPJ(const QString &CNPJ)
{
   this->m_CNPJ = CNPJ;
}

QString Di::get_UFTerceiro() const
{
    return this->m_UFTerceiro;
}

void Di::set_UFTerceiro(const QString &UFTerceiro)
{
    this->m_UFTerceiro = UFTerceiro;
}

QString Di::get_cExportador() const
{
    return this->m_cExportador;
}

void Di::set_cExportador(const QString &cExportador)
{
    this->m_cExportador = cExportador;
}
