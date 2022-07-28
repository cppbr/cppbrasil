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


#ifndef ENCERRANTE_H
#define ENCERRANTE_H

#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT Encerrante
{
public:
    Encerrante();
    ~Encerrante();
    void clear();
    int get_nBico() const;
    void set_nBico(const int &nBico);
    int get_nBomba() const;
    void set_nBomba(const int &nBomba);
    int get_nTanque() const;
    void set_nTanque(const int &nTanque);
    double get_vEncIni() const;
    void set_vEncIni(const double &vEncIni);
    double get_vEncFin() const;
    void set_vEncFin(const double &vEncFin);

private:
    int m_nBico;
    int m_nBomba;
    int m_nTanque;
    double m_vEncIni;
    double m_vEncFin;
};

#endif // ENCERRANTE_H
