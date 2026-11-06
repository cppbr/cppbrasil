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

#include "agropecuario.h"

void Defensivo::clear()
{
    m_nReceituario.clear();
    m_CPFRespTec.clear();
}

QString Defensivo::get_nReceituario() const
{
    return m_nReceituario;
}

void Defensivo::set_nReceituario(const QString &nReceituario)
{
    m_nReceituario = nReceituario;
}

QString Defensivo::get_CPFRespTec() const
{
    return m_CPFRespTec;
}

void Defensivo::set_CPFRespTec(const QString &CPFRespTec)
{
    m_CPFRespTec = CPFRespTec;
}

void GuiaTransito::clear()
{
    m_tpGuia = 0;
    m_UFGuia.clear();
    m_serieGuia.clear();
    m_nGuia.clear();
}

bool GuiaTransito::isEmpty() const
{
    return m_tpGuia == 0 && m_UFGuia.isEmpty() &&
           m_serieGuia.isEmpty() && m_nGuia.isEmpty();
}

int GuiaTransito::get_tpGuia() const
{
    return m_tpGuia;
}

void GuiaTransito::set_tpGuia(int tpGuia)
{
    m_tpGuia = tpGuia;
}

QString GuiaTransito::get_UFGuia() const
{
    return m_UFGuia;
}

void GuiaTransito::set_UFGuia(const QString &UFGuia)
{
    m_UFGuia = UFGuia;
}

QString GuiaTransito::get_serieGuia() const
{
    return m_serieGuia;
}

void GuiaTransito::set_serieGuia(const QString &serieGuia)
{
    m_serieGuia = serieGuia;
}

QString GuiaTransito::get_nGuia() const
{
    return m_nGuia;
}

void GuiaTransito::set_nGuia(const QString &nGuia)
{
    m_nGuia = nGuia;
}

Agropecuario::Agropecuario()
    : defensivo(std::make_unique<Container<Defensivo>>()),
      guiaTransito(std::make_unique<GuiaTransito>())
{
}

void Agropecuario::clear()
{
    defensivo->clear();
    guiaTransito->clear();
}
