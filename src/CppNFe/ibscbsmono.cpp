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

#include "ibscbsmono.h"

void GIBSMonoPadraoAdRem::clear()
{
    m_qBCMono = 0.0;
    m_adRemIBS = 0.0;
    m_vIBSMono = 0.0;
}

bool GIBSMonoPadraoAdRem::isEmpty() const
{
    return qFuzzyIsNull(m_qBCMono) &&
           qFuzzyIsNull(m_adRemIBS) &&
           qFuzzyIsNull(m_vIBSMono);
}

double GIBSMonoPadraoAdRem::get_qBCMono() const
{
    return m_qBCMono;
}

void GIBSMonoPadraoAdRem::set_qBCMono(const double &qBCMono)
{
    m_qBCMono = qBCMono;
}

double GIBSMonoPadraoAdRem::get_adRemIBS() const
{
    return m_adRemIBS;
}

void GIBSMonoPadraoAdRem::set_adRemIBS(const double &adRemIBS)
{
    m_adRemIBS = adRemIBS;
}

double GIBSMonoPadraoAdRem::get_vIBSMono() const
{
    return m_vIBSMono;
}

void GIBSMonoPadraoAdRem::set_vIBSMono(const double &vIBSMono)
{
    m_vIBSMono = vIBSMono;
}

void GIBSMonoRetenAdRem::clear()
{
    m_qBCMonoReten = 0.0;
    m_adRemIBSReten = 0.0;
    m_vIBSMonoReten = 0.0;
}

bool GIBSMonoRetenAdRem::isEmpty() const
{
    return qFuzzyIsNull(m_qBCMonoReten) &&
           qFuzzyIsNull(m_adRemIBSReten) &&
           qFuzzyIsNull(m_vIBSMonoReten);
}

double GIBSMonoRetenAdRem::get_qBCMonoReten() const
{
    return m_qBCMonoReten;
}

void GIBSMonoRetenAdRem::set_qBCMonoReten(const double &qBCMonoReten)
{
    m_qBCMonoReten = qBCMonoReten;
}

double GIBSMonoRetenAdRem::get_adRemIBSReten() const
{
    return m_adRemIBSReten;
}

void GIBSMonoRetenAdRem::set_adRemIBSReten(const double &adRemIBSReten)
{
    m_adRemIBSReten = adRemIBSReten;
}

double GIBSMonoRetenAdRem::get_vIBSMonoReten() const
{
    return m_vIBSMonoReten;
}

void GIBSMonoRetenAdRem::set_vIBSMonoReten(const double &vIBSMonoReten)
{
    m_vIBSMonoReten = vIBSMonoReten;
}

void GIBSMonoPadraoAdValorem::clear()
{
    m_vBCMono = 0.0;
    m_pAliqMonoUF = 0.0;
    m_vIBSMonoUF = 0.0;
    m_pAliqMonoMun = 0.0;
    m_vIBSMonoMun = 0.0;
    m_vIBSMono = 0.0;
}

bool GIBSMonoPadraoAdValorem::isEmpty() const
{
    return qFuzzyIsNull(m_vBCMono) &&
           qFuzzyIsNull(m_pAliqMonoUF) &&
           qFuzzyIsNull(m_vIBSMonoUF) &&
           qFuzzyIsNull(m_pAliqMonoMun) &&
           qFuzzyIsNull(m_vIBSMonoMun) &&
           qFuzzyIsNull(m_vIBSMono);
}

double GIBSMonoPadraoAdValorem::get_vBCMono() const
{
    return m_vBCMono;
}

void GIBSMonoPadraoAdValorem::set_vBCMono(const double &vBCMono)
{
    m_vBCMono = vBCMono;
}

double GIBSMonoPadraoAdValorem::get_pAliqMonoUF() const
{
    return m_pAliqMonoUF;
}

void GIBSMonoPadraoAdValorem::set_pAliqMonoUF(const double &pAliqMonoUF)
{
    m_pAliqMonoUF = pAliqMonoUF;
}

double GIBSMonoPadraoAdValorem::get_vIBSMonoUF() const
{
    return m_vIBSMonoUF;
}

void GIBSMonoPadraoAdValorem::set_vIBSMonoUF(const double &vIBSMonoUF)
{
    m_vIBSMonoUF = vIBSMonoUF;
}

double GIBSMonoPadraoAdValorem::get_pAliqMonoMun() const
{
    return m_pAliqMonoMun;
}

void GIBSMonoPadraoAdValorem::set_pAliqMonoMun(const double &pAliqMonoMun)
{
    m_pAliqMonoMun = pAliqMonoMun;
}

double GIBSMonoPadraoAdValorem::get_vIBSMonoMun() const
{
    return m_vIBSMonoMun;
}

void GIBSMonoPadraoAdValorem::set_vIBSMonoMun(const double &vIBSMonoMun)
{
    m_vIBSMonoMun = vIBSMonoMun;
}

double GIBSMonoPadraoAdValorem::get_vIBSMono() const
{
    return m_vIBSMono;
}

void GIBSMonoPadraoAdValorem::set_vIBSMono(const double &vIBSMono)
{
    m_vIBSMono = vIBSMono;
}

void GIBSMonoRetenAdValorem::clear()
{
    m_vBCMonoReten = 0.0;
    m_pAliqMonoReten = 0.0;
    m_vIBSMonoReten = 0.0;
}

bool GIBSMonoRetenAdValorem::isEmpty() const
{
    return qFuzzyIsNull(m_vBCMonoReten) &&
           qFuzzyIsNull(m_pAliqMonoReten) &&
           qFuzzyIsNull(m_vIBSMonoReten);
}

double GIBSMonoRetenAdValorem::get_vBCMonoReten() const
{
    return m_vBCMonoReten;
}

void GIBSMonoRetenAdValorem::set_vBCMonoReten(const double &vBCMonoReten)
{
    m_vBCMonoReten = vBCMonoReten;
}

double GIBSMonoRetenAdValorem::get_pAliqMonoReten() const
{
    return m_pAliqMonoReten;
}

void GIBSMonoRetenAdValorem::set_pAliqMonoReten(const double &pAliqMonoReten)
{
    m_pAliqMonoReten = pAliqMonoReten;
}

double GIBSMonoRetenAdValorem::get_vIBSMonoReten() const
{
    return m_vIBSMonoReten;
}

void GIBSMonoRetenAdValorem::set_vIBSMonoReten(const double &vIBSMonoReten)
{
    m_vIBSMonoReten = vIBSMonoReten;
}

void GIBSMonoRet::clear()
{
    m_vIBSMonoRet = 0.0;
}

bool GIBSMonoRet::isEmpty() const
{
    return qFuzzyIsNull(m_vIBSMonoRet);
}

double GIBSMonoRet::get_vIBSMonoRet() const
{
    return m_vIBSMonoRet;
}

void GIBSMonoRet::set_vIBSMonoRet(const double &vIBSMonoRet)
{
    m_vIBSMonoRet = vIBSMonoRet;
}

void GIBSpBioDiferenca::clear()
{
    m_qBCBioComb = 0.0;
    m_vIBSDiferenca = 0.0;
}

bool GIBSpBioDiferenca::isEmpty() const
{
    return qFuzzyIsNull(m_qBCBioComb) &&
           qFuzzyIsNull(m_vIBSDiferenca);
}

double GIBSpBioDiferenca::get_qBCBioComb() const
{
    return m_qBCBioComb;
}

void GIBSpBioDiferenca::set_qBCBioComb(const double &qBCBioComb)
{
    m_qBCBioComb = qBCBioComb;
}

double GIBSpBioDiferenca::get_vIBSDiferenca() const
{
    return m_vIBSDiferenca;
}

void GIBSpBioDiferenca::set_vIBSDiferenca(const double &vIBSDiferenca)
{
    m_vIBSDiferenca = vIBSDiferenca;
}

GIBSMonoAdRem::GIBSMonoAdRem():
    gMonoPadrao(new GIBSMonoPadraoAdRem),
    gMonoReten(new GIBSMonoRetenAdRem),
    gMonoRet(new GIBSMonoRet),
    gpBioDiferenca(new GIBSpBioDiferenca)
{
}

void GIBSMonoAdRem::clear()
{
    gMonoPadrao->clear();
    gMonoReten->clear();
    gMonoRet->clear();
    gpBioDiferenca->clear();
}

bool GIBSMonoAdRem::isEmpty() const
{
    return gMonoPadrao->isEmpty() &&
           gMonoReten->isEmpty() &&
           gMonoRet->isEmpty() &&
           gpBioDiferenca->isEmpty();
}

GIBSMonoAdValorem::GIBSMonoAdValorem():
    gMonoPadrao(new GIBSMonoPadraoAdValorem),
    gMonoReten(new GIBSMonoRetenAdValorem),
    gMonoRet(new GIBSMonoRet),
    gpBioDiferenca(new GIBSpBioDiferenca)
{
}

void GIBSMonoAdValorem::clear()
{
    gMonoPadrao->clear();
    gMonoReten->clear();
    gMonoRet->clear();
    gpBioDiferenca->clear();
}

bool GIBSMonoAdValorem::isEmpty() const
{
    return gMonoPadrao->isEmpty() &&
           gMonoReten->isEmpty() &&
           gMonoRet->isEmpty() &&
           gpBioDiferenca->isEmpty();
}

void GCBSMonoPadraoAdRem::clear()
{
    m_qBCMono = 0.0;
    m_adRemCBS = 0.0;
    m_vCBSMono = 0.0;
}

bool GCBSMonoPadraoAdRem::isEmpty() const
{
    return qFuzzyIsNull(m_qBCMono) &&
           qFuzzyIsNull(m_adRemCBS) &&
           qFuzzyIsNull(m_vCBSMono);
}

double GCBSMonoPadraoAdRem::get_qBCMono() const
{
    return m_qBCMono;
}

void GCBSMonoPadraoAdRem::set_qBCMono(const double &qBCMono)
{
    m_qBCMono = qBCMono;
}

double GCBSMonoPadraoAdRem::get_adRemCBS() const
{
    return m_adRemCBS;
}

void GCBSMonoPadraoAdRem::set_adRemCBS(const double &adRemCBS)
{
    m_adRemCBS = adRemCBS;
}

double GCBSMonoPadraoAdRem::get_vCBSMono() const
{
    return m_vCBSMono;
}

void GCBSMonoPadraoAdRem::set_vCBSMono(const double &vCBSMono)
{
    m_vCBSMono = vCBSMono;
}

void GCBSMonoRetenAdRem::clear()
{
    m_qBCMonoReten = 0.0;
    m_adRemCBSReten = 0.0;
    m_vCBSMonoReten = 0.0;
}

bool GCBSMonoRetenAdRem::isEmpty() const
{
    return qFuzzyIsNull(m_qBCMonoReten) &&
           qFuzzyIsNull(m_adRemCBSReten) &&
           qFuzzyIsNull(m_vCBSMonoReten);
}

double GCBSMonoRetenAdRem::get_qBCMonoReten() const
{
    return m_qBCMonoReten;
}

void GCBSMonoRetenAdRem::set_qBCMonoReten(const double &qBCMonoReten)
{
    m_qBCMonoReten = qBCMonoReten;
}

double GCBSMonoRetenAdRem::get_adRemCBSReten() const
{
    return m_adRemCBSReten;
}

void GCBSMonoRetenAdRem::set_adRemCBSReten(const double &adRemCBSReten)
{
    m_adRemCBSReten = adRemCBSReten;
}

double GCBSMonoRetenAdRem::get_vCBSMonoReten() const
{
    return m_vCBSMonoReten;
}

void GCBSMonoRetenAdRem::set_vCBSMonoReten(const double &vCBSMonoReten)
{
    m_vCBSMonoReten = vCBSMonoReten;
}

void GCBSMonoPadraoAdValorem::clear()
{
    m_vBCMono = 0.0;
    m_pAliqMonoCBS = 0.0;
    m_vCBSMono = 0.0;
}

bool GCBSMonoPadraoAdValorem::isEmpty() const
{
    return qFuzzyIsNull(m_vBCMono) &&
           qFuzzyIsNull(m_pAliqMonoCBS) &&
           qFuzzyIsNull(m_vCBSMono);
}

double GCBSMonoPadraoAdValorem::get_vBCMono() const
{
    return m_vBCMono;
}

void GCBSMonoPadraoAdValorem::set_vBCMono(const double &vBCMono)
{
    m_vBCMono = vBCMono;
}

double GCBSMonoPadraoAdValorem::get_pAliqMonoCBS() const
{
    return m_pAliqMonoCBS;
}

void GCBSMonoPadraoAdValorem::set_pAliqMonoCBS(const double &pAliqMonoCBS)
{
    m_pAliqMonoCBS = pAliqMonoCBS;
}

double GCBSMonoPadraoAdValorem::get_vCBSMono() const
{
    return m_vCBSMono;
}

void GCBSMonoPadraoAdValorem::set_vCBSMono(const double &vCBSMono)
{
    m_vCBSMono = vCBSMono;
}

void GCBSMonoRetenAdValorem::clear()
{
    m_vBCMonoReten = 0.0;
    m_pAliqMonoReten = 0.0;
    m_vCBSMonoReten = 0.0;
}

bool GCBSMonoRetenAdValorem::isEmpty() const
{
    return qFuzzyIsNull(m_vBCMonoReten) &&
           qFuzzyIsNull(m_pAliqMonoReten) &&
           qFuzzyIsNull(m_vCBSMonoReten);
}

double GCBSMonoRetenAdValorem::get_vBCMonoReten() const
{
    return m_vBCMonoReten;
}

void GCBSMonoRetenAdValorem::set_vBCMonoReten(const double &vBCMonoReten)
{
    m_vBCMonoReten = vBCMonoReten;
}

double GCBSMonoRetenAdValorem::get_pAliqMonoReten() const
{
    return m_pAliqMonoReten;
}

void GCBSMonoRetenAdValorem::set_pAliqMonoReten(const double &pAliqMonoReten)
{
    m_pAliqMonoReten = pAliqMonoReten;
}

double GCBSMonoRetenAdValorem::get_vCBSMonoReten() const
{
    return m_vCBSMonoReten;
}

void GCBSMonoRetenAdValorem::set_vCBSMonoReten(const double &vCBSMonoReten)
{
    m_vCBSMonoReten = vCBSMonoReten;
}

void GCBSMonoRet::clear()
{
    m_vCBSMonoRet = 0.0;
}

bool GCBSMonoRet::isEmpty() const
{
    return qFuzzyIsNull(m_vCBSMonoRet);
}

double GCBSMonoRet::get_vCBSMonoRet() const
{
    return m_vCBSMonoRet;
}

void GCBSMonoRet::set_vCBSMonoRet(const double &vCBSMonoRet)
{
    m_vCBSMonoRet = vCBSMonoRet;
}

void GCBSpBioDiferenca::clear()
{
    m_qBCBioComb = 0.0;
    m_vCBSDiferenca = 0.0;
}

bool GCBSpBioDiferenca::isEmpty() const
{
    return qFuzzyIsNull(m_qBCBioComb) &&
           qFuzzyIsNull(m_vCBSDiferenca);
}

double GCBSpBioDiferenca::get_qBCBioComb() const
{
    return m_qBCBioComb;
}

void GCBSpBioDiferenca::set_qBCBioComb(const double &qBCBioComb)
{
    m_qBCBioComb = qBCBioComb;
}

double GCBSpBioDiferenca::get_vCBSDiferenca() const
{
    return m_vCBSDiferenca;
}

void GCBSpBioDiferenca::set_vCBSDiferenca(const double &vCBSDiferenca)
{
    m_vCBSDiferenca = vCBSDiferenca;
}

GCBSMonoAdRem::GCBSMonoAdRem():
    gMonoPadrao(new GCBSMonoPadraoAdRem),
    gMonoReten(new GCBSMonoRetenAdRem),
    gMonoRet(new GCBSMonoRet),
    gpBioDiferenca(new GCBSpBioDiferenca)
{
}

void GCBSMonoAdRem::clear()
{
    gMonoPadrao->clear();
    gMonoReten->clear();
    gMonoRet->clear();
    gpBioDiferenca->clear();
}

bool GCBSMonoAdRem::isEmpty() const
{
    return gMonoPadrao->isEmpty() &&
           gMonoReten->isEmpty() &&
           gMonoRet->isEmpty() &&
           gpBioDiferenca->isEmpty();
}

GCBSMonoAdValorem::GCBSMonoAdValorem():
    gMonoPadrao(new GCBSMonoPadraoAdValorem),
    gMonoReten(new GCBSMonoRetenAdValorem),
    gMonoRet(new GCBSMonoRet),
    gpBioDiferenca(new GCBSpBioDiferenca)
{
}

void GCBSMonoAdValorem::clear()
{
    gMonoPadrao->clear();
    gMonoReten->clear();
    gMonoRet->clear();
    gpBioDiferenca->clear();
}

bool GCBSMonoAdValorem::isEmpty() const
{
    return gMonoPadrao->isEmpty() &&
           gMonoReten->isEmpty() &&
           gMonoRet->isEmpty() &&
           gpBioDiferenca->isEmpty();
}

GIBSCBSMono::GIBSCBSMono():
    gIBSMonoAdRem(new GIBSMonoAdRem),
    gIBSMonoAdValorem(new GIBSMonoAdValorem),
    gCBSMonoAdRem(new GCBSMonoAdRem),
    gCBSMonoAdValorem(new GCBSMonoAdValorem)
{
}

void GIBSCBSMono::clear()
{
    m_vTotIBSMonoItem = 0.0;
    m_vTotCBSMonoItem = 0.0;
    gIBSMonoAdRem->clear();
    gIBSMonoAdValorem->clear();
    gCBSMonoAdRem->clear();
    gCBSMonoAdValorem->clear();
}

bool GIBSCBSMono::isEmpty() const
{
    return qFuzzyIsNull(m_vTotIBSMonoItem) &&
           qFuzzyIsNull(m_vTotCBSMonoItem) &&
           gIBSMonoAdRem->isEmpty() &&
           gIBSMonoAdValorem->isEmpty() &&
           gCBSMonoAdRem->isEmpty() &&
           gCBSMonoAdValorem->isEmpty();
}

double GIBSCBSMono::get_vTotIBSMonoItem() const
{
    return m_vTotIBSMonoItem;
}

void GIBSCBSMono::set_vTotIBSMonoItem(const double &vTotIBSMonoItem)
{
    m_vTotIBSMonoItem = vTotIBSMonoItem;
}

double GIBSCBSMono::get_vTotCBSMonoItem() const
{
    return m_vTotCBSMonoItem;
}

void GIBSCBSMono::set_vTotCBSMonoItem(const double &vTotCBSMonoItem)
{
    m_vTotCBSMonoItem = vTotCBSMonoItem;
}
