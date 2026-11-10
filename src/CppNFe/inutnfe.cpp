#include "inutnfe.h"
#include <QMap>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <CppBase/cpplibxml2.h>
#include <CppNFe/wsnfe.h>

namespace {
const QString namespaceNFe = QStringLiteral("http://www.portalfiscal.inf.br/nfe");
const auto servico = WebServicesNF::NFeInutilizacao;

class WSInutilizacao : public WSNFeBase
{
public:
    using WSNFeBase::WSNFeBase;
    QByteArray enviar(const QByteArray &xml)
    {
        // Inutilização é atendida pela autorizadora normal, inclusive em contingência.
        const QString url = UrlNFe::get_urlServidor(confgNFe->webservices->get_uf(),
            confgNFe->webservices->get_tpAmb(), TpEmis::Normal,
            confgNFe->get_ModeloDF(), VersaoNF::v400, servico);
        if (url.isEmpty())
        {
            set_error("Endereço do serviço de inutilização indisponível.");
            return {};
        }
        QByteArray envelope;
        const QByteArray ns = (namespaceNFe + "/wsdl/NFeInutilizacao4").toUtf8();
        if (!get_soapEnvelop(xml, "nfeDadosMsg", ns, envelope))
            return {};
        salvarLogs(TipoArquivo::XML, TipoMsgLog::MsgEnvio, QString(), servico, envelope);
        emit wsChange(servico);
        const QByteArray resposta = sendMsg(url.toUtf8(), envelope,
            "application/soap+xml; charset=utf-8; action=\"" + ns + "/nfeInutilizacaoNF\"");
        if (!resposta.isEmpty())
            salvarLogs(TipoArquivo::XML, TipoMsgLog::MsgRetorno, QString(), servico, resposta);
        emit wsChange(WebServicesNF::None);
        return resposta;
    }
};

// Conserva o texto assinado e declara no fragmento os namespaces herdados do SOAP.
QString extrairRetorno(const QByteArray &dados)
{
    const QString fonte = QString::fromUtf8(dados);
    QXmlStreamReader xml(fonte);
    QList<QMap<QString, QString>> pilha;
    QString retorno;
    qsizetype inicio = -1;
    int nivel = -1;
    QMap<QString, QString> herdados;
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isDTD())
            return {};
        if (xml.isStartElement())
        {
            QMap<QString, QString> atual = pilha.isEmpty() ? QMap<QString, QString>() : pilha.last();
            const auto anteriores = atual;
            for (const auto &ns : xml.namespaceDeclarations())
                atual[ns.prefix().toString()] = ns.namespaceUri().toString();
            pilha.append(atual);
            if (xml.name() == QLatin1String("retInutNFe") && xml.namespaceUri() == namespaceNFe)
            {
                if (inicio >= 0 || !retorno.isEmpty())
                    return {};
                inicio = fonte.lastIndexOf('<', xml.characterOffset() - 1);
                nivel = pilha.size();
                herdados = anteriores;
                for (const auto &ns : xml.namespaceDeclarations())
                    herdados.remove(ns.prefix().toString());
            }
        }
        else if (xml.isEndElement())
        {
            if (nivel == pilha.size())
            {
                retorno = fonte.mid(inicio, xml.characterOffset() - inicio);
                QString declaracoes;
                QXmlStreamWriter escritor(&declaracoes);
                escritor.writeStartElement("a");
                for (auto it = herdados.cbegin(); it != herdados.cend(); ++it)
                    escritor.writeNamespace(it.value(), it.key());
                escritor.writeEndElement();
                // Reaproveita o escape XML das declarações, sem reescrever o retorno.
                declaracoes = declaracoes.mid(2);
                declaracoes.chop(2); // />
                retorno.insert(retorno.indexOf(QRegularExpression("[\\s/>]")), declaracoes);
                nivel = -1;
            }
            if (!pilha.isEmpty())
                pilha.removeLast();
        }
    }
    return xml.hasError() ? QString() : retorno;
}

QString semDeclaracao(QString xml)
{
    if (xml.startsWith("<?xml"))
        xml.remove(0, xml.indexOf("?>") + 2);
    return xml.trimmed();
}
} // namespace

InfInut::InfInut(ConfigNFe *config, InutNFe *inutNFe) : config(config), pedido(inutNFe)
{
    clear();
}

void InfInut::clear()
{
    m_Id = QString();
    m_tpAmb = TpAmb::None;
    m_xServ = QString();
    m_cUF = 0;
    m_ano = 0;
    m_CNPJ = QString();
    m_mod = ModeloDF::NFe;
    m_modInformado = false;
    m_serie = -1;
    m_nNFIni = 0;
    m_nNFFin = 0;
    m_xJust = QString();
    m_verAplic = QString();
    m_cStat = 0;
    m_xMotivo = QString();
    m_dhRecbto = QDateTime();
    m_nProt = QString();
    m_error.clear();
    m_xmlOriginal.clear();
    m_xmlAssinado.clear();
    m_xml.clear();
}

bool InfInut::falhar(const QString &error)
{
    m_error = error;
    emit errorOccurred(error);
    return false;
}

QString InfInut::get_error() const { return m_error; }
QString InfInut::get_XMLOriginal() const { return m_xmlOriginal; }
QString InfInut::get_XMLAssinado() const { return m_xmlAssinado; }
QString InfInut::get_XML() const { return m_xml; }

QString InfInut::get_Id() const
{
    return m_Id;
}

void InfInut::set_Id(const QString &Id)
{
    m_Id = Id;
}

TpAmb InfInut::get_tpAmb() const
{
    return m_tpAmb;
}

void InfInut::set_tpAmb(const TpAmb &tpAmb)
{
    m_tpAmb = tpAmb;
}

QString InfInut::get_xServ() const
{
    return m_xServ;
}

void InfInut::set_xServ(const QString &xServ)
{
    m_xServ = xServ;
}

int InfInut::get_cUF() const
{
    return m_cUF;
}

void InfInut::set_cUF(const int &cUF)
{
    m_cUF = cUF;
}

int InfInut::get_ano() const
{
    return m_ano;
}

void InfInut::set_ano(const int &ano)
{
    m_ano = ano;
}

QString InfInut::get_CNPJ() const
{
    return m_CNPJ;
}

void InfInut::set_CNPJ(const QString &CNPJ)
{
    m_CNPJ = CNPJ;
}

ModeloDF InfInut::get_mod() const
{
    return m_mod;
}

void InfInut::set_mod(const ModeloDF &mod)
{
    m_mod = mod;
    m_modInformado = true;
}

int InfInut::get_serie() const
{
    return m_serie;
}

void InfInut::set_serie(const int &serie)
{
    m_serie = serie;
}

int InfInut::get_nNFIni() const
{
    return m_nNFIni;
}

void InfInut::set_nNFIni(const int &nNFIni)
{
    m_nNFIni = nNFIni;
}

int InfInut::get_nNFFin() const
{
    return m_nNFFin;
}

void InfInut::set_nNFFin(const int &nNFFin)
{
    m_nNFFin = nNFFin;
}

QString InfInut::get_xJust() const
{
    return m_xJust;
}

void InfInut::set_xJust(const QString &xJust)
{
    m_xJust = xJust;
}

QString InfInut::get_verAplic() const
{
    return m_verAplic;
}

void InfInut::set_verAplic(const QString &verAplic)
{
    m_verAplic = verAplic;
}

int InfInut::get_cStat() const
{
    return m_cStat;
}

void InfInut::set_cStat(const int &cStat)
{
    m_cStat = cStat;
}

QString InfInut::get_xMotivo() const
{
    return m_xMotivo;
}

void InfInut::set_xMotivo(const QString &xMotivo)
{
    m_xMotivo = xMotivo;
}

QDateTime InfInut::get_dhRecbto() const
{
    return m_dhRecbto;
}

void InfInut::set_dhRecbto(const QDateTime &dhRecbto)
{
    m_dhRecbto = dhRecbto;
}

QString InfInut::get_nProt() const
{
    return m_nProt;
}

void InfInut::set_nProt(const QString &nProt)
{
    m_nProt = nProt;
}

bool InfInut::preparar()
{
    if (!config || !pedido)
        return falhar("O envio deve ser feito pelo infInut do pedido de inutilização.");
    if (pedido->get_versao().isEmpty())
        pedido->set_versao("4.00");
    if (m_tpAmb == TpAmb::None)
        m_tpAmb = config->webservices->get_tpAmb();
    if (m_cUF == 0)
        m_cUF = ConvNF::ufToInt(config->webservices->get_uf());
    if (!m_modInformado)
        m_mod = config->get_ModeloDF();
    if (m_xServ.isEmpty())
        m_xServ = "INUTILIZAR";
    if (pedido->get_versao() != "4.00")
        return falhar("Inutilização exige a versão 4.00.");
    if ((m_tpAmb != TpAmb::Producao && m_tpAmb != TpAmb::Homologacao) ||
        m_tpAmb != config->webservices->get_tpAmb())
        return falhar("O ambiente do pedido diverge do ambiente configurado.");
    if (m_cUF <= 0 || m_cUF != ConvNF::ufToInt(config->webservices->get_uf()))
        return falhar("A UF do pedido diverge da UF configurada.");
    if ((m_mod != ModeloDF::NFe && m_mod != ModeloDF::NFCe) || m_mod != config->get_ModeloDF())
        return falhar("Informe o modelo 55 ou 65 correspondente ao modelo configurado.");
    if (m_xServ != "INUTILIZAR")
        return falhar("O serviço solicitado deve ser INUTILIZAR.");
    if (m_ano < 6 || m_ano > QDate::currentDate().year() % 100)
        return falhar("Informe o ano com duas posições, de 06 até o ano atual.");
    if (!QRegularExpression("^[0-9A-Z]{12}[0-9]{2}\\z").match(m_CNPJ).hasMatch())
        return falhar("Informe o CNPJ com 14 posições, com letras permitidas nas primeiras 12 posições.");
    if (m_serie < 0 || m_serie > 999 || (m_serie >= 910 && m_serie <= 969))
        return falhar("Informe uma série de 0 a 999, exceto a faixa de emitente CPF (910 a 969).");
    if (m_nNFIni < 1 || m_nNFFin > 999999999 || m_nNFFin < m_nNFIni)
        return falhar("Informe uma faixa de numeração válida de 1 a 999999999.");
    if (m_nNFFin - m_nNFIni + 1 > 10000)
        return falhar("A faixa de inutilização não pode exceder 10000 números.");
    if (m_xJust.trimmed().size() < 15 || m_xJust.size() > 255)
        return falhar("Informe uma justificativa de 15 a 255 caracteres.");
    m_Id = "ID" + QString::number(m_cUF) + CppUtility::insZeroLeft(QString::number(m_ano), 2)
        + m_CNPJ + ConvNF::modeloDFToStr(m_mod) + CppUtility::insZeroLeft(QString::number(m_serie), 3)
        + CppUtility::insZeroLeft(QString::number(m_nNFIni), 9)
        + CppUtility::insZeroLeft(QString::number(m_nNFFin), 9);
    return true;
}

bool InfInut::gerarXML()
{
    m_error.clear();
    m_Id.clear();
    m_xmlOriginal.clear();
    m_xmlAssinado.clear();
    m_xml.clear();
    if (pedido)
        pedido->retInutNFe->clear();
    if (!preparar())
        return false;
    if (!config->certificado->get_cryptoLib() ||
        config->certificado->get_cryptoLib()->get_certDER().isEmpty() ||
        config->certificado->get_cryptoLib()->get_pkeyDER().isEmpty())
        return falhar("Carregue o certificado digital antes de inutilizar a numeração.");
    QXmlStreamWriter xml(&m_xmlOriginal);
    xml.writeStartElement("inutNFe");
    xml.writeDefaultNamespace(namespaceNFe);
    xml.writeAttribute("versao", pedido->get_versao());
    xml.writeStartElement("infInut");
    xml.writeAttribute("Id", m_Id);
    xml.writeTextElement("tpAmb", ConvNF::tpAmbToStr(m_tpAmb));
    xml.writeTextElement("xServ", m_xServ);
    xml.writeTextElement("cUF", QString::number(m_cUF));
    xml.writeTextElement("ano", CppUtility::insZeroLeft(QString::number(m_ano), 2));
    xml.writeTextElement("CNPJ", m_CNPJ);
    xml.writeTextElement("mod", ConvNF::modeloDFToStr(m_mod));
    xml.writeTextElement("serie", QString::number(m_serie));
    xml.writeTextElement("nNFIni", QString::number(m_nNFIni));
    xml.writeTextElement("nNFFin", QString::number(m_nNFFin));
    xml.writeTextElement("xJust", m_xJust);
    xml.writeEndElement();
    xml.writeEndElement();
    CppLibXml2 lib(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    QByteArray assinado;
    if (!lib.signXML(m_xmlOriginal.toUtf8(), HashType::sha1, "infInut", "SignedInfo",
                     m_Id.toUtf8(), XMLReturnType::XMLSign, assinado))
        return falhar(lib.get_error());
    if (!lib.parseXML(assinado, "inutNFe_v4.00.xsd"))
        return falhar(lib.get_error());
    m_xmlAssinado = QString::fromUtf8(assinado);
    return true;
}

bool InfInut::inutilizar()
{
    if (!gerarXML())
        return false;
    WSInutilizacao ws(config, config->certificado->get_cryptoLib());
    connect(&ws, &WSInutilizacao::wsChange, this, &InfInut::wsChange);
    const QByteArray resposta = ws.enviar(semDeclaracao(m_xmlAssinado).toUtf8());
    if (!ws.get_error().isEmpty())
        return falhar(ws.get_error());
    return lerRetorno(resposta);
}

bool InfInut::lerRetorno(const QByteArray &dados)
{
    m_error.clear();
    m_xml.clear();
    if (!config || !pedido)
        return falhar("Leia o retorno pelo infInut do pedido de inutilização.");
    pedido->retInutNFe->clear();
    const QString retorno = extrairRetorno(dados);
    if (retorno.isEmpty())
        return falhar("A resposta deve conter um único retInutNFe válido.");
    CppLibXml2 lib(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    if (!lib.parseXML(retorno.toUtf8(), "retInutNFe_v4.00.xsd"))
        return falhar(lib.get_error());
    RetInutNFe &ret = *pedido->retInutNFe;
    InfInut &inf = *ret.infInut;
    QXmlStreamReader xml(retorno);
    xml.readNextStartElement();
    ret.set_versao(xml.attributes().value("versao").toString());
    xml.readNextStartElement(); // infInut
    inf.set_Id(xml.attributes().value("Id").toString());
    QMap<QString, QString> campos;
    while (xml.readNextStartElement())
    {
        const QString nome = xml.name().toString();
        campos[nome] = xml.readElementText();
    }
    inf.set_tpAmb(ConvNF::strToTpAmb(campos.value("tpAmb")));
    inf.set_verAplic(campos.value("verAplic"));
    inf.set_cStat(campos.value("cStat").toInt());
    inf.set_xMotivo(campos.value("xMotivo"));
    inf.set_cUF(campos.value("cUF").toInt());
    inf.set_ano(campos.value("ano").toInt());
    inf.set_CNPJ(campos.value("CNPJ"));
    if (campos.contains("mod"))
        inf.set_mod(ConvNF::strToModeloDF(campos.value("mod")));
    if (campos.contains("serie"))
        inf.set_serie(campos.value("serie").toInt());
    inf.set_nNFIni(campos.value("nNFIni").toInt());
    inf.set_nNFFin(campos.value("nNFFin").toInt());
    inf.set_dhRecbto(QDateTime::fromString(campos.value("dhRecbto"), Qt::ISODate));
    inf.set_nProt(campos.value("nProt"));
    ret.m_xml = retorno;
    QMap<QString, QString> enviados;
    QString idEnviado;
    QXmlStreamReader original(m_xmlAssinado);
    if (original.readNextStartElement() && original.readNextStartElement())
    {
        idEnviado = original.attributes().value("Id").toString();
        while (original.readNextStartElement())
        {
            const QString nome = original.name().toString();
            enviados[nome] = original.readElementText();
        }
    }
    const TpAmb ambiente = enviados.isEmpty() ? m_tpAmb : ConvNF::strToTpAmb(enviados.value("tpAmb"));
    const int uf = enviados.isEmpty() ? m_cUF : enviados.value("cUF").toInt();
    if (inf.get_tpAmb() != ambiente || inf.get_cUF() != uf)
        return falhar("O ambiente ou a UF do retorno diverge do pedido enviado.");
    if (inf.get_cStat() != 102)
        return true;
    for (const QString &campo : {QString("ano"), QString("CNPJ"), QString("mod"),
                                 QString("serie"), QString("nNFIni"), QString("nNFFin")})
    {
        if (!campos.contains(campo) || campos.value(campo) != enviados.value(campo))
            return falhar("A confirmação de inutilização não corresponde ao pedido enviado.");
    }
    if (inf.get_nProt().isEmpty())
        return falhar("A confirmação de inutilização deve informar o protocolo.");
    if (m_xmlAssinado.isEmpty())
        return falhar("Não há pedido assinado para montar a inutilização processada.");
    const QString processado = "<ProcInutNFe xmlns=\"" + namespaceNFe + "\" versao=\"4.00\">"
        + semDeclaracao(m_xmlAssinado) + retorno + "</ProcInutNFe>";
    if (!lib.parseXML(processado.toUtf8(), "procInutNFe_v4.00.xsd"))
        return falhar(lib.get_error());
    m_xml = processado;
    if (config->arquivos->get_salvar() && !CppUtility::saveFile(config->arquivos->get_caminhoEvento(),
        idEnviado.mid(2) + "-procInutNFe", TipoArquivo::XML, m_xml.toUtf8()))
        return falhar("Não foi possível salvar o XML da inutilização processada.");
    return true;
}

RetInutNFe::RetInutNFe() : infInut(new InfInut()) {}
void RetInutNFe::clear()
{
    m_versao.clear();
    m_xml.clear();
    infInut->clear();
}
QString RetInutNFe::get_versao() const { return m_versao; }
void RetInutNFe::set_versao(const QString &versao) { m_versao = versao; }
QString RetInutNFe::get_XML() const { return m_xml; }

InutNFe::InutNFe(ConfigNFe *config) : infInut(new InfInut(config, this)),
    retInutNFe(new RetInutNFe()), m_versao("4.00")
{
    connect(infInut.get(), &InfInut::wsChange, this, &InutNFe::wsChange);
    connect(infInut.get(), &InfInut::errorOccurred, this, &InutNFe::errorOccurred);
}
void InutNFe::clear()
{
    m_versao = "4.00";
    infInut->clear();
    retInutNFe->clear();
}
QString InutNFe::get_versao() const { return m_versao; }
void InutNFe::set_versao(const QString &versao) { m_versao = versao; }
