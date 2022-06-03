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


#include "det.h"

Det::Det(): prod(new Prod), imposto(new Imposto), impostoDevol(new ImpostoDevol), obsItem(new ObsItem)
{
}

Det::~Det()
{
}

void Det::clear()
{
    this->m_infAdProd.clear();

    this->prod->clear();
    this->imposto->clear();
    this->impostoDevol->clear();
    this->obsItem->clear();
}

QString Det::get_infAdProd() const
{
    return this->m_infAdProd;
}

void Det::set_infAdProd(const QString &infAdProd)
{
    this->m_infAdProd = infAdProd;
}

