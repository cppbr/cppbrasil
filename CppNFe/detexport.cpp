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


#include "detexport.h"

DetExport::DetExport(): exportInd(new ExportInd)
{
}

DetExport::~DetExport()
{
}

void DetExport::clear()
{
    this->m_nDraw.clear();
    exportInd->clear();
}

QString DetExport::get_nDraw() const
{
    return this->m_nDraw;
}

void DetExport::set_nDraw(const QString &nDraw)
{
    this->m_nDraw = nDraw;
}

