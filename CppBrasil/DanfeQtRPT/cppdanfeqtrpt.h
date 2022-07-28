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
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppdanfe.h>
#include <qtrpt.h>


class CPPBRASIL_EXPORT CppDanfeQtRPT: public QObject, public CppDanfe
{
    Q_OBJECT

public:
    CppDanfeQtRPT(const CppNFe *cppnfe, const int &recNo = 0);
    virtual ~CppDanfeQtRPT();

    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    virtual void print();

private:
    std::shared_ptr<QtRPT> m_report;
    void loadRetEnt();
private slots:
    void setDSInfo(DataSetInfo &dsInfo);
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setField(RptFieldObject &fieldObject);
    void setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage);

};

#endif // CPPDANFEQTRPT_H
