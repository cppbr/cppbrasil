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


#ifndef INFINTERMED_H
#define INFINTERMED_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT InfIntermed
{
public:
    InfIntermed();
    ~InfIntermed();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_idCadIntTran() const;
    void set_idCadIntTran(const QString &idCadIntTran);
private:
    QString m_CNPJ;
    QString m_idCadIntTran;

};

#endif // INFINTERMED_H
