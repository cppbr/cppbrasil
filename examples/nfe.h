#ifndef EXEMPLOS_NFE_H
#define EXEMPLOS_NFE_H

#include <QSettings>
#include <QTextStream>
#include <CppNFe/CppNFe>

namespace ExemploNFe {

// Edite os dados da operação aqui e recompile os exemplos.
namespace DadosNota {
struct Item
{
    const char *codigo;
    const char *descricao;
    const char *ncm;
    double quantidade;
    double valorUnitario;
};

inline constexpr int serie = 1;
inline constexpr int lote = 1;
inline constexpr char naturezaOperacao[] = "VENDA DE MERCADORIA";
inline constexpr TpNF tipoOperacao = TpNF::Saida;
inline constexpr IdDest localOperacao = IdDest::Interna;
inline constexpr FinNFe finalidade = FinNFe::Normal;
inline constexpr IndFinal consumidorFinal = IndFinal::ConsumidorFinal;
inline constexpr IndPres presenca = IndPres::Presencial;
inline constexpr IndIntermed intermediador = IndIntermed::SemIntermediador;
inline constexpr ProcEmi processoEmissao = ProcEmi::AplicativoContribuinte;
inline constexpr char versaoAplicativo[] = "CppBrasil 1.0";
inline constexpr char justificativaContingencia[] = "Emissao de teste em contingencia";
inline constexpr char destinatarioCpf[] = "12345678909";
inline constexpr char destinatarioNome[] =
    "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL";
inline constexpr char destinatarioLogradouro[] = "RUA B";
inline constexpr int destinatarioNumero = 200;
inline constexpr char destinatarioBairro[] = "CENTRO";
inline constexpr IndIEDest destinatarioIndicadorIE = IndIEDest::NaoContribuinte;
// Deixe o código do município em zero para usar município, UF e CEP do emitente.
inline constexpr int destinatarioCodMunicipio = 0;
inline constexpr char destinatarioMunicipio[] = "";
inline constexpr char destinatarioUF[] = "";
inline constexpr int destinatarioCEP = 0;
inline constexpr char cfop[] = "5102";
inline constexpr char unidade[] = "UN";
inline constexpr char gtin[] = "SEM GTIN";
inline constexpr Orig origem = Orig::Nacional;
inline constexpr CstICMS cstICMS = CstICMS::ICMS00;
inline constexpr ModBC modalidadeICMS = ModBC::ValorOperacao;
inline constexpr CstPIS cstPIS = CstPIS::PIS49;
inline constexpr CstCOFINS cstCOFINS = CstCOFINS::COFINS49;
inline constexpr char cstIBSCBS[] = "000";
inline constexpr char classeIBSCBS[] = "000001";
inline constexpr double aliquotaICMS = 18;
inline constexpr double aliquotaIBSUF = 0.1;
inline constexpr double aliquotaIBSMun = 0;
inline constexpr double aliquotaCBS = 0.9;
inline constexpr ModFrete frete = ModFrete::SemFrete;
inline constexpr TPag pagamento = TPag::Dinheiro;
inline constexpr char informacoesComplementares[] = "Nota gerada pelo exemplo da CppBrasil.";
inline constexpr Item itens[] = {
    {"001", "PRODUTO DE TESTE 1", "22021000", 2, 5.50},
    {"002", "PRODUTO DE TESTE 2", "19053100", 1, 12.90},
};
}

struct CampoIni
{
    QString chave;
    QString rotulo;
    QVariant padrao;
    QList<QPair<QString, QString>> opcoes = {}; // valor no INI e texto exibido
    bool senha = false;
};

struct GrupoIni
{
    QString titulo;
    QList<CampoIni> campos;
};

const QList<GrupoIni> &camposConfiguracao();
const GrupoIni &camposEmitente();
void configurar(CppNFe &cppnfe, const QSettings &ini);
bool carregarCertificado(CppNFe &cppnfe, const QSettings &ini, QTextStream &out);

bool gerar(CppNFe &cppnfe, const QSettings &ini, QTextStream &out);
bool assinar(CppNFe &cppnfe, QTextStream &out);
bool enviar(CppNFe &cppnfe, int lote, QTextStream &out);
void status(CppNFe &cppnfe, QTextStream &out);
void consultar(CppNFe &cppnfe, const QString &chave, QTextStream &out);
void cancelar(CppNFe &cppnfe, const QSettings &ini, const QString &chave,
              const QString &protocolo, const QString &justificativa, QTextStream &out);
void inutilizar(CppNFe &cppnfe, const QSettings &ini, int serie, int inicio, int fim,
                const QString &justificativa, QTextStream &out);
QString nomeServico(const WebServicesNF &ws);

}

#endif
