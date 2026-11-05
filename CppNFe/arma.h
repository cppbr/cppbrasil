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


#ifndef ARMA_H
#define ARMA_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT Arma
{
public:
    Arma();
    ~Arma();
    void clear();
    TpArma get_tpArma() const;
    void set_tpArma(const TpArma &tpArma);
    QString get_nSerie() const;
    void set_nSerie(const QString &nSerie);
    QString get_nCano() const;
    void set_nCano(const QString &nCano);
    QString get_descr() const;
    void set_descr(const QString &descr);
private:
    TpArma m_tpArma;
    QString m_nSerie;
    QString m_nCano;
    QString m_descr;
};

#endif // ARMA_H
