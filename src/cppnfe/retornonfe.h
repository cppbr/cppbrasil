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


#ifndef RETORNONFE_H
#define RETORNONFE_H

#include <QString>
#include <QDateTime>
#include <memory>
#include <confignfe.h>
#include "convnf.h"
#include "container.h"
#include "cpputility.h"

#ifdef CPPBRASIL_STATIC
#else
    #include "cppbrasil_global.h"
#endif

/* As classes retEnviNFe e retConsReciNFe estão representadas no projeto somente pela classe
 * RetConsReciNFe. Essas duas classes possuem os mesmo campos, portanto não será necessário
 * fazer duas classes iguais. A classe protNFe também pertente a essas duas classes.
*/

#ifdef CPPBRASIL_STATIC
    class ProtNFe
#else
    class CPPBRASIL_EXPORT ProtNFe
#endif
{
    friend class WSNFe;
    friend class NotaFiscal;
    friend class XmlRead;
public:
    ProtNFe();
    ~ProtNFe();
    void clear();
    QString get_versao() const;
    QString get_Id() const;
    TpAmb get_tpAmb() const;
    QString get_verAplic() const;
    QString get_chNFe() const;
    QDateTime get_dhRecbto() const;
    QString get_nProt() const;
    QString get_digVal() const;
    int get_cStat() const;
    QString get_xMotivo() const;
    int get_cMsg() const;
    QString get_xMsg() const;
    QString get_xml() const;
    void salvarXML(const ConfigNFe *config);
    void salvarXML(const QString &caminho, const QString &nomeArquivo);

protected:
    void set_versao(const QString &versao);
    void set_Id(const QString &Id);
    void set_tpAmb(const TpAmb &tpAmb);
    void set_verAplic(const QString &verAplic);
    void set_chNFe(const QString &chNFe);
    void set_dhRecbto(const QDateTime &dhRecbto);
    void set_nProt(const QString &nProt);
    void set_digVal(const QString &digVal);
    void set_cStat(const int &cStat);
    void set_xMotivo(const QString &xMotivo);
    void set_cMsg(const int &cMsg);
    void set_xMsg(const QString &xMsg);
    void set_xml(const QString &xml);

private:
    //protNFe
    QString m_versao;
    //infProt
    QString m_Id;
    TpAmb m_tpAmb;
    QString m_verAplic;
    QString m_chNFe;
    QDateTime m_dhRecbto;
    QString m_nProt;
    QString m_digVal;
    int m_cStat;
    QString m_xMotivo;
    int m_cMsg;
    QString m_xMsg;
    QString m_xml;

};

#ifdef CPPBRASIL_STATIC
    class RetConsReciNFe
#else
    class CPPBRASIL_EXPORT RetConsReciNFe
#endif
{
    friend class WSNFe;
    friend class NotaFiscal;
public:
    RetConsReciNFe();
    ~RetConsReciNFe();
    void clear();
    QString get_versao() const;
    TpAmb get_tpAmb() const;
    QString get_verAplic() const;
    QString get_nRec() const;
    int get_cStat() const;
    QString get_xMotivo() const;
    int get_cUF() const;
    QDateTime get_dhRecbto() const;
    int get_cMsg() const;
    QString get_xMsg() const;

    std::shared_ptr<Container<ProtNFe>> protNFe;

protected:
    void set_versao(const QString &versao);
    void set_tpAmb(const TpAmb &tpAmb);
    void set_verAplic(const QString &verAplic);
    void set_nRec(const QString &nRec);
    void set_cStat(const int &cStat);
    void set_xMotivo(const QString &xMotivo);
    void set_cUF(const int &cUF);
    void set_dhRecbto(const QDateTime &dhRecbto);
    void set_cMsg(const int &cMsg);
    void set_xMsg(const QString &xMsg);
    void finalizarXML(); //gera o xml junto com o retorno e salva

private:
    QString m_versao;
    TpAmb m_tpAmb;
    QString m_verAplic;
    QString m_nRec;
    int m_cStat;
    QString m_xMotivo;
    int m_cUF;
    QDateTime m_dhRecbto;
    int m_cMsg;
    QString m_xMsg;

};

#endif // RETORNONFE_H
