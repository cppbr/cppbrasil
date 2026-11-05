#include "cancnfe.h"
#include <QRegularExpression>
#include <QXmlStreamWriter>
#include <CppBrasil/NFe/wsnfe.h>

namespace {

const QRegularExpression &chaveNumerica()
{
    static const QRegularExpression regra("^[0-9]{44}$");
    return regra;
}

// Séries a partir de 910 identificam emitente pessoa física na chave.
bool emitenteCPF(const QString &chave)
{
    return chave.mid(22, 3).toInt() >= 910;
}

int anoMes(const QString &chave)
{
    return chave.mid(2, 2).toInt() * 12 + chave.mid(4, 2).toInt();
}

} // namespace

CancNFe::CancNFe(ConfigNFe *confgNFe) :
    retorno(new RetEventoCancNFe()), config(confgNFe), m_retEnvEvento(new RetEnvEvento()),
    m_tpEvento(TpEvento::None)
{
}

CancNFe::~CancNFe()
{
}

void CancNFe::clear()
{
    retorno->clear();
    m_retEnvEvento->clear();
    m_tpEvento = TpEvento::None;
    m_CNPJ.clear();
    m_CPF.clear();
    m_chNFe.clear();
    m_dhEvento = QDateTime();
    m_nProt.clear();
    m_xJust.clear();
    m_chNFeRef.clear();
    m_verAplic.clear();
    m_Id.clear();
    m_error.clear();
    m_xml.clear();
    m_xmlOriginal.clear();
    m_xmlAssinado.clear();
    m_versao.clear();
}

bool CancNFe::cancelar(const int &idLote)
{
    retorno->clear();
    m_retEnvEvento->clear();
    if (!gerarXML(idLote) || !validarEvento())
    {
        emit errorOccurred(m_error);
        return false;
    }

    WSEvento ws(config, config->certificado->get_cryptoLib(), m_retEnvEvento.get());
    connect(&ws, &WSEvento::wsChange, this, &CancNFe::wsChange);
    connect(&ws, &WSEvento::errorOccurred, this, &CancNFe::onError);
    const bool enviado = ws.send(m_xml.toUtf8(), m_versao);
    if (!tratarRetorno(ws.get_mensagemRetorno()))
    {
        emit errorOccurred(m_error);
        return false;
    }
    return enviado;
}

TpEvento CancNFe::get_tpEvento() const
{
    return m_tpEvento;
}

void CancNFe::set_tpEvento(const TpEvento &tpEvento)
{
    m_tpEvento = tpEvento;
}

QString CancNFe::get_CNPJ() const
{
    return m_CNPJ;
}

void CancNFe::set_CNPJ(const QString &CNPJ)
{
    m_CNPJ = CNPJ;
}

QString CancNFe::get_CPF() const
{
    return m_CPF;
}

void CancNFe::set_CPF(const QString &CPF)
{
    m_CPF = CPF;
}

QString CancNFe::get_chNFe() const
{
    return m_chNFe;
}

void CancNFe::set_chNFe(const QString &chNFe)
{
    m_chNFe = chNFe;
}

QDateTime CancNFe::get_dhEvento() const
{
    return m_dhEvento;
}

void CancNFe::set_dhEvento(const QDateTime &dhEvento)
{
    m_dhEvento = dhEvento;
}

QString CancNFe::get_nProt() const
{
    return m_nProt;
}

void CancNFe::set_nProt(const QString &nProt)
{
    m_nProt = nProt;
}

QString CancNFe::get_xJust() const
{
    return m_xJust;
}

void CancNFe::set_xJust(const QString &xJust)
{
    m_xJust = xJust;
}

QString CancNFe::get_chNFeRef() const
{
    return m_chNFeRef;
}

void CancNFe::set_chNFeRef(const QString &chNFeRef)
{
    m_chNFeRef = chNFeRef;
}

QString CancNFe::get_verAplic() const
{
    return m_verAplic;
}

void CancNFe::set_verAplic(const QString &verAplic)
{
    m_verAplic = verAplic;
}

QString CancNFe::get_versao() const
{
    return m_versao;
}

void CancNFe::set_versao(const QString &versao)
{
    m_versao = versao;
}

QString CancNFe::get_Id() const
{
    return m_Id;
}

QString CancNFe::get_XMLOriginal() const
{
    return m_xmlOriginal;
}

QString CancNFe::get_XMLAssinado() const
{
    return m_xmlAssinado;
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

bool CancNFe::substituicao() const
{
    return m_tpEvento == TpEvento::CancelamentoSubstituicao;
}

bool CancNFe::validarSubstituta()
{
    if (m_chNFe.mid(20, 2) != "65")
        m_error = "Cancelamento por substituição é exclusivo da NFC-e modelo 65.";
    else if (m_chNFe.mid(34, 1) != "1")
        m_error = "A NFC-e a ser cancelada por substituição deve ter emissão normal.";
    else if (!chaveNumerica().match(m_chNFeRef).hasMatch() ||
             CppUtility::dvModulo11(m_chNFeRef.left(43)) != m_chNFeRef.right(1).toInt())
        m_error = "Informe a chave substituta com 44 dígitos e dígito verificador válido.";
    else if (m_chNFeRef == m_chNFe || m_chNFeRef.left(2) != m_chNFe.left(2) ||
             m_chNFeRef.mid(6, 16) != m_chNFe.mid(6, 16) || m_chNFeRef.mid(34, 1) == "1")
        m_error = "Informe uma chave substituta distinta, da mesma UF, emitente e modelo, emitida em contingência.";
    else if (anoMes(m_chNFeRef) > anoMes(m_chNFe) || anoMes(m_chNFeRef) < anoMes(m_chNFe) - 1)
        m_error = "O ano e mês da chave substituta devem ser iguais aos da NFC-e cancelada ou do mês anterior.";
    return m_error.isEmpty();
}

bool CancNFe::prepararEvento()
{
    if (m_versao.isEmpty())
        m_versao = "1.00";
    if (m_tpEvento == TpEvento::None)
        m_tpEvento = TpEvento::Cancelamento;

    if (m_tpEvento != TpEvento::Cancelamento && !substituicao())
        m_error = "CancNFe aceita somente cancelamento (110111) ou cancelamento por substituição (110112).";
    else if (m_versao != "1.00")
        m_error = "Cancelamento exige a versão 1.00.";
    else if (!chaveNumerica().match(m_chNFe).hasMatch())
        m_error = "Informe a chave da NF-e/NFC-e com 44 dígitos numéricos.";
    else if (m_chNFe.mid(20, 2) != "55" && m_chNFe.mid(20, 2) != "65")
        m_error = "Cancelamento exige uma chave de NF-e modelo 55 ou NFC-e modelo 65.";
    else if (m_chNFe.mid(20, 2) != ConvNF::modeloDFToStr(config->get_ModeloDF()))
        m_error = "O modelo da chave diverge do modelo configurado.";
    else if (m_chNFe.left(2).toInt() != ConvNF::ufToInt(config->webservices->get_uf()))
        m_error = "A UF da chave diverge da UF configurada.";
    else if (m_CNPJ.isEmpty() == m_CPF.isEmpty())
        m_error = "Informe o CNPJ ou o CPF do emitente.";
    else if (!m_CNPJ.isEmpty() && (emitenteCPF(m_chNFe) || m_CNPJ != m_chNFe.mid(6, 14)))
        m_error = "O CNPJ do autor diverge do emitente da chave.";
    else if (!m_CPF.isEmpty() && (!emitenteCPF(m_chNFe) || m_CPF != m_chNFe.mid(9, 11)))
        m_error = "O CPF do autor diverge do emitente da chave.";
    else if (substituicao())
        validarSubstituta();

    if (!m_error.isEmpty())
        return false;

    m_Id = "ID" + ConvNF::tpEventoToStr(m_tpEvento) + m_chNFe + "01";
    if (m_dhEvento.isNull())
        m_dhEvento = QDateTime::currentDateTime();
    return true;
}

void CancNFe::gerarEvento()
{
    const QString cUF = QString::number(ConvNF::ufToInt(config->webservices->get_uf()));
    QByteArray dados;
    QXmlStreamWriter xml(&dados);

    xml.writeStartElement("evento");
    xml.writeAttribute("xmlns", "http://www.portalfiscal.inf.br/nfe");
    xml.writeAttribute("versao", m_versao);
    xml.writeStartElement("infEvento");
    xml.writeAttribute("Id", m_Id);
    xml.writeTextElement("cOrgao", cUF);
    xml.writeTextElement("tpAmb", ConvNF::tpAmbToStr(config->webservices->get_tpAmb()));
    if (!m_CNPJ.isEmpty())
        xml.writeTextElement("CNPJ", m_CNPJ);
    else
        xml.writeTextElement("CPF", m_CPF);
    xml.writeTextElement("chNFe", m_chNFe);
    xml.writeTextElement("dhEvento", CppUtil::dateTimeToStr(m_dhEvento, DtH::DateTimeUTC));
    xml.writeTextElement("tpEvento", ConvNF::tpEventoToStr(m_tpEvento));
    xml.writeTextElement("nSeqEvento", "1");
    xml.writeTextElement("verEvento", m_versao);

    xml.writeStartElement("detEvento");
    xml.writeAttribute("versao", m_versao);
    xml.writeTextElement("descEvento", substituicao() ? "Cancelamento por substituicao" : "Cancelamento");
    if (substituicao())
    {
        xml.writeTextElement("cOrgaoAutor", cUF);
        xml.writeTextElement("tpAutor", ConvNF::tpAutorToStr(TpAutor::EmpresaEmitente));
        xml.writeTextElement("verAplic", m_verAplic);
    }
    xml.writeTextElement("nProt", m_nProt);
    xml.writeTextElement("xJust", m_xJust);
    if (substituicao())
        xml.writeTextElement("chNFeRef", m_chNFeRef);
    xml.writeEndElement(); // detEvento
    xml.writeEndElement(); // infEvento
    xml.writeEndElement(); // evento

    m_xmlOriginal = QString::fromUtf8(dados);
}

bool CancNFe::assinarEvento()
{
    CppLibXml2 xml(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    QByteArray assinado;
    if (!xml.signXML(m_xmlOriginal.toUtf8(), HashType::sha1, "infEvento", "SignedInfo", m_Id.toUtf8(),
                     XMLReturnType::XMLSign, assinado))
    {
        m_error = xml.get_error();
        return false;
    }
    m_xmlAssinado = QString::fromUtf8(assinado);
    return true;
}

bool CancNFe::gerarXML(const int &idLote)
{
    m_error.clear();
    m_xml.clear();
    m_xmlOriginal.clear();
    m_xmlAssinado.clear();
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
    gerarEvento();
    if (!assinarEvento())
        return false;
    m_xml = "<envEvento xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"" + m_versao + "\">"
            "<idLote>" + CppUtil::insZeroLeft(QString::number(idLote), 15) + "</idLote>" +
            CppUtility::extractStr(m_xmlAssinado, "<evento", "/evento>") + "</envEvento>";
    return true;
}

QString CancNFe::get_schemaName(const QString &prefixo) const
{
    return prefixo + (substituicao() ? "CancSubst_v" : "CancNFe_v") + m_versao + ".xsd";
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

bool CancNFe::tratarRetorno(const QByteArray &mensagem)
{
    retorno->preencher(*m_retEnvEvento);
    retorno->m_xmlLote = CppUtility::extractStr(QString::fromUtf8(mensagem), "<retEnvEvento", "retEnvEvento>");

    // Lote processado não significa que o cancelamento foi homologado.
    if (m_retEnvEvento->get_cStat() != 128)
        return true;
    if (m_retEnvEvento->retEvento->count() != 1)
    {
        m_error = "O retorno do lote deve conter somente o evento de cancelamento enviado.";
        return false;
    }
    RetEvento *ret = m_retEnvEvento->retEvento->value(0);
    const InfEventoRet *infoRet = ret->infEvento.get();
    if (infoRet->get_cStat() != 135 && infoRet->get_cStat() != 155)
        return true;
    if (infoRet->get_chNFe() != m_chNFe || infoRet->get_tpEvento() != m_tpEvento ||
        infoRet->get_nSeqEvento() != 1 || infoRet->get_tpAmb() != config->webservices->get_tpAmb())
    {
        m_error = "O retorno de cancelamento não corresponde ao evento enviado.";
        return false;
    }
    QByteArray processado = ("<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"" +
                            m_versao + "\"></procEventoNFe>").toUtf8();
    const QByteArray original = ("<a>" + CppUtility::extractStr(m_xmlAssinado, "<evento", "/evento>") + "</a>").toUtf8();
    const QByteArray resposta = ("<a>" + CppUtility::extractStr(ret->get_XML(), "<retEvento", "/retEvento>") + "</a>").toUtf8();
    CppLibXml2 xml(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    if (!xml.addChildXML(processado, original, "procEventoNFe") ||
        !xml.addChildXML(processado, resposta, "procEventoNFe") ||
        !xml.parseXML(processado, get_schemaName("procEvento").toUtf8()))
    {
        m_error = xml.get_error();
        return false;
    }
    ret->set_XML(QString::fromUtf8(processado));
    retorno->m_xml = ret->get_XML();
    if (config->arquivos->get_salvar())
        ret->salvarXML(config->arquivos->get_caminhoEvento(), m_Id.mid(2) + "-procEventoNFe");
    return true;
}
