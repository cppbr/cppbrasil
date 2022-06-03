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


#ifndef URLNFE_H
#define URLNFE_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "convnf.h"

enum class TipoInfNFeSupl{qrCode, urlChave};

#ifdef CPPBRASIL_STATIC
    class UrlNFe
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT UrlNFe
#endif
{
public:
    UrlNFe();
    static QString get_qrCode(const UF &uf, const TpAmb &tpAmb,
                              const VersaoQrCodeNF &versaoQrCodeNF);
    static QString get_urlChave(const UF &uf, const TpAmb &tpAmb,
                                const VersaoQrCodeNF &versaoQrCodeNF);
    static QString get_urlServidor(const UF &uf, const TpAmb &tpAmb, const TpEmis &tipoEmissao,
                                   const ModeloDF &modelodf, const VersaoNF &versaoNF,
                                   const WebServicesNF &webServicesNF);
private:
    static QString get_infNFesupl(const TipoInfNFeSupl &tipo , const UF &uf,
                                  const TpAmb &tpAmb, const VersaoQrCodeNF &versaoQrCodeNF);

};

#endif // URLNFE_H
