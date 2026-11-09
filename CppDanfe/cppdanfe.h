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


#ifndef CPPDANFE_H
#define CPPDANFE_H

#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/cpputility.h>
#include <CppDanfe/cppbarcode.h>
#include <CppBase/convdf.h>
#include <CppDanfe/qrcodegen.hpp>
#include <CppNFe/cppnfe.h>
#ifdef _WIN32
 #include <sstream>
#endif
using namespace qrcodegen;


class CPPDANFE_EXPORT CppDanfe
{
public:
    CppDanfe(const CppNFe *cppnfe, const int &recNo = 0);
    virtual ~CppDanfe();

    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    virtual void print();

protected:
    const CppNFe *m_cppnfe;
    QString m_pathArquivo;
    QString m_pathLogo;
    int m_recNo;
    QImage m_imBarcode;
    QImage m_imBarcodeCont;
    QImage m_imQrCode;
    QString m_modFrete;
    QString m_chave; //formatado no formato chave
    QString m_chaveCont; //FS ou FS-DA - formatado no formato chave
    QString m_protocolo;
    QString m_infCpl;
    QString m_nNF;
    QString m_serie;
    ModeloDF m_mod;
    TpEmis m_tpEmis;
    int m_cStat;

    QString tPagToNome(const TPag &tPag);

private:
    void genBarcode();
    void genQrCode();
    void getParam();
    QString formatChave(const QString &chave); //9999 9999 9999 9999 9999 9999 9999 9999 9999

};

#endif // CPPDANFE_H
