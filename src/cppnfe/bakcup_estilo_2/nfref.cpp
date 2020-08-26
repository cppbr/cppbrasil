#include "nfref.h"

class NFref::RefNF
{
  public:
    explicit RefNF(){}

    int cUF;
    QString AAMM;
    QString CNPJ;
    int mod;
    int serie;
    int nNF;
};

class NFref::RefNFP
{
  public:
    explicit RefNFP(){}

    int cUF;
    QString AAMM;
    QString CNPJ;
    QString CPF;
    QString IE;
    int mod;
    int serie;
    int nNF;
};

class NFref::RefECF
{
  public:
    explicit RefECF(){}

    int mod;
    int nECF;
    int nCOO;
};

NFref::NFref(QObject *parent) : QObject(parent),
refNF(new RefNF), refNFP(new RefNFP), efECF(new RefECF)
{
}

NFref::~NFref()
{
  delete refNF;
  delete refNFP;
  delete efECF;
}
