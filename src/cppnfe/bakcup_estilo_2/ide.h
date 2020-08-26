#ifndef IDE_H
#define IDE_H

#include <QObject>
#include <QDateTime>

#include "conv.h"

class Ide : public QObject
{
    Q_OBJECT
  public:
    explicit Ide(QObject *parent = nullptr);

    int cUF;
    int cNF;
    QString natOp;
    int mod;
    int serie;
    int nNF;
    QDateTime dhEmi;
    QDateTime dhSaiEnt;
    TipoOperacao tpNF;
    DestinoOperacao idDest;
    int cMunFG;
    FormatoImpressao tpImp;
    TipoEmissao tpEmis;
    QString cDV;
    Ambiente tpAmb;
    FinalidadeNfe finNFe;
    ConsumidorFinal indFinal;
    IndicadorPresenca indPres;
    ProcessoEmissao procEmi;
    QString verProc;
    QDateTime dhCont;
    QString xJust;
};

#endif // IDE_H
