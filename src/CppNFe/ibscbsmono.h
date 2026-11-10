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

#ifndef IBSCBSMONO_H
#define IBSCBSMONO_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>

//Grupo gIBSCBSMono (NT 2025.002 v1.50 e posteriores). Informar apenas um dos
//grupos AdRem ou AdValorem para o IBS e para a CBS.

//gMonoPadrao de gIBSMonoAdRem
class CPPNFE_EXPORT GIBSMonoPadraoAdRem
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCMono() const;
    void set_qBCMono(const double &qBCMono);
    double get_adRemIBS() const;
    void set_adRemIBS(const double &adRemIBS);
    double get_vIBSMono() const;
    void set_vIBSMono(const double &vIBSMono);

private:
    double m_qBCMono = 0.0;
    double m_adRemIBS = 0.0;
    double m_vIBSMono = 0.0;
};

//gMonoReten de gIBSMonoAdRem
class CPPNFE_EXPORT GIBSMonoRetenAdRem
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCMonoReten() const;
    void set_qBCMonoReten(const double &qBCMonoReten);
    double get_adRemIBSReten() const;
    void set_adRemIBSReten(const double &adRemIBSReten);
    double get_vIBSMonoReten() const;
    void set_vIBSMonoReten(const double &vIBSMonoReten);

private:
    double m_qBCMonoReten = 0.0;
    double m_adRemIBSReten = 0.0;
    double m_vIBSMonoReten = 0.0;
};

//gMonoPadrao de gIBSMonoAdValorem
class CPPNFE_EXPORT GIBSMonoPadraoAdValorem
{
public:
    void clear();
    bool isEmpty() const;
    double get_vBCMono() const;
    void set_vBCMono(const double &vBCMono);
    double get_pAliqMonoUF() const;
    void set_pAliqMonoUF(const double &pAliqMonoUF);
    double get_vIBSMonoUF() const;
    void set_vIBSMonoUF(const double &vIBSMonoUF);
    double get_pAliqMonoMun() const;
    void set_pAliqMonoMun(const double &pAliqMonoMun);
    double get_vIBSMonoMun() const;
    void set_vIBSMonoMun(const double &vIBSMonoMun);
    double get_vIBSMono() const;
    void set_vIBSMono(const double &vIBSMono);

private:
    double m_vBCMono = 0.0;
    double m_pAliqMonoUF = 0.0;
    double m_vIBSMonoUF = 0.0;
    double m_pAliqMonoMun = 0.0;
    double m_vIBSMonoMun = 0.0;
    double m_vIBSMono = 0.0;
};

//gMonoReten de gIBSMonoAdValorem
class CPPNFE_EXPORT GIBSMonoRetenAdValorem
{
public:
    void clear();
    bool isEmpty() const;
    double get_vBCMonoReten() const;
    void set_vBCMonoReten(const double &vBCMonoReten);
    double get_pAliqMonoReten() const;
    void set_pAliqMonoReten(const double &pAliqMonoReten);
    double get_vIBSMonoReten() const;
    void set_vIBSMonoReten(const double &vIBSMonoReten);

private:
    double m_vBCMonoReten = 0.0;
    double m_pAliqMonoReten = 0.0;
    double m_vIBSMonoReten = 0.0;
};

//gMonoRet - IBS monofásico retido anteriormente
class CPPNFE_EXPORT GIBSMonoRet
{
public:
    void clear();
    bool isEmpty() const;
    double get_vIBSMonoRet() const;
    void set_vIBSMonoRet(const double &vIBSMonoRet);

private:
    double m_vIBSMonoRet = 0.0;
};

//gpBioDiferenca - diferença do IBS no biocombustível
class CPPNFE_EXPORT GIBSpBioDiferenca
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCBioComb() const;
    void set_qBCBioComb(const double &qBCBioComb);
    double get_vIBSDiferenca() const;
    void set_vIBSDiferenca(const double &vIBSDiferenca);

private:
    double m_qBCBioComb = 0.0;
    double m_vIBSDiferenca = 0.0;
};

//gIBSMonoAdRem
class CPPNFE_EXPORT GIBSMonoAdRem
{
public:
    GIBSMonoAdRem();
    void clear();
    bool isEmpty() const;
    std::unique_ptr<GIBSMonoPadraoAdRem> gMonoPadrao;
    std::unique_ptr<GIBSMonoRetenAdRem> gMonoReten;
    std::unique_ptr<GIBSMonoRet> gMonoRet;
    std::unique_ptr<GIBSpBioDiferenca> gpBioDiferenca;
};

//gIBSMonoAdValorem
class CPPNFE_EXPORT GIBSMonoAdValorem
{
public:
    GIBSMonoAdValorem();
    void clear();
    bool isEmpty() const;
    std::unique_ptr<GIBSMonoPadraoAdValorem> gMonoPadrao;
    std::unique_ptr<GIBSMonoRetenAdValorem> gMonoReten;
    std::unique_ptr<GIBSMonoRet> gMonoRet;
    std::unique_ptr<GIBSpBioDiferenca> gpBioDiferenca;
};

//gMonoPadrao de gCBSMonoAdRem
class CPPNFE_EXPORT GCBSMonoPadraoAdRem
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCMono() const;
    void set_qBCMono(const double &qBCMono);
    double get_adRemCBS() const;
    void set_adRemCBS(const double &adRemCBS);
    double get_vCBSMono() const;
    void set_vCBSMono(const double &vCBSMono);

private:
    double m_qBCMono = 0.0;
    double m_adRemCBS = 0.0;
    double m_vCBSMono = 0.0;
};

//gMonoReten de gCBSMonoAdRem
class CPPNFE_EXPORT GCBSMonoRetenAdRem
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCMonoReten() const;
    void set_qBCMonoReten(const double &qBCMonoReten);
    double get_adRemCBSReten() const;
    void set_adRemCBSReten(const double &adRemCBSReten);
    double get_vCBSMonoReten() const;
    void set_vCBSMonoReten(const double &vCBSMonoReten);

private:
    double m_qBCMonoReten = 0.0;
    double m_adRemCBSReten = 0.0;
    double m_vCBSMonoReten = 0.0;
};

//gMonoPadrao de gCBSMonoAdValorem
class CPPNFE_EXPORT GCBSMonoPadraoAdValorem
{
public:
    void clear();
    bool isEmpty() const;
    double get_vBCMono() const;
    void set_vBCMono(const double &vBCMono);
    double get_pAliqMonoCBS() const;
    void set_pAliqMonoCBS(const double &pAliqMonoCBS);
    double get_vCBSMono() const;
    void set_vCBSMono(const double &vCBSMono);

private:
    double m_vBCMono = 0.0;
    double m_pAliqMonoCBS = 0.0;
    double m_vCBSMono = 0.0;
};

//gMonoReten de gCBSMonoAdValorem
class CPPNFE_EXPORT GCBSMonoRetenAdValorem
{
public:
    void clear();
    bool isEmpty() const;
    double get_vBCMonoReten() const;
    void set_vBCMonoReten(const double &vBCMonoReten);
    double get_pAliqMonoReten() const;
    void set_pAliqMonoReten(const double &pAliqMonoReten);
    double get_vCBSMonoReten() const;
    void set_vCBSMonoReten(const double &vCBSMonoReten);

private:
    double m_vBCMonoReten = 0.0;
    double m_pAliqMonoReten = 0.0;
    double m_vCBSMonoReten = 0.0;
};

//gMonoRet - CBS monofásico retido anteriormente
class CPPNFE_EXPORT GCBSMonoRet
{
public:
    void clear();
    bool isEmpty() const;
    double get_vCBSMonoRet() const;
    void set_vCBSMonoRet(const double &vCBSMonoRet);

private:
    double m_vCBSMonoRet = 0.0;
};

//gpBioDiferenca - diferença do CBS no biocombustível
class CPPNFE_EXPORT GCBSpBioDiferenca
{
public:
    void clear();
    bool isEmpty() const;
    double get_qBCBioComb() const;
    void set_qBCBioComb(const double &qBCBioComb);
    double get_vCBSDiferenca() const;
    void set_vCBSDiferenca(const double &vCBSDiferenca);

private:
    double m_qBCBioComb = 0.0;
    double m_vCBSDiferenca = 0.0;
};

//gCBSMonoAdRem
class CPPNFE_EXPORT GCBSMonoAdRem
{
public:
    GCBSMonoAdRem();
    void clear();
    bool isEmpty() const;
    std::unique_ptr<GCBSMonoPadraoAdRem> gMonoPadrao;
    std::unique_ptr<GCBSMonoRetenAdRem> gMonoReten;
    std::unique_ptr<GCBSMonoRet> gMonoRet;
    std::unique_ptr<GCBSpBioDiferenca> gpBioDiferenca;
};

//gCBSMonoAdValorem
class CPPNFE_EXPORT GCBSMonoAdValorem
{
public:
    GCBSMonoAdValorem();
    void clear();
    bool isEmpty() const;
    std::unique_ptr<GCBSMonoPadraoAdValorem> gMonoPadrao;
    std::unique_ptr<GCBSMonoRetenAdValorem> gMonoReten;
    std::unique_ptr<GCBSMonoRet> gMonoRet;
    std::unique_ptr<GCBSpBioDiferenca> gpBioDiferenca;
};

//UB84 - gIBSCBSMono - Informações do IBS e CBS em operações com imposto monofásico
class CPPNFE_EXPORT GIBSCBSMono
{
public:
    GIBSCBSMono();
    void clear();
    bool isEmpty() const;
    double get_vTotIBSMonoItem() const;
    void set_vTotIBSMonoItem(const double &vTotIBSMonoItem);
    double get_vTotCBSMonoItem() const;
    void set_vTotCBSMonoItem(const double &vTotCBSMonoItem);
    std::unique_ptr<GIBSMonoAdRem> gIBSMonoAdRem;
    std::unique_ptr<GIBSMonoAdValorem> gIBSMonoAdValorem;
    std::unique_ptr<GCBSMonoAdRem> gCBSMonoAdRem;
    std::unique_ptr<GCBSMonoAdValorem> gCBSMonoAdValorem;

private:
    double m_vTotIBSMonoItem = 0.0;
    double m_vTotCBSMonoItem = 0.0;
};

#endif // IBSCBSMONO_H
