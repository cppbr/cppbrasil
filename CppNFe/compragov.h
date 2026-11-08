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

#ifndef COMPRAGOV_H
#define COMPRAGOV_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>
#include <QStringList>
#include <CppNFe/convnf.h>

//BB01 - Grupo de Compras Governamentais (NT 2025.002)
class CPPNFE_EXPORT GCompraGov
{
public:
    void clear();
    bool isEmpty() const;
    TpEnteGov get_tpEnteGov() const;
    void set_tpEnteGov(const TpEnteGov &tpEnteGov);
    double get_pRedutor() const;
    void set_pRedutor(const double &pRedutor);
    TpOperGov get_tpOperGov() const;
    void set_tpOperGov(const TpOperGov &tpOperGov);
    QStringList get_refDFeAnt() const;
    void set_refDFeAnt(const QStringList &refDFeAnt);
    void add_refDFeAnt(const QString &refDFeAnt);

private:
    TpEnteGov m_tpEnteGov = TpEnteGov::None;
    double m_pRedutor = 0.0;
    TpOperGov m_tpOperGov = TpOperGov::None;
    QStringList m_refDFeAnt;
};

#endif // COMPRAGOV_H
