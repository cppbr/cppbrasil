#include "nfe.h"

#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QCommandLineParser>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QScrollArea>
#include <QSpinBox>
#include <QStandardPaths>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <functional>
#include <limits>

using namespace ExemploNFe;

namespace {

QVBoxLayout *adicionarAba(QTabWidget &abas, const QString &titulo)
{
    auto *pagina = new QWidget;
    auto *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setWidget(pagina);
    abas.addTab(area, titulo);
    return new QVBoxLayout(pagina);
}

QFormLayout *adicionarGrupo(QVBoxLayout &pagina, const QString &titulo)
{
    auto *grupo = new QGroupBox(titulo);
    pagina.addWidget(grupo);
    return new QFormLayout(grupo);
}

class JanelaNFe : public QMainWindow
{
public:
    explicit JanelaNFe(const QString &arquivoIni) : ini(arquivoIni, QSettings::IniFormat)
    {
        setWindowTitle("CppBrasil — Exemplo NF-e");
        resize(880, 730);
        auto *central = new QWidget;
        setCentralWidget(central);
        auto *layout = new QVBoxLayout(central);
        auto *abas = new QTabWidget;
        layout->addWidget(abas, 1);
        abas->setObjectName("abas");

        auto *configuracoes = adicionarAba(*abas, "Configurações");
        for (const auto &grupo : camposConfiguracao())
            adicionarCampos(*configuracoes, grupo);
        configuracoes->addStretch();

        auto *emitente = adicionarAba(*abas, "Emitente");
        adicionarCampos(*emitente, camposEmitente());
        emitente->addStretch();

        criarEventos(*adicionarAba(*abas, "Eventos"));
        criarEmissao(*adicionarAba(*abas, "Emissão da NF-e"));

        auto *salvar = new QPushButton("Salvar configurações");
        salvar->setObjectName("salvar");
        salvar->setToolTip(ini.fileName());
        layout->addWidget(salvar, 0, Qt::AlignRight);
        connect(salvar, &QPushButton::clicked, this, [this] {
            if (salvarIni())
                statusBar()->showMessage("Configurações salvas em " + ini.fileName(), 7000);
        });

        mensagens = new QPlainTextEdit;
        mensagens->setObjectName("mensagens");
        mensagens->setReadOnly(true);
        mensagens->setMaximumBlockCount(300);
        mensagens->setMaximumHeight(120);
        mensagens->setPlaceholderText("Resultados das operações");
        layout->addWidget(mensagens);
        statusBar()->showMessage(ini.fileName());
    }

protected:
    void closeEvent(QCloseEvent *evento) override
    {
        if (ocupado)
        {
            evento->ignore();
            return;
        }
        evento->setAccepted(salvarIni() || QMessageBox::warning(
            this, "Configurações", "Falha ao salvar o INI. Fechar sem salvar?",
            QMessageBox::Close | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Close);
    }

private:
    enum class Etapa { Inicial, Gerada, Assinada, Enviada };

    void adicionarCampos(QVBoxLayout &pagina, const GrupoIni &grupo)
    {
        auto *form = adicionarGrupo(pagina, grupo.titulo);
        for (const auto &campo : grupo.campos)
            adicionarCampo(*form, campo, true);
    }

    QWidget *adicionarCampo(QFormLayout &form, const CampoIni &campo, bool alteraNota = false,
                            int minimo = 0, int maximo = std::numeric_limits<int>::max())
    {
        const QVariant valor = ini.value(campo.chave, campo.padrao);
        const auto salvar = [this, chave = campo.chave](auto obter) {
            campos.append([this, chave, obter] { ini.setValue(chave, obter()); });
        };
        const auto mudou = [this, alteraNota] {
            if (alteraNota)
                invalidarNota();
        };

        QWidget *editor;
        if (!campo.opcoes.isEmpty())
        {
            auto *combo = new QComboBox;
            for (const auto &opcao : campo.opcoes)
                combo->addItem(opcao.second, opcao.first);
            const int indice = combo->findData(valor.toString());
            if (indice < 0)
                combo->addItem(valor.toString(), valor.toString());
            combo->setCurrentIndex(indice >= 0 ? indice : combo->count() - 1);
            connect(combo, &QComboBox::currentIndexChanged, this, mudou);
            salvar([combo] { return combo->currentData(); });
            editor = combo;
        }
        else if (campo.padrao.typeId() == QMetaType::Bool)
        {
            auto *check = new QCheckBox;
            check->setChecked(valor.toBool());
            connect(check, &QCheckBox::toggled, this, mudou);
            salvar([check] { return check->isChecked(); });
            editor = check;
        }
        else if (campo.padrao.typeId() == QMetaType::Int)
        {
            auto *spin = new QSpinBox;
            if (campo.chave == "proxy/porta")
                maximo = 65535;
            else if (campo.chave == "webservices/compactarAcimaDe")
                maximo = std::numeric_limits<int>::max() / 1024;
            spin->setRange(minimo, maximo);
            spin->setValue(valor.toInt());
            connect(spin, &QSpinBox::valueChanged, this, mudou);
            salvar([spin] { return spin->value(); });
            editor = spin;
        }
        else
        {
            const QString texto = campo.chave == "certificado/cadeia"
                ? valor.toStringList().join(", ") : valor.toString();
            auto *linha = new QLineEdit(texto);
            if (campo.senha)
                linha->setEchoMode(QLineEdit::Password);
            if (campo.chave == "certificado/senha")
                linha->setPlaceholderText("Em branco, usa CPPBR_CERT_SENHA");
            connect(linha, &QLineEdit::textChanged, this, mudou);
            salvar([linha] { return linha->text(); });
            editor = linha;
        }
        editor->setObjectName(campo.chave);
        if (campo.chave == "arquivos/eventos" || campo.chave == "arquivos/consultas")
            editor->setToolTip("A biblioteca grava estes XMLs em subpastas da pasta de saída.");
        form.addRow(campo.rotulo, editor);
        return editor;
    }

    bool salvarIni()
    {
        for (const auto &salvar : campos)
            salvar();
        ini.sync();
        if (ini.status() == QSettings::NoError)
            return true;
        mensagens->appendPlainText("Erro ao salvar o INI em " + ini.fileName());
        return false;
    }

    void conectarMensagens(CppNFe &cppnfe)
    {
        connect(&cppnfe, &CppNFe::errorOccurred, this, [this](const QString &erro) {
            mensagens->appendPlainText(erro);
        });
        connect(&cppnfe, &CppNFe::wsChange, this, [this](const WebServicesNF &ws) {
            if (ws != WebServicesNF::None)
                mensagens->appendPlainText(nomeServico(ws) + "…");
        });
    }

    void executar(const std::function<void(QTextStream &)> &operacao)
    {
        if (ocupado || !salvarIni())
            return;

        ocupado = true;
        centralWidget()->setEnabled(false);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString texto;
        QTextStream out(&texto);
        operacao(out);
        out.flush();
        mensagens->appendPlainText(texto.trimmed());
        QApplication::restoreOverrideCursor();
        centralWidget()->setEnabled(true);
        ocupado = false;
    }

    void executarEvento(const std::function<void(CppNFe &, QTextStream &)> &operacao)
    {
        executar([&](QTextStream &out) {
            CppNFe cppnfe;
            conectarMensagens(cppnfe);
            configurar(cppnfe, ini);
            if (carregarCertificado(cppnfe, ini, out))
                operacao(cppnfe, out);
        });
    }

    void criarEventos(QVBoxLayout &pagina)
    {
        auto *servicos = adicionarGrupo(pagina, "Consultas");
        auto *botaoStatus = new QPushButton("Consultar status do serviço");
        servicos->addRow(botaoStatus);
        connect(botaoStatus, &QPushButton::clicked, this, [this] { executarEvento(status); });

        auto *chave = qobject_cast<QLineEdit *>(adicionarCampo(
            *servicos, {"eventos/chave", "Chave de acesso", ""}));
        chave->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{44}"), chave));
        auto *consulta = new QPushButton("Consultar NF-e");
        servicos->addRow(consulta);
        connect(consulta, &QPushButton::clicked, this, [this, chave] {
            if (!chave->hasAcceptableInput())
            {
                mensagens->appendPlainText("Informe a chave de acesso com 44 dígitos.");
                return;
            }
            executarEvento([&](CppNFe &cppnfe, QTextStream &out) {
                consultar(cppnfe, chave->text(), out);
            });
        });

        auto *cancelamento = adicionarGrupo(pagina, "Cancelamento da NF-e consultada");
        auto *protocolo = qobject_cast<QLineEdit *>(adicionarCampo(
            *cancelamento, {"eventos/protocolo", "Protocolo de autorização", ""}));
        protocolo->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{15}"), protocolo));
        auto *justCancelamento = qobject_cast<QLineEdit *>(adicionarCampo(
            *cancelamento, {"eventos/justCancelamento", "Justificativa", ""}));
        justCancelamento->setMaxLength(255);
        auto *botaoCancelar = new QPushButton("Cancelar NF-e");
        cancelamento->addRow(botaoCancelar);
        connect(botaoCancelar, &QPushButton::clicked, this, [=] {
            const QString motivo = justCancelamento->text().trimmed();
            if (!chave->hasAcceptableInput() || !protocolo->hasAcceptableInput() || motivo.size() < 15)
            {
                mensagens->appendPlainText(
                    "Informe a chave (44 dígitos), o protocolo (15 dígitos) e a justificativa (mínimo de 15 caracteres).");
                return;
            }
            executarEvento([&](CppNFe &cppnfe, QTextStream &out) {
                cancelar(cppnfe, ini, chave->text(), protocolo->text(), motivo, out);
            });
        });

        auto *inutilizacao = adicionarGrupo(pagina, "Inutilização de numeração");
        auto *serie = qobject_cast<QSpinBox *>(adicionarCampo(
            *inutilizacao, {"eventos/serie", "Série", DadosNota::serie}, false, 0, 999));
        auto *inicio = qobject_cast<QSpinBox *>(adicionarCampo(
            *inutilizacao, {"eventos/inicio", "Número inicial", 1}, false, 1, 999999999));
        auto *fim = qobject_cast<QSpinBox *>(adicionarCampo(
            *inutilizacao, {"eventos/fim", "Número final", 1}, false, 1, 999999999));
        auto *justInutilizacao = qobject_cast<QLineEdit *>(adicionarCampo(
            *inutilizacao, {"eventos/justInutilizacao", "Justificativa", ""}));
        justInutilizacao->setMaxLength(255);
        auto *botaoInutilizar = new QPushButton("Inutilizar numeração");
        inutilizacao->addRow(botaoInutilizar);
        connect(botaoInutilizar, &QPushButton::clicked, this, [=] {
            const QString motivo = justInutilizacao->text().trimmed();
            if (fim->value() < inicio->value() || motivo.size() < 15)
            {
                mensagens->appendPlainText(
                    "Confira a faixa de numeração e informe uma justificativa com pelo menos 15 caracteres.");
                return;
            }
            executarEvento([&](CppNFe &cppnfe, QTextStream &out) {
                inutilizar(cppnfe, ini, serie->value(), inicio->value(), fim->value(), motivo, out);
            });
        });
        pagina.addStretch();
    }

    void criarEmissao(QVBoxLayout &pagina)
    {
        auto *form = adicionarGrupo(pagina, "Nota fiscal");
        adicionarCampo(*form, {"nota/numero", "Número da nota", 1}, true, 1, 999999999);
        auto *botoes = new QHBoxLayout;
        auto *botaoGerar = new QPushButton("Gerar");
        botaoAssinar = new QPushButton("Assinar");
        botaoEnviar = new QPushButton("Enviar");
        botaoGerar->setObjectName("gerar");
        botaoAssinar->setObjectName("assinar");
        botaoEnviar->setObjectName("enviar");
        botoes->addWidget(botaoGerar);
        botoes->addWidget(botaoAssinar);
        botoes->addWidget(botaoEnviar);
        form->addRow(botoes);

        xml = new QPlainTextEdit;
        xml->setObjectName("xml");
        xml->setReadOnly(true);
        xml->setPlaceholderText("XML da nota");
        pagina.addWidget(xml, 1);
        definirEtapa(Etapa::Inicial);

        connect(botaoGerar, &QPushButton::clicked, this, [this] {
            executar([this](QTextStream &out) {
                invalidarNota();
                emissao = std::make_unique<CppNFe>();
                conectarMensagens(*emissao);
                configurar(*emissao, ini);
                if (gerar(*emissao, ini, out))
                {
                    xml->setPlainText(emissao->notafiscal->NFe->value(0)->get_XMLOriginal());
                    definirEtapa(Etapa::Gerada);
                }
            });
        });
        connect(botaoAssinar, &QPushButton::clicked, this, [this] {
            executar([this](QTextStream &out) {
                // O certificado é carregado uma vez por nota gerada.
                if (!tentouCertificado)
                {
                    tentouCertificado = true;
                    certificadoOk = carregarCertificado(*emissao, ini, out);
                }
                else if (!certificadoOk)
                    out << "Confira o certificado e gere a nota novamente.\n";

                if (certificadoOk && assinar(*emissao, out))
                {
                    xml->setPlainText(emissao->notafiscal->NFe->value(0)->get_XMLAssinado());
                    definirEtapa(Etapa::Assinada);
                }
            });
        });
        connect(botaoEnviar, &QPushButton::clicked, this, [this] {
            executar([this](QTextStream &out) {
                if (enviar(*emissao, DadosNota::lote, out))
                {
                    const QString autorizado = emissao->notafiscal->NFe->value(0)->get_XMLAutorizado();
                    if (!autorizado.isEmpty())
                        xml->setPlainText(autorizado);
                    definirEtapa(Etapa::Enviada);
                }
            });
        });
    }

    void definirEtapa(Etapa etapa)
    {
        botaoAssinar->setEnabled(etapa == Etapa::Gerada);
        botaoEnviar->setEnabled(etapa == Etapa::Assinada);
    }

    void invalidarNota()
    {
        emissao.reset();
        tentouCertificado = false;
        certificadoOk = false;
        xml->clear();
        definirEtapa(Etapa::Inicial);
    }

    QSettings ini;
    QList<std::function<void()>> campos;
    std::unique_ptr<CppNFe> emissao;
    bool ocupado = false;
    bool tentouCertificado = false;
    bool certificadoOk = false;
    QPushButton *botaoAssinar = nullptr;
    QPushButton *botaoEnviar = nullptr;
    QPlainTextEdit *xml = nullptr;
    QPlainTextEdit *mensagens = nullptr;
};

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("widgetsNFe");

    QCommandLineParser parser;
    parser.setApplicationDescription("Exemplo Qt Widgets da libCppNFe");
    parser.addHelpOption();
    const QCommandLineOption config("config", "Arquivo INI a carregar e salvar.", "arquivo",
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/widgetsNFe.ini");
    parser.addOption(config);
    parser.process(app);

    JanelaNFe janela(parser.value(config));
    janela.show();
    return app.exec();
}
