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


#include "config.h"

ConfCertificado::ConfCertificado():
   m_crypto(NULL), m_cryptotype(CryptoType::None)
{
}

ConfCertificado::~ConfCertificado()
{
    if (this->m_crypto != NULL)
        delete m_crypto;
}

bool ConfCertificado::carregarCertificado()
{

    if ((this->m_cryptotype == CryptoType::None) || (this->m_pass.isEmpty()) ||
        (this->m_caminho.isEmpty()))
          return false;

    //at the moment only openssl
    if (this->m_cryptotype == CryptoType::OpenSSL)
      this->m_crypto = new CppOpenSSL(this->m_caminho.toLocal8Bit(), this->m_pass.toLocal8Bit());

    int _ret = this->m_crypto->load();
    //add CA after load crypto
    if (this->m_caList.count() > 0)
    {
        foreach (const QString &item, this->m_caList)
        {
           this->m_crypto->insertCAcert(item);
        }
    }

    return _ret;

}

bool ConfCertificado::descarregarCertificado()
{
    return this->m_crypto->unload();
}

void ConfCertificado::set_cryptoLib(const CryptoType &cryptotype)
{
    this->m_cryptotype = cryptotype;
}

void ConfCertificado::set_caminhoCertificado(const QString &caminho)
{
    this->m_caminho = caminho;
}

void ConfCertificado::set_senhaCertificado(const QString &senha)
{
    this->m_pass = senha;
}

QString ConfCertificado::get_error() const
{
    return this->m_crypto->get_error();
}

QString ConfCertificado::get_numeroSerie() const
{
    return this->m_crypto->get_numeroSerie();
}

QString ConfCertificado::get_issuerName() const
{
    return this->m_crypto->get_issuerName();
}

QString ConfCertificado::get_subjectName() const
{
    return this->m_crypto->get_subjectName();
}

QDateTime ConfCertificado::get_dataEmissao() const
{
    return this->m_crypto->get_dataEmissao();
}

QDateTime ConfCertificado::get_dataVencimento() const
{
    return this->m_crypto->get_dataVencimento();
}

QString ConfCertificado::get_certificadora() const
{
    return this->m_crypto->get_certificadora();
}

QString ConfCertificado::get_razaoSocial() const
{
    return this->m_crypto->get_razaoSocial();
}

QString ConfCertificado::get_cnpj() const
{
    return this->m_crypto->get_cnpj();
}

void ConfCertificado::addCaminhoCertificadoAC(const QString &caminho)
{
    this->m_caList.append(caminho);
}

void ConfCertificado::clearListaCertificadoAC()
{
    this->m_caList.clear();
}

CppCrypto *ConfCertificado::get_cryptoLib()
{
    return this->m_crypto;
}

//-------------------------------------------------------------

ConfArquivos::ConfArquivos(): m_salvar(false), m_salvarLogs(false), m_porAnoMesDia(false), m_porAnoMes(false),
    m_porAno(false), m_porMes(false), m_porDia(false), m_porCNPJ(false), m_porModelo(false)
{
}

ConfArquivos::~ConfArquivos()
{
}

QString ConfArquivos::get_caminhoSalvar() const
{
    if (this->m_caminhoSalvar.isNull())
    {
        return QCoreApplication::applicationDirPath();
    }else
    {
        return this->m_caminhoSalvar;
    }

}

void ConfArquivos::set_caminhoSalvar(const QString &caminhoSalvar)
{
    this->m_caminhoSalvar = caminhoSalvar;
}

QString ConfArquivos::get_caminhoSchema() const
{
    return this->m_caminhoSchema;
}

void ConfArquivos::set_caminhoSchema(const QString &caminhoSchema)
{
    this->m_caminhoSchema = caminhoSchema;
}

bool ConfArquivos::get_salvar() const
{
    return this->m_salvar;
}

void ConfArquivos::set_salvar(const bool &salvar)
{
    this->m_salvar = salvar;
}

bool ConfArquivos::get_salvarLogs() const
{
    return this->m_salvarLogs;
}

void ConfArquivos::set_salvarLogs(const bool &logs)
{
    this->m_salvarLogs = logs;
}

bool ConfArquivos::get_salvarPorAnoMesDia() const
{
    return this->m_porAnoMesDia;
}

void ConfArquivos::set_salvarPorAnoMesDia(const bool &porAnoMesDia)
{
    this->m_porAnoMesDia = porAnoMesDia;
}

bool ConfArquivos::get_salvarPorAnoMes() const
{
    return this->m_porAnoMes;
}

void ConfArquivos::set_salvarPorAnoMes(const bool &porAnoMes)
{
    this->m_porAnoMes = porAnoMes;
}

bool ConfArquivos::get_salvarPorAno() const
{
    return this->m_porAno;
}

void ConfArquivos::set_salvarPorAno(const bool &porAno)
{
    this->m_porAno = porAno;
}

bool ConfArquivos::get_salvarPorMes() const
{
    return this->m_porMes;
}

void ConfArquivos::set_salvarPorMes(const bool &porMes)
{
    this->m_porMes = porMes;
}

bool ConfArquivos::get_salvarPorDia() const
{
    return this->m_porDia;
}

void ConfArquivos::set_salvarPorDia(const bool &porDia)
{
    this->m_porDia = porDia;
}

bool ConfArquivos::get_salvarPorCNPJ() const
{
    return this->m_porCNPJ;
}

void ConfArquivos::set_salvarPorCNPJ(const bool &porCNPJ)
{
    this->m_porCNPJ = porCNPJ;
}

bool ConfArquivos::get_salvarPorModelo() const
{
    return this->m_porModelo;
}

void ConfArquivos::set_salvarPorModelo(const bool &porModelo)
{
    this->m_porModelo = porModelo;
}

//-------------------------------------------------------------

ConfWebServices::ConfWebServices() : m_tpAmb(TpAmb::None),  m_connTimeout(0),
    m_asynchronousTimeout(0), m_tentativas(0), m_httpType(HttpType::None), m_compactar(false),
    m_compactarAcimaDe(0), m_verificarSocket(true)
{
}

ConfWebServices::~ConfWebServices()
{

}

UF ConfWebServices::get_uf() const
{
    return this->m_uf;
}

void ConfWebServices::set_uf(const UF &uf)
{
    this->m_uf = uf;
}

TpAmb ConfWebServices::get_tpAmb() const
{
    return this->m_tpAmb;
}

void ConfWebServices::set_tpAmb(const TpAmb &ambiente)
{
    this->m_tpAmb = ambiente;
}

int ConfWebServices::get_timeoutConexao() const
{
    return this->m_connTimeout;
}

void ConfWebServices::set_timeoutConexao(const int &timeout)
{
    this->m_connTimeout = timeout;
}

int ConfWebServices::get_timeoutAssincrono() const
{
    return this->m_asynchronousTimeout;
}

void ConfWebServices::set_timeoutAssincrono(const int &timeout)
{
    this->m_asynchronousTimeout = timeout;
}

int ConfWebServices::get_tentativas() const
{
    return this->m_tentativas;
}

void ConfWebServices::set_tentativas(const int &tentativas)
{
    this->m_tentativas = tentativas;
}

QSsl::SslProtocol ConfWebServices::get_protocoloSSL() const
{
    return this->m_protocol;
}

void ConfWebServices::set_protocoloSSL(const QSsl::SslProtocol &protocol)
{
    this->m_protocol = protocol;
}

QNetworkProxy ConfWebServices::get_proxy() const
{
    return this->m_proxy;
}

QString ConfWebServices::get_proxyHostName() const
{
    return this->m_proxy.hostName();
}

void ConfWebServices::set_proxyHostName(const QString &proxyHostName)
{
    this->m_proxy.setHostName(proxyHostName);
}

int ConfWebServices::get_proxyPort() const
{
    return this->m_proxy.port();
}

void ConfWebServices::set_proxyPort(const int &proxyPort)
{
    this->m_proxy.setPort(proxyPort);
}

QString ConfWebServices::get_proxyUser() const
{
    return this->m_proxy.user();
}

void ConfWebServices::set_proxyUser(const QString &proxyUser)
{
    this->m_proxy.setUser(proxyUser);
}

QString ConfWebServices::get_proxyPassword() const
{
    return this->m_proxy.password();
}

void ConfWebServices::set_proxyPassword(const QString &proxyPassword)
{
    this->m_proxy.setPassword(proxyPassword);
}

HttpType ConfWebServices::get_httpType() const
{
    return this->m_httpType;
}

void ConfWebServices::set_httpType(const HttpType &httpType)
{
    this->m_httpType = httpType;
}

bool ConfWebServices::get_compactar() const
{
    return this->m_compactar;
}

void ConfWebServices::set_compactar(const bool &compactar)
{
    this->m_compactar = compactar;
}

int ConfWebServices::get_compactarAcimaDe() const
{
    return this->m_compactarAcimaDe;
}

void ConfWebServices::set_compactarAcimaDe(const int &compactaracimade)
{
    this->m_compactarAcimaDe = compactaracimade;
}

bool ConfWebServices::get_verificarSslSocket() const
{
    return this->m_verificarSocket;
}

void ConfWebServices::set_verificarSslSocket(const bool &verificarSocket)
{
    this->m_verificarSocket = verificarSocket;
}
