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


#ifndef XMLREAD_H
#define XMLREAD_H

#include <QXmlStreamReader>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/nfe.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT XmlRead
{
public:
    XmlRead(Nfe *NFe, const QByteArray &xml);

    bool import() const;
private:
    Nfe *m_nfe;
    const QByteArray &m_xml;

    bool nfe() const; //TAG raiz da NF-e
    void set_infNFe(QXmlStreamReader &xml) const; //A01 - Grupo A. Dados da Nota Fiscal eletrônica

    void set_ide(QXmlStreamReader &xml) const; //B01 - Grupo B. Identificação da Nota Fiscal eletrônica
    void set_ide_NFref(QXmlStreamReader &xml) const; //BA01 - Grupo BA. Documento Fiscal Referenciado
    void set_ide_NFref_refNF(QXmlStreamReader &xml) const; //BA03 - sub-grupo refNF
    void set_ide_NFref_refNFP(QXmlStreamReader &xml) const; //BA10 - sub-grupo refNFP
    void set_ide_NFref_refECF(QXmlStreamReader &xml) const;//BA20 - sub-grupo refECF

    void set_emite(QXmlStreamReader &xml) const; //C01 - Grupo C. Identificação do Emitente da Nota Fiscal eletrônica
    void set_emite_enderEmit(QXmlStreamReader &xml) const; //C05 - sub-grupo enderEmit

    void set_avulsa(QXmlStreamReader &xml) const; //D01 - Grupo D. Identificação do Fisco Emitente da NF-e

    void set_dest(QXmlStreamReader &xml) const; //E01 - Grupo E. Identificação do Destinatário da Nota Fiscal eletrônica
    void set_dest_enderDest(QXmlStreamReader &xml) const; //E05 - sub-grupo enderDest

    void set_retirada(QXmlStreamReader &xml) const; //F01 - Grupo F. Identificação do Local de Retirada

    void set_entrega(QXmlStreamReader &xml) const; //G01 - Grupo G. Identificação do Local de Entrega

    void set_autXML(QXmlStreamReader &xml) const; //GA01 - Grupo GA. Autorização para obter XML

    void set_det(QXmlStreamReader &xml) const; //H01 - Grupo H. Detalhamento de Produtos e Serviços da NF-e

    void set_prod(QXmlStreamReader &xml) const; //I01 - Grupo I. Produtos e Serviços da NF-e

    void set_DI(QXmlStreamReader &xml) const; //I18 - Grupo I01. Produtos e Serviços / Declaração de Importação
    void set_DI_adi(QXmlStreamReader &xml) const; //I25  - sub-grupo Adições

    void set_detExport(QXmlStreamReader &xml) const; //I50 - Grupo I03. Produtos e Serviços / Grupo de Exportação
    void set_detExport_exportInd(QXmlStreamReader &xml) const; //I52 - sub-grupo exportInd

    void set_rastro(QXmlStreamReader &xml) const; //I80 - Grupo I80. Rastreabilidade de produto

    void set_veicProd(QXmlStreamReader &xml) const; //J01 - Grupo JA. Detalhamento Específico de Veículos novos

    void set_med(QXmlStreamReader &xml) const; //K01 - Grupo K. Detalhamento Específico de Medicamento e de matérias-primas farmacêuticas

    void set_arma(QXmlStreamReader &xml) const; //J01 - Grupo L. Detalhamento Específico de Armamentos

    void set_comb(QXmlStreamReader &xml) const; //LA01 - Grupo LA. Detalhamento Específico de Combustíveis
    void set_comb_CIDE(QXmlStreamReader &xml) const; //LA07 - sub-grupo CIDE
    void set_comb_encerrante(QXmlStreamReader &xml) const; //LA11 - sub-grupo encerrante

    void set_imposto(QXmlStreamReader &xml) const; //M01 - Grupo M. Tributos incidentes no Produto ou Serviço
    void set_ICMS(QXmlStreamReader &xml) const; //N01 - Grupo N01. ICMS Normal e ST
    void set_ICMSUFDest(QXmlStreamReader &xml) const; //NA01 - Grupo NA. ICMS para a UF de destino
    void set_IPI(QXmlStreamReader &xml) const; //O01 - Grupo O. Imposto sobre Produtos Industrializados
    void set_II(QXmlStreamReader &xml) const; //P01 - Grupo P. Imposto de Importação
    void set_PIS(QXmlStreamReader &xml) const; //Q01 - Grupo Q. PIS
    void set_PISST(QXmlStreamReader &xml) const; //R01 - Grupo R. PIS ST
    void set_COFINS(QXmlStreamReader &xml) const; //S01 - Grupo S. COFINS
    void set_COFINSST(QXmlStreamReader &xml) const; //T01 - Grupo S. COFINS ST
    void set_ISSQN(QXmlStreamReader &xml) const; //U01 - Grupo U. ISSQN

    void set_impostoDevol(QXmlStreamReader &xml) const; //UA01 - Grupo UA. Tributos Devolvidos (para o item da NF-e)

    void set_obsItem(QXmlStreamReader &xml) const; //Grupo VA. Observações de uso livre (para o item da NF-e)

    void set_total(QXmlStreamReader &xml) const; //W01 - Grupo W. Total da NF-e
    void set_ICMSTot(QXmlStreamReader &xml) const; //W02 - Grupo Totais referentes ao ICMS
    void set_ISSQNtot(QXmlStreamReader &xml) const; //W17 - Grupo Totais referentes ao ISSQN
    void set_retTrib(QXmlStreamReader &xml) const; //W23 - Grupo W02. Total da NF-e / Retenção de Tributos

    void set_transp(QXmlStreamReader &xml) const; //X01 - Grupo X. Informações do Transporte da NF-e
    void set_transp_transporta(QXmlStreamReader &xml) const; //X03 - sub-grupo Transportador
    void set_transp_retTransp(QXmlStreamReader &xml) const; //X11 - sub-grupo Retenção ICMS transporte
    void set_transp_veicTransp(QXmlStreamReader &xml) const; //X18 - sub-grupo Veículo Transporte
    void set_transp_reboque(QXmlStreamReader &xml) const; //X22 - sub-grupo reboque
    void set_transp_vol(QXmlStreamReader &xml) const; //X26 - sub-grupo volume
    void set_transp_vol_lacres(QXmlStreamReader &xml) const; //X33 - sub-grupo lacres

    void set_cobr(QXmlStreamReader &xml) const; //Y01 - Grupo Y. Dados da Cobrança
    void set_cobr_fat(QXmlStreamReader &xml) const; //Y02 - sub-grupo fatura
    void set_cobr_dup(QXmlStreamReader &xml) const; //Y07 - sub-grupo parcelas

    void set_pag(QXmlStreamReader &xml) const; //YA01 - Grupo YA. Informações de Pagamento
    void set_pag_detPag(QXmlStreamReader &xml) const; //YA02 - sub-grupo Grupo Detalhamento do Pagamento
    void set_pag_detPag_card(QXmlStreamReader &xml) const; //YA04 - sub-grupo Cartões

    void set_infIntermed(QXmlStreamReader &xml) const; //YB01 - Grupo YB. Informações do Intermediador da Transação

    void set_infAdic(QXmlStreamReader &xml) const; //Z01 - Grupo Z. Informações Adicionais da NF-e
    void set_infAdic_obsCont(QXmlStreamReader &xml) const; //Z04 - sub-grupo Campo de uso livre do contribuinte
    void set_infAdic_obsFisco(QXmlStreamReader &xml) const; //Z07 - sub-grupo Campo de uso livre do Fisco
    void set_infAdic_procRef(QXmlStreamReader &xml) const; //Z10 - sub-grupo Processo referenciado

    void set_exporta(QXmlStreamReader &xml) const; //ZA01 - Grupo ZA. Informações de Comércio Exterior

    void set_compra(QXmlStreamReader &xml) const; //ZB01 - Grupo ZB. Informações de Compras

    void set_cana(QXmlStreamReader &xml) const; //ZC01 - Grupo ZC. Informações do Registro de Aquisição de Cana
    void set_cana_forDia(QXmlStreamReader &xml) const; //ZC04 - sub-grupo Fornecimento diário de cana
    void set_cana_deduc(QXmlStreamReader &xml) const; //ZC10 - sub-grupo Deduções – Taxas e Contribuições

    void set_infRespTec(QXmlStreamReader &xml) const; //ZD01 - Grupo ZD. Informações do Responsável Técnico

    void set_infNFeSupl(QXmlStreamReader &xml) const; //Grupo ZX. Informações Suplementares da Nota Fiscal
};

#endif // XMLREAD_H
