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


#ifndef IPI_H
#define IPI_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPBRASIL_EXPORT Ipi
{
public:
    Ipi();
    ~Ipi();
    void clear();
    QString get_CNPJProd() const;
    void set_CNPJProd(const QString &CNPJProd);
    QString get_cSelo() const;
    void set_cSelo(const QString &cSelo);
    int get_qSelo() const;
    void set_qSelo(const int &qSelo);
    QString get_cEnq() const;
    void set_cEnq(const QString &cEnq);
    CstIPI get_CST() const;
    void set_CST(const CstIPI &CST);
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_pIPI() const;
    void set_pIPI(const double &pIPI);
    double get_qUnid() const;
    void set_qUnid(const double &qUnid);
    double get_vUnid() const;
    void set_vUnid(const double &vUnid);
    double get_vIPI() const;
    void set_vIPI(const double &vIPI);

private:
    QString m_CNPJProd;
    QString m_cSelo;
    int m_qSelo;
    QString m_cEnq;
    CstIPI m_CST;
    double m_vBC;
    double m_pIPI;
    double m_qUnid;
    double m_vUnid;
    double m_vIPI;

};

#endif // IPI_H
