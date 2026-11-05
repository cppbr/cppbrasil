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


#ifndef PROCREF_H
#define PROCREF_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT ProcRef
{
public:
    ProcRef();
    ~ProcRef();
    void clear();
    QString get_nProc() const;
    void set_nProc(const QString &nProc);
    IndProc get_indProc() const;
    void set_indProc(const IndProc &indProc);
    TpAto get_tpAto() const;
    void set_tpAto(const TpAto &tpAto);
private:
    QString m_nProc;
    IndProc m_indProc;
    TpAto m_tpAto;

};

#endif // PROCREF_H
