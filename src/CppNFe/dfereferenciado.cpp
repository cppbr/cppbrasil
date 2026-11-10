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

#include "dfereferenciado.h"

void DfeReferenciado::clear()
{
    m_chaveAcesso.clear();
    m_nItem = 0;
}

bool DfeReferenciado::isEmpty() const
{
    return m_chaveAcesso.isEmpty() &&
           m_nItem == 0;
}

QString DfeReferenciado::get_chaveAcesso() const
{
    return m_chaveAcesso;
}

void DfeReferenciado::set_chaveAcesso(const QString &chaveAcesso)
{
    m_chaveAcesso = chaveAcesso;
}

int DfeReferenciado::get_nItem() const
{
    return m_nItem;
}

void DfeReferenciado::set_nItem(const int &nItem)
{
    m_nItem = nItem;
}
