#ifndef CPPDANFEQTRPT_H
#define CPPDANFEQTRPT_H

#include <memory>
#include <QString>
#include "cppdanfeqtrpt_p.h"
#include "cppdanfe.h"
#include "qtrpt.h"


class CPPDANFEQTRPT_EXPORT CppDanfeQtRpt: public QObject, public CppDanfe
{
    Q_OBJECT

public:
    CppDanfeQtRpt(const CppNFe *cppnfe, const int &recNo = 0);
    virtual ~CppDanfeQtRpt();

    virtual void caminhoArquivo(const QString &arquivo);
    virtual void caminhoLogo(const QString &logo);
    virtual void print();

private:
    std::shared_ptr<QtRPT> m_report;
    void loadRetEnt();
private slots:
    void setDSInfo(DataSetInfo &dsInfo);
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setField(RptFieldObject &fieldObject);
    void setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage);

};

#endif // CPPDANFEQTRPT_H
