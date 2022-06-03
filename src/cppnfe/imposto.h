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
#include "icms.h"
#include "icmsufdest.h"
#include "ipi.h"
#include "ii.h"
#include "pis.h"
#include "pisst.h"
#include "cofins.h"
#include "cofinsst.h"
#include "issqn.h"

#ifdef CPPBRASIL_STATIC
    class Imposto
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Imposto
#endif
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
