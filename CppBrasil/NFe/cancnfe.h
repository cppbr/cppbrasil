#ifndef CANCNFE_H
#define CANCNFE_H

#include <memory>
#include <QObject>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/retenvevento.h>
#include <CppBrasil/NFe/tevento.h>

class CPPNFE_EXPORT CancNFe : public QObject
{
    Q_OBJECT
public:
    CancNFe(ConfigNFe *confgNFe);
    ~CancNFe();
    void clear();
    // Envia um cancelamento. Consultar retorno para verificar a aceitação fiscal.
    bool cancelar(const int &idLote = 1);
    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_error() const;

    std::unique_ptr<TEvento> evento;
    std::unique_ptr<RetEnvEvento> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private slots:
    void onError(const QString &error);

private:
    ConfigNFe *config; // ponteiro de fora
    QString m_error;
    QString m_xml;
    QString m_versao;
    bool gerarXML(const int &idLote);
    bool prepararEvento();
    bool validarEvento();
    bool tratarRetorno();
    QString get_schemaName(const QString &prefixo) const;
};

#endif // CANCNFE_H
