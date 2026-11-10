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

#include "pagantecipado.h"

void GPagAntecipado::clear()
{
    m_refNFe.clear();
}

bool GPagAntecipado::isEmpty() const
{
    return m_refNFe.isEmpty();
}

QStringList GPagAntecipado::get_refNFe() const
{
    return m_refNFe;
}

void GPagAntecipado::set_refNFe(const QStringList &refNFe)
{
    m_refNFe = refNFe;
}

void GPagAntecipado::add_refNFe(const QString &refNFe)
{
    m_refNFe.append(refNFe);
}
