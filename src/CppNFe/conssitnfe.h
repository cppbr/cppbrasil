#ifndef CONSSITNFE_H
#define CONSSITNFE_H

#include <memory>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <CppBase/cppbrasil_global.h>
#include <CppBase/cpputility.h>
#include <CppBase/container.h>
#include <CppNFe/convnf.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/notafiscal.h>
#include <CppNFe/wsnfe.h>
#include <CppNFe/retconssitnfe.h>

class CPPNFE_EXPORT ConsSitNFe: public QObject
{
    Q_OBJECT
public:
    ConsSitNFe(ConfigNFe* confgNFe, NotaFiscal* notafiscal);
    ~ConsSitNFe();
    void clear();
    bool consultar(const QString &chNFe, const bool &salvarXMLNota = false, const bool &vincularEvento = false);
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_error() const;

    std::unique_ptr<RetConsSitNFe> retorno;

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
    void tratarRetorno(const bool &salvarXMLNota = false, const bool &vincularEvento = false);

};

#endif // CONSSITNFE_H
