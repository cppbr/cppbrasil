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

#ifndef ORIGCOMB_H
#define ORIGCOMB_H

#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class OrigComb
{
public:
    OrigComb();
    ~OrigComb();
    void clear();
    IndImport get_indImport() const;
    void set_indImport(const IndImport &indImport);
    int get_cUFOrig() const;
    void set_cUFOrig(const int &cUFOrig);
    double get_pOrig() const;
    void set_pOrig(const double &pOrig);
private:
    IndImport m_indImport;
    int m_cUFOrig;
    double m_pOrig;
};

#endif // ORIGCOMB_H
