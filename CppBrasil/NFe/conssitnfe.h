#ifndef CONSSITNFE_H
#define CONSSITNFE_H

#include <memory>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/container.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/notafiscal.h>
#include <CppBrasil/NFe/wsnfe.h>
#include <CppBrasil/NFe/retconssitnfe.h>

class CPPNFE_EXPORT ConsSitNFe: public QObject
{
    Q_OBJECT
public:
    ConsSitNFe(ConfigNFe* confgNFe, NotaFiscal* notafiscal);
    ~ConsSitNFe();
    void clear();
    bool consultar(const QString &chNFe, const bool &salvarNotaVinculada = false);
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_error() const;

    std::shared_ptr<RetConsSitNFe> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);
private slots:
    void onError(const QString &error);

private:
    ConfigNFe* config; //ponteiro de fora
    NotaFiscal* notafiscal; //ponteiro de fora
    QString m_error;
    QString m_xml;
    QString m_versao;
    QString m_chNFe;
    bool gerarXML(const QString &chNFe);
    bool validarEvento();
    void tratarRetorno(const bool &salvarNotaVinculada = false);

};

#endif // CONSSITNFE_H
