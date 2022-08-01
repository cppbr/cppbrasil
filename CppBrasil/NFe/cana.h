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


#ifndef CANA_H
#define CANA_H

#include <QString>
#include <memory>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/fordia.h>
#include <CppBrasil/NFe/deduc.h>

class CPPNFE_EXPORT Cana
{
public:
    Cana();
    ~Cana();
    void clear();
    QString get_safra() const;
    void set_safra(const QString &safra);
    QString get_ref() const;
    void set_ref(const QString &ref);
    double get_qTotMes() const;
    void set_qTotMes(const double &qTotMes);
    double get_qTotAnt() const;
    void set_qTotAnt(const double &qTotAnt);
    double get_qTotGer() const;
    void set_qTotGer(const double &qTotGer);
    double get_vFor() const;
    void set_vFor(const double &vFor);
    double get_vTotDed() const;
    void set_vTotDed(const double &vTotDed);
    double get_vLiqFor() const;
    void set_vLiqFor(const double &vLiqFor);

    std::shared_ptr<Container<ForDia>> forDia;
    std::shared_ptr<Container<Deduc>> deduc;
private:
    QString m_safra;
    QString m_ref;
    double m_qTotMes; //ZC07
    double m_qTotAnt; //ZC08
    double m_qTotGer; //ZC09
    double m_vFor; //ZC13
    double m_vTotDed; //ZC14
    double m_vLiqFor; //ZC15

};

#endif // CANA_H
