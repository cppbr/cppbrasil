#ifndef RETCONSSITNFE_H
#define RETCONSSITNFE_H

#include <memory>
#include <QObject>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/cpputility.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/convnf.h>
#include <CppNFe/retornonfe.h>
#include <CppNFe/retenvevento.h>
#include <CppNFe/tevento.h>

class CPPNFE_EXPORT RetCancNFe
{
public:
    RetCancNFe();
    ~RetCancNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    std::unique_ptr<InfEventoRet> infCanc;

private:
    QString m_versao;
};


class CPPNFE_EXPORT ProcEventoNFe
{
public:
    ProcEventoNFe();
    ~ProcEventoNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    std::unique_ptr<TEvento> evento;
    std::unique_ptr<RetEvento> retEvento;

private:
    QString m_versao;
};

class CPPNFE_EXPORT RetConsSitNFe
{
public:
    RetConsSitNFe();
    ~RetConsSitNFe();
    void clear();
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
    QString get_xml() const;
    void set_xml(const QString &xml);
    void salvarXML(const QString &caminho, const QString &nomeArquivo);


    std::unique_ptr<ProtNFe> protNFe;
    std::unique_ptr<RetCancNFe> retCancNFe ;
    std::unique_ptr<Container<ProcEventoNFe>> procEventoNFe;

private:
    QString m_versao;
    QString m_Id;
    TpAmb m_tpAmb;
    QString m_verAplic;
    int m_cStat;
    QString m_xMotivo;
    int m_cUF;
    QDateTime m_dhRecbto;
    QString m_chNFe;
    QString m_xml;
};

#endif // RETCONSSITNFE_H
