#ifndef CANCNFE_H
#define CANCNFE_H

#include <memory>
#include <QDateTime>
#include <QObject>
#include <CppBase/cppbrasil_global.h>
#include <CppNFe/confignfe.h>
#include <CppNFe/convnf.h>
#include <CppNFe/retenvevento.h>
#include <CppNFe/reteventocancnfe.h>

// Cancelamento (110111) e cancelamento por substituição da NFC-e (110112).
class CPPNFE_EXPORT CancNFe : public QObject
{
    Q_OBJECT
public:
    CancNFe(ConfigNFe *confgNFe);
    ~CancNFe();
    void clear();
    // Envia um cancelamento. Consultar retorno para verificar a aceitação fiscal.
    bool cancelar(const int &idLote = 1);

    // Sem informar, o tipo é TpEvento::Cancelamento.
    TpEvento get_tpEvento() const;
    void set_tpEvento(const TpEvento &tpEvento);
    QString get_CNPJ() const;
    void set_CNPJ(const QString &CNPJ);
    QString get_CPF() const;
    void set_CPF(const QString &CPF);
    QString get_chNFe() const;
    void set_chNFe(const QString &chNFe);
    // Sem informar, usa o horário do envio.
    QDateTime get_dhEvento() const;
    void set_dhEvento(const QDateTime &dhEvento);
    QString get_nProt() const;
    void set_nProt(const QString &nProt);
    QString get_xJust() const;
    void set_xJust(const QString &xJust);
    // Exclusivos do cancelamento por substituição.
    QString get_chNFeRef() const;
    void set_chNFeRef(const QString &chNFeRef);
    QString get_verAplic() const;
    void set_verAplic(const QString &verAplic);

    QString get_versao() const;
    void set_versao(const QString &versao);
    QString get_Id() const;
    QString get_XMLOriginal() const;
    QString get_XMLAssinado() const;
    QString get_error() const;

    std::unique_ptr<RetEventoCancNFe> retorno;

signals:
    void errorOccurred(const QString &error);
    void wsChange(const WebServicesNF &webServicesNF);

private slots:
    void onError(const QString &error);

private:
    ConfigNFe *config; // ponteiro de fora
    std::unique_ptr<RetEnvEvento> m_retEnvEvento; // preenchido pelo web service
    TpEvento m_tpEvento;
    QString m_CNPJ;
    QString m_CPF;
    QString m_chNFe;
    QDateTime m_dhEvento;
    QString m_nProt;
    QString m_xJust;
    QString m_chNFeRef;
    QString m_verAplic;
    QString m_Id;
    QString m_error;
    QString m_xml;
    QString m_xmlOriginal;
    QString m_xmlAssinado;
    QString m_versao;
    bool substituicao() const;
    bool gerarXML(const int &idLote);
    bool prepararEvento();
    bool validarSubstituta();
    void gerarEvento();
    bool assinarEvento();
    bool validarEvento();
    bool tratarRetorno(const QByteArray &mensagem);
    QString get_schemaName(const QString &prefixo) const;
};

#endif // CANCNFE_H
