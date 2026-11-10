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

#ifndef GCRED_H
#define GCRED_H

#include <QString>
#include <CppBase/cppbrasil_global.h>

class CPPNFE_EXPORT GCred
{
public:
    void clear();
    QString get_cCredPresumido() const;
    void set_cCredPresumido(const QString &cCredPresumido);
    double get_pCredPresumido() const;
    void set_pCredPresumido(const double &pCredPresumido);
    double get_vCredPresumido() const;
    void set_vCredPresumido(const double &vCredPresumido);

private:
    QString m_cCredPresumido;
    double m_pCredPresumido = 0.0;
    double m_vCredPresumido = 0.0;
};

#endif // GCRED_H
