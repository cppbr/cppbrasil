#ifndef INUTNFE_H
#define INUTNFE_H

#include <memory>
#include <QDateTime>
#include <QObject>
#include <CppNFe/confignfe.h>

class InutNFe;

// Campos comuns ao pedido e ao retorno de inutilização.
class CPPNFE_EXPORT InfInut : public QObject
{
    Q_OBJECT
public:
    explicit InfInut(ConfigNFe *config = nullptr, InutNFe *inutNFe = nullptr);
    void clear();
    // Gera, assina e valida sem transmitir.
    bool gerarXML();
    // O retorno bool indica processamento técnico; a homologação exige cStat=102.
    bool inutilizar();
    // Lê XML isolado ou resposta SOAP, validando o retorno pelo XSD.
    bool lerRetorno(const QByteArray &xml);
    QString get_error() const;
    QString get_XMLOriginal() const;
    QString get_XMLAssinado() const;
    // XML processado, disponível somente após confirmação correspondente ao pedido.
    QString get_XML() const;
    // Calculado automaticamente na geração do pedido.
    QString get_Id() const;
    void set_Id(const QString &Id);
    // Ambiente, UF e modelo não informados usam ConfigNFe.
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_xServ() const;
    void set_xServ(const QString &xServ);
    int get_cUF() const;
    void set_cUF(const int &cUF);
    // Ano com duas posições (por exemplo, 26 para 2026).
    int get_ano() const;
    void set_ano(const int &ano);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    ModeloDF get_mod() const;
    void set_mod(const ModeloDF &mod);
    int get_serie() const;
    void set_serie(const int &serie);
    int get_nNFIni() const;
    void set_nNFIni(const int &nNFIni);
    int get_nNFFin() const;
    void set_nNFFin(const int &nNFFin);
    QString get_xJust() const;
    void set_xJust(const QString &xJust);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    int get_cStat() const;
    void set_cStat(const int &cStat);
    QString get_xMotivo() const;
    void set_xMotivo(const QString &xMotivo);
    QDateTime get_dhRecbto() const;
    void set_dhRecbto(const QDateTime &dhRecbto);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    ConfigNFe *config;
    InutNFe *pedido;
    QString m_error;
    QString m_xmlOriginal;
    QString m_xmlAssinado;
    QString m_xml;
    bool falhar(const QString &error);
    bool preparar();
    QString m_Id;
    TpAmb m_tpAmb;
    QString m_xServ;
    int m_cUF;
    int m_ano;
    QString m_CNPJ;
    ModeloDF m_mod;
    bool m_modInformado;
    int m_serie;
    int m_nNFIni;
    int m_nNFFin;
    QString m_xJust;
    QString m_verAplic;
    int m_cStat;
    QString m_xMotivo;
    QDateTime m_dhRecbto;
    QString m_nProt;
};

class CPPNFE_EXPORT RetInutNFe
{
public:
    RetInutNFe();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_XML() const;
    std::unique_ptr<InfInut> infInut;

private:
    friend class InfInut;
    QString m_versao;
    QString m_xml;
};

class CPPNFE_EXPORT InutNFe : public QObject
{
    Q_OBJECT
public:
    explicit InutNFe(ConfigNFe *config);
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    std::unique_ptr<InfInut> infInut;
    std::unique_ptr<RetInutNFe> retInutNFe;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    QString m_versao;
};

#endif // INUTNFE_H
