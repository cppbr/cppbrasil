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


#ifndef IMPOSTO_H
#define IMPOSTO_H

#include <memory>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/icms.h>
#include <CppBrasil/NFe/icmsufdest.h>
#include <CppBrasil/NFe/ipi.h>
#include <CppBrasil/NFe/ii.h>
#include <CppBrasil/NFe/pis.h>
#include <CppBrasil/NFe/pisst.h>
#include <CppBrasil/NFe/cofins.h>
#include <CppBrasil/NFe/cofinsst.h>
#include <CppBrasil/NFe/issqn.h>

class CPPNFE_EXPORT Imposto
{
public:
    Imposto();
    ~Imposto();
    void clear();
    double get_vTotTrib() const;
    void set_vTotTrib(const double &vTotTrib);

    std::shared_ptr<Icms> ICMS;
    std::shared_ptr<IcmsUFDest> ICMSUFDest;
    std::shared_ptr<Ipi> IPI;
    std::shared_ptr<Ii> II;
    std::shared_ptr<Pis> PIS;
    std::shared_ptr<PisST> PISST;
    std::shared_ptr<Cofins> COFINS;
    std::shared_ptr<CofinsST> COFINSST;
    std::shared_ptr<IssQN> ISSQN;
private:
    double m_vTotTrib;
};

#endif // IMPOSTO_H
