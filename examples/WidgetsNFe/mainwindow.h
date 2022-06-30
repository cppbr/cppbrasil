#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nota.h"

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
    void gerarEnviar();

private slots:
    void onErrorOccurred(const QString &error);
    void onRetWSChange(const QString &webServices);
    void onRetStatusServico(const QString &status);
    void onRetLote(const QString &lote);
    void onRetXML(const QString &xml);

    void on_bt_status_clicked();

    void on_bt_enviarNF_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
