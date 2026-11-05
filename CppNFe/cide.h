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


#ifndef CIDE_H
#define CIDE_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT Cide
{
public:
    Cide();
    ~Cide();
    void clear();
    double get_qBCProd() const;
    void set_qBCProd(const double &qBCProd);
    double get_vAliqProd() const;
    void set_vAliqProd(const double &vAliqProd);
    double get_vCIDE() const;
    void set_vCIDE(const double &vCIDE);
private:
    double m_qBCProd;
    double m_vAliqProd;
    double m_vCIDE;
};

#endif // CIDE_H
