#include "cancnfe.h"
#include <QRegularExpression>
#include <CppBrasil/NFe/wsnfe.h>

CancNFe::CancNFe(ConfigNFe *confgNFe) :
    evento(new TEvento()), retorno(new RetEnvEvento()), config(confgNFe)
{
}

CancNFe::~CancNFe()
{
}

void CancNFe::clear()
{
    evento->clear();
    retorno->clear();
    m_error.clear();
    m_xml.clear();
    m_versao.clear();
}

bool CancNFe::cancelar(const int &idLote)
{
    retorno->clear();
    if (!gerarXML(idLote) || !validarEvento())
    {
        emit errorOccurred(m_error);
        return false;
    }

    WSEvento ws(config, config->certificado->get_cryptoLib(), retorno.get());
    connect(&ws, &WSEvento::wsChange, this, &CancNFe::wsChange);
    connect(&ws, &WSEvento::errorOccurred, this, &CancNFe::onError);
    if (!ws.send(m_xml.toUtf8(), m_versao))
        return false;

    if (!tratarRetorno())
    {
        emit errorOccurred(m_error);
        return false;
    }
    return true;
}

QString CancNFe::get_versao() const
{
    return m_versao;
}

void CancNFe::set_versao(const QString &versao)
{
    m_versao = versao;
}

QString CancNFe::get_error() const
{
    return m_error;
}

void CancNFe::onError(const QString &error)
{
    m_error = error;
    emit errorOccurred(error);
}

bool CancNFe::prepararEvento()
{
    auto *inf = evento->infEvento.get();
    auto *det = inf->detEvento.get();
    if (m_versao.isEmpty())
        m_versao = "1.00";
    if (inf->get_tpEvento() == TpEvento::None)
        inf->set_tpEvento(TpEvento::Cancelamento);
    const bool substituicao = inf->get_tpEvento() == TpEvento::CancelamentoSubstituicao;
    const QString chave = inf->get_chNFe();
    static const QRegularExpression chaveNumerica("^[0-9]{44}$");

    if (inf->get_tpEvento() != TpEvento::Cancelamento && !substituicao)
        m_error = "CancNFe aceita somente cancelamento (110111) ou cancelamento por substituição (110112).";
    else if (m_versao != "1.00")
        m_error = "Cancelamento exige a versão 1.00.";
    else if (!chaveNumerica.match(chave).hasMatch())
        m_error = "Informe a chave da NF-e/NFC-e com 44 dígitos numéricos.";
    else if (chave.mid(20, 2) != "55" && chave.mid(20, 2) != "65")
        m_error = "Cancelamento exige uma chave de NF-e modelo 55 ou NFC-e modelo 65.";
    else if (chave.mid(20, 2) != ConvNF::modeloDFToStr(config->get_ModeloDF()))
        m_error = "O modelo da chave diverge do modelo configurado.";
    else if (chave.left(2).toInt() != ConvNF::ufToInt(config->webservices->get_uf()))
        m_error = "A UF da chave diverge da UF configurada.";
    else if (inf->get_tpAmb() != TpAmb::None && inf->get_tpAmb() != config->webservices->get_tpAmb())
        m_error = "O ambiente do evento diverge do ambiente configurado.";
    else if (inf->get_nSeqEvento() != 0 && inf->get_nSeqEvento() != 1)
        m_error = "Cancelamento exige nSeqEvento igual a 1.";
    else if (substituicao && chave.mid(20, 2) != "65")
        m_error = "Cancelamento por substituição é exclusivo da NFC-e modelo 65.";
    else if (substituicao && chave.mid(34, 1) != "1")
        m_error = "A NFC-e a ser cancelada por substituição deve ter emissão normal.";
    else if (substituicao && (!chaveNumerica.match(det->get_chNFeRef()).hasMatch() ||
             det->get_chNFeRef() == chave || det->get_chNFeRef().left(2) != chave.left(2) ||
             det->get_chNFeRef().mid(6, 16) != chave.mid(6, 16) ||
             det->get_chNFeRef().mid(34, 1) == "1"))
        m_error = "Informe uma chave substituta distinta, da mesma UF, emitente e modelo, emitida em contingência.";

    if (!m_error.isEmpty())
        return false;

    evento->set_versao(m_versao);
    inf->set_verEvento(m_versao);
    inf->set_nSeqEvento(1);
    inf->set_Id("ID" + ConvNF::tpEventoToStr(inf->get_tpEvento()) + chave + "01");
    if (inf->get_cOrgao() == 0)
        inf->set_cOrgao(ConvNF::ufToInt(config->webservices->get_uf()));
    if (inf->get_tpAmb() == TpAmb::None)
        inf->set_tpAmb(config->webservices->get_tpAmb());
    if (inf->get_dhEvento().isNull())
        inf->set_dhEvento(QDateTime::currentDateTime());
    det->set_descEvento(substituicao ? "Cancelamento por substituicao" : "Cancelamento");
    if (substituicao)
    {
        if (det->get_cOrgaoAutor() == 0)
            det->set_cOrgaoAutor(chave.left(2).toInt());
        if (det->get_tpAutor() == TpAutor::None)
            det->set_tpAutor(TpAutor::EmpresaEmitente);
        if (det->get_cOrgaoAutor() != chave.left(2).toInt() || det->get_tpAutor() != TpAutor::EmpresaEmitente)
        {
            m_error = "Cancelamento por substituição exige cOrgaoAutor da UF da chave e tpAutor igual a 1.";
            return false;
        }
    }
    return true;
}

bool CancNFe::gerarXML(const int &idLote)
{
    m_error.clear();
    m_xml.clear();
    evento->set_XMLOriginal(QString());
    evento->set_XMLAssinado(QString());
    if (idLote < 0)
    {
        m_error = "O identificador do lote não pode ser negativo.";
        return false;
    }
    if (!prepararEvento())
        return false;
    if (!config->certificado->get_cryptoLib())
    {
        m_error = "Carregue o certificado digital antes de cancelar a NF-e/NFC-e.";
        return false;
    }
    evento->gerarXML();
    if (evento->get_error().isEmpty())
        evento->assinarXML(config);
    if (!evento->get_error().isEmpty())
    {
        m_error = evento->get_error();
        return false;
    }
    m_xml = "<envEvento xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"" + m_versao + "\">"
            "<idLote>" + CppUtil::insZeroLeft(QString::number(idLote), 15) + "</idLote>" +
            CppUtility::extractStr(evento->get_XMLAssinado(), "<evento", "/evento>") + "</envEvento>";
    return true;
}

QString CancNFe::get_schemaName(const QString &prefixo) const
{
    return prefixo + (evento->infEvento->get_tpEvento() == TpEvento::CancelamentoSubstituicao
        ? "CancSubst_v" : "CancNFe_v") + m_versao + ".xsd";
}

bool CancNFe::validarEvento()
{
    CppLibXml2 xml(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    m_error.clear();
    if (!xml.parseXML(m_xml.toUtf8(), get_schemaName("envEvento").toUtf8()))
    {
        m_error = xml.get_error();
        return false;
    }
    return true;
}

bool CancNFe::tratarRetorno()
{
    // Lote processado não significa que o cancelamento foi homologado.
    if (retorno->get_cStat() != 128)
        return true;
    auto *inf = evento->infEvento.get();
    for (RetEvento *ret : *retorno->retEvento)
    {
        auto *infoRet = ret->infEvento.get();
        if (infoRet->get_cStat() != 135 && infoRet->get_cStat() != 155)
            continue;
        if (infoRet->get_chNFe() != inf->get_chNFe() || infoRet->get_tpEvento() != inf->get_tpEvento() ||
            infoRet->get_nSeqEvento() != inf->get_nSeqEvento() || infoRet->get_tpAmb() != inf->get_tpAmb())
        {
            m_error = "O retorno de cancelamento não corresponde ao evento enviado.";
            return false;
        }
        QByteArray processado = ("<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"" +
                                m_versao + "\"></procEventoNFe>").toUtf8();
        const QByteArray original = ("<a>" + CppUtility::extractStr(evento->get_XMLAssinado(),
                                    "<evento", "/evento>") + "</a>").toUtf8();
        const QByteArray resposta = ("<a>" + CppUtility::extractStr(ret->get_XML(),
                                    "<retEvento", "/retEvento>") + "</a>").toUtf8();
        CppLibXml2 xml(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
        if (!xml.addChildXML(processado, original, "procEventoNFe") ||
            !xml.addChildXML(processado, resposta, "procEventoNFe") ||
            !xml.parseXML(processado, get_schemaName("procEvento").toUtf8()))
        {
            m_error = xml.get_error();
            return false;
        }
        ret->set_XML(QString::fromUtf8(processado));
        if (config->arquivos->get_salvar())
            ret->salvarXML(config->arquivos->get_caminhoEvento(), inf->get_Id().mid(2) + "-procEventoNFe");
    }
    return true;
}
