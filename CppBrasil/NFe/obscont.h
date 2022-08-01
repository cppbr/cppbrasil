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


#ifndef OBSCONT_H
#define OBSCONT_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>

class CPPNFE_EXPORT ObsCont
{
public:
    ObsCont();
    ~ObsCont();
    void clear();
    QString get_xCampo() const;
    void set_xCampo(const QString &xCampo);
    QString get_xTexto() const;
    void set_xTexto(const QString &xTexto);

private:
    QString m_xCampo;
    QString m_xTexto;

};

#endif // OBSCONT_H
