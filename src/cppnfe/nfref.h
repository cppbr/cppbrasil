#ifndef NFREF_H
#define NFREF_H

#include <QObject>
#include <QPointer>

class NFref : public QObject
{
    Q_OBJECT
  private:
    QString m_refNFe;
    QString m_refCTe;
    class RefNF;
  public:
    explicit NFref(QObject *parent = nullptr);
    ~NFref();
    QString get_refNFe() const;
    void set_refNFe(const QString &refNFe);
    QString get_refCTe() const;
    void set_refCTe(const QString &refCTe);
    QScopedPointer<RefNF>  *refNF;
};




#endif // NFREF_H
