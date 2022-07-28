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


#ifndef VOL_H
#define VOL_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/lacres.h>

class CPPBRASIL_EXPORT Vol
{
public:
    Vol();
    ~Vol();
    void clear();
    double get_qVol() const;
    void set_qVol(const double &qVol);
    QString get_esp() const;
    void set_esp(const QString &esp);
    QString get_marca() const;
    void set_marca(const QString &marca);
    QString get_nVol() const;
    void set_nVol(const QString &nVol);
    double get_pesoL() const;
    void set_pesoL(const double &pesoL);
    double get_pesoB() const;
    void set_pesoB(const double &pesoB);

    std::shared_ptr<Container<Lacres>> lacres;

private:
    double m_qVol;
    QString m_esp;
    QString m_marca;
    QString m_nVol;
    double m_pesoL;
    double m_pesoB;
};

#endif // VOL_H
