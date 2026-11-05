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


#ifndef INFRESPTEC_H
#define INFRESPTEC_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT InfRespTec
{
public:
    InfRespTec();
    ~InfRespTec();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_xContato() const;
    void set_xContato(const QString &xContato);
    QString get_email() const;
    void set_email(const QString &email);
    QString get_fone() const;
    void set_fone(const QString &fone);
    int get_idCSRT() const;
    void set_idCSRT(const int &idCSRT);
    QString get_hashCSRT() const;
    void set_hashCSRT(const QString &hashCSRT);

private:
    QString m_CNPJ;
    QString m_xContato;
    QString m_email;
    QString m_fone;
    int m_idCSRT;
    QString m_hashCSRT;

};

#endif // INFRESPTEC_H
