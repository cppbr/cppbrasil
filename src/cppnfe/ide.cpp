#include "ide.h"

Ide::Ide(QObject *parent) : QObject(parent)
{
}

int Ide::get_cUF() const
{
  return this->m_cUF;
}

void Ide::set_cUF(const int &cUF)
{
  this->m_cUF = cUF;
}

int Ide::get_cNF() const
{
  return  this->m_cNF;
}

void Ide::set_cNF(const int &cNF)
{
  this->m_cNF = cNF;
}

QString Ide::get_natOp() const
{
  return this->m_natOp;
}

void Ide::set_natOp(const QString &natOp)
{
  this->m_natOp = natOp;
}

int Ide::get_mod() const
{
  return this->m_mod;
}

void Ide::set_mod(const int &mod)
{
  this->m_mod = mod;
}

int Ide::get_serie() const
{
  return this->m_serie;
}

void Ide::set_serie(const int &serie)
{
  this->m_serie = serie;
}

int Ide::get_nNF() const
{
  return this->m_nNF;
}

void Ide::set_nNF(const int &nNF)
{
  this->m_nNF = nNF;
}

QDateTime Ide::get_dhEmi() const
{
  return this->m_dhEmi;
}

void Ide::set_dhEmi(const QDateTime &dhEmi)
{
  this->m_dhEmi = dhEmi;
}

QDateTime Ide::get_dhSaiEnt() const
{
  return this->m_dhSaiEnt;
}

void Ide::set_dhSaiEnt(const QDateTime &dhSaiEnt)
{
  this->m_dhSaiEnt = dhSaiEnt;
}

TipoOperacao Ide::get_tpNF() const
{
  return this->m_tpNF;
}

void Ide::set_tpNF(const TipoOperacao &tpNF)
{
  this->m_tpNF = tpNF;
}

DestinoOperacao Ide::get_idDest() const
{
  return this->m_idDest;
}

void Ide::set_idDest(const DestinoOperacao &idDest)
{
  this->m_idDest = idDest;
}

int Ide::get_cMunFG() const
{
  return this->m_cMunFG;
}

void Ide::set_cMunFG(const int &cMunFG)
{
  this->m_cMunFG = cMunFG;
}

FormatoImpressao Ide::get_tpImp() const
{
  return this->m_tpImp;
}

void Ide::set_tpImp(const FormatoImpressao &tpImp)
{
  this->m_tpImp = tpImp;
}

TipoEmissao Ide::get_tpEmis() const
{
  return this->m_tpEmis;
}

void Ide::set_tpEmis(const TipoEmissao &tpEmis)
{
  this->m_tpEmis = tpEmis;
}

QString Ide::get_cDV() const
{
  return this->m_cDV;
}

void Ide::set_cDV(const QString &cDV)
{
  this->m_cDV = cDV;
}

Ambiente Ide::get_tpAmb() const
{
  return this->m_tpAmb;
}

void Ide::set_tpAmb(const Ambiente &tpAmb)
{
  this->m_tpAmb = tpAmb;
}

FinalidadeNfe Ide::get_finNFe() const
{
  return this->m_finNFe;
}

void Ide::set_finNFe(const FinalidadeNfe &finNFe)
{
  this->m_finNFe = finNFe;
}

ConsumidorFinal Ide::get_indFinal() const
{
  return this->m_indFinal;
}

void Ide::set_indFinal(const ConsumidorFinal &indFinal)
{
  this->m_indFinal = indFinal;
}

IndicadorPresenca Ide::get_indPres() const
{
  return this->m_indPres;
}

void Ide::set_indPres(const IndicadorPresenca &indPres)
{
  this->m_indPres = indPres;
}

ProcessoEmissao Ide::get_procEmi() const
{
  return this->m_procEmi;
}

void Ide::set_procEmi(const ProcessoEmissao &procEmi)
{
  this->m_procEmi = procEmi;
}

QString Ide::get_verProc() const
{
  return this->m_verProc;
}

void Ide::set_verProc(const QString &verProc)
{
  this->m_verProc = verProc;
}

QDateTime Ide::get_dhCont() const
{
  return this->m_dhCont;
}

void Ide::set_dhCont(const QDateTime &dhCont)
{
  this->m_dhCont = dhCont;
}

QString Ide::get_xJust() const
{
  return this->m_xJust;
}

void Ide::set_xJust(const QString &xJust)
{
  this->m_xJust = xJust;
}



