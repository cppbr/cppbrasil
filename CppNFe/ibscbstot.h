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

#ifndef IBSCBSTOT_H
#define IBSCBSTOT_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>

//W31 - ISTot - Grupo total do Imposto Seletivo
class CPPNFE_EXPORT IsTot
{
public:
    void clear();
    bool isEmpty() const;
    double get_vIS() const;
    void set_vIS(const double &vIS);

private:
    double m_vIS = 0.0;
};

//W37 - gIBSUF - Grupo total do IBS da UF
class CPPNFE_EXPORT GIBSUFTot
{
public:
    void clear();
    bool isEmpty() const;
    double get_vDif() const;
    void set_vDif(const double &vDif);
    double get_vDevTrib() const;
    void set_vDevTrib(const double &vDevTrib);
    double get_vIBSUF() const;
    void set_vIBSUF(const double &vIBSUF);

private:
    double m_vDif = 0.0;
    double m_vDevTrib = 0.0;
    double m_vIBSUF = 0.0;
};

//W42 - gIBSMun - Grupo total do IBS do município
class CPPNFE_EXPORT GIBSMunTot
{
public:
    void clear();
    bool isEmpty() const;
    double get_vDif() const;
    void set_vDif(const double &vDif);
    double get_vDevTrib() const;
    void set_vDevTrib(const double &vDevTrib);
    double get_vIBSMun() const;
    void set_vIBSMun(const double &vIBSMun);

private:
    double m_vDif = 0.0;
    double m_vDevTrib = 0.0;
    double m_vIBSMun = 0.0;
};

//W36 - gIBS - Grupo total do IBS
class CPPNFE_EXPORT GIBSTot
{
public:
    GIBSTot();
    void clear();
    bool isEmpty() const;
    double get_vIBS() const;
    void set_vIBS(const double &vIBS);
    double get_vCredPres() const;
    void set_vCredPres(const double &vCredPres);
    double get_vCredPresCondSus() const;
    void set_vCredPresCondSus(const double &vCredPresCondSus);
    std::unique_ptr<GIBSUFTot> gIBSUF;
    std::unique_ptr<GIBSMunTot> gIBSMun;

private:
    double m_vIBS = 0.0;
    double m_vCredPres = 0.0;
    double m_vCredPresCondSus = 0.0;
};

//W50 - gCBS - Grupo total da CBS
class CPPNFE_EXPORT GCBSTot
{
public:
    void clear();
    bool isEmpty() const;
    double get_vDif() const;
    void set_vDif(const double &vDif);
    double get_vDevTrib() const;
    void set_vDevTrib(const double &vDevTrib);
    double get_vCBS() const;
    void set_vCBS(const double &vCBS);
    double get_vCredPres() const;
    void set_vCredPres(const double &vCredPres);
    double get_vCredPresCondSus() const;
    void set_vCredPresCondSus(const double &vCredPresCondSus);

private:
    double m_vDif = 0.0;
    double m_vDevTrib = 0.0;
    double m_vCBS = 0.0;
    double m_vCredPres = 0.0;
    double m_vCredPresCondSus = 0.0;
};

//W57 - gMono - Grupo total da monofasia
class CPPNFE_EXPORT GMonoTot
{
public:
    void clear();
    bool isEmpty() const;
    double get_vIBSMono() const;
    void set_vIBSMono(const double &vIBSMono);
    double get_vCBSMono() const;
    void set_vCBSMono(const double &vCBSMono);
    double get_vIBSMonoReten() const;
    void set_vIBSMonoReten(const double &vIBSMonoReten);
    double get_vCBSMonoReten() const;
    void set_vCBSMonoReten(const double &vCBSMonoReten);
    double get_vIBSMonoRet() const;
    void set_vIBSMonoRet(const double &vIBSMonoRet);
    double get_vCBSMonoRet() const;
    void set_vCBSMonoRet(const double &vCBSMonoRet);

private:
    double m_vIBSMono = 0.0;
    double m_vCBSMono = 0.0;
    double m_vIBSMonoReten = 0.0;
    double m_vCBSMonoReten = 0.0;
    double m_vIBSMonoRet = 0.0;
    double m_vCBSMonoRet = 0.0;
};

//W59e - gEstornoCred - Grupo total do estorno de crédito
class CPPNFE_EXPORT GEstornoCredTot
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

//W34 - IBSCBSTot - Totais da NF-e com IBS e CBS
class CPPNFE_EXPORT IbsCbsTot
{
public:
    IbsCbsTot();
    void clear();
    bool isEmpty() const;
    double get_vBCIBSCBS() const;
    void set_vBCIBSCBS(const double &vBCIBSCBS);
    std::unique_ptr<GIBSTot> gIBS;
    std::unique_ptr<GCBSTot> gCBS;
    std::unique_ptr<GMonoTot> gMono;
    std::unique_ptr<GEstornoCredTot> gEstornoCred;

private:
    double m_vBCIBSCBS = 0.0;
};

#endif // IBSCBSTOT_H
