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


#ifndef CONFIGNFE_H
#define CONFIGNFE_H

#include <QString>
#include <QDir>
#include <memory>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/config.h>
#include <CppBrasil/cppxml.h>


class CPPNFE_EXPORT Certificado: public ConfCertificado
{
    friend class Nfe;
    friend class NotaFiscal;
    friend class StatusServico;
    friend class EnvEvento;
    friend class TEvento;
    friend class ConsSitNFe;
public:
     Certificado();
     ~Certificado();
};

//-----------------------------------------------------

class CPPNFE_EXPORT ArquivosNF: public ConfArquivos
{
public:
    ArquivosNF(Certificado *certificado);
    ~ArquivosNF();
    QString get_caminhoSalvarEvento() const;
    void set_caminhoSalvarEvento(const QString &caminhoSalvarEvento);
    QString get_caminhoSalvarConsulta() const;
    void set_caminhoSalvarConsulta(const QString &caminhoSalvarConsulta);

    QString get_caminhoNF() const;
    QString get_caminhoLogs() const;
    QString get_caminhoEvento() const;
    QString get_caminhoConsulta() const;
    void set_ModeloDF(const ModeloDF &modeloDF);

private:
    bool m_salvarEventosPorDatas;
    QString m_caminhoSalvarEvento;
    QString m_caminhoSalvarConsulta;
    Certificado *certificado;
    ModeloDF m_ModeloDF; //para salvar por modelo
    QString get_caminho(const QString &pastaBase, const bool &porCNPJ, const bool &porModelo,
                        const bool &porAnoMesDia, const bool &porAnoMes, const bool &porAno,
                        const bool &porMes, const bool &porDia) const;
};

//-----------------------------------------------------

class CPPNFE_EXPORT ConfigNFe
{
    friend class Nfe;
    friend class NotaFiscal;
    friend class WSNFeBase;
    friend class ProtNFe;
    friend class EnvEvento;
    friend class TEvento;
    friend class ConsSitNFe;
public:
    ConfigNFe();
    ~ConfigNFe();
    ModeloDF get_ModeloDF() const;
    void set_ModeloDF(const ModeloDF &modeloDF);
    VersaoNF get_VersaoNF() const;
    void set_VersaoNF(const VersaoNF &versaoNF);
    QString get_IdCSC() const;
    void set_IdCSC(const QString &idCSC);
    QString get_CSC() const;
    void set_CSC(const QString &CSC);
    VersaoQrCodeNF get_VersaoQrCodeNF() const;
    void set_VersaoQrCodeNF(const VersaoQrCodeNF &versaoQRCodeNF);
    IndSinc get_indicadorSincrono() const;
    void set_indicadorSincrono(const IndSinc &indSinc);
    TpEmis get_TipoEmissao() const;
    void set_TipoEmissao(const TpEmis &tipoEmissao);


    std::shared_ptr<Certificado> certificado;
    std::shared_ptr<ArquivosNF> arquivos;
    std::shared_ptr<ConfWebServices> webservices;

private:
    ModeloDF m_ModeloDF;
    VersaoNF m_VersaoNF;
    QString m_IdCSC;
    QString m_CSC;
    VersaoQrCodeNF m_VersaoQRCodeNF;
    IndSinc m_indSinc;
    TpEmis m_tipoEmissao;
};

#endif // CONFIGNFE_H
