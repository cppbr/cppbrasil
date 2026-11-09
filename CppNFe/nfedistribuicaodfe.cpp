#include "nfedistribuicaodfe.h"

#include <QHash>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <CppBase/cpplibxml2.h>
#include <CppBase/cppzip.h>
#include <CppNFe/wsnfe.h>
#include <CppNFe/xmlread.h>

namespace {

const QString namespaceNFe = QStringLiteral("http://www.portalfiscal.inf.br/nfe");
const QString namespaceWS = namespaceNFe + QStringLiteral("/wsdl/NFeDistribuicaoDFe");
const auto servico = WebServicesNF::NFeDistribuicaoDFe;

class WSDistribuicao : public WSNFeBase
{
public:
    using WSNFeBase::WSNFeBase;

    QByteArray consultar(const QByteArray &xml, TpAmb ambiente)
    {
        const QString url = UrlNFe::get_urlServidor(UF::None, ambiente, TpEmis::Normal,
                                                    ModeloDF::NFe, VersaoNF::v400, servico);
        if (url.isEmpty())
        {
            set_error(QStringLiteral("Endereço do serviço de distribuição indisponível."));
            return {};
        }

        const QByteArray operacao = "<nfeDistDFeInteresse xmlns=\"" + namespaceWS.toUtf8()
            + "\"><nfeDadosMsg>" + xml + "</nfeDadosMsg></nfeDistDFeInteresse>";
        QByteArray envelope;
        // O serviço exige o elemento da operação no corpo SOAP 1.2.
        const QByteArray soap = "http://www.w3.org/2003/05/soap-envelope";
        envelope = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<soap:Envelope xmlns:soap=\"" + soap + "\"><soap:Body>"
            + operacao + "</soap:Body></soap:Envelope>";
        const QByteArray contentType = "application/soap+xml; charset=utf-8; action=\""
            + namespaceWS.toUtf8() + "/nfeDistDFeInteresse\"";

        salvarLogs(TipoArquivo::XML, TipoMsgLog::MsgEnvio, QString(), servico, envelope);
        emit wsChange(servico);
        const QByteArray resposta = sendMsg(url.toUtf8(), envelope, contentType);
        if (!resposta.isEmpty())
            salvarLogs(TipoArquivo::XML, TipoMsgLog::MsgRetorno, QString(), servico, resposta);
        emit wsChange(WebServicesNF::None);
        return resposta;
    }
};

bool elemento(const QXmlStreamReader &xml, const QString &nome, const QString &ns = namespaceNFe)
{
    return xml.isStartElement() && xml.name() == nome && xml.namespaceUri() == ns;
}

using Campos = QHash<QString, QString>;

Campos lerCampos(QXmlStreamReader &xml)
{
    Campos campos;
    while (xml.readNextStartElement())
    {
        if (xml.namespaceUri() != namespaceNFe)
        {
            xml.skipCurrentElement();
            continue;
        }
        const QString nome = xml.name().toString();
        campos.insert(nome, xml.readElementText(QXmlStreamReader::SkipChildElements));
    }
    return campos;
}

QDateTime data(const QString &valor)
{
    return QDateTime::fromString(valor, Qt::ISODate);
}

void lerResumoNFe(QXmlStreamReader &xml, ResNFe &res)
{
    res.set_versao(xml.attributes().value("versao").toString());
    const auto campos = lerCampos(xml);
    res.set_chNFe(campos.value("chNFe"));
    res.set_CNPJ(campos.value("CNPJ"));
    res.set_CPF(campos.value("CPF"));
    res.set_xNome(campos.value("xNome"));
    res.set_IE(campos.value("IE"));
    res.set_dhEmi(data(campos.value("dhEmi")));
    res.set_tpNF(campos.value("tpNF").toInt());
    res.set_vNF(campos.value("vNF").toDouble());
    res.set_digVal(campos.value("digVal"));
    res.set_dhRecbto(data(campos.value("dhRecbto")));
    res.set_nProt(campos.value("nProt"));
    res.set_cSitNFe(campos.value("cSitNFe").toInt());
}

void lerResumoEvento(QXmlStreamReader &xml, ResEvento &res)
{
    res.set_versao(xml.attributes().value("versao").toString());
    const auto campos = lerCampos(xml);
    res.set_cOrgao(campos.value("cOrgao").toInt());
    res.set_CNPJ(campos.value("CNPJ"));
    res.set_CPF(campos.value("CPF"));
    res.set_chNFe(campos.value("chNFe"));
    res.set_dhEvento(data(campos.value("dhEvento")));
    res.set_tpEvento(campos.value("tpEvento").toInt());
    res.set_nSeqEvento(campos.value("nSeqEvento").toInt());
    res.set_xEvento(campos.value("xEvento"));
    res.set_dhRecbto(data(campos.value("dhRecbto")));
    res.set_nProt(campos.value("nProt"));
}

void lerInfEvento(QXmlStreamReader &xml, InfEvento &inf)
{
    inf.set_Id(xml.attributes().value("Id").toString());
    Campos campos;
    while (xml.readNextStartElement())
    {
        const QString nome = xml.name().toString();
        if (elemento(xml, QStringLiteral("detEvento")))
        {
            const auto det = lerCampos(xml);
            inf.detEvento->set_descEvento(det.value("descEvento"));
            inf.detEvento->set_nProt(det.value("nProt"));
            inf.detEvento->set_xJust(det.value("xJust"));
            inf.detEvento->set_xCorrecao(det.value("xCorrecao"));
            inf.detEvento->set_xCondUso(det.value("xCondUso"));
            const auto tipo = ConvNF::strToTpEvento(campos.value("tpEvento"));
            if (ConvNF::orgaoEventoEntrega(tipo))
            {
                auto &entrega = *inf.detEvento;
                entrega.set_cOrgaoAutor(det.value("cOrgaoAutor").toInt());
                entrega.set_tpAutor(ConvNF::strToTpAutor(det.value("tpAutor")));
                entrega.set_verAplic(det.value("verAplic"));
                entrega.set_nProtEvento(det.value("nProtEvento"));
                entrega.set_latGPS(det.value("latGPS"));
                entrega.set_longGPS(det.value("longGPS"));
                if (tipo == TpEvento::ComprovanteEntregaNFe)
                {
                    entrega.set_dhEntrega(data(det.value("dhEntrega")));
                    entrega.set_nDoc(det.value("nDoc"));
                    entrega.set_xNome(det.value("xNome"));
                    entrega.set_hashComprovante(det.value("hashComprovante"));
                    entrega.set_dhHashComprovante(data(det.value("dhHashComprovante")));
                }
                else if (tipo == TpEvento::InsucessoEntregaNFe)
                {
                    entrega.set_dhTentativaEntrega(data(det.value("dhTentativaEntrega")));
                    entrega.set_nTentativa(det.value("nTentativa").toInt());
                    entrega.set_tpMotivo(ConvNF::strToTpMotivo(det.value("tpMotivo")));
                    entrega.set_xJustMotivo(det.value("xJustMotivo"));
                    entrega.set_hashTentativaEntrega(det.value("hashTentativaEntrega"));
                    entrega.set_dhHashTentativaEntrega(data(det.value("dhHashTentativaEntrega")));
                }
            }
        }
        else
            campos.insert(nome, xml.readElementText(QXmlStreamReader::SkipChildElements));
    }
    inf.set_cOrgao(campos.value("cOrgao").toInt());
    inf.set_tpAmb(ConvNF::strToTpAmb(campos.value("tpAmb")));
    inf.set_CNPJ(campos.value("CNPJ"));
    inf.set_CPF(campos.value("CPF"));
    inf.set_chNFe(campos.value("chNFe"));
    inf.set_dhEvento(data(campos.value("dhEvento")));
    inf.set_tpEvento(ConvNF::strToTpEvento(campos.value("tpEvento")));
    inf.set_nSeqEvento(campos.value("nSeqEvento").toInt());
    inf.set_verEvento(campos.value("verEvento"));
}

void lerInfRetEvento(QXmlStreamReader &xml, InfEventoRet &inf)
{
    inf.set_Id(xml.attributes().value("Id").toString());
    const auto campos = lerCampos(xml);
    inf.set_tpAmb(ConvNF::strToTpAmb(campos.value("tpAmb")));
    inf.set_verAplic(campos.value("verAplic"));
    inf.set_cOrgao(campos.value("cOrgao").toInt());
    inf.set_cStat(campos.value("cStat").toInt());
    inf.set_xMotivo(campos.value("xMotivo"));
    inf.set_chNFe(campos.value("chNFe"));
    inf.set_tpEvento(ConvNF::strToTpEvento(campos.value("tpEvento")));
    inf.set_xEvento(campos.value("xEvento"));
    inf.set_nSeqEvento(campos.value("nSeqEvento").toInt());
    inf.set_CNPJDest(campos.value("CNPJDest"));
    inf.set_CPFDest(campos.value("CPFDest"));
    inf.set_emailDest(campos.value("emailDest"));
    inf.set_dhRegEvento(data(campos.value("dhRegEvento")));
    inf.set_nProt(campos.value("nProt"));
}

void lerEventoProcessado(QXmlStreamReader &xml, ProcEventoNFe &proc)
{
    proc.set_versao(xml.attributes().value("versao").toString());
    while (xml.readNextStartElement())
    {
        if (elemento(xml, QStringLiteral("evento")))
        {
            proc.evento->set_versao(xml.attributes().value("versao").toString());
            while (xml.readNextStartElement())
            {
                if (elemento(xml, QStringLiteral("infEvento")))
                    lerInfEvento(xml, *proc.evento->infEvento);
                else
                    xml.skipCurrentElement();
            }
        }
        else if (elemento(xml, QStringLiteral("retEvento")))
        {
            proc.retEvento->set_versao(xml.attributes().value("versao").toString());
            while (xml.readNextStartElement())
            {
                if (elemento(xml, QStringLiteral("infEvento")))
                    lerInfRetEvento(xml, *proc.retEvento->infEvento);
                else
                    xml.skipCurrentElement();
            }
        }
        else
            xml.skipCurrentElement();
    }
}

bool lerDocZip(QXmlStreamReader &xml, DocZip &doc)
{
    doc.set_NSU(xml.attributes().value("NSU").toString());
    doc.set_schema(xml.attributes().value("schema").toString());
    doc.set_zip(xml.readElementText());
    if (xml.hasError() || doc.get_schema().isEmpty())
        return false;

    QString base64 = doc.get_zip();
    base64.remove(QRegularExpression(QStringLiteral("[ \\t\\r\\n]")));
    const auto gzip = QByteArray::fromBase64(base64.toLatin1(), QByteArray::AbortOnBase64DecodingErrors);
    QByteArray bytes;
    if (!gzip.startsWith("\x1f\x8b") || !CppZip::decompress(gzip, bytes))
        return false;

    QXmlStreamReader leitor(bytes);
    if (!leitor.readNextStartElement() || leitor.namespaceUri() != namespaceNFe)
        return false;

    const QString raiz = leitor.name().toString();
    // O nome do XSD da NF-e processada difere do nome da raiz.
    const QString tipo = raiz == QStringLiteral("nfeProc") ? QStringLiteral("procNFe") : raiz;
    if (doc.get_schema().section('_', 0, 0) != tipo)
        return false;

    if (raiz == QStringLiteral("resNFe"))
    {
        doc.resNFe = std::make_unique<ResNFe>();
        lerResumoNFe(leitor, *doc.resNFe);
    }
    else if (raiz == QStringLiteral("resEvento"))
    {
        doc.resEvento = std::make_unique<ResEvento>();
        lerResumoEvento(leitor, *doc.resEvento);
    }
    else if (raiz == QStringLiteral("procEventoNFe"))
    {
        doc.procEventoNFe = std::make_unique<ProcEventoNFe>();
        lerEventoProcessado(leitor, *doc.procEventoNFe);
    }
    else
        leitor.skipCurrentElement();

    while (!leitor.atEnd())
        leitor.readNext();
    if (leitor.hasError())
        return false;

    doc.set_xml(QString::fromUtf8(bytes));
    if (raiz == QStringLiteral("nfeProc"))
    {
        doc.procNFe = std::make_unique<Nfe>();
        if (!XmlRead(doc.procNFe.get(), bytes).import())
            return false;
    }
    return true;
}

bool localizarRetorno(QXmlStreamReader &xml)
{
    if (!xml.readNextStartElement())
        return false;
    if (elemento(xml, QStringLiteral("retDistDFeInt")))
        return true;

    const QString soap = xml.namespaceUri().toString();
    if (!elemento(xml, QStringLiteral("Envelope"), soap)
        || (soap != QStringLiteral("http://www.w3.org/2003/05/soap-envelope")
            && soap != QStringLiteral("http://schemas.xmlsoap.org/soap/envelope/")))
        return false;
    if (!xml.readNextStartElement())
        return false;
    if (elemento(xml, QStringLiteral("Header"), soap))
    {
        xml.skipCurrentElement();
        if (!xml.readNextStartElement())
            return false;
    }
    return elemento(xml, QStringLiteral("Body"), soap)
        && xml.readNextStartElement() && elemento(xml, QStringLiteral("nfeDistDFeInteresseResponse"), namespaceWS)
        && xml.readNextStartElement() && elemento(xml, QStringLiteral("nfeDistDFeInteresseResult"), namespaceWS)
        && xml.readNextStartElement() && elemento(xml, QStringLiteral("retDistDFeInt"));
}

} // namespace

QString DistNSU::get_ultNSU() const
{
    return m_ultNSU;
}

void DistNSU::set_ultNSU(const QString &ultNSU)
{
    m_ultNSU = ultNSU;
}

QString ConsNSU::get_NSU() const
{
    return m_NSU;
}

void ConsNSU::set_NSU(const QString &NSU)
{
    m_NSU = NSU;
}

QString ConsChNFe::get_chNFe() const
{
    return m_chNFe;
}

void ConsChNFe::set_chNFe(const QString &chNFe)
{
    m_chNFe = chNFe;
}

QString DistDFeInt::get_versao() const
{
    return m_versao;
}

void DistDFeInt::set_versao(const QString &versao)
{
    m_versao = versao;
}

TpAmb DistDFeInt::get_tpAmb() const
{
    return m_tpAmb;
}

void DistDFeInt::set_tpAmb(const TpAmb &tpAmb)
{
    m_tpAmb = tpAmb;
}

QString DistDFeInt::get_cUFAutor() const
{
    return m_cUFAutor;
}

void DistDFeInt::set_cUFAutor(const QString &cUFAutor)
{
    m_cUFAutor = cUFAutor;
}

QString DistDFeInt::get_CNPJ() const
{
    return m_CNPJ;
}

void DistDFeInt::set_CNPJ(const QString &CNPJ)
{
    m_CNPJ = CNPJ;
}

QString DistDFeInt::get_CPF() const
{
    return m_CPF;
}

void DistDFeInt::set_CPF(const QString &CPF)
{
    m_CPF = CPF;
}

QString ResNFe::get_versao() const
{
    return m_versao;
}

void ResNFe::set_versao(const QString &versao)
{
    m_versao = versao;
}

QString ResNFe::get_chNFe() const
{
    return m_chNFe;
}

void ResNFe::set_chNFe(const QString &chNFe)
{
    m_chNFe = chNFe;
}

QString ResNFe::get_CNPJ() const
{
    return m_CNPJ;
}

void ResNFe::set_CNPJ(const QString &CNPJ)
{
    m_CNPJ = CNPJ;
}

QString ResNFe::get_CPF() const
{
    return m_CPF;
}

void ResNFe::set_CPF(const QString &CPF)
{
    m_CPF = CPF;
}

QString ResNFe::get_xNome() const
{
    return m_xNome;
}

void ResNFe::set_xNome(const QString &xNome)
{
    m_xNome = xNome;
}

QString ResNFe::get_IE() const
{
    return m_IE;
}

void ResNFe::set_IE(const QString &IE)
{
    m_IE = IE;
}

QDateTime ResNFe::get_dhEmi() const
{
    return m_dhEmi;
}

void ResNFe::set_dhEmi(const QDateTime &dhEmi)
{
    m_dhEmi = dhEmi;
}

int ResNFe::get_tpNF() const
{
    return m_tpNF;
}

void ResNFe::set_tpNF(const int &tpNF)
{
    m_tpNF = tpNF;
}

double ResNFe::get_vNF() const
{
    return m_vNF;
}

void ResNFe::set_vNF(const double &vNF)
{
    m_vNF = vNF;
}

QString ResNFe::get_digVal() const
{
    return m_digVal;
}

void ResNFe::set_digVal(const QString &digVal)
{
    m_digVal = digVal;
}

QDateTime ResNFe::get_dhRecbto() const
{
    return m_dhRecbto;
}

void ResNFe::set_dhRecbto(const QDateTime &dhRecbto)
{
    m_dhRecbto = dhRecbto;
}

QString ResNFe::get_nProt() const
{
    return m_nProt;
}

void ResNFe::set_nProt(const QString &nProt)
{
    m_nProt = nProt;
}

int ResNFe::get_cSitNFe() const
{
    return m_cSitNFe;
}

void ResNFe::set_cSitNFe(const int &cSitNFe)
{
    m_cSitNFe = cSitNFe;
}

QString ResEvento::get_versao() const
{
    return m_versao;
}

void ResEvento::set_versao(const QString &versao)
{
    m_versao = versao;
}

int ResEvento::get_cOrgao() const
{
    return m_cOrgao;
}

void ResEvento::set_cOrgao(const int &cOrgao)
{
    m_cOrgao = cOrgao;
}

QString ResEvento::get_CNPJ() const
{
    return m_CNPJ;
}

void ResEvento::set_CNPJ(const QString &CNPJ)
{
    m_CNPJ = CNPJ;
}

QString ResEvento::get_CPF() const
{
    return m_CPF;
}

void ResEvento::set_CPF(const QString &CPF)
{
    m_CPF = CPF;
}

QString ResEvento::get_chNFe() const
{
    return m_chNFe;
}

void ResEvento::set_chNFe(const QString &chNFe)
{
    m_chNFe = chNFe;
}

QDateTime ResEvento::get_dhEvento() const
{
    return m_dhEvento;
}

void ResEvento::set_dhEvento(const QDateTime &dhEvento)
{
    m_dhEvento = dhEvento;
}

int ResEvento::get_tpEvento() const
{
    return m_tpEvento;
}

void ResEvento::set_tpEvento(const int &tpEvento)
{
    m_tpEvento = tpEvento;
}

int ResEvento::get_nSeqEvento() const
{
    return m_nSeqEvento;
}

void ResEvento::set_nSeqEvento(const int &nSeqEvento)
{
    m_nSeqEvento = nSeqEvento;
}

QString ResEvento::get_xEvento() const
{
    return m_xEvento;
}

void ResEvento::set_xEvento(const QString &xEvento)
{
    m_xEvento = xEvento;
}

QDateTime ResEvento::get_dhRecbto() const
{
    return m_dhRecbto;
}

void ResEvento::set_dhRecbto(const QDateTime &dhRecbto)
{
    m_dhRecbto = dhRecbto;
}

QString ResEvento::get_nProt() const
{
    return m_nProt;
}

void ResEvento::set_nProt(const QString &nProt)
{
    m_nProt = nProt;
}

QString DocZip::get_NSU() const
{
    return m_NSU;
}

void DocZip::set_NSU(const QString &NSU)
{
    m_NSU = NSU;
}

QString DocZip::get_schema() const
{
    return m_schema;
}

void DocZip::set_schema(const QString &schema)
{
    m_schema = schema;
}

QString DocZip::get_zip() const
{
    return m_zip;
}

void DocZip::set_zip(const QString &zip)
{
    m_zip = zip;
}

QString DocZip::get_xml() const
{
    return m_xml;
}

void DocZip::set_xml(const QString &xml)
{
    m_xml = xml;
}

QString RetDistDFeInt::get_versao() const
{
    return m_versao;
}

void RetDistDFeInt::set_versao(const QString &versao)
{
    m_versao = versao;
}

TpAmb RetDistDFeInt::get_tpAmb() const
{
    return m_tpAmb;
}

void RetDistDFeInt::set_tpAmb(const TpAmb &tpAmb)
{
    m_tpAmb = tpAmb;
}

QString RetDistDFeInt::get_verAplic() const
{
    return m_verAplic;
}

void RetDistDFeInt::set_verAplic(const QString &verAplic)
{
    m_verAplic = verAplic;
}

int RetDistDFeInt::get_cStat() const
{
    return m_cStat;
}

void RetDistDFeInt::set_cStat(const int &cStat)
{
    m_cStat = cStat;
}

QString RetDistDFeInt::get_xMotivo() const
{
    return m_xMotivo;
}

void RetDistDFeInt::set_xMotivo(const QString &xMotivo)
{
    m_xMotivo = xMotivo;
}

QDateTime RetDistDFeInt::get_dhResp() const
{
    return m_dhResp;
}

void RetDistDFeInt::set_dhResp(const QDateTime &dhResp)
{
    m_dhResp = dhResp;
}

QString RetDistDFeInt::get_ultNSU() const
{
    return m_ultNSU;
}

void RetDistDFeInt::set_ultNSU(const QString &ultNSU)
{
    m_ultNSU = ultNSU;
}

QString RetDistDFeInt::get_maxNSU() const
{
    return m_maxNSU;
}

void RetDistDFeInt::set_maxNSU(const QString &maxNSU)
{
    m_maxNSU = maxNSU;
}

void DistDFeInt::clear()
{
    m_versao = ConvNF::versaoLayout(servico);
    m_tpAmb = TpAmb::None;
    m_cUFAutor.clear();
    m_CNPJ.clear();
    m_CPF.clear();
    distNSU->set_ultNSU({});
    consNSU->set_NSU({});
    consChNFe->set_chNFe({});
}

void DocZip::clear()
{
    *this = DocZip();
}

void RetDistDFeInt::clear()
{
    *this = RetDistDFeInt();
}

NFeDistribuicaoDFe::NFeDistribuicaoDFe(ConfigNFe *config) : config(config)
{
}

void NFeDistribuicaoDFe::clear()
{
    distDFeInt->clear();
    retDistDFeInt->clear();
    m_error.clear();
}

QString NFeDistribuicaoDFe::get_error() const
{
    return m_error;
}

bool NFeDistribuicaoDFe::falhar(const QString &error)
{
    m_error = error;
    emit errorOccurred(error);
    return false;
}

bool NFeDistribuicaoDFe::gerarXML(QByteArray &xml)
{
    m_error.clear();
    xml.clear();

    const auto &pedido = *distDFeInt;
    if (pedido.get_CNPJ().isEmpty() == pedido.get_CPF().isEmpty())
        return falhar(QStringLiteral("Informe somente o CNPJ ou o CPF do interessado."));
    const auto ambiente = pedido.get_tpAmb() == TpAmb::None ? config->webservices->get_tpAmb() : pedido.get_tpAmb();
    if (ambiente != TpAmb::Producao && ambiente != TpAmb::Homologacao)
        return falhar(QStringLiteral("Informe o ambiente da distribuição de DF-e."));

    const auto ultNSU = pedido.distNSU->get_ultNSU();
    const auto nsu = pedido.consNSU->get_NSU();
    const auto chave = pedido.consChNFe->get_chNFe();
    if ((!ultNSU.isEmpty() + !nsu.isEmpty() + !chave.isEmpty()) != 1)
        return falhar(QStringLiteral("Preencha somente um grupo entre distNSU, consNSU e consChNFe."));

    QByteArray mensagem;
    QXmlStreamWriter writer(&mensagem);
    writer.writeStartElement("distDFeInt");
    writer.writeDefaultNamespace(namespaceNFe);
    writer.writeAttribute("versao", pedido.get_versao());
    writer.writeTextElement("tpAmb", ConvNF::tpAmbToStr(ambiente));
    if (!pedido.get_cUFAutor().isEmpty())
        writer.writeTextElement("cUFAutor", pedido.get_cUFAutor());
    if (!pedido.get_CNPJ().isEmpty())
        writer.writeTextElement("CNPJ", pedido.get_CNPJ());
    else
        writer.writeTextElement("CPF", pedido.get_CPF());

    if (!ultNSU.isEmpty())
    {
        writer.writeStartElement("distNSU");
        writer.writeTextElement("ultNSU", ultNSU.rightJustified(15, '0'));
    }
    else if (!nsu.isEmpty())
    {
        writer.writeStartElement("consNSU");
        writer.writeTextElement("NSU", nsu.rightJustified(15, '0'));
    }
    else
    {
        writer.writeStartElement("consChNFe");
        writer.writeTextElement("chNFe", chave);
    }
    writer.writeEndElement();
    writer.writeEndElement();

    if (pedido.get_versao() != QStringLiteral("1.00") && pedido.get_versao() != ConvNF::versaoLayout(servico))
        return falhar(QStringLiteral("Versão de distribuição de DF-e inválida."));
    CppLibXml2 validador(nullptr, config->arquivos->get_caminhoSchema().toLocal8Bit());
    if (!validador.parseXML(mensagem, "distDFeInt_v" + pedido.get_versao().toLatin1() + ".xsd"))
        return falhar(validador.get_error());
    xml = mensagem;
    return true;
}

bool NFeDistribuicaoDFe::lerRetorno(const QByteArray &xml)
{
    m_error.clear();
    retDistDFeInt->clear();
    QXmlStreamReader leitor(xml);
    if (!localizarRetorno(leitor))
        return falhar(QStringLiteral("Resposta sem retDistDFeInt."));

    RetDistDFeInt retorno;
    retorno.set_versao(leitor.attributes().value("versao").toString());
    auto &documentos = *retorno.loteDistDFeInt->docZip;
    bool encontrouLote = false;
    Campos campos;
    while (leitor.readNextStartElement())
    {
        if (elemento(leitor, QStringLiteral("loteDistDFeInt")))
        {
            if (encontrouLote)
                return falhar(QStringLiteral("A resposta contém mais de um loteDistDFeInt."));
            encontrouLote = true;
            while (leitor.readNextStartElement())
            {
                if (!elemento(leitor, QStringLiteral("docZip")) || documentos.count() == 50)
                    return falhar(QStringLiteral("Lote de distribuição inválido."));
                auto doc = std::make_unique<DocZip>();
                if (!lerDocZip(leitor, *doc))
                    return falhar(QStringLiteral("Falha ao interpretar docZip (%1, NSU %2).")
                                      .arg(doc->get_schema(), doc->get_NSU()));
                documentos.add(std::move(doc));
            }
            if (documentos.isEmpty())
                return falhar(QStringLiteral("Lote de distribuição vazio."));
        }
        else if (leitor.namespaceUri() == namespaceNFe)
        {
            const QString nome = leitor.name().toString();
            if (campos.contains(nome))
                return falhar(QStringLiteral("Campo duplicado na resposta de distribuição."));
            campos.insert(nome, leitor.readElementText());
        }
        else
            leitor.skipCurrentElement();
    }
    while (!leitor.atEnd())
        leitor.readNext();

    retorno.set_tpAmb(ConvNF::strToTpAmb(campos.value("tpAmb")));
    retorno.set_verAplic(campos.value("verAplic"));
    retorno.set_cStat(campos.value("cStat").toInt());
    retorno.set_xMotivo(campos.value("xMotivo"));
    retorno.set_dhResp(data(campos.value("dhResp")));
    retorno.set_ultNSU(campos.value("ultNSU"));
    retorno.set_maxNSU(campos.value("maxNSU"));

    if (leitor.hasError() || retorno.get_versao().isEmpty() || retorno.get_tpAmb() == TpAmb::None
        || retorno.get_verAplic().isEmpty() || retorno.get_cStat() < 100 || retorno.get_cStat() > 999
        || retorno.get_xMotivo().isEmpty() || !retorno.get_dhResp().isValid()
        || (retorno.get_cStat() == 138 && !encontrouLote))
        return falhar(QStringLiteral("Leiaute de retDistDFeInt inválido."));

    *retDistDFeInt = std::move(retorno);
    return true;
}

bool NFeDistribuicaoDFe::consultar()
{
    retDistDFeInt->clear();
    QByteArray xml;
    if (!gerarXML(xml))
        return false;

    auto *crypto = config->certificado->get_cryptoLib();
    if (!crypto || crypto->get_certDER().isEmpty() || crypto->get_pkeyDER().isEmpty())
        return falhar(QStringLiteral("Carregue o certificado digital para consultar a distribuição de DF-e."));

    WSDistribuicao ws(config, crypto);
    connect(&ws, &WSDistribuicao::wsChange, this, &NFeDistribuicaoDFe::wsChange);
    const auto ambiente = distDFeInt->get_tpAmb() == TpAmb::None ? config->webservices->get_tpAmb() : distDFeInt->get_tpAmb();
    const QByteArray retorno = ws.consultar(xml, ambiente);
    if (!ws.get_error().isEmpty())
        return falhar(ws.get_error());
    return lerRetorno(retorno);
}
