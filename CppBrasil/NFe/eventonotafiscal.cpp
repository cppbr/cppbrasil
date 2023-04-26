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


#include "eventonotafiscal.h"


EventoNotaFiscal::EventoNotaFiscal(ConfigNFe* confgNFe, NotaFiscal* notafiscal) :
    statusServico(new StatusServico(confgNFe)), envEvento(new EnvEvento(confgNFe, notafiscal)),
    consSitNFe(new ConsSitNFe(confgNFe, notafiscal))
{
    connect(statusServico.get(), SIGNAL(wsChange(WebServicesNF)), this, SIGNAL(wsChange(WebServicesNF)));
    connect(statusServico.get(), SIGNAL(errorOccurred(QString)), this, SIGNAL(errorOccurred(QString)));
    //pode ser feito da forma acima, mas serve apenas para saber que estamos conectando um signal a outro signal
    connect(envEvento.get(), &EnvEvento::wsChange, this, &EventoNotaFiscal::wsChange);
    connect(envEvento.get(), &EnvEvento::errorOccurred, this, &EventoNotaFiscal::errorOccurred);

    connect(consSitNFe.get(), &ConsSitNFe::wsChange, this, &EventoNotaFiscal::wsChange);
    connect(consSitNFe.get(), &ConsSitNFe::errorOccurred, this, &EventoNotaFiscal::errorOccurred);

}

EventoNotaFiscal::~EventoNotaFiscal()
{
}
