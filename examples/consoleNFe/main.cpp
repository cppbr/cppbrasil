#include <QCoreApplication>
#include <QDebug>
#include "nota.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Nota nota;
    //retorno do webservices
    QObject::connect(&nota, &Nota::retWSChange,
                     [=] (const QString &webServices){

        qInfo().noquote() << webServices;
    });
    //caso ocorra erro
    QObject::connect(&nota, &Nota::errorOccurred,
                     [=] (const QString &error){

        qInfo().noquote() << error;
    });

    //status do serviço
    QObject::connect(&nota, &Nota::retStatusServico,
                     [=] (const QString &status){

        qInfo().noquote() << status;
    });

    //nota.onReqStatusServico();

    //retorno lote
    QObject::connect(&nota, &Nota::retLote,
                     [=] (const QString &lote){

        qInfo().noquote() << lote;
    });

    nota.onReqGerarEnviar();


    return a.exec();
}
