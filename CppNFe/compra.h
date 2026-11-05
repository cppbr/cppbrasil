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


#ifndef COMPRA_H
#define COMPRA_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT Compra
{
public:
    Compra();
    ~Compra();
    void clear();
    QString get_xNEmp() const;
    void set_xNEmp(const QString &xNEmp);
    QString get_xPed() const;
    void set_xPed(const QString &xPed);
    QString get_xCont() const;
    void set_xCont(const QString &xCont);
private:
    QString m_xNEmp;
    QString m_xPed;
    QString m_xCont;
};

#endif // COMPRA_H
