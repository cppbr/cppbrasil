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
}

