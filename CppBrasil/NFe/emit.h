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


#ifndef EMIT_H
#define EMIT_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/enderemit.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT Emit
{
public:
    Emit();
    ~Emit();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_xNome() const;
    void set_xNome(const QString &xNome);
    QString get_xFant() const;
    void set_xFant(const QString &xFant);
    QString get_IE() const;
    void set_IE(const QString &IE);
    QString get_IEST() const;
    void set_IEST(const QString &IEST);
    QString get_IM() const;
    void set_IM(const QString &IM);
    QString get_CNAE() const;
    void set_CNAE(const QString &CNAE);
    Crt get_CRT() const;
    void set_CRT(const Crt &CRT);

    std::shared_ptr<EnderEmit> enderEmit;

private:
    QString m_CNPJ;
    QString m_CPF;
    QString m_xNome;
    QString m_xFant;
    QString m_IE;
    QString m_IEST;
    QString m_IM;
    QString m_CNAE;
    Crt m_CRT;
};

#endif // EMIT_H
