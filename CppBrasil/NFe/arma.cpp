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


#include "arma.h"

Arma::Arma(): m_tpArma(TpArma::None)
{
}

Arma::~Arma()
{
}

void Arma::clear()
{
    this->m_tpArma = TpArma::None;
    this->m_nSerie.clear();
    this->m_nCano.clear();
    this->m_descr.clear();
}

TpArma Arma::get_tpArma() const
{
    return this->m_tpArma;
}

void Arma::set_tpArma(const TpArma &tpArma)
{
    this->m_tpArma = tpArma;
}

QString Arma::get_nSerie() const
{
    return this->m_nSerie;
}

void Arma::set_nSerie(const QString &nSerie)
{
    this->m_nSerie = nSerie;
}

QString Arma::get_nCano() const
{
    return this->m_nCano;
}

void Arma::set_nCano(const QString &nCano)
{
    this->m_nCano = nCano;
}

QString Arma::get_descr() const
{
    return this->m_descr;
}

void Arma::set_descr(const QString &descr)
{
    this->m_descr = descr;
}
