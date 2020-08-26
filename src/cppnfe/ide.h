#ifndef IDE_H
#define IDE_H

#include <QObject>
#include <QDateTime>

#include "conv.h"

class Ide : public QObject
{
    Q_OBJECT
  private:
    int m_cUF;
    int m_cNF;
    QString m_natOp;
    int m_mod;
    int m_serie;
    int m_nNF;
    QDateTime m_dhEmi;
    QDateTime m_dhSaiEnt;
    TipoOperacao m_tpNF;
    DestinoOperacao m_idDest;
    int m_cMunFG;
    FormatoImpressao m_tpImp;
    TipoEmissao m_tpEmis;
    QString m_cDV;
    Ambiente m_tpAmb;
    FinalidadeNfe m_finNFe;
    ConsumidorFinal m_indFinal;
    IndicadorPresenca m_indPres;
    ProcessoEmissao m_procEmi;
    QString m_verProc;
    QDateTime m_dhCont;
    QString m_xJust;
  public:
    explicit Ide(QObject *parent = nullptr);

    int get_cUF() const;
    void set_cUF(const int &cUF);
    int get_cNF() const;
    void set_cNF(const int &cNF);
    QString get_natOp() const;
    void set_natOp(const QString &natOp);
    int get_mod() const;
    void set_mod(const int &mod);
    int get_serie() const;
    void set_serie(const int &serie);
    int get_nNF() const;
    void set_nNF(const int &nNF);
    QDateTime get_dhEmi() const;
    void set_dhEmi(const QDateTime &dhEmi);
    QDateTime get_dhSaiEnt() const;
    void set_dhSaiEnt(const QDateTime &dhSaiEnt);
    TipoOperacao get_tpNF() const;
    void set_tpNF(const TipoOperacao &tpNF);
    DestinoOperacao get_idDest() const;
    void set_idDest(const DestinoOperacao &idDest);
    int get_cMunFG() const;
    void set_cMunFG(const int &cMunFG);
    FormatoImpressao get_tpImp() const;
    void set_tpImp(const FormatoImpressao &tpImp);
    TipoEmissao get_tpEmis() const;
    void set_tpEmis(const TipoEmissao &tpEmis);
    QString get_cDV() const;
    void set_cDV(const QString &cDV);
    Ambiente get_tpAmb() const;
    void set_tpAmb(const Ambiente &tpAmb);
    FinalidadeNfe get_finNFe() const;
    void set_finNFe(const FinalidadeNfe &finNFe);
    ConsumidorFinal get_indFinal() const;
    void set_indFinal(const ConsumidorFinal &indFinal);
    IndicadorPresenca get_indPres() const;
    void set_indPres(const IndicadorPresenca &indPres);
    ProcessoEmissao get_procEmi() const;
    void set_procEmi(const ProcessoEmissao &procEmi);
    QString get_verProc() const;
    void set_verProc(const QString &verProc);
    QDateTime get_dhCont() const;
    void set_dhCont(const QDateTime &dhCont);
    QString get_xJust() const;
    void set_xJust(const QString &xJust);
};

#endif // IDE_H
