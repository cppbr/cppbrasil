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


#ifndef CONVDF_H
#define CONVDF_H

#include <QString>
#include <QList>
#include <QSsl>
#include <CppBrasil/cppbrasil_global.h>

//TIPOS GERAIS PARA TODOS OS PROJETOS CppBrasil
//UF
enum class UF{None = -1,
              RO = 11, AC = 12, AM = 13, RR = 14, PA = 15, AP = 16, TO = 17, MA = 21, PI = 22,
              CE = 23, RN = 24, PB = 25, PE = 26, AL = 27, SE = 28, BA = 29, MG = 31, ES = 32,
              RJ = 33, SP = 35, PR = 41, SC = 42, RS = 43, MS = 50, MT = 51, GO = 52, DF = 53};

//modelo DFe (obs: apenas documentos fiscais eletrônicos)
enum class ModeloDF{NFe = 55, NFCe = 65};

//tpAmb - Identificação do Ambiente
enum class TpAmb {None = -1, Producao = 1, Homologacao = 2};

//tipo de log
enum class TipoMsgLog{MsgErro, MsgEnvio, MsgRetorno};

class CPPNFE_EXPORT ConvDF
{
public:
    ConvDF();
    //CONVERSÕES GERAIS
    static QString ufToStr(const UF &value);
    static UF strToUF(const QString &value);
    static int ufToInt(const UF &value);
    static UF intToUF(const int &value);
    static QString modeloDFToStr(const ModeloDF &value);
    static ModeloDF strToModeloDF(const QString &value);
    static QString tpAmbToStr(const TpAmb &value);
    static TpAmb strToTpAmb(const QString &value);
    static QSsl::SslProtocol strToQSslProtocol(const QString &value);
    static QString qSslProtocolToStr(const QSsl::SslProtocol &value);
};

#endif // CONVDF_H
