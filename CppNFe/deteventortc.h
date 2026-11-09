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

#ifndef DETEVENTORTC_H
#define DETEVENTORTC_H

#include <memory>
#include <QDate>
#include <CppBase/container.h>
#include <CppNFe/convnf.h>

class CPPNFE_EXPORT EstoqueEventoRTC
{
public:
    void clear();
    double get_quantidade() const;
    void set_quantidade(double valor);
    QString get_unidade() const;
    void set_unidade(const QString &valor);
    double get_vIBSEstorno() const;
    void set_vIBSEstorno(double valor);
    double get_vCBSEstorno() const;
    void set_vCBSEstorno(double valor);

private:
    double m_quantidade = 0.0;
    QString m_unidade;
    double m_vIBSEstorno = 0.0;
    double m_vCBSEstorno = 0.0;
};

class CPPNFE_EXPORT ItemEventoRTC
{
public:
    ItemEventoRTC();
    void clear();
    int get_nItem() const;
    void set_nItem(int valor);
    double get_vIBS() const;
    void set_vIBS(double valor);
    double get_vCBS() const;
    void set_vCBS(double valor);

    std::unique_ptr<EstoqueEventoRTC> gControleEstoque;

private:
    int m_nItem = 0;
    double m_vIBS = 0.0;
    double m_vCBS = 0.0;
};

class CPPNFE_EXPORT CreditoPresumidoEvento
{
public:
    void clear();
    double get_pCredPres() const;
    void set_pCredPres(double valor);
    double get_vCredPres() const;
    void set_vCredPres(double valor);

private:
    double m_pCredPres = 0.0;
    double m_vCredPres = 0.0;
};

class CPPNFE_EXPORT ItemCreditoPresumidoEvento
{
public:
    void clear();
    int get_nItem() const;
    void set_nItem(int valor);
    double get_vBCCredPres() const;
    void set_vBCCredPres(double valor);
    QString get_cCredPres() const;
    void set_cCredPres(const QString &valor);

    std::unique_ptr<CreditoPresumidoEvento> gIBSCredPres;
    std::unique_ptr<CreditoPresumidoEvento> gCBSCredPres;

private:
    int m_nItem = 0;
    double m_vBCCredPres = 0.0;
    QString m_cCredPres;
};

class CPPNFE_EXPORT PagamentoVinculadoEvento
{
public:
    void clear();
    QString get_idTransacao() const;
    void set_idTransacao(const QString &valor);
    QString get_tpMeioPgto() const;
    void set_tpMeioPgto(const QString &valor);
    QString get_CNPJReceb() const;
    void set_CNPJReceb(const QString &valor);
    QString get_CNPJBasePSP() const;
    void set_CNPJBasePSP(const QString &valor);

private:
    QString m_idTransacao;
    QString m_tpMeioPgto;
    QString m_CNPJReceb;
    QString m_CNPJBasePSP;
};

class CPPNFE_EXPORT DetEventoRTC
{
public:
    DetEventoRTC();
    void clear();
    int get_indAceitacao() const;
    void set_indAceitacao(int valor);
    QDate get_dPrevEntrega() const;
    void set_dPrevEntrega(const QDate &valor);
    TpEvento get_tpEventoAut() const;
    void set_tpEventoAut(TpEvento valor);

    // Para o evento 211150, vIBS e vCBS dos itens são serializados como vCredIBS e vCredCBS.
    // Os demais eventos com itens de estoque também usam gControleEstoque.
    std::unique_ptr<Container<ItemEventoRTC>> itens;
    std::unique_ptr<Container<ItemCreditoPresumidoEvento>> gCredPresOper;
    std::unique_ptr<PagamentoVinculadoEvento> gPgto;

private:
    int m_indAceitacao = -1;
    QDate m_dPrevEntrega;
    TpEvento m_tpEventoAut = TpEvento::None;
};

#endif // DETEVENTORTC_H
