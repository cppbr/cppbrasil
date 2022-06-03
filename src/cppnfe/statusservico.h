#ifndef STATUSSERVICO_H
#define STATUSSERVICO_H

#include <memory>
#include <QObject>
#include "confignfe.h"
#include "wsnfe.h"
#include "retconsstatserv.h"


#ifdef CPPBRASIL_STATIC
    class StatusServico : public QObject
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT StatusServico : public QObject
#endif
{
    Q_OBJECT
public:
    StatusServico(ConfigNFe* confgNFe);
    ~StatusServico();
    void clear();
    QString get_error() const;
    bool get_status();

    std::shared_ptr<RetConsStatServ> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    ConfigNFe* m_config;
    QString m_error;
};

#endif // STATUSSERVICO_H
