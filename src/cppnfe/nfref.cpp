#include "nfref.h"

class NFref::RefNF
{
  private:
    int m_cUF;
    QString m_AAMM;
    QString m_CNPJ;
    int m_mod;
    int m_serie;
    int m_nNF;
  public:
    explicit RefNF(){}

    int get_cUF() const { return this->m_cUF; }
    void set_cUF(const int &cUF) { this->m_cUF = cUF; }
    QString get_AAMM() const { return  this->m_AAMM; }
    void set_AAMM(const QString &AAMM) { this->m_AAMM = AAMM;}
    QString get_CNPJ() const { return this->m_CNPJ; }
    void set_CNPJ(const QString &CNPJ) { this->m_CNPJ = CNPJ; }
    int get_mod() const { return this->m_mod; }
    void set_mod(const int &mod) { this->m_mod = mod; }
    int get_serie() const { return this->m_serie; }
    void set_serie(const int &serie) { this->m_serie = serie; }
    int get_nNF() const { return this->m_nNF; }
    void set_nNF(const int &nNF) { this->m_nNF = nNF; }
};

NFref::NFref(QObject *parent) : QObject(parent)
{
  refNF = new RefNF;
}

NFref::~NFref()
{
  delete refNF;
}

QString NFref::get_refNFe() const
{
  return this->m_refNFe;
}

void NFref::set_refNFe(const QString &refNFe)
{
  this->m_refNFe = refNFe;
}

QString NFref::get_refCTe() const
{
  return this->m_refCTe;
}

void NFref::set_refCTe(const QString &refCTe)
{
  this->m_refCTe = refCTe;
}
