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
#include<QString>
#include "card.h"
#include "convnf.h"


#ifdef CPPBRASIL_STATIC
    class DetPag
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT DetPag
#endif
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

    std::shared_ptr<Card> card;

private:
    IndPag m_indPag;
    TPag m_tPag;
    double m_vPag;
    QString m_xPag;
};

#endif // DETPAG_H
