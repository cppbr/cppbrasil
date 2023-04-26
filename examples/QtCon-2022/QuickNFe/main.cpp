#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSslSocket>
#include <QDebug>

#include "nota.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    #ifdef Q_OS_ANDROID
        qputenv("libcrypto_1_1", ".so");
        qputenv("libssl_1_1", ".so");
        qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl() <<  "-------------------------------";
    #endif
    Nota nota;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("notaFiscal", &nota);
    engine.addImportPath(":/QuickNFe/");
    const QUrl url(u"qrc:/QuickNFe/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
