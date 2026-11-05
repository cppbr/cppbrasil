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


#ifndef EXPORTIND_H
#define EXPORTIND_H
#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT ExportInd
{
public:
    ExportInd();
    ~ExportInd();
    void clear();
    QString get_nRE() const;
    void set_nRE(const QString &nRE);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    double get_qExport() const;
    void set_qExport(const double &qExport);

private:
    QString m_nRE;
    QString m_chNFe;
    double m_qExport;
};

#endif // EXPORTIND_H
