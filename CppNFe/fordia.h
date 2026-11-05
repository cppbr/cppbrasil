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


#ifndef FORDIA_H
#define FORDIA_H
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT ForDia
{
public:
    ForDia();
    ~ForDia();
    void clear();
    int get_dia() const;
    void set_dia(const int &dia);
    double get_qtde() const;
    void set_qtde(const double &qtde);

private:
    int m_dia;
    double m_qtde;
};

#endif // FORDIA_H
