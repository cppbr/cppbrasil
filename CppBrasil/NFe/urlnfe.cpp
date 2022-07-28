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


#include "urlnfe.h"

UrlNFe::UrlNFe()
{

}

QString UrlNFe::get_qrCode(const UF &uf, const TpAmb &tpAmb,
                        const VersaoQrCodeNF &versaoQrCodeNF)
{
    return get_infNFesupl(TipoInfNFeSupl::qrCode, uf, tpAmb, versaoQrCodeNF);
}

QString UrlNFe::get_urlChave(const UF &uf, const TpAmb &tpAmb,
                          const VersaoQrCodeNF &versaoQrCodeNF)
{
    return get_infNFesupl(TipoInfNFeSupl::urlChave, uf, tpAmb, versaoQrCodeNF);
}

QString UrlNFe::get_urlServidor(const UF &uf, const TpAmb &tpAmb, const TpEmis &tipoEmissao,
                             const ModeloDF &modelodf, const VersaoNF &versaoNF,
                             const WebServicesNF &webServicesNF)
{
    QString _uf, _ambiente, _modelo, _versao, _server;

    _uf = ConvDF::ufToStr(uf);
    _ambiente = (tpAmb == TpAmb::Producao) ? "producao" : "homologacao";
    _modelo = (modelodf == ModeloDF::NFe) ? "nfe" : "nfce";
    _versao =  ConvNF::versaoNFToStr(versaoNF);
    Q_INIT_RESOURCE(resourcenfe);
    QFile _file;
    _file.setFileName(":/resnfe/urlnfe.json");
    _file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString _strJs;
    _strJs = _file.readAll();
    _file.close();

    QJsonParseError _errorJs;
    QJsonDocument _docJs = QJsonDocument::fromJson(_strJs.toUtf8(),&_errorJs);

    //se houver erro no arquivo
    if (_errorJs.error != QJsonParseError::NoError){
        qWarning() << _errorJs.errorString();
    }


    //primeiro pega o nome do servidor
    QJsonObject _joDf, _joUF, _joUFSearch, _joServerName;
    QJsonValue  _jvDf, _jvUF, _jvUFSearch, _jvServerName;
    //buscando modelo
    _joDf = _docJs.object();
    _jvDf = _joDf.value(_modelo);

    //caso seja contingência NFe, só existem dois servidores: SVC-AN e SVC-RS
    if ((tipoEmissao == TpEmis::Contingencia_SVCAN ||
         tipoEmissao == TpEmis::Contingencia_SVCRS) &&
        (modelodf == ModeloDF::NFe) )
    {
        _server =  (tpAmb == TpAmb::Homologacao) ? "WH_" : "WP_";
        _server += (tipoEmissao == TpEmis::Contingencia_SVCAN) ? "SVC-AN" : "SVC-RS";

    } else
    {
        //caso não seja contingência busca por UF
        _joUF = _jvDf.toObject();
        _jvUF = _joUF.value(QString("uf"));

        _joUFSearch = _jvUF.toObject();
        _jvUFSearch = _joUFSearch.value(_uf);

        _joServerName = _jvUFSearch.toObject();
        _jvServerName = _joServerName[_ambiente];
        _server = _jvServerName.toString();
    }

    //pegando UrlNFe do servidor
    //voltando ao modelo df e buscando a raiz server
    QJsonObject _joServer = _jvDf.toObject();
    QJsonValue  _jvServer = _joServer.value(QString("server"));
    //buscando o servidor pelo seu nome
    QJsonObject _joServerSearch  = _jvServer.toObject();
    QJsonValue  _jvServerSearch  = _joServerSearch[_server];
    //buscando a versao
    QJsonObject _joVersaoDF = _jvServerSearch.toObject();
    QJsonValue  _jvVersaoDF = _joVersaoDF.value(_versao);
    //buscando a UrlNFe pelo nome do web services
    QJsonObject _joUrlNFe = _jvVersaoDF.toObject();
    QJsonValue  _jvUrlNFe = _joUrlNFe[ConvNF::webServicesNFToMetodo(webServicesNF)];

    return _jvUrlNFe.toString();
}

QString UrlNFe::get_infNFesupl(const TipoInfNFeSupl &tipo, const UF &uf, const TpAmb &tpAmb,
                            const VersaoQrCodeNF &versaoQrCodeNF)
{
    QString _sAmbiente, _sUF, _sVersaoqrcode;
    if (tipo == TipoInfNFeSupl::qrCode)
    {
        _sAmbiente = (tpAmb == TpAmb::Producao) ? "qrcode_p" : "qrcode_h";
    } else
    {
        _sAmbiente = (tpAmb == TpAmb::Producao) ? "urlchave_p" : "urlchave_h";
    }

    _sUF = ConvDF::ufToStr(uf);
    _sVersaoqrcode = QString("%1%2").arg("qr_", ConvNF::versaoQrCodeNFToStr(versaoQrCodeNF));
    Q_INIT_RESOURCE(resourcenfe);
    QFile _file;
    _file.setFileName(":/resnfe/urlnfe.json");
    _file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString _jstr;
    _jstr = _file.readAll();
    _file.close();

    QJsonParseError _jerror;
    QJsonDocument _jdoc = QJsonDocument::fromJson(_jstr.toUtf8(),&_jerror);

    if (_jerror.error != QJsonParseError::NoError){
        qWarning() << _jerror.errorString();
    }

    QJsonObject _jo_df = _jdoc.object();
    QJsonValue _jv_df = _jo_df.value(QString("nfce"));

    QJsonObject _jo_qr = _jv_df.toObject();
    QJsonValue  _jv_qr = _jo_qr.value(QString("qrcode"));

    QJsonObject _jo_uf = _jv_qr.toObject();
    QJsonValue  _jv_uf = _jo_uf.value(_sUF);

    QJsonObject _jo_versaoQr = _jv_uf.toObject();
    QJsonValue  _jv_versaoQr = _jo_versaoQr.value(_sVersaoqrcode);;

    QJsonObject _jo_qrSearch = _jv_versaoQr.toObject();
    QJsonValue  _jv_qrSearch = _jo_qrSearch[_sAmbiente];

    return _jv_qrSearch.toString();
}

