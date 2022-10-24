#include "retenvevento.h"

//------------------------------------------------------------------------
// InfEventoRet class
//------------------------------------------------------------------------

InfEventoRet::InfEventoRet(): m_cOrgao(0), m_cStat(0), m_tpEvento(TpEvento::None),
   m_nSeqEvento(0), m_cOrgaoAutor(0)
{

}

InfEventoRet::~InfEventoRet()
{

}

void InfEventoRet::clear()
{
    this->m_Id.clear();
    this->m_tpAmb = TpAmb::None;
    this->m_verAplic.clear();
    this->m_cOrgao = 0;
    this->m_cStat = 0;
    this->m_xMotivo.clear();
    this->m_chNFe.clear();
    this->m_tpEvento = TpEvento::None;
    this->m_xEvento.clear();
    this->m_nSeqEvento = 0;
    this->m_cOrgaoAutor = 0;
    this->m_CNPJDest.clear();
    this->m_CPFDest.clear();
    this->m_emailDest.clear();
    CppUtil::clearDateTime(this->m_dhRegEvento);
    this->m_nProt.clear();
    this->m_chNFePend.clear();
}

QString InfEventoRet::get_Id() const
{
    return this->m_Id;
}

void InfEventoRet::set_Id(const QString &Id)
{
    this->m_Id = Id;
}

TpAmb InfEventoRet::get_tpAmb() const
{
    return this->m_tpAmb;
}

void InfEventoRet::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

QString InfEventoRet::get_verAplic() const
{
    return this->m_verAplic;
}

void InfEventoRet::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

int InfEventoRet::get_cOrgao() const
{
    return this->m_cOrgao;
}

void InfEventoRet::set_cOrgao(const int &cOrgao)
{
    this->m_cOrgao = cOrgao;
}

int InfEventoRet::get_cStat() const
{
    return this->m_cStat;
}

void InfEventoRet::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

QString InfEventoRet::get_xMotivo() const
{
    return this->m_xMotivo;
}

void InfEventoRet::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

QString InfEventoRet::get_chNFe() const
{
    return this->m_chNFe;
}

void InfEventoRet::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

TpEvento InfEventoRet::get_tpEvento() const
{
    return this->m_tpEvento;
}

void InfEventoRet::set_tpEvento(const TpEvento &tpEvento)
{
    this->m_tpEvento = tpEvento;
}

QString InfEventoRet::get_xEvento() const
{
    return this->m_xEvento;
}

void InfEventoRet::set_xEvento(const QString &xEvento)
{
    this->m_xEvento = xEvento;
}

int InfEventoRet::get_nSeqEvento() const
{
    return this->m_nSeqEvento;
}

void InfEventoRet::set_nSeqEvento(const int &nSeqEvento)
{
    this->m_nSeqEvento = nSeqEvento;
}

int InfEventoRet::get_cOrgaoAutor() const
{
    return this->m_cOrgaoAutor;
}

void InfEventoRet::set_cOrgaoAutor(const int &cOrgaoAutor)
{
    this->m_cOrgaoAutor = cOrgaoAutor;
}

QString InfEventoRet::get_CNPJDest() const
{
    return this->m_CNPJDest;
}

void InfEventoRet::set_CNPJDest(const QString &CNPJDest)
{
    this->m_CNPJDest = CNPJDest;
}

QString InfEventoRet::get_CPFDest() const
{
    return this->m_CPFDest;
}

void InfEventoRet::set_CPFDest(const QString &CPFDest)
{
    this->m_CPFDest = CPFDest;
}

QString InfEventoRet::get_emailDest() const
{
    return this->m_emailDest;
}

void InfEventoRet::set_emailDest(const QString &emailDest)
{
    this->m_emailDest = emailDest;
}

QDateTime InfEventoRet::get_dhRegEvento() const
{
    return this->m_dhRegEvento;
}

void InfEventoRet::set_dhRegEvento(const QDateTime &dhRegEvento)
{
    this->m_dhRegEvento = dhRegEvento;
}

QString InfEventoRet::get_nProt() const
{
    return this->m_nProt;
}

void InfEventoRet::set_nProt(const QString &nProt)
{
    this->m_nProt = nProt;
}

QString InfEventoRet::get_chNFePend() const
{
    return this->m_chNFePend;
}

void InfEventoRet::set_chNFePend(const QString &chNFePend)
{
    this->m_chNFePend = chNFePend;
}

//------------------------------------------------------------------------
// RetEvento class
//------------------------------------------------------------------------

RetEvento::RetEvento(): infEvento(new InfEventoRet())
{

}

RetEvento::~RetEvento()
{

}

void RetEvento::clear()
{
    this->infEvento->clear();
    this->m_versao.clear();
    this->m_xml.clear();
}

QString RetEvento::get_versao() const
{
    return this->m_versao;
}

void RetEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

QString RetEvento::get_XML() const
{
    return this->m_xml;
}

void RetEvento::set_XML(const QString &xml)
{
    this->m_xml = xml;
}

void RetEvento::salvarXML(const QString &caminho, const QString &nomeArquivo)
{
    QString nome;
    if (nomeArquivo.isEmpty())
        nome = this->infEvento->get_Id() + "-procEventoNFe"; //se não informar o nome, será salvo com o Id retornado
    else
        nome = nomeArquivo;
    CppUtility::saveFile(caminho, nome, TipoArquivo::XML, this->m_xml.toLocal8Bit());
}

//------------------------------------------------------------------------
// RetEnvEvento class
//------------------------------------------------------------------------


RetEnvEvento::RetEnvEvento(): retEvento(new Container<RetEvento>), m_idLote(0),
    m_tpAmb(TpAmb::None), m_cOrgao(0), m_cStat(0)
{

}

RetEnvEvento::~RetEnvEvento()
{

}

void RetEnvEvento::clear()
{
    this->m_versao.clear();
    this->m_idLote = 0;
    this->m_tpAmb = TpAmb::None;
    this->m_verAplic.clear();
    this->m_cOrgao = 0;
    this->m_cStat = 0;
    this->m_xMotivo.clear();
}

QString RetEnvEvento::get_versao() const
{
    return this->m_versao;
}

void RetEnvEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

TpAmb RetEnvEvento::get_tpAmb() const
{
    return this->m_tpAmb;
}

void RetEnvEvento::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

int RetEnvEvento::get_idLote() const
{
    return this->m_idLote;
}

void RetEnvEvento::set_idLote(const int &idLote)
{
    this->m_idLote = idLote;
}

QString RetEnvEvento::get_verAplic() const
{
    return this->m_verAplic;
}

void RetEnvEvento::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

int RetEnvEvento::get_cOrgao() const
{
    return this->m_cOrgao;
}

void RetEnvEvento::set_cOrgao(const int &cOrgao)
{
    this->m_cOrgao = cOrgao;
}

int RetEnvEvento::get_cStat() const
{
    return this->m_cStat;
}

void RetEnvEvento::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

QString RetEnvEvento::get_xMotivo() const
{
    return this->m_xMotivo;
}

void RetEnvEvento::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

