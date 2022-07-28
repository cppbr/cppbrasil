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


#ifndef DETEXPORT_H
#define DETEXPORT_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/exportind.h>

class CPPBRASIL_EXPORT DetExport
{
public:
    DetExport();
    ~DetExport();
    void clear();
    QString get_nDraw() const;
    void set_nDraw(const QString &nDraw);

    std::shared_ptr<ExportInd> exportInd;

private:
    QString m_nDraw;
};

#endif // DETEXPORT_H
