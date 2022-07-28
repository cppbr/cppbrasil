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


#ifndef ADI_H
#define ADI_H

#include<QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT Adi
{
public:
    Adi();
    ~Adi();
    void clear();
    int get_nAdicao() const;
    void set_nAdicao(const int &nAdicao);
    int get_nSeqAdic() const;
    void set_nSeqAdic(const int &nSeqAdic);
    QString get_cFabricante() const;
    void set_cFabricante(const QString &cFabricante);
    double get_vDescDI() const;
    void set_vDescDI(const double &vDescDI);
    QString get_nDraw() const;
    void set_nDraw(const QString &nDraw);
private:
    int m_nAdicao;
    int m_nSeqAdic;
    QString m_cFabricante;
    double m_vDescDI;
    QString m_nDraw;
};

#endif // ADI_H
