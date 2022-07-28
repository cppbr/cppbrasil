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


#ifndef REFECF_H
#define REFECF_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPBRASIL_EXPORT RefECF
{
public:
    RefECF();
    ~RefECF();
    void clear();
    QString get_mod() const;
    void set_mod(const QString &mod);
    QString get_nECF() const;
    void set_nECF(const QString &nECF);
    QString get_nCOO() const;
    void set_nCOO(const QString &nCOO);
private:
    QString m_mod;
    QString m_nECF;
    QString m_nCOO;

};

#endif // REFNF_H
