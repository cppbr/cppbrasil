#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "nota.h"
#include <CppBrasil/NFe/CppNFe>
#include <CppBrasil/DanfeQtRPT/CppDanfeQtRPT>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Nota nota;

signals:
    void statusServico();
    void gerarEnviar(const int &numNota = 0, const int &tipoDocumento = 0);

private slots:
    void onErrorOccurred(const QString &error);
    void onRetWSChange(const QString &webServices);
    void onRetStatusServico(const QString &status);
    void onRetLote(const QString &lote);
    void onRetXML(const QString &xml);

    void on_bt_status_clicked();
    void on_bt_enviarNF_clicked();

    void on_btDanfe_clicked();

    void on_cbTipoDoc_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void imprimirDANFE(const CppNFe *cppnfe);
    void initComponests();
    int m_tipoDocumento;
};
#endif // MAINWINDOW_H
