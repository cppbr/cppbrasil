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


#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <QString>
#include <QCoreApplication>
#include <QNetworkProxy>
#include <QSsl>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppcrypto.h>
#include <CppBrasil/cppopenssl.h>
#include <CppBrasil/cppxml.h>
#include <CppBrasil/cpplibxml2.h>
#include <CppBrasil/convdf.h>
#include <CppBrasil/httpclient.h>


class CPPNFE_EXPORT ConfCertificado
{
public:
    ConfCertificado();
    ~ConfCertificado();
    bool carregarCertificado();
    bool descarregarCertificado();
    void set_cryptoLib(const CryptoType &cryptotype);
    void set_caminhoCertificado(const QString &caminho);
    void set_senhaCertificado(const QString &senha);
    QString get_error() const;
    QString get_numeroSerie() const;
    QString get_issuerName() const;
    QString get_subjectName() const;
    QDateTime get_dataEmissao() const;
    QDateTime get_dataVencimento() const;
    QString get_certificadora() const;
    QString get_razaoSocial() const;
    QString get_cnpj() const;
    void addCaminhoCertificadoAC(const QString &caminho);
    void clearListaCertificadoAC();
protected:
    CppCrypto *get_cryptoLib();
private:
    CppCrypto* m_crypto;
    CryptoType m_cryptotype;
    QString m_caminho;
    QString m_pass;
    QList<QString> m_caList;

};

//---------------------------------------------------

class CPPNFE_EXPORT ConfArquivos
{
public:
    ConfArquivos();
    ~ConfArquivos();
    QString get_caminhoSalvar() const;
    void set_caminhoSalvar(const QString &caminhoSalvar);
    QString get_caminhoSchema() const;
    void set_caminhoSchema(const QString &caminhoSchema);
    bool get_salvar() const;
    void set_salvar(const bool &salvar);
    bool get_salvarLogs() const;
    void set_salvarLogs(const bool &logs);
    bool get_salvarPorAnoMesDia() const;
    void set_salvarPorAnoMesDia(const bool &porAnoMesDia);
    bool get_salvarPorAnoMes() const;
    void set_salvarPorAnoMes(const bool &porAnoMes);
    bool get_salvarPorAno() const;
    void set_salvarPorAno(const bool &porAno);
    bool get_salvarPorMes() const;
    void set_salvarPorMes(const bool &porMes);
    bool get_salvarPorDia() const;
    void set_salvarPorDia(const bool &porDia);
    bool get_salvarPorCNPJ() const;
    void set_salvarPorCNPJ(const bool &porCNPJ);
    bool get_salvarPorModelo() const;
    void set_salvarPorModelo(const bool &porModelo);
private:
    QString m_caminhoSalvar;
    QString m_caminhoSchema;
    bool m_salvar;
    bool m_salvarLogs;
    bool m_porAnoMesDia;
    bool m_porAnoMes;
    bool m_porAno;
    bool m_porMes;
    bool m_porDia;
    bool m_porCNPJ;
    bool m_porModelo;
};

//---------------------------------------------------

class CPPNFE_EXPORT ConfWebServices
{
 public:
    ConfWebServices();
    ~ConfWebServices();
    UF get_uf() const;
    void set_uf(const UF &uf);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    int get_timeoutConexao() const;
    void set_timeoutConexao(const int &timeout); //milisegundos
    int get_timeoutAssincrono() const;
    void set_timeoutAssincrono(const int &timeout); //milisegundos
    int get_tentativas() const;
    void set_tentativas(const int &tentativas);
    QSsl::SslProtocol get_protocoloSSL() const;
    void set_protocoloSSL(const QSsl::SslProtocol &protocol);
    QNetworkProxy get_proxy() const;
    QString get_proxyHostName() const;
    void set_proxyHostName(const QString &proxyHostName);
    int get_proxyPort() const;
    void set_proxyPort(const int &proxyPort);
    QString get_proxyUser() const;
    void set_proxyUser(const QString &proxyUser);
    QString get_proxyPassword() const;
    void set_proxyPassword(const QString &proxyPassword);
    HttpType get_httpType() const;
    void set_httpType(const HttpType &httpType);
    bool get_compactar() const;
    void set_compactar(const bool &compactar);
    int get_compactarAcimaDe() const;
    void set_compactarAcimaDe(const int &compactaracimade);
    bool get_verificarSslSocket() const;
    void set_verificarSslSocket(const bool &verificarSocket);

private:
    UF m_uf;
    TpAmb m_tpAmb;
    int m_connTimeout;
    int m_asynchronousTimeout;
    int m_tentativas;
    QSsl::SslProtocol m_protocol;
    QNetworkProxy m_proxy;
    HttpType m_httpType;
    bool m_compactar;
    int m_compactarAcimaDe;
    bool m_verificarSocket;
};

#endif // CONFIG_H
