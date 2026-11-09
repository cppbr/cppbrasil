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


#ifndef CPPDANFEBRREPORT_H
#define CPPDANFEBRREPORT_H

#include <memory>
#include <QString>
#include <QVariantMap>
#include <CppBase/cppbrasil_global.h>
#include <CppDanfe/cppdanfe.h>

class BrReport;
class BrListDataSource;

/*
DANFE da NF-e (modelo 55) impresso com o BrReport, no leiaute retrato da
NT 2026.010 (IBS, CBS e IS).

O leiaute fica embutido na biblioteca. Para personalizar, grave-o com
salvarModelo(), altere o arquivo .brt no brdesigner e informe-o em
caminhoArquivo(). Os dados chegam ao modelo por variáveis do relatório
e pelas fontes de dados "itens", "duplicatas", "volumes" e "locais".
Mantenha os nomes dos objetos "barcodeChave", "barcodeContingencia",
"qrCode", "logo", "infCpl" e "dadosAdicionais", preenchidos pelo código.
*/
class CPPDANFEBRREPORT_EXPORT CppDanfeBrReport: public CppDanfe
{
public:
    CppDanfeBrReport(const CppNFe *cppnfe, const int &recNo = 0);
    virtual ~CppDanfeBrReport();

    //modelo .brt opcional. Vazio (padrão) usa o leiaute embutido.
    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    //abre a janela de visualização.
    virtual void print();

    //o canhoto é facultativo (NT 2026.010, item 4.4). Padrão: exibir.
    void exibirCanhoto(const bool &exibir);
    //envia para a impressora, com ou sem o diálogo de impressão do sistema.
    bool imprimir(const bool &exibirDialogo = true);
    bool exportarPDF(const QString &arquivo);
    //grava o leiaute embutido em um arquivo .brt.
    bool salvarModelo(const QString &arquivo);
    //descrição da falha quando print(), imprimir(), exportarPDF() ou salvarModelo() não concluem.
    QString erro() const;

private:
    Q_DISABLE_COPY(CppDanfeBrReport)

    //as fontes de dados precisam existir enquanto o relatório existir: declaradas antes dele.
    std::unique_ptr<BrListDataSource> m_dsItens;
    std::unique_ptr<BrListDataSource> m_dsDuplicatas;
    std::unique_ptr<BrListDataSource> m_dsVolumes;
    std::unique_ptr<BrListDataSource> m_dsLocais;
    std::unique_ptr<BrReport> m_report;
    bool m_exibirCanhoto = true;
    QString m_erro;

    bool preparar();
    void montarLeiaute(BrReport &report) const;
    void carregarDados();
    QVariantMap variaveis() const;
    void vincular(BrReport &report);
    void ajustarDadosAdicionais(BrReport &report) const;
};

#endif // CPPDANFEBRREPORT_H
