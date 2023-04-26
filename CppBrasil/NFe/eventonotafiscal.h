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


#ifndef EVENTONOTAFISCAL_H
#define EVENTONOTAFISCAL_H

#include <memory>
#include <QObject>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/wsnfe.h>
#include <CppBrasil/NFe/statusservico.h>
#include <CppBrasil/NFe/notafiscal.h>
#include <CppBrasil/NFe/envevento.h>
#include <CppBrasil/NFe/conssitnfe.h>


class CPPNFE_EXPORT EventoNotaFiscal : public QObject
{
    Q_OBJECT
public:
    EventoNotaFiscal(ConfigNFe* confgNFe, NotaFiscal* notafiscal);
    ~EventoNotaFiscal();
    std::shared_ptr<StatusServico> statusServico;
    std::shared_ptr<EnvEvento> envEvento;
    std::shared_ptr<ConsSitNFe> consSitNFe;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

};

#endif // EVENTONOTAFISCAL_H
