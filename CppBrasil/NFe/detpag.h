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


#ifndef DETPAG_H
#define DETPAG_H

#include <memory>
#include <QString>
#include <QDate>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/card.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT DetPag
{
public:
    DetPag();
    ~DetPag();
    void clear();
    IndPag get_indPag() const;
    void set_indPag(const IndPag &indPag);
    TPag get_tPag() const;
    void set_tPag(const TPag &tPag);
    double get_vPag() const;
    void set_vPag(const double &vPag);
    QString get_xPag() const;
    void set_xPag(const QString &xPag);

    QDate get_dPag() const;
    void set_dPag(const QDate &dPag);
    QString get_CNPJPag() const;
    void set_CNPJPag(const QString &CNPJPag);
    UF get_UFPag() const;
    void set_UFPag(const UF &UFPag);
    QString get_CNPJReceb() const;
    void set_CNPJReceb(const QString &CNPJReceb);
    UF get_UFReceb() const;
    void set_UFReceb(const UF &UFReceb);

    std::unique_ptr<Card> card;

private:
    IndPag m_indPag;
    TPag m_tPag;
    double m_vPag;
    QString m_xPag;
    QDate m_dPag;
    QString m_CNPJPag;
    UF m_UFPag;
    QString m_CNPJReceb;
    UF m_UFReceb;
};

#endif // DETPAG_H
