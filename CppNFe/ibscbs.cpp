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

#include "ibscbs.h"

void GDif::clear()
{
    m_pDif = 0.0;
    m_vDif = 0.0;
}

bool GDif::isEmpty() const
{
    return qFuzzyIsNull(m_pDif) &&
           qFuzzyIsNull(m_vDif);
}

double GDif::get_pDif() const
{
    return m_pDif;
}

void GDif::set_pDif(const double &pDif)
{
    m_pDif = pDif;
}

double GDif::get_vDif() const
{
    return m_vDif;
}

void GDif::set_vDif(const double &vDif)
{
    m_vDif = vDif;
}

void GDevTrib::clear()
{
    m_pDevTrib = 0.0;
    m_vDevTrib = 0.0;
}

bool GDevTrib::isEmpty() const
{
    return qFuzzyIsNull(m_pDevTrib) &&
           qFuzzyIsNull(m_vDevTrib);
}

double GDevTrib::get_pDevTrib() const
{
    return m_pDevTrib;
}

void GDevTrib::set_pDevTrib(const double &pDevTrib)
{
    m_pDevTrib = pDevTrib;
}

double GDevTrib::get_vDevTrib() const
{
    return m_vDevTrib;
}

void GDevTrib::set_vDevTrib(const double &vDevTrib)
{
    m_vDevTrib = vDevTrib;
}

void GRed::clear()
{
    m_pRedAliq = 0.0;
    m_pAliqEfet = 0.0;
}

bool GRed::isEmpty() const
{
    return qFuzzyIsNull(m_pRedAliq) &&
           qFuzzyIsNull(m_pAliqEfet);
}

double GRed::get_pRedAliq() const
{
    return m_pRedAliq;
}

void GRed::set_pRedAliq(const double &pRedAliq)
{
    m_pRedAliq = pRedAliq;
}

double GRed::get_pAliqEfet() const
{
    return m_pAliqEfet;
}

void GRed::set_pAliqEfet(const double &pAliqEfet)
{
    m_pAliqEfet = pAliqEfet;
}

GIBSUF::GIBSUF():
    gDif(new GDif),
    gDevTrib(new GDevTrib),
    gRed(new GRed)
{
}

void GIBSUF::clear()
{
    m_pIBSUF = 0.0;
    m_vIBSUF = 0.0;
    gDif->clear();
    gDevTrib->clear();
    gRed->clear();
}

bool GIBSUF::isEmpty() const
{
    return qFuzzyIsNull(m_pIBSUF) &&
           qFuzzyIsNull(m_vIBSUF) &&
           gDif->isEmpty() &&
           gDevTrib->isEmpty() &&
           gRed->isEmpty();
}

double GIBSUF::get_pIBSUF() const
{
    return m_pIBSUF;
}

void GIBSUF::set_pIBSUF(const double &pIBSUF)
{
    m_pIBSUF = pIBSUF;
}

double GIBSUF::get_vIBSUF() const
{
    return m_vIBSUF;
}

void GIBSUF::set_vIBSUF(const double &vIBSUF)
{
    m_vIBSUF = vIBSUF;
}

GIBSMun::GIBSMun():
    gDif(new GDif),
    gDevTrib(new GDevTrib),
    gRed(new GRed)
{
}

void GIBSMun::clear()
{
    m_pIBSMun = 0.0;
    m_vIBSMun = 0.0;
    gDif->clear();
    gDevTrib->clear();
    gRed->clear();
}

bool GIBSMun::isEmpty() const
{
    return qFuzzyIsNull(m_pIBSMun) &&
           qFuzzyIsNull(m_vIBSMun) &&
           gDif->isEmpty() &&
           gDevTrib->isEmpty() &&
           gRed->isEmpty();
}

double GIBSMun::get_pIBSMun() const
{
    return m_pIBSMun;
}

void GIBSMun::set_pIBSMun(const double &pIBSMun)
{
    m_pIBSMun = pIBSMun;
}

double GIBSMun::get_vIBSMun() const
{
    return m_vIBSMun;
}

void GIBSMun::set_vIBSMun(const double &vIBSMun)
{
    m_vIBSMun = vIBSMun;
}

void GALCZFMCBS::clear()
{
    m_tpALCZFMCBS = TpALCZFMCBS::None;
    m_nProcSuframa.clear();
    m_pAliqEfetRegCBS = 0.0;
    m_vTribRegCBS = 0.0;
}

bool GALCZFMCBS::isEmpty() const
{
    return m_tpALCZFMCBS == TpALCZFMCBS::None &&
           m_nProcSuframa.isEmpty() &&
           qFuzzyIsNull(m_pAliqEfetRegCBS) &&
           qFuzzyIsNull(m_vTribRegCBS);
}

TpALCZFMCBS GALCZFMCBS::get_tpALCZFMCBS() const
{
    return m_tpALCZFMCBS;
}

void GALCZFMCBS::set_tpALCZFMCBS(const TpALCZFMCBS &tpALCZFMCBS)
{
    m_tpALCZFMCBS = tpALCZFMCBS;
}

QString GALCZFMCBS::get_nProcSuframa() const
{
    return m_nProcSuframa;
}

void GALCZFMCBS::set_nProcSuframa(const QString &nProcSuframa)
{
    m_nProcSuframa = nProcSuframa;
}

double GALCZFMCBS::get_pAliqEfetRegCBS() const
{
    return m_pAliqEfetRegCBS;
}

void GALCZFMCBS::set_pAliqEfetRegCBS(const double &pAliqEfetRegCBS)
{
    m_pAliqEfetRegCBS = pAliqEfetRegCBS;
}

double GALCZFMCBS::get_vTribRegCBS() const
{
    return m_vTribRegCBS;
}

void GALCZFMCBS::set_vTribRegCBS(const double &vTribRegCBS)
{
    m_vTribRegCBS = vTribRegCBS;
}

GCBS::GCBS():
    gDif(new GDif),
    gDevTrib(new GDevTrib),
    gRed(new GRed),
    gALCZFMCBS(new GALCZFMCBS)
{
}

void GCBS::clear()
{
    m_pCBS = 0.0;
    m_vCBS = 0.0;
    gDif->clear();
    gDevTrib->clear();
    gRed->clear();
    gALCZFMCBS->clear();
}

bool GCBS::isEmpty() const
{
    return qFuzzyIsNull(m_pCBS) &&
           qFuzzyIsNull(m_vCBS) &&
           gDif->isEmpty() &&
           gDevTrib->isEmpty() &&
           gRed->isEmpty() &&
           gALCZFMCBS->isEmpty();
}

double GCBS::get_pCBS() const
{
    return m_pCBS;
}

void GCBS::set_pCBS(const double &pCBS)
{
    m_pCBS = pCBS;
}

double GCBS::get_vCBS() const
{
    return m_vCBS;
}

void GCBS::set_vCBS(const double &vCBS)
{
    m_vCBS = vCBS;
}

void GTribRegular::clear()
{
    m_CSTReg.clear();
    m_cClassTribReg.clear();
    m_pAliqEfetRegIBSUF = 0.0;
    m_vTribRegIBSUF = 0.0;
    m_pAliqEfetRegIBSMun = 0.0;
    m_vTribRegIBSMun = 0.0;
    m_pAliqEfetRegCBS = 0.0;
    m_vTribRegCBS = 0.0;
}

bool GTribRegular::isEmpty() const
{
    return m_CSTReg.isEmpty() &&
           m_cClassTribReg.isEmpty() &&
           qFuzzyIsNull(m_pAliqEfetRegIBSUF) &&
           qFuzzyIsNull(m_vTribRegIBSUF) &&
           qFuzzyIsNull(m_pAliqEfetRegIBSMun) &&
           qFuzzyIsNull(m_vTribRegIBSMun) &&
           qFuzzyIsNull(m_pAliqEfetRegCBS) &&
           qFuzzyIsNull(m_vTribRegCBS);
}

QString GTribRegular::get_CSTReg() const
{
    return m_CSTReg;
}

void GTribRegular::set_CSTReg(const QString &CSTReg)
{
    m_CSTReg = CSTReg;
}

QString GTribRegular::get_cClassTribReg() const
{
    return m_cClassTribReg;
}

void GTribRegular::set_cClassTribReg(const QString &cClassTribReg)
{
    m_cClassTribReg = cClassTribReg;
}

double GTribRegular::get_pAliqEfetRegIBSUF() const
{
    return m_pAliqEfetRegIBSUF;
}

void GTribRegular::set_pAliqEfetRegIBSUF(const double &pAliqEfetRegIBSUF)
{
    m_pAliqEfetRegIBSUF = pAliqEfetRegIBSUF;
}

double GTribRegular::get_vTribRegIBSUF() const
{
    return m_vTribRegIBSUF;
}

void GTribRegular::set_vTribRegIBSUF(const double &vTribRegIBSUF)
{
    m_vTribRegIBSUF = vTribRegIBSUF;
}

double GTribRegular::get_pAliqEfetRegIBSMun() const
{
    return m_pAliqEfetRegIBSMun;
}

void GTribRegular::set_pAliqEfetRegIBSMun(const double &pAliqEfetRegIBSMun)
{
    m_pAliqEfetRegIBSMun = pAliqEfetRegIBSMun;
}

double GTribRegular::get_vTribRegIBSMun() const
{
    return m_vTribRegIBSMun;
}

void GTribRegular::set_vTribRegIBSMun(const double &vTribRegIBSMun)
{
    m_vTribRegIBSMun = vTribRegIBSMun;
}

double GTribRegular::get_pAliqEfetRegCBS() const
{
    return m_pAliqEfetRegCBS;
}

void GTribRegular::set_pAliqEfetRegCBS(const double &pAliqEfetRegCBS)
{
    m_pAliqEfetRegCBS = pAliqEfetRegCBS;
}

double GTribRegular::get_vTribRegCBS() const
{
    return m_vTribRegCBS;
}

void GTribRegular::set_vTribRegCBS(const double &vTribRegCBS)
{
    m_vTribRegCBS = vTribRegCBS;
}

void GTribCompraGov::clear()
{
    m_pAliqIBSUF = 0.0;
    m_vTribIBSUF = 0.0;
    m_pAliqIBSMun = 0.0;
    m_vTribIBSMun = 0.0;
    m_pAliqCBS = 0.0;
    m_vTribCBS = 0.0;
}

bool GTribCompraGov::isEmpty() const
{
    return qFuzzyIsNull(m_pAliqIBSUF) &&
           qFuzzyIsNull(m_vTribIBSUF) &&
           qFuzzyIsNull(m_pAliqIBSMun) &&
           qFuzzyIsNull(m_vTribIBSMun) &&
           qFuzzyIsNull(m_pAliqCBS) &&
           qFuzzyIsNull(m_vTribCBS);
}

double GTribCompraGov::get_pAliqIBSUF() const
{
    return m_pAliqIBSUF;
}

void GTribCompraGov::set_pAliqIBSUF(const double &pAliqIBSUF)
{
    m_pAliqIBSUF = pAliqIBSUF;
}

double GTribCompraGov::get_vTribIBSUF() const
{
    return m_vTribIBSUF;
}

void GTribCompraGov::set_vTribIBSUF(const double &vTribIBSUF)
{
    m_vTribIBSUF = vTribIBSUF;
}

double GTribCompraGov::get_pAliqIBSMun() const
{
    return m_pAliqIBSMun;
}

void GTribCompraGov::set_pAliqIBSMun(const double &pAliqIBSMun)
{
    m_pAliqIBSMun = pAliqIBSMun;
}

double GTribCompraGov::get_vTribIBSMun() const
{
    return m_vTribIBSMun;
}

void GTribCompraGov::set_vTribIBSMun(const double &vTribIBSMun)
{
    m_vTribIBSMun = vTribIBSMun;
}

double GTribCompraGov::get_pAliqCBS() const
{
    return m_pAliqCBS;
}

void GTribCompraGov::set_pAliqCBS(const double &pAliqCBS)
{
    m_pAliqCBS = pAliqCBS;
}

double GTribCompraGov::get_vTribCBS() const
{
    return m_vTribCBS;
}

void GTribCompraGov::set_vTribCBS(const double &vTribCBS)
{
    m_vTribCBS = vTribCBS;
}

GIBSCBS::GIBSCBS():
    gIBSUF(new GIBSUF),
    gIBSMun(new GIBSMun),
    gCBS(new GCBS),
    gTribRegular(new GTribRegular),
    gTribCompraGov(new GTribCompraGov)
{
}

void GIBSCBS::clear()
{
    m_vBC = 0.0;
    m_vIBS = 0.0;
    gIBSUF->clear();
    gIBSMun->clear();
    gCBS->clear();
    gTribRegular->clear();
    gTribCompraGov->clear();
}

bool GIBSCBS::isEmpty() const
{
    return qFuzzyIsNull(m_vBC) &&
           qFuzzyIsNull(m_vIBS) &&
           gIBSUF->isEmpty() &&
           gIBSMun->isEmpty() &&
           gCBS->isEmpty() &&
           gTribRegular->isEmpty() &&
           gTribCompraGov->isEmpty();
}

double GIBSCBS::get_vBC() const
{
    return m_vBC;
}

void GIBSCBS::set_vBC(const double &vBC)
{
    m_vBC = vBC;
}

double GIBSCBS::get_vIBS() const
{
    return m_vIBS;
}

void GIBSCBS::set_vIBS(const double &vIBS)
{
    m_vIBS = vIBS;
}

void GTransfCred::clear()
{
    m_vIBS = 0.0;
    m_vCBS = 0.0;
}

bool GTransfCred::isEmpty() const
{
    return qFuzzyIsNull(m_vIBS) &&
           qFuzzyIsNull(m_vCBS);
}

double GTransfCred::get_vIBS() const
{
    return m_vIBS;
}

void GTransfCred::set_vIBS(const double &vIBS)
{
    m_vIBS = vIBS;
}

double GTransfCred::get_vCBS() const
{
    return m_vCBS;
}

void GTransfCred::set_vCBS(const double &vCBS)
{
    m_vCBS = vCBS;
}

void GAjusteCompet::clear()
{
    m_competApur = QDate();
    m_vIBS = 0.0;
    m_vCBS = 0.0;
}

bool GAjusteCompet::isEmpty() const
{
    return !m_competApur.isValid() &&
           qFuzzyIsNull(m_vIBS) &&
           qFuzzyIsNull(m_vCBS);
}

QDate GAjusteCompet::get_competApur() const
{
    return m_competApur;
}

void GAjusteCompet::set_competApur(const QDate &competApur)
{
    m_competApur = competApur;
}

double GAjusteCompet::get_vIBS() const
{
    return m_vIBS;
}

void GAjusteCompet::set_vIBS(const double &vIBS)
{
    m_vIBS = vIBS;
}

double GAjusteCompet::get_vCBS() const
{
    return m_vCBS;
}

void GAjusteCompet::set_vCBS(const double &vCBS)
{
    m_vCBS = vCBS;
}

void GEstornoCred::clear()
{
    m_vIBSEstCred = 0.0;
    m_vCBSEstCred = 0.0;
}

bool GEstornoCred::isEmpty() const
{
    return qFuzzyIsNull(m_vIBSEstCred) &&
           qFuzzyIsNull(m_vCBSEstCred);
}

double GEstornoCred::get_vIBSEstCred() const
{
    return m_vIBSEstCred;
}

void GEstornoCred::set_vIBSEstCred(const double &vIBSEstCred)
{
    m_vIBSEstCred = vIBSEstCred;
}

double GEstornoCred::get_vCBSEstCred() const
{
    return m_vCBSEstCred;
}

void GEstornoCred::set_vCBSEstCred(const double &vCBSEstCred)
{
    m_vCBSEstCred = vCBSEstCred;
}

void GCredPres::clear()
{
    m_pCredPres = 0.0;
    m_vCredPres = 0.0;
    m_vCredPresCondSus = 0.0;
}

bool GCredPres::isEmpty() const
{
    return qFuzzyIsNull(m_pCredPres) &&
           qFuzzyIsNull(m_vCredPres) &&
           qFuzzyIsNull(m_vCredPresCondSus);
}

double GCredPres::get_pCredPres() const
{
    return m_pCredPres;
}

void GCredPres::set_pCredPres(const double &pCredPres)
{
    m_pCredPres = pCredPres;
}

double GCredPres::get_vCredPres() const
{
    return m_vCredPres;
}

void GCredPres::set_vCredPres(const double &vCredPres)
{
    m_vCredPres = vCredPres;
}

double GCredPres::get_vCredPresCondSus() const
{
    return m_vCredPresCondSus;
}

void GCredPres::set_vCredPresCondSus(const double &vCredPresCondSus)
{
    m_vCredPresCondSus = vCredPresCondSus;
}

GCredPresOper::GCredPresOper():
    gIBSCredPres(new GCredPres),
    gCBSCredPres(new GCredPres)
{
}

void GCredPresOper::clear()
{
    m_vBCCredPres = 0.0;
    m_cCredPres.clear();
    gIBSCredPres->clear();
    gCBSCredPres->clear();
}

bool GCredPresOper::isEmpty() const
{
    return qFuzzyIsNull(m_vBCCredPres) &&
           m_cCredPres.isEmpty() &&
           gIBSCredPres->isEmpty() &&
           gCBSCredPres->isEmpty();
}

double GCredPresOper::get_vBCCredPres() const
{
    return m_vBCCredPres;
}

void GCredPresOper::set_vBCCredPres(const double &vBCCredPres)
{
    m_vBCCredPres = vBCCredPres;
}

QString GCredPresOper::get_cCredPres() const
{
    return m_cCredPres;
}

void GCredPresOper::set_cCredPres(const QString &cCredPres)
{
    m_cCredPres = cCredPres;
}

void GCredPresIBSZFM::clear()
{
    m_competApur = QDate();
    m_tpCredPresIBSZFM = TpCredPresIBSZFM::None;
    m_vCredPresIBSZFM = 0.0;
}

bool GCredPresIBSZFM::isEmpty() const
{
    return !m_competApur.isValid() &&
           m_tpCredPresIBSZFM == TpCredPresIBSZFM::None &&
           qFuzzyIsNull(m_vCredPresIBSZFM);
}

QDate GCredPresIBSZFM::get_competApur() const
{
    return m_competApur;
}

void GCredPresIBSZFM::set_competApur(const QDate &competApur)
{
    m_competApur = competApur;
}

TpCredPresIBSZFM GCredPresIBSZFM::get_tpCredPresIBSZFM() const
{
    return m_tpCredPresIBSZFM;
}

void GCredPresIBSZFM::set_tpCredPresIBSZFM(const TpCredPresIBSZFM &tpCredPresIBSZFM)
{
    m_tpCredPresIBSZFM = tpCredPresIBSZFM;
}

double GCredPresIBSZFM::get_vCredPresIBSZFM() const
{
    return m_vCredPresIBSZFM;
}

void GCredPresIBSZFM::set_vCredPresIBSZFM(const double &vCredPresIBSZFM)
{
    m_vCredPresIBSZFM = vCredPresIBSZFM;
}

IbsCbs::IbsCbs():
    gIBSCBS(new GIBSCBS),
    gIBSCBSMono(new GIBSCBSMono),
    gTransfCred(new GTransfCred),
    gAjusteCompet(new GAjusteCompet),
    gEstornoCred(new GEstornoCred),
    gCredPresOper(new GCredPresOper),
    gCredPresIBSZFM(new GCredPresIBSZFM)
{
}

void IbsCbs::clear()
{
    m_CST.clear();
    m_cClassTrib.clear();
    m_indDoacao = IndDoacao::None;
    gIBSCBS->clear();
    gIBSCBSMono->clear();
    gTransfCred->clear();
    gAjusteCompet->clear();
    gEstornoCred->clear();
    gCredPresOper->clear();
    gCredPresIBSZFM->clear();
}

bool IbsCbs::isEmpty() const
{
    return m_CST.isEmpty() &&
           m_cClassTrib.isEmpty() &&
           m_indDoacao == IndDoacao::None &&
           gIBSCBS->isEmpty() &&
           gIBSCBSMono->isEmpty() &&
           gTransfCred->isEmpty() &&
           gAjusteCompet->isEmpty() &&
           gEstornoCred->isEmpty() &&
           gCredPresOper->isEmpty() &&
           gCredPresIBSZFM->isEmpty();
}

QString IbsCbs::get_CST() const
{
    return m_CST;
}

void IbsCbs::set_CST(const QString &CST)
{
    m_CST = CST;
}

QString IbsCbs::get_cClassTrib() const
{
    return m_cClassTrib;
}

void IbsCbs::set_cClassTrib(const QString &cClassTrib)
{
    m_cClassTrib = cClassTrib;
}

IndDoacao IbsCbs::get_indDoacao() const
{
    return m_indDoacao;
}

void IbsCbs::set_indDoacao(const IndDoacao &indDoacao)
{
    m_indDoacao = indDoacao;
}
