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


#include "ide.h"

Ide::Ide(): NFref(new Container<NFRef>), m_cUF(0), m_cNF(0), m_mod(ModeloDF::NFe),
    m_serie(0), m_nNF(0), m_tpNF(TpNF::None), m_idDest(IdDest::None),
    m_cMunFG(0), m_tpImp(TpImp::None), m_tpEmis(TpEmis::None),
    m_tpAmb(TpAmb::None), m_finNFe(FinNFe::None),
    m_indFinal(IndFinal::None), m_indPres(IndPres::None),
    m_indIntermed(IndIntermed::None), m_procEmi(ProcEmi::None)

{
}

Ide::~Ide()
{
}

void Ide::clear()
{
    this->m_cUF = 0;
    this->m_cNF = 0;
    this->m_natOp.clear();
    this->m_mod = ModeloDF::NFe;
    this->m_serie = 0;
    this->m_nNF = 0;
    CppUtility::clearDateTime(this->m_dhEmi);
    CppUtility::clearDateTime(this->m_dhSaiEnt);
    this->m_tpNF = TpNF::None;
    this->m_idDest = IdDest::None;
    this->m_cMunFG = 0;
    this->m_tpImp = TpImp::None;
    this->m_tpEmis = TpEmis::None;
    this->m_cDV.clear();
    this->m_tpAmb = TpAmb::None;
    this->m_finNFe = FinNFe::None;
    this->m_indFinal = IndFinal::None;
    this->m_indPres = IndPres::None;
    this->m_indIntermed = IndIntermed::None;
    this->m_procEmi = ProcEmi::None;
    this->m_verProc.clear();
    CppUtility::clearDateTime(this->m_dhCont);
    this->m_xJust.clear();

    this->NFref->clear();
}

int Ide::get_cUF() const
{
  return this->m_cUF;
}

void Ide::set_cUF(const int &cUF)
{
  this->m_cUF = cUF;
}

int Ide::get_cNF() const
{
  return  this->m_cNF;
}

void Ide::set_cNF(const int &cNF)
{
  this->m_cNF = cNF;
}

QString Ide::get_natOp() const
{
  return this->m_natOp;
}

void Ide::set_natOp(const QString &natOp)
{
  this->m_natOp = natOp;
}

ModeloDF Ide::get_mod() const
{
  return this->m_mod;
}

void Ide::set_mod(const ModeloDF &mod)
{
  this->m_mod = mod;
}

int Ide::get_serie() const
{
  return this->m_serie;
}

void Ide::set_serie(const int &serie)
{
  this->m_serie = serie;
}

int Ide::get_nNF() const
{
  return this->m_nNF;
}

void Ide::set_nNF(const int &nNF)
{
  this->m_nNF = nNF;
}

QDateTime Ide::get_dhEmi() const
{
  return this->m_dhEmi;
}

void Ide::set_dhEmi(const QDateTime &dhEmi)
{
  this->m_dhEmi = dhEmi;
}

QDateTime Ide::get_dhSaiEnt() const
{
  return this->m_dhSaiEnt;
}

void Ide::set_dhSaiEnt(const QDateTime &dhSaiEnt)
{
  this->m_dhSaiEnt = dhSaiEnt;
}

TpNF Ide::get_tpNF() const
{
  return this->m_tpNF;
}

void Ide::set_tpNF(const TpNF &tpNF)
{
  this->m_tpNF = tpNF;
}

IdDest Ide::get_idDest() const
{
  return this->m_idDest;
}

void Ide::set_idDest(const IdDest &idDest)
{
  this->m_idDest = idDest;
}

int Ide::get_cMunFG() const
{
  return this->m_cMunFG;
}

void Ide::set_cMunFG(const int &cMunFG)
{
  this->m_cMunFG = cMunFG;
}

TpImp Ide::get_tpImp() const
{
  return this->m_tpImp;
}

void Ide::set_tpImp(const TpImp &tpImp)
{
  this->m_tpImp = tpImp;
}

TpEmis Ide::get_tpEmis() const
{
  return this->m_tpEmis;
}

void Ide::set_tpEmis(const TpEmis &tpEmis)
{
  this->m_tpEmis = tpEmis;
}

QString Ide::get_cDV() const
{
  return this->m_cDV;
}

void Ide::set_cDV(const QString &cDV)
{
  this->m_cDV = cDV;
}

TpAmb Ide::get_tpAmb() const
{
  return this->m_tpAmb;
}

void Ide::set_tpAmb(const TpAmb &tpAmb)
{
  this->m_tpAmb = tpAmb;
}

FinNFe Ide::get_finNFe() const
{
  return this->m_finNFe;
}

void Ide::set_finNFe(const FinNFe &finNFe)
{
  this->m_finNFe = finNFe;
}

IndFinal Ide::get_indFinal() const
{
  return this->m_indFinal;
}

void Ide::set_indFinal(const IndFinal &indFinal)
{
  this->m_indFinal = indFinal;
}

IndPres Ide::get_indPres() const
{
  return this->m_indPres;
}

void Ide::set_indPres(const IndPres &indPres)
{
    this->m_indPres = indPres;
}

IndIntermed Ide::get_indIntermed() const
{
    return this->m_indIntermed;
}

void Ide::set_indIntermed(const IndIntermed &indIntermed)
{
    this->m_indIntermed = indIntermed;
}

ProcEmi Ide::get_procEmi() const
{
  return this->m_procEmi;
}

void Ide::set_procEmi(const ProcEmi &procEmi)
{
  this->m_procEmi = procEmi;
}

QString Ide::get_verProc() const
{
  return this->m_verProc;
}

void Ide::set_verProc(const QString &verProc)
{
  this->m_verProc = verProc;
}

QDateTime Ide::get_dhCont() const
{
  return this->m_dhCont;
}

void Ide::set_dhCont(const QDateTime &dhCont)
{
  this->m_dhCont = dhCont;
}

QString Ide::get_xJust() const
{
  return this->m_xJust;
}

void Ide::set_xJust(const QString &xJust)
{
  this->m_xJust = xJust;
}



