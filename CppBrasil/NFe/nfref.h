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


#ifndef NFREF_H
#define NFREF_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/refnf.h>
#include <CppBrasil/NFe/refnfp.h>
#include <CppBrasil/NFe/refecf.h>

class CPPBRASIL_EXPORT NFRef
{
public:
    NFRef();
    ~NFRef();
    void clear();
    QString get_refNFe() const;
    void set_refNFe(const QString &refNFe);
    QString get_refCTe() const;
    void set_refCTe(const QString &refCTe);

    std::shared_ptr<RefNF> refNF;
    std::shared_ptr<RefNFP> refNFP;
    std::shared_ptr<RefECF> refECF;

private:
    QString m_refNFe;
    QString m_refCTe;
};




#endif // NFREF_H
