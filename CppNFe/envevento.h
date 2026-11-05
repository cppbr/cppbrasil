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


#ifndef ENVEVENTO_H
#define ENVEVENTO_H

#include <memory>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/cpputility.h>
#include <CppBase/container.h>
#include <CppNFe/convnf.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/notafiscal.h>
#include <CppNFe/wsnfe.h>
#include <CppNFe/retenvevento.h>
#include <CppNFe/tevento.h>


class CPPNFE_EXPORT EnvEvento : public QObject
{
    Q_OBJECT
public:
    EnvEvento(ConfigNFe* confgNFe, NotaFiscal* notafiscal);
    ~EnvEvento();
    void clear();
    bool enviarEvento(const int &idLote);
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_error() const;

    std::unique_ptr<Container<TEvento>> evento;
    std::unique_ptr<RetEnvEvento> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private slots:
    void onError(const QString &error);

private:
    ConfigNFe* config; //ponteiro de fora
    NotaFiscal* notafiscal; //ponteiro de fora
    QString m_error;
    QString m_xml;
    QString m_versao;
    bool assinarEvento(const QString &evento, const QString &uri, QString &output);
    bool gerarXML(const int &idLote);
    void updateEvento(int &index);
    bool validarEvento();
    void tratarRetorno();
    bool tratarRetornoProrrogacao();
    bool get_servidorEvento(QString &servidor);
};



#endif // ENVEVENTO_H
