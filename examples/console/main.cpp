/*
 * consoleNFe — exemplo de uso da libCppNFe em modo console.
 *
 * Uso:
 *   consoleNFe [--config arquivo.ini] <comando> [argumentos]
 *
 * Comandos:
 *   status                                        status do serviço da SEFAZ
 *   gerar                                         gera, assina e valida a NF-e (sem transmitir)
 *   enviar [lote]                                 gera, assina, valida e transmite a NF-e
 *   consultar <chave>                             consulta a situação de uma NF-e
 *   cancelar <chave> <protocolo> <justificativa>  cancela uma NF-e autorizada
 *   inutilizar <serie> <inicio> <fim> <justif.>   inutiliza uma faixa de numeração
 *
 * Sem comando, abre um menu interativo. A configuração vem de consoleNFe.ini
 * (veja consoleNFe.ini.exemplo) e a senha do certificado pode vir da variável
 * de ambiente CPPBR_CERT_SENHA.
 *
 * O exemplo mostra o acesso aos objetos da biblioteca por referências e ponteiros
 * locais, em vez de repetir o caminho inteiro a cada chamada. Por exemplo, em vez de
 *
 *   cppnfe.notafiscal->NFe->obj->infNFe->ide->set_nNF(1);
 *
 * usa-se
 *
 *   auto &notas = *cppnfe.notafiscal->NFe;   // Container<Nfe>
 *   InfNFe *inf = notas.obj->infNFe.get();   // nota em preenchimento
 *   Ide *ide = inf->ide.get();
 *   ide->set_nNF(1);
 *
 * Os objetos continuam pertencendo à CppNFe (std::unique_ptr); as referências e
 * os ponteiros locais apenas encurtam o acesso e não devem ser liberados.
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include "nfe.h"

using namespace ExemploNFe;

namespace {

QTextStream out(stdout);
QTextStream in(stdin);

QString perguntar(const QString &texto)
{
    out << texto << ": " << Qt::flush;
    return in.readLine().trimmed();
}

bool gerarCompleta(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk)
{
    if (!gerar(cppnfe, ini, out))
        return false;
    if (!certificadoOk)
    {
        out << "XML gerado (sem assinatura):\n" << cppnfe.notafiscal->NFe->value(0)->get_XMLOriginal() << "\n";
        return false;
    }
    return assinar(cppnfe, out);
}

int executar(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk, QStringList args)
{
    const QString comando = args.isEmpty() ? QString() : args.takeFirst().toLower();

    if (comando == "gerar")
        return gerarCompleta(cppnfe, ini, certificadoOk) ? 0 : 1;
    if (!certificadoOk && (comando == "status" || comando == "enviar" || comando == "consultar"
                          || comando == "cancelar" || comando == "inutilizar"))
        return 1;

    if (comando == "status")
        status(cppnfe, out);
    else if (comando == "enviar")
        return gerarCompleta(cppnfe, ini, certificadoOk)
            && enviar(cppnfe, args.value(0, QString::number(DadosNota::lote)).toInt(), out) ? 0 : 1;
    else if (comando == "consultar" && args.size() == 1)
        consultar(cppnfe, args[0], out);
    else if (comando == "cancelar" && args.size() >= 3)
        cancelar(cppnfe, ini, args[0], args[1], args.mid(2).join(' '), out);
    else if (comando == "inutilizar" && args.size() >= 4)
        inutilizar(cppnfe, ini, args[0].toInt(), args[1].toInt(), args[2].toInt(), args.mid(3).join(' '), out);
    else
    {
        out << "Comando inválido. Use --help para ver os comandos.\n";
        return 2;
    }
    return 0;
}

void menu(CppNFe &cppnfe, const QSettings &ini, bool certificadoOk)
{
    for (;;)
    {
        out << "\n=== consoleNFe ===\n"
               "1 - Status do serviço\n"
               "2 - Gerar, assinar e validar NF-e\n"
               "3 - Gerar e enviar NF-e\n"
               "4 - Consultar NF-e\n"
               "5 - Cancelar NF-e\n"
               "6 - Inutilizar numeração\n"
               "0 - Sair\n";
        const QString opcao = perguntar("Opção");
        if (opcao == "0" || in.atEnd())
            return;
        if (!certificadoOk && QStringList{"1", "3", "4", "5", "6"}.contains(opcao))
        {
            out << "Certificado digital necessário para usar a SEFAZ.\n";
            continue;
        }
        if (opcao == "1")
            status(cppnfe, out);
        else if (opcao == "2")
            gerarCompleta(cppnfe, ini, certificadoOk);
        else if (opcao == "3")
        {
            const int lote = perguntar("Número do lote").toInt();
            if (gerarCompleta(cppnfe, ini, certificadoOk))
                enviar(cppnfe, lote, out);
        }
        else if (opcao == "4")
            consultar(cppnfe, perguntar("Chave"), out);
        else if (opcao == "5")
        {
            const QString chave = perguntar("Chave");
            const QString protocolo = perguntar("Protocolo");
            cancelar(cppnfe, ini, chave, protocolo, perguntar("Justificativa"), out);
        }
        else if (opcao == "6")
        {
            const int serie = perguntar("Série").toInt();
            const int inicio = perguntar("Número inicial").toInt();
            const int fim = perguntar("Número final").toInt();
            inutilizar(cppnfe, ini, serie, inicio, fim, perguntar("Justificativa"), out);
        }
    }
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("consoleNFe");

    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Exemplo da libCppNFe.\n\nComandos:\n"
        "  status\n  gerar\n  enviar [lote]\n  consultar <chave>\n"
        "  cancelar <chave> <protocolo> <justificativa>\n"
        "  inutilizar <serie> <inicio> <fim> <justificativa>\n\n"
        "Sem comando, abre o menu interativo.");
    parser.addHelpOption();
    QCommandLineOption optConfig("config", "Arquivo de configuração.", "arquivo",
                                 QCoreApplication::applicationDirPath() + "/consoleNFe.ini");
    parser.addOption(optConfig);
    parser.addPositionalArgument("comando", "Comando a executar.");
    parser.process(app);

    const QString arquivoIni = parser.value(optConfig);
    if (!QFileInfo::exists(arquivoIni))
    {
        out << "Arquivo de configuração não encontrado: " << QDir::toNativeSeparators(arquivoIni) << "\n"
            << "Copie consoleNFe.ini.exemplo para consoleNFe.ini e ajuste os dados.\n";
        return 1;
    }
    const QSettings ini(arquivoIni, QSettings::IniFormat);

    CppNFe cppnfe;
    QObject::connect(&cppnfe, &CppNFe::errorOccurred,
                     [](const QString &erro) { out << "[erro] " << erro << Qt::endl; });
    QObject::connect(&cppnfe, &CppNFe::wsChange, [](const WebServicesNF &ws) {
        if (ws != WebServicesNF::None)
            out << "[sefaz] " << nomeServico(ws) << "..." << Qt::endl;
    });

    configurar(cppnfe, ini);
    const bool certificadoOk = carregarCertificado(cppnfe, ini, out);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty())
    {
        menu(cppnfe, ini, certificadoOk);
        return 0;
    }
    return executar(cppnfe, ini, certificadoOk, args);
}
