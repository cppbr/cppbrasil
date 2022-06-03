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


#ifndef RASTRO_H
#define RASTRO_H

#include<QString>
#include<QDateTime>
#include "cpputility.h"

#ifdef CPPBRASIL_STATIC
    class Rastro
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Rastro
#endif
{
public:
    Rastro();
    ~Rastro();
    void clear();
    QString get_nLote() const;
    void set_nLote(const QString &nLote);
    double get_qLote() const;
    void set_qLote(const double &qLote);
    QDateTime get_dFab() const;
    void set_dFab(const QDateTime &dFab);
    QDateTime get_dVal() const;
    void set_dVal(const QDateTime &dVal);
    QString get_cAgreg() const;
    void set_cAgreg(const QString &cAgreg);
private:
    QString m_nLote;
    double m_qLote;
    QDateTime m_dFab; //Formato: “AAAA-MM-DD”
    QDateTime m_dVal; //Formato: “AAAA-MM-DD”
    QString m_cAgreg;

};

#endif // RASTRO_H
