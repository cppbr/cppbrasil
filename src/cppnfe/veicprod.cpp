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


#include "veicprod.h"

VeicProd::VeicProd(): m_tpOp(TpOp::None), m_anoMod(0), m_anoFab(0),
    m_condVeic(CondVeic::None), m_lota(0), m_tpRest(TpRest::None)
{
}

VeicProd::~VeicProd()
{
}

void VeicProd::clear()
{
    this->m_tpOp = TpOp::None;
    this->m_chassi.clear();
    this->m_cCor.clear();
    this->m_xCor.clear();
    this->m_pot.clear();
    this->m_cilin.clear();
    this->m_pesoL.clear();
    this->m_pesoB.clear();
    this->m_nSerie.clear();
    this->m_tpComb.clear();
    this->m_nMotor.clear();
    this->m_CMT.clear();
    this->m_dist.clear();
    this->m_anoMod = 0;
    this->m_anoFab = 0;
    this->m_tpPint.clear();
    this->m_tpVeic.clear();
    this->m_espVeic.clear();
    this->m_VIN.clear();
    this->m_condVeic = CondVeic::None;
    this->m_cMod.clear();
    this->m_cCorDENATRAN.clear();
    this->m_lota = 0;
    this->m_tpRest = TpRest::None;
}

TpOp VeicProd::get_tpOp() const
{
    return this->m_tpOp;
}

void VeicProd::set_tpOp(const TpOp &tpOp)
{
    this->m_tpOp = tpOp;
}

QString VeicProd::get_chassi() const
{
    return this->m_chassi;
}

void VeicProd::set_chassi(const QString &chassi)
{
    this->m_chassi = chassi;
}

QString VeicProd::get_cCor() const
{
    return this->m_cCor;
}

void VeicProd::set_cCor(const QString &cCor)
{
    this->m_cCor = cCor;
}

QString VeicProd::get_xCor() const
{
    return this->m_xCor;
}

void VeicProd::set_xCor(const QString &xCor)
{
    this->m_xCor = xCor;
}

QString VeicProd::get_pot() const
{
    return this->m_pot;
}

void VeicProd::set_pot(const QString &pot)
{
    this->m_pot = pot;
}

QString VeicProd::get_cilin() const
{
    return this->m_cilin;
}

void VeicProd::set_cilin(const QString &cilin)
{
    this->m_cilin = cilin;
}

QString VeicProd::get_pesoL() const
{
    return this->m_pesoL;
}

void VeicProd::set_pesoL(const QString &pesoL)
{
    this->m_pesoL = pesoL;
}

QString VeicProd::get_pesoB() const
{
    return this->m_pesoB;
}

void VeicProd::set_pesoB(const QString &pesoB)
{
    this->m_pesoB = pesoB;
}

QString VeicProd::get_nSerie() const
{
    return this->m_nSerie;
}

void VeicProd::set_nSerie(const QString &nSerie)
{
    this->m_nSerie = nSerie;
}

QString VeicProd::get_tpComb() const
{
    return this->m_tpComb;
}

void VeicProd::set_tpComb(const QString &tpComb)
{
    this->m_tpComb = tpComb;
}

QString VeicProd::get_nMotor() const
{
    return this->m_nMotor;
}

void VeicProd::set_nMotor(const QString &nMotor)
{
    this->m_nMotor = nMotor;
}

QString VeicProd::get_CMT() const
{
    return this->m_CMT;
}

void VeicProd::set_CMT(const QString &CMT)
{
    this->m_CMT = CMT;
}

QString VeicProd::get_dist() const
{
    return this->m_dist;
}

void VeicProd::set_dist(const QString &dist)
{
    this->m_dist = dist;
}

int VeicProd::get_anoMod() const
{
    return this->m_anoMod;
}

void VeicProd::set_anoMod(const int &anoMod)
{
    this->m_anoMod = anoMod;
}

int VeicProd::get_anoFab() const
{
    return this->m_anoFab;
}

void VeicProd::set_anoFab(const int &anoFab)
{
    this->m_anoFab = anoFab;
}

QString VeicProd::get_tpPint() const
{
    return this->m_tpPint;
}

void VeicProd::set_tpPint(const QString &tpPint)
{
    this->m_tpPint = tpPint;
}

QString VeicProd::get_tpVeic() const
{
    return this->m_tpVeic;
}

void VeicProd::set_tpVeic(const QString &tpVeic)
{
    this->m_tpVeic = tpVeic;
}

QString VeicProd::get_espVeic() const
{
    return this->m_espVeic;
}

void VeicProd::set_espVeic(const QString &espVeic)
{
    this->m_espVeic = espVeic;
}

QString VeicProd::get_VIN() const
{
    return this->m_VIN;
}

void VeicProd::set_VIN(const QString &VIN)
{
    this->m_VIN = VIN;
}

CondVeic VeicProd::get_condVeic() const
{
    return this->m_condVeic;
}

void VeicProd::set_condVeic(const CondVeic &condVeic)
{
    this->m_condVeic = condVeic;
}

QString VeicProd::get_cMod() const
{
    return this->m_cMod;
}

void VeicProd::set_cMod(const QString &cMod)
{
    this->m_cMod = cMod;
}

QString VeicProd::get_cCorDENATRAN() const
{
    return this->m_cCorDENATRAN;
}

void VeicProd::set_cCorDENATRAN(const QString &cCorDENATRAN)
{
    this->m_cCorDENATRAN = cCorDENATRAN;
}

int VeicProd::get_lota() const
{
    return this->m_lota;
}

void VeicProd::set_lota(const int &lota)
{
    this->m_lota = lota;
}

TpRest VeicProd::get_tpRest() const
{
    return this->m_tpRest;
}

void VeicProd::set_tpRest(const TpRest &tpRest)
{
    this->m_tpRest = tpRest;
}
