#include "retconssitnfe.h"

RetConsSitBase::RetConsSitBase():
    m_tpAmb{TpAmb::None}, m_cStat{0}, m_cUF{0}
{

}

RetConsSitBase::~RetConsSitBase()
{

}

void RetConsSitBase::clear()
{
    m_versao.clear();
    m_tpAmb = TpAmb::None;
    m_verAplic.clear();
    m_cStat = 0;
    m_xMotivo.clear();
    m_cUF = 0;
    CppUtil::clearDateTime(m_dhRecbto);
    m_chNFe.clear();
}

QString RetConsSitBase::get_versao() const
{
    return this->m_versao;
}

void RetConsSitBase::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

TpAmb RetConsSitBase::get_tpAmb() const
{
    return this->m_tpAmb;
}

void RetConsSitBase::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

QString RetConsSitBase::get_verAplic() const
{
    return this->m_verAplic;
}

void RetConsSitBase::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

int RetConsSitBase::get_cStat() const
{
    return this->m_cStat;
}

void RetConsSitBase::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

QString RetConsSitBase::get_xMotivo() const
{
    return this->m_xMotivo;
}

void RetConsSitBase::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

int RetConsSitBase::get_cUF() const
{
    return this->m_cUF;
}

void RetConsSitBase::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}

QDateTime RetConsSitBase::get_dhRecbto() const
{
    return this->m_dhRecbto;
}

void RetConsSitBase::set_dhRecbto(const QDateTime &dhRecbto)
{
    this->m_dhRecbto = dhRecbto;
}

QString RetConsSitBase::get_chNFe() const
{
    return this->m_chNFe;
}

void RetConsSitBase::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

//RetCancNFe---------------------------------------------------

RetCancNFe::RetCancNFe() :
   RetConsSitBase()
{

}

RetCancNFe::~RetCancNFe()
{

}

void RetCancNFe::clear()
{
    RetConsSitBase::clear();
    this->m_nProt.clear();
}

QString RetCancNFe::get_nProt() const
{
    return this->m_nProt;
}

void RetCancNFe::set_nProt(const QString &nProt)
{
    this->m_nProt = nProt;
}

//ProcEventoNFe---------------------------------------------------

ProcEventoNFe::ProcEventoNFe() :
    evento(new EnvEventoEvento()), retEvento(new RetEvento)
{

}

ProcEventoNFe::~ProcEventoNFe()
{

}

void ProcEventoNFe::clear()
{
    this->evento->clear();
    this->retEvento->clear();
    this->m_versao.clear();
}

//RetConsSitNFe---------------------------------------------------

RetConsSitNFe::RetConsSitNFe() :
    RetConsSitBase(), protNFe(new ProtNFe()), retCancNFe(new RetCancNFe()), procEventoNFe(new ProcEventoNFe())
{

}

RetConsSitNFe::~RetConsSitNFe()
{

}

void RetConsSitNFe::clear()
{
    protNFe->clear();
    retCancNFe->clear();
    procEventoNFe->clear();
    RetConsSitBase::clear();
}
