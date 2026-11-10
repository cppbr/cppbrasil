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

#include "ibscbstot.h"

void IsTot::clear()
{
    m_vIS = 0.0;
}

bool IsTot::isEmpty() const
{
    return qFuzzyIsNull(m_vIS);
}

double IsTot::get_vIS() const
{
    return m_vIS;
}

void IsTot::set_vIS(const double &vIS)
{
    m_vIS = vIS;
}

void GIBSUFTot::clear()
{
    m_vDif = 0.0;
    m_vDevTrib = 0.0;
    m_vIBSUF = 0.0;
}

bool GIBSUFTot::isEmpty() const
{
    return qFuzzyIsNull(m_vDif) &&
           qFuzzyIsNull(m_vDevTrib) &&
           qFuzzyIsNull(m_vIBSUF);
}

double GIBSUFTot::get_vDif() const
{
    return m_vDif;
}

void GIBSUFTot::set_vDif(const double &vDif)
{
    m_vDif = vDif;
}

double GIBSUFTot::get_vDevTrib() const
{
    return m_vDevTrib;
}

void GIBSUFTot::set_vDevTrib(const double &vDevTrib)
{
    m_vDevTrib = vDevTrib;
}

double GIBSUFTot::get_vIBSUF() const
{
    return m_vIBSUF;
}

void GIBSUFTot::set_vIBSUF(const double &vIBSUF)
{
    m_vIBSUF = vIBSUF;
}

void GIBSMunTot::clear()
{
    m_vDif = 0.0;
    m_vDevTrib = 0.0;
    m_vIBSMun = 0.0;
}

bool GIBSMunTot::isEmpty() const
{
    return qFuzzyIsNull(m_vDif) &&
           qFuzzyIsNull(m_vDevTrib) &&
           qFuzzyIsNull(m_vIBSMun);
}

double GIBSMunTot::get_vDif() const
{
    return m_vDif;
}

void GIBSMunTot::set_vDif(const double &vDif)
{
    m_vDif = vDif;
}

double GIBSMunTot::get_vDevTrib() const
{
    return m_vDevTrib;
}

void GIBSMunTot::set_vDevTrib(const double &vDevTrib)
{
    m_vDevTrib = vDevTrib;
}

double GIBSMunTot::get_vIBSMun() const
{
    return m_vIBSMun;
}

void GIBSMunTot::set_vIBSMun(const double &vIBSMun)
{
    m_vIBSMun = vIBSMun;
}

GIBSTot::GIBSTot():
    gIBSUF(new GIBSUFTot),
    gIBSMun(new GIBSMunTot)
{
}

void GIBSTot::clear()
{
    m_vIBS = 0.0;
    m_vCredPres = 0.0;
    m_vCredPresCondSus = 0.0;
    gIBSUF->clear();
    gIBSMun->clear();
}

bool GIBSTot::isEmpty() const
{
    return qFuzzyIsNull(m_vIBS) &&
           qFuzzyIsNull(m_vCredPres) &&
           qFuzzyIsNull(m_vCredPresCondSus) &&
           gIBSUF->isEmpty() &&
           gIBSMun->isEmpty();
}

double GIBSTot::get_vIBS() const
{
    return m_vIBS;
}

void GIBSTot::set_vIBS(const double &vIBS)
{
    m_vIBS = vIBS;
}

double GIBSTot::get_vCredPres() const
{
    return m_vCredPres;
}

void GIBSTot::set_vCredPres(const double &vCredPres)
{
    m_vCredPres = vCredPres;
}

double GIBSTot::get_vCredPresCondSus() const
{
    return m_vCredPresCondSus;
}

void GIBSTot::set_vCredPresCondSus(const double &vCredPresCondSus)
{
    m_vCredPresCondSus = vCredPresCondSus;
}

void GCBSTot::clear()
{
    m_vDif = 0.0;
    m_vDevTrib = 0.0;
    m_vCBS = 0.0;
    m_vCredPres = 0.0;
    m_vCredPresCondSus = 0.0;
}

bool GCBSTot::isEmpty() const
{
    return qFuzzyIsNull(m_vDif) &&
           qFuzzyIsNull(m_vDevTrib) &&
           qFuzzyIsNull(m_vCBS) &&
           qFuzzyIsNull(m_vCredPres) &&
           qFuzzyIsNull(m_vCredPresCondSus);
}

double GCBSTot::get_vDif() const
{
    return m_vDif;
}

void GCBSTot::set_vDif(const double &vDif)
{
    m_vDif = vDif;
}

double GCBSTot::get_vDevTrib() const
{
    return m_vDevTrib;
}

void GCBSTot::set_vDevTrib(const double &vDevTrib)
{
    m_vDevTrib = vDevTrib;
}

double GCBSTot::get_vCBS() const
{
    return m_vCBS;
}

void GCBSTot::set_vCBS(const double &vCBS)
{
    m_vCBS = vCBS;
}

double GCBSTot::get_vCredPres() const
{
    return m_vCredPres;
}

void GCBSTot::set_vCredPres(const double &vCredPres)
{
    m_vCredPres = vCredPres;
}

double GCBSTot::get_vCredPresCondSus() const
{
    return m_vCredPresCondSus;
}

void GCBSTot::set_vCredPresCondSus(const double &vCredPresCondSus)
{
    m_vCredPresCondSus = vCredPresCondSus;
}

void GMonoTot::clear()
{
    m_vIBSMono = 0.0;
    m_vCBSMono = 0.0;
    m_vIBSMonoReten = 0.0;
    m_vCBSMonoReten = 0.0;
    m_vIBSMonoRet = 0.0;
    m_vCBSMonoRet = 0.0;
}

bool GMonoTot::isEmpty() const
{
    return qFuzzyIsNull(m_vIBSMono) &&
           qFuzzyIsNull(m_vCBSMono) &&
           qFuzzyIsNull(m_vIBSMonoReten) &&
           qFuzzyIsNull(m_vCBSMonoReten) &&
           qFuzzyIsNull(m_vIBSMonoRet) &&
           qFuzzyIsNull(m_vCBSMonoRet);
}

double GMonoTot::get_vIBSMono() const
{
    return m_vIBSMono;
}

void GMonoTot::set_vIBSMono(const double &vIBSMono)
{
    m_vIBSMono = vIBSMono;
}

double GMonoTot::get_vCBSMono() const
{
    return m_vCBSMono;
}

void GMonoTot::set_vCBSMono(const double &vCBSMono)
{
    m_vCBSMono = vCBSMono;
}

double GMonoTot::get_vIBSMonoReten() const
{
    return m_vIBSMonoReten;
}

void GMonoTot::set_vIBSMonoReten(const double &vIBSMonoReten)
{
    m_vIBSMonoReten = vIBSMonoReten;
}

double GMonoTot::get_vCBSMonoReten() const
{
    return m_vCBSMonoReten;
}

void GMonoTot::set_vCBSMonoReten(const double &vCBSMonoReten)
{
    m_vCBSMonoReten = vCBSMonoReten;
}

double GMonoTot::get_vIBSMonoRet() const
{
    return m_vIBSMonoRet;
}

void GMonoTot::set_vIBSMonoRet(const double &vIBSMonoRet)
{
    m_vIBSMonoRet = vIBSMonoRet;
}

double GMonoTot::get_vCBSMonoRet() const
{
    return m_vCBSMonoRet;
}

void GMonoTot::set_vCBSMonoRet(const double &vCBSMonoRet)
{
    m_vCBSMonoRet = vCBSMonoRet;
}

void GEstornoCredTot::clear()
{
    m_vIBSEstCred = 0.0;
    m_vCBSEstCred = 0.0;
}

bool GEstornoCredTot::isEmpty() const
{
    return qFuzzyIsNull(m_vIBSEstCred) &&
           qFuzzyIsNull(m_vCBSEstCred);
}

double GEstornoCredTot::get_vIBSEstCred() const
{
    return m_vIBSEstCred;
}

void GEstornoCredTot::set_vIBSEstCred(const double &vIBSEstCred)
{
    m_vIBSEstCred = vIBSEstCred;
}

double GEstornoCredTot::get_vCBSEstCred() const
{
    return m_vCBSEstCred;
}

void GEstornoCredTot::set_vCBSEstCred(const double &vCBSEstCred)
{
    m_vCBSEstCred = vCBSEstCred;
}

IbsCbsTot::IbsCbsTot():
    gIBS(new GIBSTot),
    gCBS(new GCBSTot),
    gMono(new GMonoTot),
    gEstornoCred(new GEstornoCredTot)
{
}

void IbsCbsTot::clear()
{
    m_vBCIBSCBS = 0.0;
    gIBS->clear();
    gCBS->clear();
    gMono->clear();
    gEstornoCred->clear();
}

bool IbsCbsTot::isEmpty() const
{
    return qFuzzyIsNull(m_vBCIBSCBS) &&
           gIBS->isEmpty() &&
           gCBS->isEmpty() &&
           gMono->isEmpty() &&
           gEstornoCred->isEmpty();
}

double IbsCbsTot::get_vBCIBSCBS() const
{
    return m_vBCIBSCBS;
}

void IbsCbsTot::set_vBCIBSCBS(const double &vBCIBSCBS)
{
    m_vBCIBSCBS = vBCIBSCBS;
}
