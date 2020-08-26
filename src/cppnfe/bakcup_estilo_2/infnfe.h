#ifndef INFNFE_H
#define INFNFE_H

#include <QObject>

class InfNFe : public QObject
{
    Q_OBJECT
  private:
    QString m_versao;
    QString m_Id;
  public:
    explicit InfNFe(QObject *parent = nullptr);
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_Id() const;
    void set_Id(const QString &id);
};

#endif // INFNFE_H
