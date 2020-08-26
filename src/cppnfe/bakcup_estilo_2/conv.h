#ifndef CONV_H
#define CONV_H

enum class TipoOperacao {Entrada, Saida};

enum class DestinoOperacao {Interna = 1, Interestadual = 2, Exterior = 3};

enum class FormatoImpressao {SemGeracao, Retrato, Paisagem, Simplificado, NFce, MsgEletronica};

enum class TipoEmissao {Normal = 1, Contingencia_FSIA = 2, Contingencia_SCAN = 3, Contingencia_EPEC = 4,
                        Contingencia_FSDA = 5, Contingencia_SVCAN = 6, Contingencia_SVCRS = 7,
                        Contingencia_SVCSP = 8, Contingencia_NFCe = 9};

enum class Ambiente {Producao = 1, Homologacao = 2};

enum class FinalidadeNfe {Normal = 1, Complementar = 2, Ajuste = 3, Devolucao = 4};

enum class ConsumidorFinal {Nao, ConsumidorFinal};

enum class IndicadorPresenca {NaoSeAplica, Presencial, PelaInternet, Teleatendimento, EntregaDomicilio,
                               PresencialForaEstabelecimento, Outros};


enum class ProcessoEmissao {AplicativoContribuinte, AvulsaFisco, AvulsaContribuinte,
                            ContribuinteAplicativoFisco};





#endif // CONV_H
