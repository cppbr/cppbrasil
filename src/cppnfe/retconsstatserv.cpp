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


#include "retconsstatserv.h"

RetConsStatServ::RetConsStatServ(): m_tpAmb(TpAmb::None), m_cStat(0), m_cUF(0), m_tMed(0)
{
}

RetConsStatServ::~RetConsStatServ()
{
}

void RetConsStatServ::clear()
{
    m_versao.clear();
    m_tpAmb = TpAmb::None;
    m_verAplic.clear();
    m_cStat = 0;
    m_xMotivo.clear();
    m_cUF = 0;
    CppUtility::clearDateTime(m_dhRecbto);
    m_tMed = 0;
    CppUtility::clearDateTime(m_dhRetorno);
    m_xObs.clear();
}

QString RetConsStatServ::get_versao() const
{
    return this->m_versao;
}

TpAmb RetConsStatServ::get_tpAmb() const
{
    return this->m_tpAmb;
}

QString RetConsStatServ::get_verAplic() const
{
    return this->m_verAplic;
}

int RetConsStatServ::get_cStat() const
{
    return this->m_cStat;
}

QString RetConsStatServ::get_xMotivo() const
{
    return this->m_xMotivo;
}

int RetConsStatServ::get_cUF() const
{
    return this->m_cUF;
}

QDateTime RetConsStatServ::get_dhRecbto() const
{
    return this->m_dhRecbto;
}

int RetConsStatServ::get_tMed() const
{
    return this->m_tMed;
}

QDateTime RetConsStatServ::get_dhRetorno() const
{
    return this->m_dhRetorno;
}

QString RetConsStatServ::get_xObs() const
{
    return this->m_xObs;
}

void RetConsStatServ::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

void RetConsStatServ::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

void RetConsStatServ::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

void RetConsStatServ::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

void RetConsStatServ::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

void RetConsStatServ::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}

void RetConsStatServ::set_dhRecbto(const QDateTime &dhRecbto)
{
    this->m_dhRecbto = dhRecbto;
}

void RetConsStatServ::set_tMed(const int &tMed)
{
    this->m_tMed = tMed;
}

void RetConsStatServ::set_dhRetorno(const QDateTime &dhRetorno)
{
    this->m_dhRetorno = dhRetorno;
}

void RetConsStatServ::set_xObs(const QString &xObs)
{
    this->m_xObs = xObs;
}
