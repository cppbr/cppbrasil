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


#ifndef MED_H
#define MED_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT Med
{
public:
    Med();
    ~Med();
    void clear();
    QString get_cProdANVISA() const;
    void set_cProdANVISA(const QString &cProdANVISA);
    QString get_xMotivoIsencao() const;
    void set_xMotivoIsencao(const QString &xMotivoIsencao);
    double get_vPMC() const;
    void set_vPMC(const double &vPMC);
private:
    QString m_cProdANVISA;
    QString m_xMotivoIsencao;
    double m_vPMC;
};

#endif // MED_H