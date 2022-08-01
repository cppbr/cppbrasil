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


#ifndef DET_H
#define DET_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/prod.h>
#include <CppBrasil/NFe/imposto.h>
#include <CppBrasil/NFe/impostodevol.h>
#include <CppBrasil/NFe/obsitem.h>

class CPPNFE_EXPORT Det
{
public:
    Det();
    ~Det();
    void clear();
    QString get_infAdProd() const;
    void set_infAdProd(const QString &infAdProd);

    std::shared_ptr<Prod> prod;
    std::shared_ptr<Imposto> imposto;
    std::shared_ptr<ImpostoDevol> impostoDevol;
    std::shared_ptr<ObsItem> obsItem;

private:
    QString m_infAdProd; //V01 abaixo de impostoDevol
};

#endif // DET_H
