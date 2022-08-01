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


#ifndef TRANSP_H
#define TRANSP_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/transporta.h>
#include <CppBrasil/NFe/rettransp.h>
#include <CppBrasil/NFe/veictransp.h>
#include <CppBrasil/NFe/reboque.h>
#include <CppBrasil/NFe/vol.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT Transp
{
public:
    Transp();
    ~Transp();
    void clear();
    ModFrete get_modFrete() const;
    void set_modFrete(const ModFrete &modFrete);
    QString get_vagao() const;
    void set_vagao(const QString &vagao);
    QString get_balsa() const;
    void set_balsa(const QString &balsa);

    std::shared_ptr<Transporta> transporta;
    std::shared_ptr<RetTransp> retTransp;
    std::shared_ptr<VeicTransp> veicTransp;
    std::shared_ptr<Container<Reboque>> reboque;
    std::shared_ptr<Container<Vol>> vol;

private:
    ModFrete m_modFrete;
    //em baixo de reboque
    QString m_vagao;
    QString m_balsa;
};

#endif // TRANSP_H
