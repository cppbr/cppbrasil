#ifndef ENVEVENTOEVENTO_H
#define ENVEVENTOEVENTO_H


#include <memory>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/NFe/confignfe.h>


class CPPNFE_EXPORT AutXMLAtor
{
public:
    AutXMLAtor();
    ~AutXMLAtor();
    void clear();
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);

private:
    QString m_CNPJ;
    QString m_CPF;
};

//-----------------------------------------------------

class CPPNFE_EXPORT ItemPedProrrog
{
public:
    ItemPedProrrog();
    ~ItemPedProrrog();
    void clear();
    int get_numItem() const;
    void set_numItem(const int &numItem);
    double get_qtdeItem() const;
    void set_qtdeItem(const double &qtdeItem);

private:
    int m_numItem;
    double m_qtdeItem;
};

//-----------------------------------------------------

class CPPNFE_EXPORT DestEPEC
{
public:
    DestEPEC();
    ~DestEPEC();
    void clear();
    int get_UF() const;
    void set_UF(const int &uf);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_idEstrangeiro() const;
    void set_idEstrangeiro(const QString &idEstrangeiro);
    QString get_IE() const;
    void set_IE(const QString &IE);

private:
    int m_UF;
    QString m_CNPJ;
    QString m_CPF;
    QString m_idEstrangeiro;
    QString m_IE;
};

//-----------------------------------------------------

class CPPNFE_EXPORT DetEvento
{
public:
    DetEvento();
    ~DetEvento();
    void clear();
    QString get_descEvento() const;
    void set_descEvento(const QString &descEvento);
    int get_cOrgaoAutor() const;
    void set_cOrgaoAutor(const int &cOrgaoAutor);
    TpAutor get_tpAutor() const;
    void set_tpAutor(const TpAutor &tpAutor);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);
    QString get_xJust() const;
    void set_xJust(const QString &xJust);
    QString get_chNFeRef() const;
    void set_chNFeRef(const QString &chNFeRef);
    QString get_xCorrecao() const;
    void set_xCorrecao(const QString &xCorrecao);
    QString get_xCondUso() const;
    void set_xCondUso(const QString &xCondUso);
    QDateTime get_dhEmi() const;
    void set_dhEmi(const QDateTime &dhEmi);
    TpNF get_tpNF() const;
    void set_tpNF(const TpNF &tpNF);
    QString get_IE() const;
    void set_IE(const QString &IE);
    double get_vNF() const;
    void set_vNF(const double &vNF);
    double get_vICMS() const;
    void set_vICMS(const double &vICMS);
    double get_vST() const;
    void set_vST(const double &vST);
    TpAutorizacao get_tpAutorizacao() const;
    void set_tpAutorizacao(const TpAutorizacao &tpAutorizacao);
    QString get_nDoc() const;
    void set_nDoc(const QString &nDoc);
    QString get_xNome() const;
    void set_xNome(const QString &xNome);
    QString get_latGPS() const;
    void set_latGPS(const QString &latGPS);
    QString get_longGPS() const;
    void set_longGPS(const QString &longGPS);
    QString get_hashComprovante() const;
    void set_hashComprovante(const QString &hashComprovante);
    QDateTime get_dhHashComprovante() const;
    void set_dhHashComprovante(const QDateTime &dhHashComprovante);

    std::shared_ptr<DestEPEC> dest;
    std::shared_ptr<Container<ItemPedProrrog>> itemPedido;
    std::shared_ptr<AutXMLAtor> autXML;
private:
    QString m_versao;
    QString m_descEvento;
    int m_cOrgaoAutor;
    TpAutor m_tpAutor;
    QString m_verAplic;
    QString m_nProt;
    QString m_xJust;
    QString m_chNFeRef;
    QString m_xCorrecao;
    QString m_xCondUso;
    //EPEC
    QDateTime m_dhEmi;
    TpNF m_tpNF;
    QString m_IE;
    double m_vNF;
    double m_vICMS;
    double m_vST;
    //Ator Interessado na NF-e
    TpAutorizacao m_tpAutorizacao;
    //EntregaNFe
    QString m_nDoc;
    QString m_xNome;
    QString m_latGPS;
    QString m_longGPS;
    QString m_hashComprovante;
    QDateTime m_dhHashComprovante;
};

//-----------------------------------------------------

class CPPNFE_EXPORT InfEvento
{
public:
    InfEvento();
    ~InfEvento();
    void clear();
    QString get_Id() const;
    void set_Id(const QString &Id);
    int get_cOrgao() const;
    void set_cOrgao(const int &cOrgao);
    TpAmb get_tpAmb() const;
    void set_tpAmb(const TpAmb &tpAmb);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    QDateTime get_dhEvento() const;
    void set_dhEvento(const QDateTime &dhEvento);
    TpEvento get_tpEvento() const;
    void set_tpEvento(const TpEvento &tpEvento);
    int get_nSeqEvento() const;
    void set_nSeqEvento(const int &nSeqEvento);

    std::shared_ptr<DetEvento> detEvento;

private:
    QString m_Id;
    int m_cOrgao;
    TpAmb m_tpAmb;
    QString m_CNPJ;
    QString m_CPF;
    QString m_chNFe;
    QDateTime m_dhEvento;
    TpEvento m_tpEvento;
    int m_nSeqEvento;

};

//-----------------------------------------------------

class CPPNFE_EXPORT EnvEventoEvento
{
public:
    EnvEventoEvento();
    ~EnvEventoEvento();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_XMLOriginal() const;
    void set_XMLOriginal(const QString &xmlOriginal);
    QString get_XMLAssinado() const;
    void set_XMLAssinado(const QString &xmlAssinado);

    void assinarXML(const ConfigNFe *config);
    void gerarXML();
    QString get_error() const;

    std::shared_ptr<InfEvento> infEvento;

private:
    QString m_versao;
    QString m_xml_original;
    QString m_xml_assinado;
    QString m_error;
};

#endif // ENVEVENTOEVENTO_H
