#include "retconssitnfe.h"


//RetCancNFe---------------------------------------------------

RetCancNFe::RetCancNFe():
    infCanc(new InfEventoRet())
{

}

RetCancNFe::~RetCancNFe()
{

}

void RetCancNFe::clear()
{
    this->m_versao.clear();
    this->infCanc->clear();
}

QString RetCancNFe::get_versao() const
{
    return this->m_versao;
}

void RetCancNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

//ProcEventoNFe---------------------------------------------------

ProcEventoNFe::ProcEventoNFe():
    evento(new TEvento()), retEvento(new RetEvento())
{

}

ProcEventoNFe::~ProcEventoNFe()
{

}

void ProcEventoNFe::clear()
{
    this->m_versao.clear();
    this->evento->clear();
    this->retEvento->clear();
}

QString ProcEventoNFe::get_versao() const
{
    return this->m_versao;
}

void ProcEventoNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

//RetConsSitNFe---------------------------------------------------

RetConsSitNFe::RetConsSitNFe() :
    protNFe(new ProtNFe()), retCancNFe(new RetCancNFe()), procEventoNFe(new Container<ProcEventoNFe>)
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
}

QString RetConsSitNFe::get_versao() const
{
    return this->m_versao;
}

void RetConsSitNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

TpAmb RetConsSitNFe::get_tpAmb() const
{
    return this->m_tpAmb;
}

void RetConsSitNFe::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

QString RetConsSitNFe::get_verAplic() const
{
    return this->m_verAplic;
}

void RetConsSitNFe::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

int RetConsSitNFe::get_cStat() const
{
    return this->m_cStat;
}

void RetConsSitNFe::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

QString RetConsSitNFe::get_xMotivo() const
{
    return this->m_xMotivo;
}

void RetConsSitNFe::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

int RetConsSitNFe::get_cUF() const
{
    return this->m_cUF;
}

void RetConsSitNFe::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}

QDateTime RetConsSitNFe::get_dhRecbto() const
{
    return this->m_dhRecbto;
}

void RetConsSitNFe::set_dhRecbto(const QDateTime &dhRecbto)
{
    this->m_dhRecbto = dhRecbto;
}

QString RetConsSitNFe::get_chNFe() const
{
    return this->m_chNFe;
}

void RetConsSitNFe::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

QString RetConsSitNFe::get_xml() const
{
    return this->m_xml;
}

void RetConsSitNFe::set_xml(const QString &xml)
{
    this->m_xml = xml;
}

void RetConsSitNFe::salvarXML(const QString &caminho, const QString &nomeArquivo)
{
    QString nome;
    if (nomeArquivo.isEmpty())
        nome = this->get_chNFe() + "-consSitNFe"; //se não informar o nome, será salvo com o chNFe
    else
        nome = nomeArquivo;
    CppUtility::saveFile(caminho, nome, TipoArquivo::XML, this->m_xml.toLocal8Bit());
}

