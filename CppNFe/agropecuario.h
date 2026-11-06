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

#ifndef AGROPECUARIO_H
#define AGROPECUARIO_H

#include <memory>
#include <QString>
#include <CppBase/container.h>
#include <CppBase/cppbrasil_global.h>

class CPPNFE_EXPORT Defensivo
{
public:
    void clear();
    QString get_nReceituario() const;
    void set_nReceituario(const QString &nReceituario);
    QString get_CPFRespTec() const;
    void set_CPFRespTec(const QString &CPFRespTec);

private:
    QString m_nReceituario;
    QString m_CPFRespTec;
};

class CPPNFE_EXPORT GuiaTransito
{
public:
    void clear();
    bool isEmpty() const;
    int get_tpGuia() const;
    void set_tpGuia(int tpGuia);
    QString get_UFGuia() const;
    void set_UFGuia(const QString &UFGuia);
    QString get_serieGuia() const;
    void set_serieGuia(const QString &serieGuia);
    QString get_nGuia() const;
    void set_nGuia(const QString &nGuia);

private:
    int m_tpGuia = 0;
    QString m_UFGuia;
    QString m_serieGuia;
    QString m_nGuia;
};

class CPPNFE_EXPORT Agropecuario
{
public:
    Agropecuario();
    void clear();

    std::unique_ptr<Container<Defensivo>> defensivo;
    std::unique_ptr<GuiaTransito> guiaTransito;
};

#endif // AGROPECUARIO_H
