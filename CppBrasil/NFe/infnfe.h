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
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/ide.h>
#include <CppBrasil/NFe/emit.h>
#include <CppBrasil/NFe/avulsa.h>
#include <CppBrasil/NFe/dest.h>
#include <CppBrasil/NFe/retirada.h>
#include <CppBrasil/NFe/entrega.h>
#include <CppBrasil/NFe/autxml.h>
#include <CppBrasil/NFe/det.h>
#include <CppBrasil/NFe/total.h>
#include <CppBrasil/NFe/transp.h>
#include <CppBrasil/NFe/cobr.h>
#include <CppBrasil/NFe/pag.h>
#include <CppBrasil/NFe/infadic.h>
#include <CppBrasil/NFe/exporta.h>
#include <CppBrasil/NFe/compra.h>
#include <CppBrasil/NFe/cana.h>
#include <CppBrasil/NFe/infresptec.h>
#include <CppBrasil/NFe/infintermed.h>

class CPPNFE_EXPORT InfNFe
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
