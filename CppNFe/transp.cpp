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


#include "transp.h"

Transp::Transp(): transporta(new Transporta), retTransp(new RetTransp),
    veicTransp(new VeicTransp), reboque(new Container<Reboque>),
    vol(new Container<Vol>), m_modFrete(ModFrete::None)
{
}

Transp::~Transp()
{
}

void Transp::clear()
{
    this->m_modFrete = ModFrete::None;
    this->m_vagao.clear();
    this->m_balsa.clear();

    this->transporta->clear();
    this->retTransp->clear();
    this->veicTransp->clear();
    this->reboque->clear();
    this->vol->clear();
}

ModFrete Transp::get_modFrete() const
{
    return this->m_modFrete;
}

void Transp::set_modFrete(const ModFrete &modFrete)
{
    this->m_modFrete = modFrete;
}

QString Transp::get_vagao() const
{
    return this->m_vagao;
}

void Transp::set_vagao(const QString &vagao)
{
    this->m_vagao = vagao;
}

QString Transp::get_balsa() const
{
    return this->m_balsa;
}

void Transp::set_balsa(const QString &balsa)
{
    this->m_balsa = balsa;
}

