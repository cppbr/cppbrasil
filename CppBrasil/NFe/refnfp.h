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


#ifndef REFNFP_H
#define REFNFP_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT RefNFP
{
public:
    RefNFP();
    ~RefNFP();
    void clear();
    int get_cUF() const;
    void set_cUF(const int &cUF);
    QString get_AAMM() const;
    void set_AAMM(const QString &AAMM);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_IE() const;
    void set_IE(const QString &IE);
    QString get_mod() const;
    void set_mod(const QString &mod);
    int get_serie() const;
    void set_serie(const int &serie);
    int get_nNF() const;
    void set_nNF(const int &nNF);
private:
    int m_cUF;
    QString m_AAMM;
    QString m_CNPJ;
    QString m_CPF;
    QString m_IE;
    QString m_mod;
    int m_serie;
    int m_nNF;
};

#endif // REFNFP_H
