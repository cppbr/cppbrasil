#include "reteventocancnfe.h"

RetEventoCancNFe::RetEventoCancNFe()
{
    clear();
}

RetEventoCancNFe::~RetEventoCancNFe()
{
}

void RetEventoCancNFe::clear()
{
    m_cStatLote = 0;
    m_xMotivoLote.clear();
    m_idLote = 0;
    m_xmlLote.clear();
    m_versao.clear();
    m_Id.clear();
    m_tpAmb = TpAmb::None;
    m_verAplic.clear();
    m_cOrgao = 0;
    m_cStat = 0;
    m_xMotivo.clear();
    m_chNFe.clear();
    m_tpEvento = TpEvento::None;
    m_xEvento.clear();
    m_nSeqEvento = 0;
    m_cOrgaoAutor = 0;
    m_CNPJDest.clear();
    m_CPFDest.clear();
    m_emailDest.clear();
    m_dhRegEvento = QDateTime();
    m_nProt.clear();
    m_xml.clear();
}

void RetEventoCancNFe::preencher(const RetEnvEvento &retEnvEvento)
{
    m_cStatLote = retEnvEvento.get_cStat();
    m_xMotivoLote = retEnvEvento.get_xMotivo();
    m_idLote = retEnvEvento.get_idLote();
    const RetEvento *ret = retEnvEvento.retEvento->value(0);
    if (!ret)
        return;
    const InfEventoRet *inf = ret->infEvento.get();
    m_versao = ret->get_versao();
    m_Id = inf->get_Id();
    m_tpAmb = inf->get_tpAmb();
    m_verAplic = inf->get_verAplic();
    m_cOrgao = inf->get_cOrgao();
    m_cStat = inf->get_cStat();
    m_xMotivo = inf->get_xMotivo();
    m_chNFe = inf->get_chNFe();
    m_tpEvento = inf->get_tpEvento();
    m_xEvento = inf->get_xEvento();
    m_nSeqEvento = inf->get_nSeqEvento();
    m_cOrgaoAutor = inf->get_cOrgaoAutor();
    m_CNPJDest = inf->get_CNPJDest();
    m_CPFDest = inf->get_CPFDest();
    m_emailDest = inf->get_emailDest();
    m_dhRegEvento = inf->get_dhRegEvento();
    m_nProt = inf->get_nProt();
    m_xml = ret->get_XML();
}

int RetEventoCancNFe::get_cStatLote() const
{
    return m_cStatLote;
}

QString RetEventoCancNFe::get_xMotivoLote() const
{
    return m_xMotivoLote;
}

int RetEventoCancNFe::get_idLote() const
{
    return m_idLote;
}

QString RetEventoCancNFe::get_XMLLote() const
{
    return m_xmlLote;
}

QString RetEventoCancNFe::get_versao() const
{
    return m_versao;
}

QString RetEventoCancNFe::get_Id() const
{
    return m_Id;
}

TpAmb RetEventoCancNFe::get_tpAmb() const
{
    return m_tpAmb;
}

QString RetEventoCancNFe::get_verAplic() const
{
    return m_verAplic;
}

int RetEventoCancNFe::get_cOrgao() const
{
    return m_cOrgao;
}

int RetEventoCancNFe::get_cStat() const
{
    return m_cStat;
}

QString RetEventoCancNFe::get_xMotivo() const
{
    return m_xMotivo;
}

QString RetEventoCancNFe::get_chNFe() const
{
    return m_chNFe;
}

TpEvento RetEventoCancNFe::get_tpEvento() const
{
    return m_tpEvento;
}

QString RetEventoCancNFe::get_xEvento() const
{
    return m_xEvento;
}

int RetEventoCancNFe::get_nSeqEvento() const
{
    return m_nSeqEvento;
}

int RetEventoCancNFe::get_cOrgaoAutor() const
{
    return m_cOrgaoAutor;
}

QString RetEventoCancNFe::get_CNPJDest() const
{
    return m_CNPJDest;
}

QString RetEventoCancNFe::get_CPFDest() const
{
    return m_CPFDest;
}

QString RetEventoCancNFe::get_emailDest() const
{
    return m_emailDest;
}

QDateTime RetEventoCancNFe::get_dhRegEvento() const
{
    return m_dhRegEvento;
}

QString RetEventoCancNFe::get_nProt() const
{
    return m_nProt;
}

QString RetEventoCancNFe::get_XML() const
{
    return m_xml;
}
