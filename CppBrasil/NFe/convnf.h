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


#ifndef CONVNF_H
#define CONVNF_H
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/convdf.h>

//TIPOS NFe e NFCe

//Tipo de serviço utilizados pela projeto NFe/NFCe
enum class WebServicesNF{None = -1, NFeInutilizacao, NFeConsultaProtocolo,  NFeStatusServico,
                         NFeConsultaCadastro, NFeRecepcaoEvento, NFeAutorizacao, NFeRetAutorizacao,
                         NFeDistribuicaoDFe};

//versao NFe/NFCe
enum class VersaoNF{None = -1, v400};

//versao QRCode NFe/NFCe
enum class VersaoQrCodeNF{None = -1, v200};

//indSinc - indentificador Sincrono
enum class IndSinc {Nao = 0, Sim = 1};

//tpNF - Tipo de Operação
enum class TpNF {None = -1, Entrada = 0, Saida = 1};
//idDest - Identificador de local de destino da operação
enum class IdDest {None = -1, Interna = 1, Interestadual = 2, Exterior = 3};
//tpImp - Formato de Impressão do DANFE
enum class TpImp {None = -1, SemGeracao = 0, Retrato = 1, Paisagem = 2,
                             Simplificado = 3, NFCe = 4, MsgEletronica = 5};
//tpEmis - Tipo de Emissão da NF-e
enum class TpEmis {None = -1, Normal = 1, Contingencia_FSIA = 2,
                   RegimeEspecialNFF_NaoUsarEste = 3, //Antes Contingência SCAN desativado na NT 2015/002. Agora Regime Especial NFF (NT 2021.002)
                   Contingencia_EPEC = 4, Contingencia_FSDA = 5, Contingencia_SVCAN = 6,
                   Contingencia_SVCRS = 7, Contingencia_SVCSP = 8, Contingencia_NFCe = 9};
//finNFe - Finalidade de emissão da NF-e
enum class FinNFe {None = -1, Normal = 1, Complementar = 2, Ajuste = 3, Devolucao = 4};
//indFinal - Indica operação com Consumidor final
enum class IndFinal {None = -1, Normal = 0, ConsumidorFinal = 1};
//indPres - Indicador de presença do comprador no estabelecimento comercial no momento da operação
enum class IndPres {None = -1, NaoSeAplica = 0, Presencial = 1, PelaInternet = 2,
                   Teleatendimento = 3, EntregaDomicilio = 4,
                   PresencialForaEstabelecimento = 5, Outros = 9};
//indIntermed - Indicador de intermediador/marketplace
enum class IndIntermed {None = -1, SemIntermediador = 0, SitePlataformaTerceiros = 1};

//procEmi - Processo de emissão da NF-e
enum class ProcEmi {None = -1, AplicativoContribuinte = 0, AvulsaFisco = 1,
                   AvulsaContribuinte = 2, ContribuinteAplicativoFisco = 3};

//CRT - Código de Regime Tributário
enum class Crt {None = -1, SimplesNacional = 1, SimplesExcesso = 2, RegimeNormal = 3};

//indIEDest - Indicador da IE do Destinatário
enum class IndIEDest {None = -1, ContribuinteICMS = 1, ContribuinteIsento = 2, NaoContribuinte = 9};

//indTot - Indica se valor do Item (vProd) entra no valor total da NF-e (vProd)
enum class IndTot {None = -1, NaoCompoeTotalNF = 0, CompoeTotalNF = 1};

//indEscala - Indicador de Escala Relevante - S - Produzido em Escala Relevante; N – Produzido em Escala NÃO Relevante
enum class IndEscala {None = -1, EscalaRelevante, EscalaNaoRelevante};

//tpViaTransp - Via de transporte internacional informada na Declaração de Importação (DI)
enum class TpViaTransp{None = -1, Maritima = 1, Fluvial = 2, Lacustre = 3, Aerea = 4, Postal = 5,
                             Ferroviaria = 6, Rodoviaria = 7, CondutoRedeTransmissao = 8, MeiosProprios = 9,
                             EntradaSaidaFicta = 10, Courier = 11, EmMaos = 12, PorReboque = 13};

//tpIntermedio - Forma de importação quanto a intermediação
enum class TpIntermedio {None = -1, ContaPropria = 1, ContaOrdem = 2, Encomenda = 3};

//tpOp - Tipo da operação Veículos
enum class TpOp {None = -1, Outros = 0, VendaConcessionaria = 1, FaturamentoConsumidorFinal = 2,
                VendaGrandesConsumidores = 3};

//condVeic - Condição do Veículo
enum class CondVeic {None = -1, Acabado = 1, Inacabado = 2, Semiacabado = 3};

//tpRest - Restrição do Veículo
enum class TpRest {None = -1, Naoha = 0, AlienacaoFiduciaria = 1, ArrendamentoMercantil = 2,
                             ReservaDominio = 3, PenhorVeiculos = 4, Outras = 9};

//tpArma - Indicador do tipo de arma de fogo
enum class TpArma {None = -1, UsoPermitido = 0, UsoRestrito = 1};

//orig - Origem da mercadoria
enum class Orig {None = -1, Nacional = 0, EstrangeiraImportacaoDireta = 1,
  EstrangeiraAdquiridaMercadoInterno = 2, NacionalImportacaoSuperior40InferiorIgual70 = 3,
  NacionalProcessosProdutivosBasicos = 4, NacionalImportacaoInferiorIgual40 = 5,
  EstrangeiraImportacaoDiretaSemSimilarNacional = 6,
  EstrangeiraAdquiridaMercadoInternoSemSimilarNacional = 7, NacionalImportacaoSuperior70 = 8};

//cst Tributação do ICMS
enum class CstICMS{None = -1, ICMS00, ICMS02, ICMS10, ICMS15, ICMS20, ICMS30, ICMS40, ICMS41, ICMS50, ICMS51,
                   ICMS53, ICMS60, ICMS61, ICMS70, ICMS90, ICMSPart10, ICMSPart90, ICMSST41, ICMSST60};

//csosn CRT=1 – Simples Nacional
enum class CsosnICMS{None = -1, ICMSSN101 = 101, ICMSSN102 = 102, ICMSSN103 = 103, ICMSSN201 = 201,
                     ICMSSN202 = 202, ICMSSN203 = 203, ICMSSN300 = 300, ICMSSN400  = 400,
                     ICMSSN500 = 500, ICMSSN900 = 900};

//modBC - Modalidade de determinação da BC do ICMS
enum class ModBC{None = -1, MargemValorAgregado = 0, Pauta = 1, PrecoTabeladoMax = 2,
                 ValorOperacao = 3};

//modBCST - Modalidade de determinação da BC do ICMS ST
enum class ModBCST {None = -1, PrecoTabelado = 0, ListaNegativa = 1, ListaPositiva = 2,
                    ListaNeutra = 3, MargemValorAgregado = 4, Pauta = 5, ValorOperacao = 6};

//motDesICMS - Motivo da desoneração do ICMS
enum class MotDesICMS {None = -1, Taxi = 1, ProdutorAgropecuario = 3, FrotistaLocadora = 4,
                       DiplomaticoConsular = 5, UtilMotocicletasAmazoniaLivreComercio = 6,
                       Suframa = 7, VendaOrgaoPublico = 8, Outros = 9, DeficienteCondutor = 10,
                       DeficienteNaoCondutor = 11, OrgaoFomento = 12, OlimpiadasRio2016 = 16,
                       SolicitadoPeloFisco = 90};

//cst - Código da situação tributária do IPI
enum class CstIPI {None = -1, IPI00 = 0, IPI01 = 1, IPI02 = 2, IPI03 = 3, IPI04 = 4, IPI05 = 5,
                   IPI49 = 49, IPI50 = 50, IPI51 = 51, IPI52 = 52, IPI53 = 53, IPI54 = 54,
                   IPI55 = 55, IPI99 = 99};

//CST - Código de Situação Tributária do PIS
enum class CstPIS {None = -1, PIS01 = 1, PIS02 = 2, PIS03 = 3, PIS04 = 4, PIS05 = 5, PIS06 = 6,
                   PIS07 = 7, PIS08 = 8, PIS09 = 9, PIS49 = 49, PIS50 = 50, PIS51 = 51, PIS52 = 52,
                   PIS53 = 53, PIS54 = 54, PIS55 = 55, PIS56 = 56, PIS60 = 60, PIS61 = 61, PIS62 = 62,
                   PIS63 = 63, PIS64 = 64, PIS65 = 65, PIS66 = 66, PIS67 = 67, PIS70 = 70, PIS71 = 71,
                   PIS72 = 72, PIS73 = 73, PIS74 = 74, PIS75 = 75, PIS98 = 98, PIS99 = 99};

//CST - Código de Situação Tributária do COFINS
enum class CstCOFINS {None = -1, COFINS01 = 1, COFINS02 = 2, COFINS03 = 3, COFINS04 = 4,
                      COFINS05 = 5, COFINS06 = 6, COFINS07 = 7, COFINS08 = 8, COFINS09 = 9,
                      COFINS49 = 49, COFINS50 = 50, COFINS51 = 51, COFINS52 = 52, COFINS53 = 53,
                      COFINS54 = 54, COFINS55 = 55, COFINS56 = 56, COFINS60 = 60, COFINS61 = 61,
                      COFINS62 = 62, COFINS63 = 63, COFINS64 = 64, COFINS65 = 65, COFINS66 = 66,
                      COFINS67 = 67, COFINS70 = 70, COFINS71 = 71, COFINS72 = 72, COFINS73 = 73,
                      COFINS74 = 74, COFINS75 = 75, COFINS98 = 98, COFINS99 = 99};

//indISS - Indicador da exigibilidade do ISS
enum class IndISS {None = -1, Exigivel = 1, NaoIncidencia = 2, Isencao = 3, Exportacao = 4,
                   Imunidade = 5, ExigibilidadeSuspensaDecisaoJudicial = 6,
                   ExigibilidadeSuspensaProcessoAdministrativo = 7};

//indIncentivo - Indicador de incentivo Fiscal
enum class IndIncentivo {None = -1, Sim = 1, Nao = 2};

//cRegTrib - Código do Regime Especial de Tributação
enum class  CRegTrib {None = -1, MicroempresaMunicipal = 1, Estimativa = 2,
                     SociedadeProfissionais = 3, Cooperativa = 4, MEI = 5, MEPP = 6};

//modFrete - Modalidade do frete
enum class ModFrete {None = -1, PorContaRemetente = 0, PorContaDestinatario = 1,
                           PorContaTerceiros = 2, ProprioContaRemetente = 3,
                           ProprioContaDestinatario = 4, SemFrete = 9};

//indPag - Indicador da Forma de Pagamento
enum class IndPag {None = -1, aVista = 1, aPrazo = 2};

//tPag - Meio de pagamento
enum class TPag {None = -1, Dinheiro = 1, Cheque = 2, CartaoCredito = 3,
                CartaoDebito = 4, CreditoLoja = 5, ValeAlimentacao = 10,
                ValeRefeicao = 11, ValePresente = 12, ValeCombustivel = 13,
                DuplicataMercanti = 14, BoletoBancario = 15, DepositoBancario = 16,
                Pix = 17, TranferenciaBancaria = 18, ProgramaFidelidade = 19,
                SemPagamento = 90, Outros = 99};

//tpIntegra - Tipo de Integração para pagamento
enum class TpIntegra {None = -1, PagamentoIntegrado = 1, PagamentoNaoIntegrado = 2};

//tBand - Bandeira da operadora de cartão de crédito e/ou débito
enum class TBand {None = -1, Visa = 1, Mastercard = 2, AmericanExpress = 3,
                  Sorocred = 4, DinersClub = 5, Elo = 6, Hipercard = 7, Aura = 8,
                  Cabal = 9, Alelo = 10, BanesCard = 11, CalCard = 12, Credz = 13,
                  Discover = 14, GoodCard = 15, GreenCard = 16, Hiper = 17, JcB = 18,
                  Mais = 19, MaxVan = 20, Policard = 21, RedeCompras = 22, Sodexo = 23,
                  ValeCard = 24, Verocheque = 25, VR = 26, Ticket = 27, Outros = 99};

//indProc - Indicador da origem do processo
enum class IndProc {None = -1, SEFAZ = 0, JusticaFederal = 1,
                    JusticaEstadual = 2, SecexRFB = 3, Outros = 9};

//tpAto - Tipo do ato concessório
enum class TpAto {None = -1, TermoAcordo=8, RegimeEspecial=10, AutorizacaoEspecifica=12 };

//tpEvento - Eventos Registrados pelo Emitente / Eventos Registrados pelo Destinatário
enum class TpEvento {None = -1, CartaCorrecao = 110110, Cancelamento = 110111,
                     CancelamentoSubstituicao = 110112, ComprovanteEntregaNFe = 110130,
                     CancelComprovanteEntregaNFe = 110131, EPEC = 110140,
                     PedidoProrrogacao1 = 111500, PedidoProrrogacao2 = 111501,
                     CancelPedidoProrrogacao1 = 111502, CancelPedidoProrrogacao2 = 111503,
                     AtorInteressadoNFeTransp = 110150, ConfirmacaoOperacaoDest = 210200,
                     CienciaOperacaoDest = 210210,  DesconhecimentoOperacaoDest = 210220,
                     OperacaoNaoRealizadaDest = 210240};

//tpAutor - Tipo de autor do evento
//obs: 3-EmpresaTerceiros, serve para terceiros como no caso de evento Ator Interessado. 3=Empresa Transportadora.
enum class TpAutor {None = -1, EmpresaEmitente = 1, EmpresaDestinataria = 2, EmpresaTerceiros = 3,
                    Fisco = 5, RFB = 6, OutrosOrgaos = 9};

//tpAutorizacao - tipo de autorização para o transportador
enum class TpAutorizacao {None = -1, NaoPermite = 0, PermiteTranspAutTransp = 1};
//indImport - Indicador de importação
enum class IndImport {None = -1, Nacional = 0, Importado = 1};
//motRedAdRem - Motivo da redução do adrem
enum class MotRedAdRem {None = -1, TransporteColetivoPassageiros = 1, Outros = 9};

class CPPNFE_EXPORT ConvNF : public ConvDF
{
public:
    ConvNF();

    //CONVERSÕES NFe-NFCe
    static QString webServicesNFToMetodo(const WebServicesNF &value);
    static QString versaoLayout(const WebServicesNF &value);
    static QString versaoNFToStr(const VersaoNF &value);
    static VersaoNF strToVersaoNF(const QString &value);
    static QString versaoQrCodeNFToStr(const VersaoQrCodeNF &value);
    static VersaoQrCodeNF strToVersaoQrCodeNF(const QString &value);
    static QString indSincToStr(const IndSinc &value);
    static IndSinc strToIndSinc(const QString &value);
    static QString tpNFToStr(const TpNF &value);
    static TpNF strToTpNF(const QString &value);
    static QString idDestToStr(const IdDest &value);
    static IdDest strToIdDest(const QString &value);
    static QString tpImpToStr(const TpImp &value);
    static TpImp strToTpImp(const QString &value);
    static QString tpEmisToStr(const TpEmis &value);
    static TpEmis strToTpEmis(const QString &value);
    static QString finNFeToStr(const FinNFe &value);
    static FinNFe strToFinNFe(const QString &value);
    static QString indFinalToStr(const IndFinal &value);
    static IndFinal strToIndFinal(const QString &value);
    static QString indPresToStr(const IndPres &value);
    static IndPres strToIndPres(const QString &value);
    static QString indIntermedToStr(const IndIntermed &value);
    static IndIntermed strToIndIntermed(const QString &value);
    static QString procEmiToStr(const ProcEmi &value);
    static ProcEmi strToProcEmi(const QString &value);
    static QString crtToStr(const Crt &value);
    static Crt strToCrt(const QString &value);
    static QString indIEDestToStr(const IndIEDest &value);
    static IndIEDest strToIndIEDest(const QString &value);
    static QString indTotToStr(const IndTot &value);
    static IndTot strToIndTot(const QString &value);
    static QString indEscalaToStr(const IndEscala &value);
    static IndEscala strToIndEscala(const QString &value);
    static QString tpViaTranspToStr(const TpViaTransp &value);
    static TpViaTransp strToTpViaTransp(const QString &value);
    static QString tpIntermedioToStr(const TpIntermedio &value);
    static TpIntermedio strToTpIntermedio(const QString &value);
    static QString tpOpToStr(const TpOp &value);
    static TpOp strToTpOp(const QString &value);
    static QString condVeicToStr(const CondVeic &value);
    static CondVeic strToCondVeic(const QString &value);
    static QString tpRestToStr(const TpRest &value);
    static TpRest strToTpRest(const QString &value);
    static QString tpArmaToStr(const TpArma &value);
    static TpArma strToTpArma(const QString &value);
    static QString origToStr(const Orig &value);
    static Orig strToOrig(const QString &value);
    static QString cstICMSToStr(const CstICMS &value);
    static CstICMS strToCstICMS(const QString &value);
    static QString csosnICMSToStr(const CsosnICMS &value);
    static CsosnICMS strToCsosnICMS(const QString &value);
    static QString modBCToStr(const ModBC &value);
    static ModBC strToModBC (const QString &value);
    static QString modBCSTToStr(const ModBCST &value);
    static ModBCST strToModBCST(const QString &value);
    static QString motDesICMSToStr(const MotDesICMS &value);
    static MotDesICMS strToMotDesICMS(const QString &value);
    static QString cstIPIToStr(const CstIPI &value);
    static CstIPI strToCstIPI(const QString &value);
    static QString cstPISToStr(const CstPIS &value);
    static CstPIS strToCstPIS(const QString &value);
    static QString cstCOFINSToStr(const CstCOFINS &value);
    static CstCOFINS strToCstCOFINS(const QString &value);
    static QString indISSToStr(const IndISS &value);
    static IndISS strToIndISS(const QString &value);
    static QString indIncentivoToStr(const IndIncentivo &value);
    static IndIncentivo strToIndIncentivo(const QString &value);
    static QString cRegTribToStr(const CRegTrib &value);
    static CRegTrib strToCRegTrib(const QString &value);
    static QString modFreteToStr(const ModFrete &value);
    static ModFrete strToModFrete(const QString &value);
    static QString indPagToStr(const IndPag &value);
    static IndPag strToIndPag(const QString &value);
    static QString tPagToStr(const TPag &value);
    static TPag strToTPag(const QString &value);
    static QString tpIntegraToStr(const TpIntegra &value);
    static TpIntegra strToTpIntegra(const QString &value);
    static QString tBandToStr(const TBand &value);
    static TBand strToTBand(const QString &value);
    static QString indProcToStr(const IndProc &value);
    static IndProc strToIndProc(const QString &value);
    static QString tpAtoToStr(const TpAto &value);
    static TpAto strToTpAto(const QString &value);
    static QString tpEventoToStr(const TpEvento &value);
    static TpEvento strToTpEvento(const QString &value);
    static QString tpAutorToStr(const TpAutor &value);
    static TpAutor strToTpAutor(const QString &value);
    static QString tpAutorizacaoToStr(const TpAutorizacao &value);
    static TpAutorizacao strToTpAutorizacao(const QString &value);
    static QString indImportToStr(const IndImport &value);
    static IndImport strToIndImport(const QString &value);
    static QString motRedAdRemToStr(const MotRedAdRem &value);
    static MotRedAdRem strToMotRedAdRem(const QString &value);
};

#endif // CONVNF_H
