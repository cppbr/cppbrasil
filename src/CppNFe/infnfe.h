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
#include <CppBase/cppbrasil_global.h>
#include <CppBase/container.h>
#include <CppNFe/ide.h>
#include <CppNFe/emit.h>
#include <CppNFe/avulsa.h>
#include <CppNFe/dest.h>
#include <CppNFe/retirada.h>
#include <CppNFe/entrega.h>
#include <CppNFe/autxml.h>
#include <CppNFe/det.h>
#include <CppNFe/total.h>
#include <CppNFe/transp.h>
#include <CppNFe/cobr.h>
#include <CppNFe/pag.h>
#include <CppNFe/infadic.h>
#include <CppNFe/exporta.h>
#include <CppNFe/compra.h>
#include <CppNFe/cana.h>
#include <CppNFe/infresptec.h>
#include <CppNFe/infintermed.h>
#include <CppNFe/agropecuario.h>

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

    std::unique_ptr<Ide> ide;
    std::unique_ptr<Emit> emite; //obs. emit é uma palavra reservada QT
    std::unique_ptr<Avulsa> avulsa;
    std::unique_ptr<Dest> dest;
    std::unique_ptr<Retirada> retirada;
    std::unique_ptr<Entrega> entrega;
    std::unique_ptr<Container<AutXML>> autXML;
    std::unique_ptr<Container<Det>> det;
    std::unique_ptr<Total> total;
    std::unique_ptr<Transp> transp;
    std::unique_ptr<Cobr> cobr;
    std::unique_ptr<Pag> pag;
    std::unique_ptr<InfIntermed> infIntermed;
    std::unique_ptr<InfAdic> infAdic;
    std::unique_ptr<Exporta> exporta;
    std::unique_ptr<Compra> compra;
    std::unique_ptr<Cana> cana;
    std::unique_ptr<InfRespTec> infRespTec;
    std::unique_ptr<Agropecuario> agropecuario;

private:
    QString m_versao;
    QString m_Id;

};

#endif // INFNFE_H
