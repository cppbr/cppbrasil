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


#ifndef FAT_H
#define FAT_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPBRASIL_EXPORT Fat
{
public:
    Fat();
    ~Fat();
    void clear();
    QString get_nFat() const;
    void set_nFat(const QString &nFat);
    double get_vOrig() const;
    void set_vOrig(const double &vOrig);
    double get_vDesc() const;
    void set_vDesc(const double &vDesc);
    double get_vLiq() const;
    void set_vLiq(const double &vLiq);
private:
    QString m_nFat;
    double m_vOrig;
    double m_vDesc;
    double m_vLiq;
};

#endif // FAT_H
