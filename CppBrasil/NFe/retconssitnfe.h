#ifndef RETCONSSITNFE_H
#define RETCONSSITNFE_H

#include <memory>
#include <QObject>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/NFe/retornonfe.h>
#include <CppBrasil/NFe/retenvevento.h>
#include <CppBrasil/NFe/enveventoevento.h>

class CPPNFE_EXPORT RetConsSitBase
{
public:
    RetConsSitBase();
    ~RetConsSitBase();
    virtual void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);
    int get_cUF() const;
    void set_cUF(const int &cUF);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);

private:
    QString m_versao;
    TpAmb m_tpAmb;
    QString m_verAplic;
    int m_cStat;
    QString m_xMotivo;
    int m_cUF;
    QDateTime m_dhRecbto;
    QString m_chNFe;
};


class CPPNFE_EXPORT RetCancNFe: public RetConsSitBase
{
public:
    RetCancNFe();
    ~RetCancNFe();
    void clear() override;
    QString get_nProt() const;
    void set_nProt(const QString &nProt);

private:
    QString m_nProt;
};

class CPPNFE_EXPORT ProcEventoNFe
{
public:
    ProcEventoNFe();
    ~ProcEventoNFe();
    void clear();
    std::shared_ptr<EnvEventoEvento> evento;
    std::shared_ptr<RetEvento> retEvento;
private:
    QString m_versao;
};


class CPPNFE_EXPORT RetConsSitNFe : public RetConsSitBase
{
public:
    RetConsSitNFe();
    ~RetConsSitNFe();
    void clear() override;
    std::shared_ptr<ProtNFe> protNFe;
    std::shared_ptr<RetCancNFe> retCancNFe ;
    std::shared_ptr<ProcEventoNFe> procEventoNFe;

};

#endif // RETCONSSITNFE_H
