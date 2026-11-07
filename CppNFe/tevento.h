#ifndef TEVENTO_H
#define TEVENTO_H


#include <memory>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QXmlStreamReader>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/cpputility.h>
#include <CppBase/container.h>
#include <CppNFe/convnf.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/detpag.h>


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

// Respostas do Fisco (NT 2015.001). Os códigos são os do leiaute.
class CPPNFE_EXPORT ItemRespPedido
{
public:
    ItemRespPedido();
    void clear();
    int get_numItem() const;
    void set_numItem(const int &valor);
    int get_statPedido() const;
    void set_statPedido(const int &valor);
    int get_justStatus() const;
    void set_justStatus(const int &valor);
    QString get_justStaOutra() const;
    void set_justStaOutra(const QString &valor);
private:
    int m_numItem;
    int m_statPedido;
    int m_justStatus;
    QString m_justStaOutra;
};

class CPPNFE_EXPORT RespPedido
{
public:
    RespPedido();
    void clear();
    int get_statPrazo() const;
    void set_statPrazo(const int &valor);
    std::unique_ptr<Container<ItemRespPedido>> itemPedido;
private:
    int m_statPrazo; // -1: não informado; 0: após o prazo; 1: dentro do prazo.
};

class CPPNFE_EXPORT RespCancPedido
{
public:
    RespCancPedido();
    void clear();
    int get_statCancPedido() const;
    void set_statCancPedido(const int &valor);
    int get_justStatus() const;
    void set_justStatus(const int &valor);
    QString get_justStaOutra() const;
    void set_justStaOutra(const QString &valor);
private:
    int m_statCancPedido;
    int m_justStatus;
    QString m_justStaOutra;
};

//-----------------------------------------------------

class CPPNFE_EXPORT DestEPEC
{
public:
    DestEPEC();
    ~DestEPEC();
    void clear();
    int get_UF() const;
    void set_UF(const int &uf); // Código IBGE da UF ou 99 para o exterior.
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
    QString get_idPedidoCancelado() const;
    void set_idPedidoCancelado(const QString &valor);
    QString get_idPedido() const;
    void set_idPedido(const QString &valor);
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
    QDateTime get_dhEntrega() const;
    void set_dhEntrega(const QDateTime &dhEntrega);
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
    QString get_nProtEvento() const;
    void set_nProtEvento(const QString &nProtEvento);
    QDateTime get_dhTentativaEntrega() const;
    void set_dhTentativaEntrega(const QDateTime &dhTentativaEntrega);
    int get_nTentativa() const;
    void set_nTentativa(const int &nTentativa);
    TpMotivo get_tpMotivo() const;
    void set_tpMotivo(const TpMotivo &tpMotivo);
    QString get_xJustMotivo() const;
    void set_xJustMotivo(const QString &xJustMotivo);
    QString get_hashTentativaEntrega() const;
    void set_hashTentativaEntrega(const QString &hashTentativaEntrega);
    QDateTime get_dhHashTentativaEntrega() const;
    void set_dhHashTentativaEntrega(const QDateTime &dhHashTentativaEntrega);

    std::unique_ptr<DestEPEC> dest;
    std::unique_ptr<Container<ItemPedProrrog>> itemPedido;
    std::unique_ptr<AutXMLAtor> autXML;
    std::unique_ptr<Container<DetPag>> detPag;
    std::unique_ptr<RespPedido> respPedido;
    std::unique_ptr<RespCancPedido> respCancPedido;
private:
    friend class TEvento;
    friend class WSConsultaProtocolo;
    bool lerProrrogacao(QXmlStreamReader &xml);
    QString m_idPedidoCancelado;
    QString m_idPedido;
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
    QDateTime m_dhEntrega;
    QString m_nDoc;
    QString m_xNome;
    QString m_latGPS;
    QString m_longGPS;
    QString m_hashComprovante;
    QDateTime m_dhHashComprovante;
    QString m_nProtEvento;
    //Insucesso na Entrega da NF-e
    QDateTime m_dhTentativaEntrega;
    int m_nTentativa;
    TpMotivo m_tpMotivo;
    QString m_xJustMotivo;
    QString m_hashTentativaEntrega;
    QDateTime m_dhHashTentativaEntrega;

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
    QString get_verEvento() const;
    void set_verEvento(const QString &verEvento);

    std::unique_ptr<DetEvento> detEvento;

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
    QString m_verEvento;
};

//-----------------------------------------------------

class CPPNFE_EXPORT TEvento
{
public:
    TEvento();
    ~TEvento();
    void clear();
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_XMLOriginal() const;
    void set_XMLOriginal(const QString &xmlOriginal);
    QString get_XMLAssinado() const;
    void set_XMLAssinado(const QString &xmlAssinado);

    void assinarXML(const ConfigNFe *config);
    void gerarXML();
    // Importa evento/procEventoNFe da NT 2015.001 e valida envelope e detalhe por XSD.
    // Não consulta NFeDistribuicaoDFe nem verifica a assinatura digital do XML importado.
    bool lerXMLProrrogacao(const QString &xml, const ConfigNFe *config);
    bool validarProrrogacao(const ConfigNFe *config);
    QString get_error() const;

    std::unique_ptr<InfEvento> infEvento;

private:
    bool validarDadosManifestacao();
    bool validarDadosProrrogacao();
    QString m_versao;
    QString m_xml_original;
    QString m_xml_assinado;
    QString m_error;
};

#endif // TEVENTO_H
