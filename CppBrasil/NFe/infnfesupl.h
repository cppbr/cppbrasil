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


#ifndef INFNFESUPL_H
#define INFNFESUPL_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>

class CPPBRASIL_EXPORT InfNFeSupl
{
    friend class Nfe;
public:
    InfNFeSupl();
    ~InfNFeSupl();
    void clear();
    QString get_qrCode() const;
    void set_qrCode(const QString &qrCode);
    QString get_urlChave() const;
    void set_urlChave(const QString &urlChave);

protected:
    void setFromNode(const QString &infNFeSupl);
private:
    QString m_qrCode;
    QString m_urlChave;
};

#endif // INFNFESUPL_H
