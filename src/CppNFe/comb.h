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


#ifndef COMB_H
#define COMB_H

#include <memory>
#include <QString>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/container.h>
#include <CppNFe/cide.h>
#include <CppNFe/encerrante.h>
#include <CppNFe/origcomb.h>

class CPPNFE_EXPORT Comb
{
public:
    Comb();
    ~Comb();
    void clear();
    int get_cProdANP() const;
    void set_cProdANP(const int &cProdANP);
    QString get_descANP() const;
    void set_descANP(const QString &descANP);
    double get_pGLP() const;
    void set_pGLP(const double &pGLP);
    double get_pGNn() const;
    void set_pGNn(const double &pGNn);
    double get_pGNi() const;
    void set_pGNi(const double &pGNi);
    double get_vPart() const;
    void set_vPart(const double &vPart);
    QString get_CODIF() const;
    void set_CODIF(const QString &CODIF);
    double get_qTemp() const;
    void set_qTemp(const double &qTemp);
    QString get_UFCons() const;
    void set_UFCons(const QString &UFCons);

    double get_pBio() const;
    void set_pBio(const double &pBio);

    std::unique_ptr<Cide> CIDE;
    std::unique_ptr<Encerrante> encerrante;
    std::unique_ptr<Container<OrigComb>> origComb;
private:
    // O setter de Comb prevalece sobre o acesso legado em Encerrante.
    double m_pBio;
    bool m_pBioInformado;
    int m_cProdANP;
    QString m_descANP;
    double m_pGLP;
    double m_pGNn;
    double m_pGNi;
    double m_vPart;
    QString m_CODIF;
    double m_qTemp;
    QString m_UFCons;
};

#endif // COMB_H
