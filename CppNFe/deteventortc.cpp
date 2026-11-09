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

#include "deteventortc.h"

void EstoqueEventoRTC::clear()
{
    m_quantidade = 0.0;
    m_unidade = {};
    m_vIBSEstorno = 0.0;
    m_vCBSEstorno = 0.0;
}

double EstoqueEventoRTC::get_quantidade() const { return m_quantidade; }
void EstoqueEventoRTC::set_quantidade(double valor) { m_quantidade = valor; }

QString EstoqueEventoRTC::get_unidade() const { return m_unidade; }
void EstoqueEventoRTC::set_unidade(const QString &valor) { m_unidade = valor; }

double EstoqueEventoRTC::get_vIBSEstorno() const { return m_vIBSEstorno; }
void EstoqueEventoRTC::set_vIBSEstorno(double valor) { m_vIBSEstorno = valor; }

double EstoqueEventoRTC::get_vCBSEstorno() const { return m_vCBSEstorno; }
void EstoqueEventoRTC::set_vCBSEstorno(double valor) { m_vCBSEstorno = valor; }

ItemEventoRTC::ItemEventoRTC()
    : gControleEstoque(std::make_unique<EstoqueEventoRTC>())
{
}

void ItemEventoRTC::clear()
{
    m_nItem = 0;
    m_vIBS = 0.0;
    m_vCBS = 0.0;
    gControleEstoque->clear();
}

int ItemEventoRTC::get_nItem() const { return m_nItem; }
void ItemEventoRTC::set_nItem(int valor) { m_nItem = valor; }

double ItemEventoRTC::get_vIBS() const { return m_vIBS; }
void ItemEventoRTC::set_vIBS(double valor) { m_vIBS = valor; }

double ItemEventoRTC::get_vCBS() const { return m_vCBS; }
void ItemEventoRTC::set_vCBS(double valor) { m_vCBS = valor; }

void CreditoPresumidoEvento::clear()
{
    m_pCredPres = 0.0;
    m_vCredPres = 0.0;
}

double CreditoPresumidoEvento::get_pCredPres() const { return m_pCredPres; }
void CreditoPresumidoEvento::set_pCredPres(double valor) { m_pCredPres = valor; }

double CreditoPresumidoEvento::get_vCredPres() const { return m_vCredPres; }
void CreditoPresumidoEvento::set_vCredPres(double valor) { m_vCredPres = valor; }

void ItemCreditoPresumidoEvento::clear()
{
    m_nItem = 0;
    m_vBCCredPres = 0.0;
    m_cCredPres = {};
    gIBSCredPres.reset();
    gCBSCredPres.reset();
}

int ItemCreditoPresumidoEvento::get_nItem() const { return m_nItem; }
void ItemCreditoPresumidoEvento::set_nItem(int valor) { m_nItem = valor; }

double ItemCreditoPresumidoEvento::get_vBCCredPres() const { return m_vBCCredPres; }
void ItemCreditoPresumidoEvento::set_vBCCredPres(double valor) { m_vBCCredPres = valor; }

QString ItemCreditoPresumidoEvento::get_cCredPres() const { return m_cCredPres; }
void ItemCreditoPresumidoEvento::set_cCredPres(const QString &valor) { m_cCredPres = valor; }

void PagamentoVinculadoEvento::clear()
{
    m_idTransacao = {};
    m_tpMeioPgto = {};
    m_CNPJReceb = {};
    m_CNPJBasePSP = {};
}

QString PagamentoVinculadoEvento::get_idTransacao() const { return m_idTransacao; }
void PagamentoVinculadoEvento::set_idTransacao(const QString &valor) { m_idTransacao = valor; }

QString PagamentoVinculadoEvento::get_tpMeioPgto() const { return m_tpMeioPgto; }
void PagamentoVinculadoEvento::set_tpMeioPgto(const QString &valor) { m_tpMeioPgto = valor; }

QString PagamentoVinculadoEvento::get_CNPJReceb() const { return m_CNPJReceb; }
void PagamentoVinculadoEvento::set_CNPJReceb(const QString &valor) { m_CNPJReceb = valor; }

QString PagamentoVinculadoEvento::get_CNPJBasePSP() const { return m_CNPJBasePSP; }
void PagamentoVinculadoEvento::set_CNPJBasePSP(const QString &valor) { m_CNPJBasePSP = valor; }

DetEventoRTC::DetEventoRTC()
    : itens(std::make_unique<Container<ItemEventoRTC>>()),
      gCredPresOper(std::make_unique<Container<ItemCreditoPresumidoEvento>>()),
      gPgto(std::make_unique<PagamentoVinculadoEvento>())
{
}

void DetEventoRTC::clear()
{
    m_indAceitacao = -1;
    m_dPrevEntrega = {};
    m_tpEventoAut = TpEvento::None;
    itens->clear();
    gCredPresOper->clear();
    gPgto->clear();
}

int DetEventoRTC::get_indAceitacao() const { return m_indAceitacao; }
void DetEventoRTC::set_indAceitacao(int valor) { m_indAceitacao = valor; }

QDate DetEventoRTC::get_dPrevEntrega() const { return m_dPrevEntrega; }
void DetEventoRTC::set_dPrevEntrega(const QDate &valor) { m_dPrevEntrega = valor; }

TpEvento DetEventoRTC::get_tpEventoAut() const { return m_tpEventoAut; }
void DetEventoRTC::set_tpEventoAut(TpEvento valor) { m_tpEventoAut = valor; }
