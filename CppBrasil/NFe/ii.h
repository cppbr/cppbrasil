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


#ifndef II_H
#define II_H

#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT Ii
{
public:
    Ii();
    ~Ii();
    void clear();
    double get_vBC() const;
    void set_vBC(const double &vBC);
    double get_vDespAdu() const;
    void set_vDespAdu(const double &vDespAdu);
    double get_vII() const;
    void set_vII(const double &vII);
    double get_vIOF() const;
    void set_vIOF(const double &vIOF);

private:
    double m_vBC;
    double m_vDespAdu;
    double m_vII;
    double m_vIOF;

};

#endif // II_H
