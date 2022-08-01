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


#ifndef XMLWRITE_H
#define XMLWRITE_H

#include <QString>
#include <QByteArray>
#include <QXmlStreamWriter>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QTextCodec>
#endif
#include <QFile>
#include <QDebug>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/cppcrypto.h>
#include <CppBrasil/cpplibxml2.h>
#include <CppBrasil/NFe/infnfe.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/urlnfe.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT XmlWrite
{
public:
    explicit XmlWrite(const InfNFe *infNFe, const ConfigNFe *confgNFe);
    ~XmlWrite();
    bool gerarXML(QByteArray *output);
    static QByteArray get_infNFeSupl_OFFLINE(const QString &chave, const VersaoQrCodeNF &versaoQrcodeNF,
                                         const TpAmb &tpAmb, const QDateTime &emissao,
                                         const double &vNF, const QString &digestValue,
                                         const QString &idCSC, const QString &CSC,
                                         const int &codigoUF, const bool &element_infNFeSupl);

    static QByteArray get_protNFe(const QString &versao, const QString &Id,const TpAmb &tpAmb,
                                  const QString &verAplic, const QString &chNFe, const QDateTime &dhRecbto,
                                  const QString &nProt, const QString &digVal, const int &cStat,
                                  const QString &xMotivo, const int &cMsg, const QString &xMsg);

    static QByteArray get_nfeProc(const QString &versao);


private:
    const InfNFe *m_infNFe;
    const ConfigNFe *m_confgNFe;
    QXmlStreamWriter *m_xmlw;
    QByteArray m_strXML;

    void get_infNFe();
    void get_ide();
    void get_NFref();
    void get_emit();
    void get_avulsa();
    void get_dest();
    void get_retirada();
    void get_entrega();
    void get_autXML();
    void get_det();
    void get_prod(const int &i);
    void get_imposto(const int &i);
    void get_ICMS(const int &i);
    void get_ICMSUFDest(const int &i);
    void get_IPI(const int &i);
    void get_II(const int &i);
    void get_PIS(const int &i);
    void get_PISST(const int &i);
    void get_COFINS(const int &i);
    void get_COFINSST(const int &i);
    void get_ISSQN(const int &i);
    void get_impostoDevol(const int &i);
    void get_obsItem(const int &i);
    void get_total();
    void get_transp();
    void get_cobr();
    void get_pag();
    void get_infIntermed();
    void get_infAdic();
    void get_exporta();
    void get_compra();
    void get_cana();
    void get_infRespTec();
    void get_infNFeSupl_ONLINE();
    void gerarNFE();
};

#endif // XMLWRITE_H
