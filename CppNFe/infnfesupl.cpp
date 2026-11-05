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


#include "infnfesupl.h"

InfNFeSupl::InfNFeSupl()
{
}

InfNFeSupl::~InfNFeSupl()
{
}

void InfNFeSupl::clear()
{
    this->m_qrCode.clear();
    this->m_urlChave.clear();
}

QString InfNFeSupl::get_qrCode() const
{
    return this->m_qrCode;
}

void InfNFeSupl::set_qrCode(const QString &qrCode)
{
    this->m_qrCode = qrCode;
}

QString InfNFeSupl::get_urlChave() const
{
    return this->m_urlChave;
}

void InfNFeSupl::set_urlChave(const QString &urlChave)
{
    this->m_urlChave = urlChave;
}

void InfNFeSupl::setFromNode(const QString &infNFeSupl)
{
    QString _qr, _url;
    _qr = CppUtility::extractStr(infNFeSupl, QStringLiteral("<qrCode>"), QStringLiteral("</qrCode>"));
    _qr = _qr.mid(QStringLiteral("<qrCode>").length(), _qr.length() - QStringLiteral("<qrCode></qrCode>").length());

    _url = CppUtility::extractStr(infNFeSupl, QStringLiteral("<urlChave>"), QStringLiteral("</urlChave>"));
    _url = _url.mid(QStringLiteral("<urlChave>").length(), _url.length() - QStringLiteral("<urlChave></urlChave>").length());

    this->m_qrCode = _qr;
    this->m_urlChave = _url;
}
