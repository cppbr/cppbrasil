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


#ifndef DI_H
#define DI_H

#include<memory>
#include<QString>
#include<QDateTime>
#include "adi.h"
#include "convnf.h"
#include "cpputility.h"
#include "container.h"

#ifdef CPPBRASIL_STATIC
    class Di
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Di
#endif
{
public:
    Di();
    ~Di();
    void clear();
    QString get_nDI() const;
    void set_nDI(const QString &nDI);
    QDateTime get_dDI() const;
    void set_dDI(const QDateTime &dDI);
    QString get_xLocDesemb() const;
    void set_xLocDesemb(const QString &xLocDesemb);
    QString get_UFDesemb() const;
    void set_UFDesemb(const QString &UFDesemb);
    QDateTime get_dDesemb() const;
    void set_dDesemb(const QDateTime &dDesemb);
    TpViaTransp get_tpViaTransp() const;
    void set_tpViaTransp(const TpViaTransp &tpViaTransp);
    double get_vAFRMM() const;
    void set_vAFRMM(const double &vAFRMM);
    TpIntermedio get_tpIntermedio() const;
    void set_tpIntermedio(const TpIntermedio &tpIntermedio);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_UFTerceiro() const;
    void set_UFTerceiro(const QString &UFTerceiro);
    QString get_cExportador() const;
    void set_cExportador(const QString &cExportador);

    std::shared_ptr<Container<Adi>> adi;

private:
    QString m_nDI;
    QDateTime m_dDI; //Formato: “AAAA-MM-DD”
    QString m_xLocDesemb;
    QString m_UFDesemb;
    QDateTime m_dDesemb; //Formato: “AAAA-MM-DD”
    TpViaTransp m_tpViaTransp;
    double m_vAFRMM;
    TpIntermedio m_tpIntermedio;
    QString m_CNPJ;
    QString m_UFTerceiro;
    QString m_cExportador;
};

#endif // DI_H
