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


#ifndef CPPUTILITY_H
#define CPPUTILITY_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QRegularExpression>

enum class DtH{Date,           //yyyy-MM-dd
               DateTime,       //yyyy-MM-dd hh:mm:ss
               DateTimeUTC,    //ISO 8601 UTC
               Time,           //hh:mm:ss
               DateTimeNumber, //yyyyMMddhhmmss
               DateBr,         //dd/MM/yyyy
               DateTimeBr};    //dd/MM/yyyy hh:mm:ss

enum class TipoArquivo{HTML, TXT, XML};

#ifdef CPPBRASIL_STATIC
    class CppUtility
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT CppUtility
#endif
{
public:
    CppUtility();
    static void clearDateTime(QDateTime &datetime);
    static int dvModulo11(const QString &str);
    static QString insZeroLeft(const QString &value, const int &size);
    static QString dateTimeToStr(const QDateTime &datetime, const DtH &dateReturn);
    static QString doubleToStrDecimal(const double &value, const int &decimalSize);
    static QString extractStr(const QString &str, const QString &beginStr, const QString &endStr); //retorna uma string extraída do str do ponto beginStr ao endStr
    static bool fileExists(const QString &path);
    static bool parsePath(const QString &path, QString &output); //retorna o correto em output
    static bool parsePath(const QString &path); //apenas analisa se está ok
    static bool saveFile(const QString &path, const QString &fileName,
                         const TipoArquivo &tArquivo, const QByteArray &dados);
    static QString formatCPF(const QString &str);
    static QString formatCNPJ(const QString &str);
    static QString formatMoney(const double &value, const int &decimalSize);

    static int random(const int &digitos = 0);
};

typedef CppUtility CppUtil;
#endif // CPPUTILITY_H
