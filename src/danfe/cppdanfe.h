#ifndef CPPDANFE_H
#define CPPDANFE_H

#include <QObject>
#include <cppnfe.h>
#include <QPixmap>
#include <QPainter>
#include "cpputility.h"
#include "barcode128.h"

class CppDanfe
{
public:
    CppDanfe(const CppNFe *cppnfe, const int &recNo = 0);
    ~CppDanfe();

    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    virtual void print();

protected:
    const CppNFe *m_cppnfe;
    QString m_pathArquivo;
    QString m_pathLogo;
    int m_recNo;
    QImage m_imBarcode;
    QImage m_imBarcodeCont;
    QString m_modFrete;
    QString m_chave; //formatado no formato chave
    QString m_chaveCont; //FS ou FS-DA - formatado no formato chave
    QString m_protocolo;
    QString m_infCpl;
    QString m_nNF;
    QString m_serie;
    ModeloDF m_mod;
    TpEmis m_tpEmis;
    int m_cStat;


private:
    void genBarcode();
    void getParam();
    QString formatChave(const QString &chave); //9999 9999 9999 9999 9999 9999 9999 9999 9999

};

#endif // CPPDANFE_H
