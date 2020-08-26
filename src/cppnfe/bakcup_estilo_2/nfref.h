#ifndef NFREF_H
#define NFREF_H

#include <QObject>

class NFref : public QObject
{
    Q_OBJECT
  private:
    class RefNF;
    class RefNFP;
    class RefECF;
  public:
    explicit NFref(QObject *parent = nullptr);
    ~NFref();
    QString refNFe;
    QString refCTe;
    RefNF *refNF;
    RefNFP *refNFP;
    RefECF *efECF;
};




#endif // NFREF_H
