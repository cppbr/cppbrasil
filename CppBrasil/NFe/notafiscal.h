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


#ifndef NOTAFISCAL_H
#define NOTAFISCAL_H

#include <memory>
#include <QObject>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/nfe.h>
#include <CppBrasil/NFe/xmlwrite.h>
#include <CppBrasil/NFe/retornonfe.h>
#include <CppBrasil/NFe/wsnfe.h>
#include <CppBrasil/NFe/xmlread.h>

class CPPNFE_EXPORT NotaFiscal : public QObject
{
    Q_OBJECT

public:
    NotaFiscal(ConfigNFe *config);
    ~NotaFiscal();

    void clear();

    bool assinar();
    bool gerar();
    bool validar();
    bool enviar(const int &numLote);
    bool gerarEnviar(const int &numLote);

    bool loadFromFile(const QString &caminhoArquivo);
    bool loadFromString(const QString &xmlString);

    int get_totalErros() const;
    QString get_error() const;

    std::shared_ptr<Container<Nfe>> NFe;
    std::shared_ptr<RetConsReciNFe> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    int m_tError;
    QString m_error;
    ConfigNFe *m_config;
    void tratarRetorno();
    bool importXML(const QByteArray &xml);
    void set_error(const QString &error, const int &tError = 0);

};

#endif // NOTAFISCAL_H
