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

#ifndef IBSCBS_H
#define IBSCBS_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>
#include <QDate>
#include <CppNFe/convnf.h>
#include <CppNFe/ibscbsmono.h>

//Grupo IBSCBS (UB12). Os grupos gIBSCBS, gIBSCBSMono, gTransfCred e gAjusteCompet
//são mutuamente exclusivos, assim como gCredPresOper e gCredPresIBSZFM.
//Na geração do XML é emitido o grupo que estiver preenchido.

//gDif - Grupo de informações do diferimento
class CPPNFE_EXPORT GDif
{
public:
    void clear();
    bool isEmpty() const;
    double get_pDif() const;
    void set_pDif(const double &pDif);
    double get_vDif() const;
    void set_vDif(const double &vDif);

private:
    double m_pDif = 0.0;
    double m_vDif = 0.0;
};

//gDevTrib - Grupo de informações da devolução de tributos
class CPPNFE_EXPORT GDevTrib
{
public:
    void clear();
    bool isEmpty() const;
    double get_pDevTrib() const;
    void set_pDevTrib(const double &pDevTrib);
    double get_vDevTrib() const;
    void set_vDevTrib(const double &vDevTrib);

private:
    double m_pDevTrib = 0.0;
    double m_vDevTrib = 0.0;
};

//gRed - Grupo de informações da redução da alíquota
class CPPNFE_EXPORT GRed
{
public:
    void clear();
    bool isEmpty() const;
    double get_pRedAliq() const;
    void set_pRedAliq(const double &pRedAliq);
    double get_pAliqEfet() const;
    void set_pAliqEfet(const double &pAliqEfet);

private:
    double m_pRedAliq = 0.0;
    double m_pAliqEfet = 0.0;
};

//UB17 - gIBSUF - Grupo de informações do IBS para a UF
class CPPNFE_EXPORT GIBSUF
{
public:
    GIBSUF();
    void clear();
    bool isEmpty() const;
    double get_pIBSUF() const;
    void set_pIBSUF(const double &pIBSUF);
    double get_vIBSUF() const;
    void set_vIBSUF(const double &vIBSUF);
    std::unique_ptr<GDif> gDif;
    std::unique_ptr<GDevTrib> gDevTrib;
    std::unique_ptr<GRed> gRed;

private:
    double m_pIBSUF = 0.0;
    double m_vIBSUF = 0.0;
};

//UB36 - gIBSMun - Grupo de informações do IBS para o município
class CPPNFE_EXPORT GIBSMun
{
public:
    GIBSMun();
    void clear();
    bool isEmpty() const;
    double get_pIBSMun() const;
    void set_pIBSMun(const double &pIBSMun);
    double get_vIBSMun() const;
    void set_vIBSMun(const double &vIBSMun);
    std::unique_ptr<GDif> gDif;
    std::unique_ptr<GDevTrib> gDevTrib;
    std::unique_ptr<GRed> gRed;

private:
    double m_pIBSMun = 0.0;
    double m_vIBSMun = 0.0;
};

//UB66a - gALCZFMCBS - Operações em áreas incentivadas (ALC/ZFM) - CBS alíquota zero
class CPPNFE_EXPORT GALCZFMCBS
{
public:
    void clear();
    bool isEmpty() const;
    TpALCZFMCBS get_tpALCZFMCBS() const;
    void set_tpALCZFMCBS(const TpALCZFMCBS &tpALCZFMCBS);
    QString get_nProcSuframa() const;
    void set_nProcSuframa(const QString &nProcSuframa);
    double get_pAliqEfetRegCBS() const;
    void set_pAliqEfetRegCBS(const double &pAliqEfetRegCBS);
    double get_vTribRegCBS() const;
    void set_vTribRegCBS(const double &vTribRegCBS);

private:
    TpALCZFMCBS m_tpALCZFMCBS = TpALCZFMCBS::None;
    QString m_nProcSuframa;
    double m_pAliqEfetRegCBS = 0.0;
    double m_vTribRegCBS = 0.0;
};

//UB55 - gCBS - Grupo de informações da CBS
class CPPNFE_EXPORT GCBS
{
public:
    GCBS();
    void clear();
    bool isEmpty() const;
    double get_pCBS() const;
    void set_pCBS(const double &pCBS);
    double get_vCBS() const;
    void set_vCBS(const double &vCBS);
    std::unique_ptr<GDif> gDif;
    std::unique_ptr<GDevTrib> gDevTrib;
    std::unique_ptr<GRed> gRed;
    std::unique_ptr<GALCZFMCBS> gALCZFMCBS;

private:
    double m_pCBS = 0.0;
    double m_vCBS = 0.0;
};

//UB68 - gTribRegular - Grupo de informações da tributação regular
class CPPNFE_EXPORT GTribRegular
{
public:
    void clear();
    bool isEmpty() const;
    QString get_CSTReg() const;
    void set_CSTReg(const QString &CSTReg);
    QString get_cClassTribReg() const;
    void set_cClassTribReg(const QString &cClassTribReg);
    double get_pAliqEfetRegIBSUF() const;
    void set_pAliqEfetRegIBSUF(const double &pAliqEfetRegIBSUF);
    double get_vTribRegIBSUF() const;
    void set_vTribRegIBSUF(const double &vTribRegIBSUF);
    double get_pAliqEfetRegIBSMun() const;
    void set_pAliqEfetRegIBSMun(const double &pAliqEfetRegIBSMun);
    double get_vTribRegIBSMun() const;
    void set_vTribRegIBSMun(const double &vTribRegIBSMun);
    double get_pAliqEfetRegCBS() const;
    void set_pAliqEfetRegCBS(const double &pAliqEfetRegCBS);
    double get_vTribRegCBS() const;
    void set_vTribRegCBS(const double &vTribRegCBS);

private:
    QString m_CSTReg;
    QString m_cClassTribReg;
    double m_pAliqEfetRegIBSUF = 0.0;
    double m_vTribRegIBSUF = 0.0;
    double m_pAliqEfetRegIBSMun = 0.0;
    double m_vTribRegIBSMun = 0.0;
    double m_pAliqEfetRegCBS = 0.0;
    double m_vTribRegCBS = 0.0;
};

//UB82a - gTribCompraGov - Tributação em compras governamentais
class CPPNFE_EXPORT GTribCompraGov
{
public:
    void clear();
    bool isEmpty() const;
    double get_pAliqIBSUF() const;
    void set_pAliqIBSUF(const double &pAliqIBSUF);
    double get_vTribIBSUF() const;
    void set_vTribIBSUF(const double &vTribIBSUF);
    double get_pAliqIBSMun() const;
    void set_pAliqIBSMun(const double &pAliqIBSMun);
    double get_vTribIBSMun() const;
    void set_vTribIBSMun(const double &vTribIBSMun);
    double get_pAliqCBS() const;
    void set_pAliqCBS(const double &pAliqCBS);
    double get_vTribCBS() const;
    void set_vTribCBS(const double &vTribCBS);

private:
    double m_pAliqIBSUF = 0.0;
    double m_vTribIBSUF = 0.0;
    double m_pAliqIBSMun = 0.0;
    double m_vTribIBSMun = 0.0;
    double m_pAliqCBS = 0.0;
    double m_vTribCBS = 0.0;
};

//UB15 - gIBSCBS - Grupo de informações do IBS e da CBS
class CPPNFE_EXPORT GIBSCBS
{
public:
    GIBSCBS();
    void clear();
    bool isEmpty() const;
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_vIBS() const;
    void set_vIBS(const double &vIBS);
    std::unique_ptr<GIBSUF> gIBSUF;
    std::unique_ptr<GIBSMun> gIBSMun;
    std::unique_ptr<GCBS> gCBS;
    std::unique_ptr<GTribRegular> gTribRegular;
    std::unique_ptr<GTribCompraGov> gTribCompraGov;

private:
    double m_vBC = 0.0;
    double m_vIBS = 0.0;
};

//UB106 - gTransfCred - Transferências de crédito
class CPPNFE_EXPORT GTransfCred
{
public:
    void clear();
    bool isEmpty() const;
    double get_vIBS() const;
    void set_vIBS(const double &vIBS);
    double get_vCBS() const;
    void set_vCBS(const double &vCBS);

private:
    double m_vIBS = 0.0;
    double m_vCBS = 0.0;
};

//UB112 - gAjusteCompet - Ajuste de competência
class CPPNFE_EXPORT GAjusteCompet
{
public:
    void clear();
    bool isEmpty() const;
    QDate get_competApur() const;
    void set_competApur(const QDate &competApur);
    double get_vIBS() const;
    void set_vIBS(const double &vIBS);
    double get_vCBS() const;
    void set_vCBS(const double &vCBS);

private:
    QDate m_competApur;
    double m_vIBS = 0.0;
    double m_vCBS = 0.0;
};

//UB116 - gEstornoCred - Estorno de crédito
class CPPNFE_EXPORT GEstornoCred
{
public:
    void clear();
    bool isEmpty() const;
    double get_vIBSEstCred() const;
    void set_vIBSEstCred(const double &vIBSEstCred);
    double get_vCBSEstCred() const;
    void set_vCBSEstCred(const double &vCBSEstCred);

private:
    double m_vIBSEstCred = 0.0;
    double m_vCBSEstCred = 0.0;
};

//gIBSCredPres / gCBSCredPres - Crédito presumido. Informar vCredPres ou vCredPresCondSus
class CPPNFE_EXPORT GCredPres
{
public:
    void clear();
    bool isEmpty() const;
    double get_pCredPres() const;
    void set_pCredPres(const double &pCredPres);
    double get_vCredPres() const;
    void set_vCredPres(const double &vCredPres);
    double get_vCredPresCondSus() const;
    void set_vCredPresCondSus(const double &vCredPresCondSus);

private:
    double m_pCredPres = 0.0;
    double m_vCredPres = 0.0;
    double m_vCredPresCondSus = 0.0;
};

//UB120 - gCredPresOper - Crédito presumido da operação
class CPPNFE_EXPORT GCredPresOper
{
public:
    GCredPresOper();
    void clear();
    bool isEmpty() const;
    double get_vBCCredPres() const;
    void set_vBCCredPres(const double &vBCCredPres);
    QString get_cCredPres() const;
    void set_cCredPres(const QString &cCredPres);
    std::unique_ptr<GCredPres> gIBSCredPres;
    std::unique_ptr<GCredPres> gCBSCredPres;

private:
    double m_vBCCredPres = 0.0;
    QString m_cCredPres;
};

//UB131 - gCredPresIBSZFM - Crédito presumido do IBS na ZFM
class CPPNFE_EXPORT GCredPresIBSZFM
{
public:
    void clear();
    bool isEmpty() const;
    QDate get_competApur() const;
    void set_competApur(const QDate &competApur);
    TpCredPresIBSZFM get_tpCredPresIBSZFM() const;
    void set_tpCredPresIBSZFM(const TpCredPresIBSZFM &tpCredPresIBSZFM);
    double get_vCredPresIBSZFM() const;
    void set_vCredPresIBSZFM(const double &vCredPresIBSZFM);

private:
    QDate m_competApur;
    TpCredPresIBSZFM m_tpCredPresIBSZFM = TpCredPresIBSZFM::None;
    double m_vCredPresIBSZFM = 0.0;
};

//UB12 - IBSCBS - Informações do IBS e da CBS
class CPPNFE_EXPORT IbsCbs
{
public:
    IbsCbs();
    void clear();
    bool isEmpty() const;
    QString get_CST() const;
    void set_CST(const QString &CST);
    QString get_cClassTrib() const;
    void set_cClassTrib(const QString &cClassTrib);
    IndDoacao get_indDoacao() const;
    void set_indDoacao(const IndDoacao &indDoacao);
    std::unique_ptr<GIBSCBS> gIBSCBS;
    std::unique_ptr<GIBSCBSMono> gIBSCBSMono;
    std::unique_ptr<GTransfCred> gTransfCred;
    std::unique_ptr<GAjusteCompet> gAjusteCompet;
    std::unique_ptr<GEstornoCred> gEstornoCred;
    std::unique_ptr<GCredPresOper> gCredPresOper;
    std::unique_ptr<GCredPresIBSZFM> gCredPresIBSZFM;

private:
    QString m_CST;
    QString m_cClassTrib;
    IndDoacao m_indDoacao = IndDoacao::None;
};

#endif // IBSCBS_H
