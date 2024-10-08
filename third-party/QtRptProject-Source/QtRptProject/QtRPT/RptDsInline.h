#ifndef RPTDSINLINE_H
#define RPTDSINLINE_H

#include <QObject>
#include "RptDsAbstract.h"
#include <qtrpt_global.h>

class QTRPTSHARED_EXPORT RptDsInline : public RptDsAbstract
{
    Q_OBJECT
public:
    explicit RptDsInline(QObject *parent = nullptr);
    virtual void loadXML(QDomElement dsElement) override;
    virtual QString getFieldValue(QString fieldName, int recNo) override;

signals:

public slots:
};

#endif // RPTDSINLINE_H
