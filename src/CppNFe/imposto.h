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
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/icms.h>
#include <CppNFe/icmsufdest.h>
#include <CppNFe/ipi.h>
#include <CppNFe/ii.h>
#include <CppNFe/pis.h>
#include <CppNFe/pisst.h>
#include <CppNFe/cofins.h>
#include <CppNFe/cofinsst.h>
#include <CppNFe/issqn.h>
#include <CppNFe/is.h>
#include <CppNFe/ibscbs.h>

class CPPNFE_EXPORT Imposto
{
public:
    Imposto();
    ~Imposto();
    void clear();
    double get_vTotTrib() const;
    void set_vTotTrib(const double &vTotTrib);

    std::unique_ptr<Icms> ICMS;
    std::unique_ptr<IcmsUFDest> ICMSUFDest;
    std::unique_ptr<Ipi> IPI;
    std::unique_ptr<Ii> II;
    std::unique_ptr<Pis> PIS;
    std::unique_ptr<PisST> PISST;
    std::unique_ptr<Cofins> COFINS;
    std::unique_ptr<CofinsST> COFINSST;
    std::unique_ptr<IssQN> ISSQN;
    std::unique_ptr<Is> IS; //UB01
    std::unique_ptr<IbsCbs> IBSCBS; //UB12
private:
    double m_vTotTrib;
};

#endif // IMPOSTO_H
