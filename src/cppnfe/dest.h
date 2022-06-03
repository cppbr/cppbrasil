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


#ifndef DEST_H
#define DEST_H

#include <memory>
#include <QString>
#include "enderdest.h"
#include "convnf.h"

#ifdef CPPBRASIL_STATIC
    class Dest
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Dest
#endif
{
public:
    Dest();
    ~Dest();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_idEstrangeiro() const;
    void set_idEstrangeiro(const QString &idEstrangeiro);
    QString get_xNome() const;
    void set_xNome(const QString &xNome);
    IndIEDest get_indIEDest() const;
    void set_indIEDest(const IndIEDest &indIEDest);
    QString get_IE() const;
    void set_IE(const QString &IE);
    QString get_ISUF() const;
    void set_ISUF(const QString &ISUF);
    QString get_IM() const;
    void set_IM(const QString &IM);
    QString get_email() const;
    void set_email(const QString &email);

    std::shared_ptr<EnderDest> enderDest;
private:
    QString m_CNPJ;
    QString m_CPF;
    QString m_idEstrangeiro;
    QString m_xNome;
    IndIEDest m_indIEDest;
    QString m_IE;
    QString m_ISUF;
    QString m_IM;
    QString m_email;
};

#endif // DEST_H
