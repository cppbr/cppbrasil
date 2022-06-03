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


#ifndef INFNFE_H
#define INFNFE_H

#include <memory>
#include <QString>
#include "container.h"
#include "ide.h"
#include "emit.h"
#include "avulsa.h"
#include "dest.h"
#include "retirada.h"
#include "entrega.h"
#include "autxml.h"
#include "det.h"
#include "total.h"
#include "transp.h"
#include "cobr.h"
#include "pag.h"
#include "infadic.h"
#include "exporta.h"
#include "compra.h"
#include "cana.h"
#include "infresptec.h"
#include "infintermed.h"

#ifdef CPPBRASIL_STATIC
    class InfNFe
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT InfNFe
#endif
{
public:
    InfNFe();
    ~InfNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_Id() const;
    void set_Id(const QString &id);

    std::shared_ptr<Ide> ide;
    std::shared_ptr<Emit> emite; //obs. emit é uma palavra reservada QT
    std::shared_ptr<Avulsa> avulsa;
    std::shared_ptr<Dest> dest;
    std::shared_ptr<Retirada> retirada;
    std::shared_ptr<Entrega> entrega;
    std::shared_ptr<Container<AutXML>> autXML;
    std::shared_ptr<Container<Det>> det;
    std::shared_ptr<Total> total;
    std::shared_ptr<Transp> transp;
    std::shared_ptr<Cobr> cobr;
    std::shared_ptr<Pag> pag;
    std::shared_ptr<InfIntermed> infIntermed;
    std::shared_ptr<InfAdic> infAdic;
    std::shared_ptr<Exporta> exporta;
    std::shared_ptr<Compra> compra;
    std::shared_ptr<Cana> cana;
    std::shared_ptr<InfRespTec> infRespTec;

private:
    QString m_versao;
    QString m_Id;

};

#endif // INFNFE_H
