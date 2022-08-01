#ifndef STATUSSERVICO_H
#define STATUSSERVICO_H

#include <memory>
#include <QObject>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/wsnfe.h>
#include <CppBrasil/NFe/retconsstatserv.h>


class CPPNFE_EXPORT StatusServico : public QObject
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
