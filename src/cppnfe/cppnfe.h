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


#ifndef CPPNFE_H
#define CPPNFE_H

#include <memory>
#include <QObject>
#include "confignfe.h"
#include "notafiscal.h"
#include "eventos.h"

#ifdef CPPBRASIL_STATIC
    class CppNFe : public QObject
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT CppNFe : public QObject
#endif
{
    Q_OBJECT
public:
    CppNFe();
    ~CppNFe();
    void clear();
    std::shared_ptr<ConfigNFe> configuracoes;
    std::shared_ptr<NotaFiscal> notafiscal;
    std::shared_ptr<Eventos> eventos;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);
};

#endif // CPPNFE_H
