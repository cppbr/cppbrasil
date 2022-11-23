#include "root.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include "nota.h"

using namespace Cutelyst;

Root::Root(QObject *parent) : Controller(parent)
{
}

Root::~Root()
{
}

void Root::nota_POST(Context *c)
{
    const QJsonObject data = c->request()->bodyJsonObject();
    QString _nNFe = data[u"nNFe"].toString();
    QString _tpDoc = data[u"tpDoc"].toString();
    int _num = _nNFe.toInt();
    int _tipo = 65;
    if (!_tpDoc.isEmpty())
     _tipo = _tpDoc.toInt();
    
    Nota nota;

    //status do serviço
    QString _xml;

    QObject::connect(&nota, &Nota::retXML,
                         [&] (const QString &xml){
        _xml = xml;
    });

    nota.onReqGerarEnviar(_num, _tipo);


    c->response()->body() = _xml.toLocal8Bit();

}

void Root::index(Context *c)
{
    c->response()->body() = "Welcome to Cutelyst!";
}

void Root::defaultPage(Context *c)
{
    c->response()->body() = "Page not found!";
    c->response()->setStatus(404);
}

