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


#ifndef CPPDANFEQTRPT_H
#define CPPDANFEQTRPT_H

#include <memory>
#include <QString>
#include <CppBase/cppbrasil_global.h>
#include <CppDanfe/cppdanfe.h>
#include <qtrpt.h>


class CPPDANFEQTRPT_EXPORT CppDanfeQtRPT: public QObject, public CppDanfe
{
    Q_OBJECT

public:
    CppDanfeQtRPT(const CppNFe *cppnfe, const int &recNo = 0);
    virtual ~CppDanfeQtRPT();

    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    virtual void print();

private:
    //local de retirada/entrega exibido no DANFE (dsRetEnt)
    struct RetEnt{
        QString m_descricao;
        QString m_CNPJ;
        QString m_CPF;
        QString m_xNome;
        QString m_xLgr;
        int m_nro = 0;
        QString m_xCpl;
        QString m_xBairro;
        int m_cMun = 0;
        QString m_xMun;
        QString m_UF;
        int m_CEP = 0;
        int m_cPais = 0;
        QString m_xPais;
        QString m_fone;
        QString m_email;
        QString m_IE;

        RetEnt(){};

        RetEnt(QString descricao, QString CNPJ, QString CPF, QString xNome, QString xLgr, int nro, QString xCpl,
               QString xBairro, int cMun, QString xMun, QString UF, int CEP, int cPais, QString xPais, QString fone,
               QString email, QString IE): m_descricao(descricao), m_CNPJ(CNPJ), m_CPF(CPF), m_xNome(xNome),
                                           m_xLgr(xLgr), m_nro(nro), m_xCpl(xCpl), m_xBairro(xBairro), m_cMun(cMun),
                                           m_xMun(xMun), m_UF(UF), m_CEP(CEP), m_cPais(cPais), m_xPais(xPais),
                                           m_fone(fone), m_email(email), m_IE(IE) {};
    };

    std::unique_ptr<QtRPT> m_report; //sem parent, o unique_ptr e o unico dono.
    QList<RetEnt> m_retEntList; //por instancia: antes era global e compartilhada entre DANFEs.
    void loadRetEnt();
private slots:
    void setDSInfo(DataSetInfo &dsInfo);
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setField(RptFieldObject &fieldObject);
    void setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage);

};

#endif // CPPDANFEQTRPT_H
