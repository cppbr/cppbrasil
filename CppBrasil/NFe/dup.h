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


#ifndef DUP_H
#define DUP_H

#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>

class CPPBRASIL_EXPORT Dup
{
public:
    Dup();
    ~Dup();
    void clear();
    QString get_nDup() const;
    void set_nDup(const QString &nDup);
    QDateTime get_dVenc() const;
    void set_dVenc(const QDateTime &dVenc);
    double get_vDup() const;
    void set_vDup(const double &vDup);

private:
    QString m_nDup;
    QDateTime m_dVenc; //AAAA-MM-DD
    double m_vDup;
};

#endif // DUP_H
