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


#ifndef CARD_H
#define CARD_H

#include<QString>
#include "convnf.h"

#ifdef CPPBRASIL_STATIC
    class Card
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Card
#endif
{
public:
    Card();
    ~Card();
    void clear();
    TpIntegra get_tpIntegra() const;
    void set_tpIntegra(const TpIntegra &tpIntegra);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    TBand get_tBand() const;
    void set_tBand(const TBand &tBand);
    QString get_cAut() const;
    void set_cAut(const QString &cAut);
private:
    TpIntegra m_tpIntegra;
    QString m_CNPJ;
    TBand m_tBand;
    QString m_cAut;
};

#endif // CARD_H
