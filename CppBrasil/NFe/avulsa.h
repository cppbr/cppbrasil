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


#ifndef AVULSA_H
#define AVULSA_H

#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>


class CPPNFE_EXPORT Avulsa
{
public:
    Avulsa();
    ~Avulsa();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_xOrgao() const;
    void set_xOrgao(const QString &xOrgao);
    QString get_matr() const;
    void set_matr(const QString &matr);
    QString get_xAgente() const;
    void set_xAgente(const QString &xAgente);
    QString get_fone() const;
    void set_fone(const QString &fone);
    QString get_UF() const;
    void set_UF(const QString &UF);
    QString get_nDAR() const;
    void set_nDAR(const QString &nDAR);
    QDateTime get_dEmi() const;
    void set_dEmi(const QDateTime &dEmi);
    double get_vDAR() const;
    void set_vDAR(const double &vDAR);
    QString get_repEmi() const;
    void set_repEmi(const QString &repEmi);
    QDateTime get_dPag() const;
    void set_dPag(const QDateTime &dPag);
private:
    QString m_CNPJ;
    QString m_xOrgao;
    QString m_matr;
    QString m_xAgente;
    QString m_fone;
    QString m_UF;
    QString m_nDAR;
    QDateTime m_dEmi;
    double m_vDAR;
    QString m_repEmi;
    QDateTime m_dPag;
};

#endif // AVULSA_H
