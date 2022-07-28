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


#include "exportind.h"

ExportInd::ExportInd():m_qExport(0.0)
{
}

ExportInd::~ExportInd()
{
}

void ExportInd::clear()
{
    this->m_nRE.clear();
    this->m_chNFe.clear();
    this->m_qExport = 0.0;
}
QString ExportInd::get_nRE() const
{
    return this->m_nRE;
}

void ExportInd::set_nRE(const QString &nRE)
{
    this->m_nRE = nRE;
}

QString ExportInd::get_chNFe() const
{
    return this->m_chNFe;
}

void ExportInd::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

double ExportInd::get_qExport() const
{
    return this->m_qExport;
}

void ExportInd::set_qExport(const double &qExport)
{
    this->m_qExport = qExport;
}
