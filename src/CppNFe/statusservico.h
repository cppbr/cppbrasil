#ifndef STATUSSERVICO_H
#define STATUSSERVICO_H

#include <memory>
#include <QObject>
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/convnf.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/wsnfe.h>
#include <CppNFe/retconsstatserv.h>


class CPPNFE_EXPORT StatusServico : public QObject
{
    Q_OBJECT
public:
    StatusServico(ConfigNFe* confgNFe);
    ~StatusServico();
    void clear();
    bool status();
    QString get_error() const;

    std::unique_ptr<RetConsStatServ> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private:
    ConfigNFe* config; //ponteiro de fora
    QString m_error;
};

#endif // STATUSSERVICO_H
