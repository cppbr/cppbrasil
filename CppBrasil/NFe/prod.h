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


#ifndef PROD_H
#define PROD_H

#include <QString>
#include <memory>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/di.h>
#include <CppBrasil/NFe/detexport.h>
#include <CppBrasil/NFe/rastro.h>
#include <CppBrasil/NFe/veicprod.h>
#include <CppBrasil/NFe/med.h>
#include <CppBrasil/NFe/arma.h>
#include <CppBrasil/NFe/comb.h>
#include <CppBrasil/NFe/convnf.h>

class CPPBRASIL_EXPORT Prod
{
public:
    Prod();
    ~Prod();
    void clear();
    int get_nItem() const;
    void set_nItem(const int &nItem);
    QString get_cProd() const;
    void set_cProd(const QString &cProd);
    QString get_cEAN() const;
    void set_cEAN(const QString &cEAN);
    QString get_cBarra() const;
    void set_cBarra(const QString &cBarra);
    QString get_xProd() const;
    void set_xProd(const QString &xProd);
    QString get_NCM() const;
    void set_NCM(const QString &NCM);
    QString get_NVE() const;
    void set_NVE(const QString &NVE);
    QString get_EXTIPI() const;
    void set_EXTIPI(const QString &EXTIPI);
    QString get_CFOP() const;
    void set_CFOP(const QString &CFOP);
    QString get_uCom() const;
    void set_uCom(const QString &uCom);
    double get_qCom() const;
    void set_qCom(const double &qCom);
    double get_vUnCom() const;
    void set_vUnCom(const double &vUnCom);
    double get_vProd() const;
    void set_vProd(const double &vProd);
    QString get_cEANTrib() const;
    void set_cEANTrib(const QString &cEANTrib);
    QString get_cBarraTrib() const;
    void set_cBarraTrib(const QString &cBarraTrib);
    QString get_uTrib() const;
    void set_uTrib(const QString &uTrib);
    double get_qTrib() const;
    void set_qTrib(const double &qTrib);
    double get_vUnTrib() const;
    void set_vUnTrib(const double &vUnTrib);
    double get_vFrete() const;
    void set_vFrete(const double &vFrete);
    double get_vSeg() const;
    void set_vSeg(const double &vSeg);
    double get_vDesc() const;
    void set_vDesc(const double &vDesc);
    double get_vOutro() const;
    void set_vOutro(const double &vOutro);
    IndTot get_indTot() const;
    void set_indTot(const IndTot &indTot);
    QString get_CEST() const;
    void set_CEST(const QString &CEST);
    IndEscala get_indEscala() const;
    void set_indEscala(const IndEscala &indEscala);
    QString get_CNPJFab() const;
    void set_CNPJFab(const QString &CNPJFab);
    QString get_cBenef() const;
    void set_cBenef(const QString &cBenef);
    QString get_xPed() const;
    void set_xPed(const QString &xPed);
    int get_nItemPed() const;
    void set_nItemPed(const int &nItemPed);
    QString get_nFCI() const;
    void set_nFCI(const QString &nFCI);
    QString get_nRECOPI() const;
    void set_nRECOPI(const QString &nRECOPI);

    std::shared_ptr<Container<Di>> DI;
    std::shared_ptr<Container<DetExport>> detExport;
    std::shared_ptr<Container<Rastro>> rastro;
    std::shared_ptr<VeicProd> veicProd;
    std::shared_ptr<Med> med;
    std::shared_ptr<Container<Arma>> arma;
    std::shared_ptr<Comb> comb;

private:
    int m_nItem; //pertence a tag det H01 - Número do item
    QString m_cProd;
    QString m_cEAN;
    QString m_cBarra;
    QString m_xProd;
    QString m_NCM;
    QString m_NVE;
    QString m_EXTIPI;
    QString m_CFOP;
    QString m_uCom;
    double m_qCom;
    double m_vUnCom;
    double m_vProd;
    QString m_cEANTrib;
    QString m_cBarraTrib;
    QString m_uTrib;
    double m_qTrib;
    double m_vUnTrib;
    double m_vFrete;
    double m_vSeg;
    double m_vDesc;
    double m_vOutro;
    IndTot m_indTot;
    QString m_CEST;
    IndEscala m_indEscala;
    QString m_CNPJFab;
    QString m_cBenef;
    //abaixo de detExpor
    QString m_xPed;
    int m_nItemPed;
    QString m_nFCI;
    //abaixo de arma
    QString m_nRECOPI;
};

#endif // PROD_H
