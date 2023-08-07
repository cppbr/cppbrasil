#ifndef RETENVEVENTO_H
#define RETENVEVENTO_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/NFe/confignfe.h>

class CPPNFE_EXPORT InfEventoRet
{
public:
    InfEventoRet();
    ~InfEventoRet();
    void clear();
    QString get_Id() const;
    void set_Id(const QString &Id);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    int get_cOrgao() const;
    void set_cOrgao(const int &cOrgao);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    TpEvento get_tpEvento() const;
    void set_tpEvento(const TpEvento &tpEvento);
    QString get_xEvento() const;
    void set_xEvento(const QString &xEvento);
    int get_nSeqEvento() const;
    void set_nSeqEvento(const int &nSeqEvento);
    int get_cOrgaoAutor() const;
    void set_cOrgaoAutor(const int &cOrgaoAutor);
    QString get_CNPJDest() const;
    void set_CNPJDest(const QString &CNPJDest);
    QString get_CPFDest() const;
    void set_CPFDest(const QString &CPFDest);
    QString get_emailDest() const;
    void set_emailDest(const QString &emailDest);
    QDateTime get_dhRegEvento() const;
    void set_dhRegEvento(const QDateTime &dhRegEvento);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);
    QString get_chNFePend() const;
    void set_chNFePend(const QString &chNFePend);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    int get_cUF() const;
    void set_cUF(const int &cUF);
private:
    QString m_Id;
    TpAmb m_tpAmb;
    QString m_verAplic;
    int m_cOrgao;
    int m_cStat;
    QString m_xMotivo;
    QString m_chNFe;
    TpEvento m_tpEvento;
    QString m_xEvento;
    int m_nSeqEvento;
    int m_cOrgaoAutor;
    QString m_CNPJDest;
    QString m_CPFDest;
    QString m_emailDest;
    QDateTime m_dhRegEvento;
    QString m_nProt;
    QString m_chNFePend;
    QDateTime m_dhRecbto;
    int m_cUF;
};

//-----------------------------------------------------

class CPPNFE_EXPORT RetEvento
{
public:
    RetEvento();
    ~RetEvento();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_XML() const;
    void set_XML(const QString &xml);
    void salvarXML(const QString &caminho, const QString &nomeArquivo);

    std::shared_ptr<InfEventoRet> infEvento;
private:
    QString m_versao;
    QString m_xml; //xml completo
};

//-----------------------------------------------------

class CPPNFE_EXPORT RetEnvEvento : public QObject
{
    Q_OBJECT
public:
    RetEnvEvento();
    ~RetEnvEvento();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    int get_idLote() const;
    void set_idLote(const int &idLote);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    int get_cOrgao() const;
    void set_cOrgao(const int &cOrgao);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);

    std::shared_ptr<Container<RetEvento>> retEvento;

private:
    QString m_versao;
    int m_idLote;
    TpAmb m_tpAmb;
    QString m_verAplic;
    int m_cOrgao;
    int m_cStat;
    QString m_xMotivo;
};

#endif // RETENVEVENTO_H
