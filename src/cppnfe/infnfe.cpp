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


#include "infnfe.h"

InfNFe::InfNFe():ide(new Ide), emite(new Emit), avulsa(new Avulsa), dest(new Dest),
    retirada(new Retirada), entrega(new Entrega), autXML(new Container<AutXML>),
    det(new Container<Det>), total(new Total), transp(new Transp), cobr(new Cobr),
    pag(new Pag), infIntermed(new InfIntermed), infAdic(new InfAdic), exporta(new Exporta),
    compra(new Compra), cana(new Cana), infRespTec(new InfRespTec)
{
}

InfNFe::~InfNFe()
{
}

void InfNFe::clear()
{
    this->m_versao.clear();
    this->m_Id.clear();

    this->ide->clear();
    this->emite->clear();
    this->avulsa->clear();
    this->dest->clear();
    this->retirada->clear();
    this->entrega->clear();
    this->autXML->clear();
    this->det->clear();
    this->total->clear();
    this->transp->clear();
    this->cobr->clear();
    this->pag->clear();
    this->infIntermed->clear();
    this->infAdic->clear();
    this->exporta->clear();
    this->compra->clear();
    this->cana->clear();
    this->infRespTec->clear();
}

QString InfNFe::get_versao() const
{
  return this->m_versao;
}

void InfNFe::set_versao(const QString &versao)
{
  this->m_versao = versao;
}

QString InfNFe::get_Id() const
{
  return this->m_Id;
}

void InfNFe::set_Id(const QString &id)
{
  this->m_Id = id;
}
