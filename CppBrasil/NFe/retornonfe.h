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
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/convnf.h>


/* As classes retEnviNFe e retConsReciNFe estão representadas no projeto somente pela classe
 * RetConsReciNFe. Essas duas classes possuem os mesmo campos, portanto, não será necessário
 * fazer duas classes iguais. A classe protNFe também pertente a essas duas classes.
*/

class CPPNFE_EXPORT ProtNFe
{
public:
    ProtNFe();
    ~ProtNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_Id() const;
    void set_Id(const QString &Id);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);
    QString get_digVal() const;
    void set_digVal(const QString &digVal);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);
    int get_cMsg() const;
    void set_cMsg(const int &cMsg);
    QString get_xMsg() const;
    void set_xMsg(const QString &xMsg);
    QString get_xml() const;
    void set_xml(const QString &xml);
    void salvarXML(const QString &caminho, const QString &nomeArquivo);

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

class CPPNFE_EXPORT RetConsReciNFe
{
public:
    RetConsReciNFe();
    ~RetConsReciNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    QString get_nRec() const;
    void set_nRec(const QString &nRec);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);
    int get_cUF() const;
    void set_cUF(const int &cUF);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    int get_cMsg() const;
    void set_cMsg(const int &cMsg);
    QString get_xMsg() const;
    void set_xMsg(const QString &xMsg);

    std::shared_ptr<Container<ProtNFe>> protNFe;

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
