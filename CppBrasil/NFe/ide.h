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


#ifndef IDE_H
#define IDE_H

#include <memory>
#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/nfref.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/container.h>
#include <CppBrasil/cpputility.h>

class CPPNFE_EXPORT Ide
{

public:
    Ide();
    ~Ide();
    void clear();
    int get_cUF() const;
    void set_cUF(const int &cUF);
    int get_cNF() const;
    void set_cNF(const int &cNF);
    QString get_natOp() const;
    void set_natOp(const QString &natOp);
    ModeloDF get_mod() const;
    void set_mod(const ModeloDF &mod);
    int get_serie() const;
    void set_serie(const int &serie);
    int get_nNF() const;
    void set_nNF(const int &nNF);
    QDateTime get_dhEmi() const;
    void set_dhEmi(const QDateTime &dhEmi);
    QDateTime get_dhSaiEnt() const;
    void set_dhSaiEnt(const QDateTime &dhSaiEnt);
    TpNF get_tpNF() const;
    void set_tpNF(const TpNF &tpNF);
    IdDest get_idDest() const;
    void set_idDest(const IdDest &idDest);
    int get_cMunFG() const;
    void set_cMunFG(const int &cMunFG);
    TpImp get_tpImp() const;
    void set_tpImp(const TpImp &tpImp);
    TpEmis get_tpEmis() const;
    void set_tpEmis(const TpEmis &tpEmis);
    QString get_cDV() const;
    void set_cDV(const QString &cDV);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    FinNFe get_finNFe() const;
    void set_finNFe(const FinNFe &finNFe);
    IndFinal get_indFinal() const;
    void set_indFinal(const IndFinal &indFinal);
    IndPres get_indPres() const;
    void set_indPres(const IndPres &indPres);
    IndIntermed get_indIntermed() const;
    void set_indIntermed(const IndIntermed &indIntermed);
    ProcEmi get_procEmi() const;
    void set_procEmi(const ProcEmi &procEmi);
    QString get_verProc() const;
    void set_verProc(const QString &verProc);
    QDateTime get_dhCont() const;
    void set_dhCont(const QDateTime &dhCont);
    QString get_xJust() const;
    void set_xJust(const QString &xJust);

    std::shared_ptr<Container<NFRef>> NFref;

private:
    int m_cUF;
    int m_cNF;
    QString m_natOp;
    ModeloDF m_mod;
    int m_serie;
    int m_nNF;
    QDateTime m_dhEmi;
    QDateTime m_dhSaiEnt;
    TpNF m_tpNF;
    IdDest m_idDest;
    int m_cMunFG;
    TpImp m_tpImp;
    TpEmis m_tpEmis;
    QString m_cDV;
    TpAmb m_tpAmb;
    FinNFe m_finNFe;
    IndFinal m_indFinal;
    IndPres m_indPres;
    IndIntermed m_indIntermed;
    ProcEmi m_procEmi;
    QString m_verProc;
    QDateTime m_dhCont;
    QString m_xJust;

};

#endif // IDE_H
