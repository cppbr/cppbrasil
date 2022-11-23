#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_tipoDocumento(0)
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

    initComponests();

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
    int _numNota = 0;
    if (!ui->edtNumNota->text().isEmpty())
      _numNota = ui->edtNumNota->text().toInt();

    emit gerarEnviar(_numNota, this->m_tipoDocumento);
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
                danfe.caminhoArquivo("/home/usuario/cppbrasil/examples/report/DANFE-NFe.xml");
            else
                danfe.caminhoArquivo("/home/usuario/cppbrasil/examples/report//DANFE-NFCe.xml");
            qDebug() << "Retorno lote";
            danfe.print();
        }
    } else
    {
        if (cppnfe->notafiscal->NFe->items->value(0)->infNFe->ide->get_mod() == ModeloDF::NFe)
            danfe.caminhoArquivo("/home/usuario/cppbrasil/examples/report/DANFE-NFe.xml");
        else
            danfe.caminhoArquivo("/home/usuario/cppbrasil/examples/report//DANFE-NFCe.xml");

        qDebug() << "Abertura xml";
        danfe.print();
    }
}

void MainWindow::initComponests()
{
    ui->cbTipoDoc->addItem("NFe");
    ui->cbTipoDoc->addItem("NFCe");
}


void MainWindow::on_cbTipoDoc_currentIndexChanged(int index)
{
    if (index == 0)
        m_tipoDocumento = 55;
    else
        m_tipoDocumento = 65;
}

