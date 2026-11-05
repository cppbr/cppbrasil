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


#include "convdf.h"



ConvDF::ConvDF()
{

}

QString ConvDF::ufToStr(const UF &value)
{
    QList<int> _ir =     {-1, 11, 12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 25, 26, 27,
                              28, 29, 31, 32, 33, 35, 41, 42, 43, 50, 51, 52, 53};
    QList<QString> _sr = {"None",
                          "RO", "AC", "AM", "RR", "PA", "AP", "TO", "MA", "PI",
                          "CE", "RN", "PB", "PE", "AL", "SE", "BA", "MG", "ES",
                          "RJ", "SP", "PR", "SC", "RS", "MS", "MT", "GO", "DF"};

    int _i = _ir.indexOf(static_cast<int>(value));
    return _sr[_i];
}

UF ConvDF::strToUF(const QString &value)
{
    QList<QString> _ir = {"None",
                          "RO", "AC", "AM", "RR", "PA", "AP", "TO", "MA", "PI",
                          "CE", "RN", "PB", "PE", "AL", "SE", "BA", "MG", "ES",
                          "RJ", "SP", "PR", "SC", "RS", "MS", "MT", "GO", "DF"};

    QList<int> _sr =     {-1, 11, 12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 25, 26, 27,
                              28, 29, 31, 32, 33, 35, 41, 42, 43, 50, 51, 52, 53};

    int _i = _ir.indexOf(value);
    return static_cast<UF>(_sr[_i]);
}

int ConvDF::ufToInt(const UF &value)
{
   return static_cast<int>(value);
}

UF ConvDF::intToUF(const int &value)
{
    if (value < 11 || value > 53)
        return UF::None;
    else
        return static_cast<UF>(value);
}

QString ConvDF::modeloDFToStr(const ModeloDF &value)
{
    return QString::number(static_cast<int>(value));
}

ModeloDF ConvDF::strToModeloDF(const QString &value)
{
   QList<QString> _r = {"55","65"};
   int _i = _r.indexOf(value);
   return static_cast<ModeloDF>(_r[_i].toInt());
}

QString ConvDF::tpAmbToStr(const TpAmb &value)
{
    if (value == TpAmb::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpAmb ConvDF::strToTpAmb(const QString &value)
{
    QList<QString> _r = {"-1","1","2"};
    int _i = _r.indexOf(value);

    if (_i == -1 || value.isEmpty())
        return TpAmb::None;
    else
        return static_cast<TpAmb>(_r[_i].toInt());
}

QSsl::SslProtocol ConvDF::strToQSslProtocol(const QString &value)
{
    QList<QString> _r = {"SslV3", "SslV2", "TlsV1_0", "TlsV1_1", "TlsV1_2",
                         "AnyProtocol", "TlsV1SslV3", "SecureProtocols", "TlsV1_0OrLater",
                         "TlsV1_1OrLater", "TlsV1_2OrLater", "DtlsV1_0", "DtlsV1_0OrLater",
                         "DtlsV1_2", "DtlsV1_2OrLater", "TlsV1_3", "TlsV1_3OrLater"};

    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty() || value == "UnknownProtocol")
        return QSsl::SslProtocol::UnknownProtocol;
    else
        return static_cast<QSsl::SslProtocol>(_i);
}

QString ConvDF::qSslProtocolToStr(const QSsl::SslProtocol &value)
{
    QList<QString> _r = {"SslV3", "SslV2", "TlsV1_0", "TlsV1_1", "TlsV1_2",
                         "AnyProtocol", "TlsV1SslV3", "SecureProtocols", "TlsV1_0OrLater",
                         "TlsV1_1OrLater", "TlsV1_2OrLater", "DtlsV1_0", "DtlsV1_0OrLater",
                         "DtlsV1_2", "DtlsV1_2OrLater", "TlsV1_3", "TlsV1_3OrLater"};
    if (value == QSsl::SslProtocol::UnknownProtocol)
        return "UnknownProtocol";
    else
        return _r[static_cast<int>(value)];
}
