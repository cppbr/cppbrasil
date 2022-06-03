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


#ifndef DEDUC_H
#define DEDUC_H

#include <QString>

#ifdef CPPBRASIL_STATIC
    class Deduc
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Deduc
#endif

{
public:
    Deduc();
    ~Deduc();
    void clear();
    QString get_xDed() const;
    void set_xDed(const QString &xDed);
    double get_vDed() const;
    void set_vDed(const double &vDed);
private:
    QString m_xDed;
    double m_vDed;
};

#endif // DEDUC_H
