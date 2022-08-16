#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //slots
    //retorno webservices
    connect(&nota, &Nota::retWSChange, this, &MainWindow::onRetWSChange);
    //caso ocorra erro
    connect(&nota, &Nota::errorOccurred, this, &MainWindow::onErrorOccurred);
    //status do serviço
    connect(&nota, &Nota::retStatusServico, this, &MainWindow::onRetStatusServico);
    //retorno lote
    connect(&nota, &Nota::retLote, this, &MainWindow::onRetLote);
    connect(&nota, &Nota::retXML, this, &MainWindow::onRetXML);

    //signals
    connect(this, &MainWindow::statusServico, &nota, &Nota::onReqStatusServico);
    connect(this, &MainWindow::gerarEnviar, &nota, &Nota::onReqGerarEnviar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onErrorOccurred(const QString &error)
{
    ui->textSaida->insertPlainText(error);
}

void MainWindow::onRetWSChange(const QString &webServices)
{
    ui->textSaida->insertPlainText(webServices);
}

void MainWindow::onRetStatusServico(const QString &status)
{
    ui->textSaida->insertPlainText(status);
}

void MainWindow::onRetLote(const QString &lote)
{
    ui->textSaida->insertPlainText(lote);
}

void MainWindow::onRetXML(const QString &xml)
{
    ui->textXML->insertPlainText(xml);
}


void MainWindow::on_bt_status_clicked()
{
    emit statusServico();
}


void MainWindow::on_bt_enviarNF_clicked()
{
    emit gerarEnviar();
    imprimirDANFE(this->nota.getCppNFe());
}


void MainWindow::on_btDanfe_clicked()
{
    QString _xml = QFileDialog::getOpenFileName(this, "XML", "/home", "XML FILE (*.xml)");
    if (!_xml.isEmpty())
    {
        CppNFe* _nfe = new CppNFe;
        _nfe->notafiscal->loadFromFile(_xml);
        imprimirDANFE(_nfe);
        delete _nfe;
    }
}

void MainWindow::imprimirDANFE(const CppNFe *cppnfe)
{
    CppDanfeQtRPT danfe(cppnfe, 0);
    //danfe.caminhoLogo("C:/cppbrasil/suaLogo.jpg");
    if (cppnfe->notafiscal->retorno->protNFe->items->count() > 0)
    {
        if ((cppnfe->notafiscal->retorno->protNFe->items->value(0)->get_cStat() == 100) ||
            (cppnfe->notafiscal->retorno->protNFe->items->value(0)->get_cStat() == 150))
        {
            if (cppnfe->notafiscal->NFe->items->value(0)->infNFe->ide->get_mod() == ModeloDF::NFe)
                danfe.caminhoArquivo(QDir::currentPath() +  "/../report/DANFE-NFe.xml");
            else
                danfe.caminhoArquivo(QDir::currentPath() +  "/../report/DANFE-NFCe.xml");

            danfe.print();
        }
    } else
    {
        if (cppnfe->notafiscal->NFe->items->value(0)->infNFe->ide->get_mod() == ModeloDF::NFe)
            danfe.caminhoArquivo(QDir::currentPath() +  "/../report/DANFE-NFe.xml");
        else
            danfe.caminhoArquivo(QDir::currentPath() +  "/../report/DANFE-NFCe.xml");

        danfe.print();
    }
}

