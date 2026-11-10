#ifndef RETEVENTOCANCNFE_H
#define RETEVENTOCANCNFE_H

#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/convnf.h>
#include <CppNFe/retenvevento.h>

// Resultado de um pedido de cancelamento: os dados do lote e do único evento enviado.
class CPPNFE_EXPORT RetEventoCancNFe
{
    friend class CancNFe;
public:
    RetEventoCancNFe();
    ~RetEventoCancNFe();
    void clear();

    // Lote. Quando a SEFAZ recusa o lote, não há evento e get_cStat() permanece 0.
    int get_cStatLote() const;
    QString get_xMotivoLote() const;
    int get_idLote() const;
    QString get_XMLLote() const;

    // Evento. A aceitação fiscal é indicada por get_cStat() igual a 135 ou 155.
    QString get_versao() const;
    QString get_Id() const;
    TpAmb get_tpAmb() const;
    QString get_verAplic() const;
    int get_cOrgao() const;
    int get_cStat() const;
    QString get_xMotivo() const;
    QString get_chNFe() const;
    TpEvento get_tpEvento() const;
    QString get_xEvento() const;
    int get_nSeqEvento() const;
    int get_cOrgaoAutor() const;
    QString get_CNPJDest() const;
    QString get_CPFDest() const;
    QString get_emailDest() const;
    QDateTime get_dhRegEvento() const;
    QString get_nProt() const;
    // procEventoNFe quando homologado; nos demais casos, o retEvento recebido.
    QString get_XML() const;

private:
    void preencher(const RetEnvEvento &retEnvEvento);
    int m_cStatLote;
    QString m_xMotivoLote;
    int m_idLote;
    QString m_xmlLote;
    QString m_versao;
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
    QString m_xml;
};

#endif // RETEVENTOCANCNFE_H
