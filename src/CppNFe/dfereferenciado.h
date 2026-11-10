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

#ifndef DFEREFERENCIADO_H
#define DFEREFERENCIADO_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>

//VC01 - Referenciamento de item de outro DF-e (NT 2025.002)
class CPPNFE_EXPORT DfeReferenciado
{
public:
    void clear();
    bool isEmpty() const;
    QString get_chaveAcesso() const;
    void set_chaveAcesso(const QString &chaveAcesso);
    int get_nItem() const;
    void set_nItem(const int &nItem);

private:
    QString m_chaveAcesso;
    int m_nItem = 0;
};

#endif // DFEREFERENCIADO_H
