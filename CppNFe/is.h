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

#ifndef IS_H
#define IS_H

#include <memory>
#include <QString>
#include <QtGlobal>
#include <CppBase/cppbrasil_global.h>

//UB01 - Informações do Imposto Seletivo (NT 2025.002)
class CPPNFE_EXPORT Is
{
public:
    void clear();
    bool isEmpty() const;
    QString get_CSTIS() const;
    void set_CSTIS(const QString &CSTIS);
    QString get_cClassTribIS() const;
    void set_cClassTribIS(const QString &cClassTribIS);
    double get_vBCIS() const;
    void set_vBCIS(const double &vBCIS);
    double get_pIS() const;
    void set_pIS(const double &pIS);
    double get_adRemIS() const;
    void set_adRemIS(const double &adRemIS);
    QString get_uTrib() const;
    void set_uTrib(const QString &uTrib);
    double get_qTrib() const;
    void set_qTrib(const double &qTrib);
    double get_vIS() const;
    void set_vIS(const double &vIS);

private:
    QString m_CSTIS;
    QString m_cClassTribIS;
    double m_vBCIS = 0.0;
    double m_pIS = 0.0;
    double m_adRemIS = 0.0;
    QString m_uTrib;
    double m_qTrib = 0.0;
    double m_vIS = 0.0;
};

#endif // IS_H
