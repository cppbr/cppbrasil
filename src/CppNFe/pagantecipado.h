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

#ifndef PAGANTECIPADO_H
#define PAGANTECIPADO_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>
#include <QStringList>

//BC01 - Grupo de notas de antecipação de pagamento (NT 2025.002)
class CPPNFE_EXPORT GPagAntecipado
{
public:
    void clear();
    bool isEmpty() const;
    QStringList get_refNFe() const;
    void set_refNFe(const QStringList &refNFe);
    void add_refNFe(const QString &refNFe);

private:
    QStringList m_refNFe;
};

#endif // PAGANTECIPADO_H
