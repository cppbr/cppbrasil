#ifndef NFEDISTRIBUICAODFE_H
#define NFEDISTRIBUICAODFE_H

#include <memory>
#include <QByteArray>
#include <QDateTime>
#include <QObject>
#include <CppBase/container.h>
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/convnf.h>
#include <CppNFe/nfe.h>
#include <CppNFe/retconssitnfe.h>

class CPPNFE_EXPORT DistNSU
{
public:
    QString get_ultNSU() const;
    void set_ultNSU(const QString &ultNSU);

private:
    QString m_ultNSU;
};

class CPPNFE_EXPORT ConsNSU
{
public:
    QString get_NSU() const;
    void set_NSU(const QString &NSU);

private:
    QString m_NSU;
};

class CPPNFE_EXPORT ConsChNFe
{
public:
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);

private:
    QString m_chNFe;
};

class CPPNFE_EXPORT DistDFeInt
{
public:
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    // TpAmb::None usa o ambiente configurado em ConfigNFe.
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_cUFAutor() const;
    void set_cUFAutor(const QString &cUFAutor);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);

    std::unique_ptr<DistNSU> distNSU = std::make_unique<DistNSU>();
    std::unique_ptr<ConsNSU> consNSU = std::make_unique<ConsNSU>();
    std::unique_ptr<ConsChNFe> consChNFe = std::make_unique<ConsChNFe>();

private:
    QString m_versao = ConvNF::versaoLayout(WebServicesNF::NFeDistribuicaoDFe);
    TpAmb m_tpAmb = TpAmb::None;
    QString m_cUFAutor;
    QString m_CNPJ;
    QString m_CPF;
};

class CPPNFE_EXPORT ResNFe
{
public:
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_xNome() const;
    void set_xNome(const QString &xNome);
    QString get_IE() const;
    void set_IE(const QString &IE);
    QDateTime get_dhEmi() const;
    void set_dhEmi(const QDateTime &dhEmi);
    int get_tpNF() const;
    void set_tpNF(const int &tpNF);
    double get_vNF() const;
    void set_vNF(const double &vNF);
    QString get_digVal() const;
    void set_digVal(const QString &digVal);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);
    int get_cSitNFe() const;
    void set_cSitNFe(const int &cSitNFe);

private:
    QString m_versao;
    QString m_chNFe;
    QString m_CNPJ;
    QString m_CPF;
    QString m_xNome;
    QString m_IE;
    QDateTime m_dhEmi;
    int m_tpNF = -1;
    double m_vNF = 0;
    QString m_digVal;
    QDateTime m_dhRecbto;
    QString m_nProt;
    int m_cSitNFe = 0;
};

class CPPNFE_EXPORT ResEvento
{
public:
    QString get_versao() const;
    void set_versao(const QString &versao);
    int get_cOrgao() const;
    void set_cOrgao(const int &cOrgao);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    QDateTime get_dhEvento() const;
    void set_dhEvento(const QDateTime &dhEvento);
    int get_tpEvento() const;
    void set_tpEvento(const int &tpEvento);
    int get_nSeqEvento() const;
    void set_nSeqEvento(const int &nSeqEvento);
    QString get_xEvento() const;
    void set_xEvento(const QString &xEvento);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);

private:
    QString m_versao;
    int m_cOrgao = 0;
    QString m_CNPJ;
    QString m_CPF;
    QString m_chNFe;
    QDateTime m_dhEvento;
    int m_tpEvento = 0;
    int m_nSeqEvento = 0;
    QString m_xEvento;
    QDateTime m_dhRecbto;
    QString m_nProt;
};

class CPPNFE_EXPORT DocZip
{
public:
    void clear();
    QString get_NSU() const;
    void set_NSU(const QString &NSU);
    QString get_schema() const;
    void set_schema(const QString &schema);
    // Base64 recebido.
    QString get_zip() const;
    void set_zip(const QString &zip);
    // XML descompactado, inclusive para schemas novos.
    QString get_xml() const;
    void set_xml(const QString &xml);

    std::unique_ptr<ResNFe> resNFe;
    std::unique_ptr<ResEvento> resEvento;
    std::unique_ptr<ProcEventoNFe> procEventoNFe;
    std::unique_ptr<Nfe> procNFe;

private:
    QString m_NSU;
    QString m_schema;
    QString m_zip;
    QString m_xml;
};

class CPPNFE_EXPORT LoteDistDFeInt
{
public:
    std::unique_ptr<Container<DocZip>> docZip = std::make_unique<Container<DocZip>>();
};

class CPPNFE_EXPORT RetDistDFeInt
{
public:
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
    QDateTime get_dhResp() const;
    void set_dhResp(const QDateTime &dhResp);
    QString get_ultNSU() const;
    void set_ultNSU(const QString &ultNSU);
    QString get_maxNSU() const;
    void set_maxNSU(const QString &maxNSU);

    std::unique_ptr<LoteDistDFeInt> loteDistDFeInt = std::make_unique<LoteDistDFeInt>();

private:
    QString m_versao;
    TpAmb m_tpAmb = TpAmb::None;
    QString m_verAplic;
    int m_cStat = 0;
    QString m_xMotivo;
    QDateTime m_dhResp;
    QString m_ultNSU;
    QString m_maxNSU;
};

class CPPNFE_EXPORT NFeDistribuicaoDFe : public QObject
{
    Q_OBJECT
public:
    explicit NFeDistribuicaoDFe(ConfigNFe *config);
    void clear();
    QString get_error() const;

    // Preencha um documento do interessado e um grupo de consulta.
    bool gerarXML(QByteArray &xml);
    bool lerRetorno(const QByteArray &xml);
    // Faz uma consulta. O resultado fiscal fica em retDistDFeInt->get_cStat().
    bool consultar();

    std::unique_ptr<DistDFeInt> distDFeInt = std::make_unique<DistDFeInt>();
    std::unique_ptr<RetDistDFeInt> retDistDFeInt = std::make_unique<RetDistDFeInt>();

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    ConfigNFe *config;
    QString m_error;
    bool falhar(const QString &error);
};

#endif // NFEDISTRIBUICAODFE_H
