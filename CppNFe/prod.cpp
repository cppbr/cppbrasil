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


#include "prod.h"

Prod::Prod(): DI(new Container<Di>), detExport(new Container<DetExport>),
    rastro(new Container<Rastro>), veicProd(new VeicProd), med(new Med),
    arma(new Container<Arma>), comb(new Comb),
    m_nItem(0), m_qCom(0.0), m_vUnCom(0.0), m_vProd(0.0), m_qTrib(0.0),
    m_vUnTrib(0.0), m_vFrete(0.0), m_vSeg(0.0), m_vDesc(0.0), m_vOutro(0.0),
    m_indTot(IndTot::None), m_indEscala(IndEscala::None),
    m_nItemPed(0)
{
}

Prod::~Prod()
{
}

void Prod::clear()
{
    this->m_nItem = 0;
    this->m_cProd.clear();
    this->m_cEAN.clear();
    this->m_xProd.clear();
    this->m_NCM.clear();
    this->m_NVE.clear();
    this->m_EXTIPI.clear();
    this->m_CFOP.clear();
    this->m_uCom.clear();
    this->m_qCom = 0.0;
    this->m_vUnCom = 0.0;
    this->m_vProd = 0.0;
    this->m_cEANTrib.clear();
    this->m_uTrib.clear();
    this->m_qTrib = 0.0;
    this->m_vUnTrib = 0.0;
    this->m_vFrete = 0.0;
    this->m_vSeg = 0.0;
    this->m_vDesc = 0.0;
    this->m_vOutro = 0.0;
    this->m_indTot = IndTot::None;
    this->m_CEST.clear();
    this->m_indEscala = IndEscala::None;
    this->m_CNPJFab.clear();
    this->m_cBenef.clear();
    this->m_xPed.clear();
    this->m_nItemPed = 0;
    this->m_nFCI.clear();
    this->m_nRECOPI.clear();

    this->DI->clear();
    this->DI->clear();
    this->detExport->clear();
    this->rastro->clear();
    this->veicProd->clear();
    this->med->clear();
    this->arma->clear();
    this->comb->clear();
}

int Prod::get_nItem() const
{
    return this->m_nItem;
}

void Prod::set_nItem(const int &nItem)
{
    this->m_nItem = nItem;
}

QString Prod::get_cProd() const
{
    return this->m_cProd;
}

void Prod::set_cProd(const QString &cProd)
{
    this->m_cProd = cProd;
}

QString Prod::get_cEAN() const
{
    return this->m_cEAN;
}

void Prod::set_cEAN(const QString &cEAN)
{
    this->m_cEAN = cEAN;
}

QString Prod::get_cBarra() const
{
    return  this->m_cBarra;
}

void Prod::set_cBarra(const QString &cBarra)
{
    this->m_cBarra = cBarra;
}

QString Prod::get_xProd() const
{
    return this->m_xProd;
}

void Prod::set_xProd(const QString &xProd)
{
    this->m_xProd = xProd;
}

QString Prod::get_NCM() const
{
    return this->m_NCM;
}

void Prod::set_NCM(const QString &NCM)
{
    this->m_NCM = NCM;
}

QString Prod::get_NVE() const
{
    return this->m_NVE;
}

void Prod::set_NVE(const QString &NVE)
{
    this->m_NVE = NVE;
}

QString Prod::get_EXTIPI() const
{
    return this->m_EXTIPI;
}

void Prod::set_EXTIPI(const QString &EXTIPI)
{
    this->m_EXTIPI = EXTIPI;
}

QString Prod::get_CFOP() const
{
    return this->m_CFOP;
}

void Prod::set_CFOP(const QString &CFOP)
{
    this->m_CFOP = CFOP;
}

QString Prod::get_uCom() const
{
    return this->m_uCom;
}

void Prod::set_uCom(const QString &uCom)
{
    this->m_uCom = uCom;
}

double Prod::get_qCom() const
{
    return this->m_qCom;
}

void Prod::set_qCom(const double &qCom)
{
    this->m_qCom = qCom;
}

double Prod::get_vUnCom() const
{
    return this->m_vUnCom;
}

void Prod::set_vUnCom(const double &vUnCom)
{
    this->m_vUnCom = vUnCom;
}

double Prod::get_vProd() const
{
    return this->m_vProd;
}

void Prod::set_vProd(const double &vProd)
{
    this->m_vProd = vProd;
}

QString Prod::get_cEANTrib() const
{
    return this->m_cEANTrib;
}

void Prod::set_cEANTrib(const QString &cEANTrib)
{
    this->m_cEANTrib = cEANTrib;
}

QString Prod::get_cBarraTrib() const
{
    return this->m_cBarraTrib;
}

void Prod::set_cBarraTrib(const QString &cBarraTrib)
{
    this->m_cBarraTrib = cBarraTrib;
}

QString Prod::get_uTrib() const
{
    return this->m_uTrib;
}

void Prod::set_uTrib(const QString &uTrib)
{
    this->m_uTrib = uTrib;
}

double Prod::get_qTrib() const
{
    return this->m_qTrib;
}

void Prod::set_qTrib(const double &qTrib)
{
    this->m_qTrib = qTrib;
}

double Prod::get_vUnTrib() const
{
    return this->m_vUnTrib;
}

void Prod::set_vUnTrib(const double &vUnTrib)
{
    this->m_vUnTrib = vUnTrib;
}

double Prod::get_vFrete() const
{
    return this->m_vFrete;
}

void Prod::set_vFrete(const double &vFrete)
{
    this->m_vFrete = vFrete;
}

double Prod::get_vSeg() const
{
    return this->m_vSeg;
}

void Prod::set_vSeg(const double &vSeg)
{
    this->m_vSeg = vSeg;
}

double Prod::get_vDesc() const
{
    return this->m_vDesc;
}

void Prod::set_vDesc(const double &vDesc)
{
    this->m_vDesc = vDesc;
}

double Prod::get_vOutro() const
{
    return this->m_vOutro;
}

void Prod::set_vOutro(const double &vOutro)
{
    this->m_vOutro = vOutro;
}

IndTot Prod::get_indTot() const
{
    return this->m_indTot;
}

void Prod::set_indTot(const IndTot &indTot)
{
    this->m_indTot = indTot;
}

QString Prod::get_CEST() const
{
    return this->m_CEST;
}

void Prod::set_CEST(const QString &CEST)
{
    this->m_CEST = CEST;
}

IndEscala Prod::get_indEscala() const
{
    return this->m_indEscala;
}

void Prod::set_indEscala(const IndEscala &indEscala)
{
    this->m_indEscala = indEscala;
}

QString Prod::get_CNPJFab() const
{
    return this->m_CNPJFab;
}

void Prod::set_CNPJFab(const QString &CNPJFab)
{
    this->m_CNPJFab = CNPJFab;
}

QString Prod::get_cBenef() const
{
    return this->m_cBenef;
}

void Prod::set_cBenef(const QString &cBenef)
{
    this->m_cBenef = cBenef;
}

QString Prod::get_xPed() const
{
    return this->m_xPed;
}

void Prod::set_xPed(const QString &xPed)
{
    this->m_xPed = xPed;
}

int Prod::get_nItemPed() const
{
    return this->m_nItemPed;
}

void Prod::set_nItemPed(const int &nItemPed)
{
    this->m_nItemPed = nItemPed;
}

QString Prod::get_nFCI() const
{
    return this->m_nFCI;
}

void Prod::set_nFCI(const QString &nFCI)
{
    this->m_nFCI = nFCI;
}

QString Prod::get_nRECOPI() const
{
    return this->m_nRECOPI;
}

void Prod::set_nRECOPI(const QString &nRECOPI)
{
    this->m_nRECOPI = nRECOPI;
}
