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


#include "nfref.h"

NFRef::NFRef(): refNF(new RefNF), refNFP(new RefNFP), refECF(new RefECF)
{
}

NFRef::~NFRef()
{
}

void NFRef::clear()
{
    this->m_refNFe.clear();
    this->m_refCTe.clear();

    this->refNF->clear();
    this->refNFP->clear();
    this->refECF->clear();
}

QString NFRef::get_refNFe() const
{
  return this->m_refNFe;
}

void NFRef::set_refNFe(const QString &refNFe)
{
  this->m_refNFe = refNFe;
}

QString NFRef::get_refCTe() const
{
  return this->m_refCTe;
}

void NFRef::set_refCTe(const QString &refCTe)
{
  this->m_refCTe = refCTe;
}
