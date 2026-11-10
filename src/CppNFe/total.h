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


#ifndef TOTAL_H
#define TOTAL_H

#include <memory>
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/icmstot.h>
#include <CppNFe/issqntot.h>
#include <CppNFe/rettrib.h>
#include <CppNFe/ibscbstot.h>

class CPPNFE_EXPORT Total
{
public:
    Total();
    ~Total();
    void clear();
    double get_vNFTot() const;
    void set_vNFTot(const double &vNFTot);

    std::unique_ptr<IcmsTot> ICMSTot;
    std::unique_ptr<IssQNtot> ISSQNtot;
    std::unique_ptr<RetTrib> retTrib;
    std::unique_ptr<IsTot> ISTot; //W31
    std::unique_ptr<IbsCbsTot> IBSCBSTot; //W34

private:
    double m_vNFTot = 0.0; //W60
};

#endif // TOTAL_H
