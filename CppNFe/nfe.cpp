/*
Copyright 2022 João Elson
jetmorju@gmail.com

Web-site: https://cppbrasil.com.br

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "nfe.h"
#include <QRegularExpression>

namespace {

QString validarEmissaoNFCe(const InfNFe &infNFe)
{
    const auto &ide = *infNFe.ide;
    const auto processo = ide.get_procEmi();
    const auto tipo = ide.get_tpEmis();
    const bool sc = ConvNF::intToUF(ide.get_cUF()) == UF::SC;
    const bool avulsa = processo == ProcEmi::AvulsaFisco || processo == ProcEmi::AvulsaContribuinte;
    const bool nff = tipo == TpEmis::RegimeEspecialNFF_NaoUsarEste;

    if (avulsa && tipo != TpEmis::Normal && tipo != TpEmis::Contingencia_SVCAN &&
        tipo != TpEmis::Contingencia_SVCRS && !(sc && tipo == TpEmis::Contingencia_NFCe))
        return QStringLiteral("Tipo de emissão incompatível com a emissão pelo Fisco (B26-30).");
    if (!nff && processo != ProcEmi::AplicativoContribuinte && !(sc && avulsa))
        return QStringLiteral("Processo de emissão incompatível com a NFC-e (B26-50).");

    const auto cpf = infNFe.emite->get_CPF();
    if (cpf.isEmpty())
        return {};
    if (!infNFe.emite->get_CNPJ().isEmpty())
        return QStringLiteral("Informe apenas o CPF ou o CNPJ do emitente da NFC-e.");
    if (!CppUtility::cpfValido(cpf))
        return QStringLiteral("CPF do emitente da NFC-e inválido (C02a-20).");

    const int serie = ide.get_serie();
    const bool serieAvulsa = (processo == ProcEmi::AvulsaFisco && serie >= 890 && serie <= 899) ||
                            (avulsa && serie >= 910 && serie <= 919);
    if (!nff && (avulsa ? !serieAvulsa : (serie < 920 || serie > 969)))
        return QStringLiteral("Série incompatível com o CPF e o processo de emissão da NFC-e (C02a-10).");

    return {};
}

} // namespace

Nfe::Nfe(): infNFe(new InfNFe), protNFe(new ProtNFe), infNFeSupl(new InfNFeSupl)
{
}

Nfe::~Nfe()
{
}

void Nfe::clear()
{
    this->m_chaveacesso.clear();
    this->m_error.clear();
    this->m_xml_original.clear();
    this->m_xml_assinado.clear();
    this->m_xml_autorizado.clear();
    this->infNFe->clear();
    this->protNFe->clear();
    this->infNFeSupl->clear();
}

QString Nfe::get_error() const
{
    return this->m_error;
}

QString Nfe::get_chNFe() const
{
    return this->m_chaveacesso;
}

void Nfe::set_chNFe(const QString &chNFe)
{
    this->m_chaveacesso = chNFe;
}

QString Nfe::get_XMLOriginal() const
{
    return this->m_xml_original;
}

QString Nfe::get_XMLAssinado() const
{
    return this->m_xml_assinado;
}

void Nfe::assinarXML(const ConfigNFe *config)
{
    this->m_error.clear();
    this->m_xml_assinado.clear();
    this->infNFeSupl->clear();

    auto *crypto = config->certificado->get_cryptoLib();
    if (!crypto || crypto->get_certBase64().isEmpty())
    {
        this->m_error = QStringLiteral("Carregue o certificado digital antes de assinar a nota.");
        return;
    }

    const auto &ide = *this->infNFe->ide;
    const bool nfce = ide.get_mod() == ModeloDF::NFCe;
    const auto versaoQr = XmlWrite::versaoQrCode(*this->infNFe, *config);
    const bool offline = nfce && (versaoQr == VersaoQrCodeNF::v300
                                      ? ide.get_tpEmis() == TpEmis::Contingencia_NFCe
                                      : ide.get_tpEmis() != TpEmis::Normal);
    CppLibXml2 libxml(crypto, config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray xml = this->m_xml_original.toLocal8Bit();
    const QByteArray uri = QByteArray("NFe") + this->m_chaveacesso.toLocal8Bit();

    QByteArray resultado;
    if (!libxml.signXML(xml, HashType::sha1, "infNFe", "SignedInfo", uri,
                        offline ? XMLReturnType::SignNode : XMLReturnType::XMLSign, resultado))
    {
        this->m_error = libxml.get_error();
        return;
    }

    if (offline)
    {
        QByteArray suplemento;
        if (versaoQr == VersaoQrCodeNF::v300)
        {
            suplemento = XmlWrite::get_infNFeSupl_OFFLINE_V3(get_chNFe(), *this->infNFe,
                                                            *crypto, this->m_error);
            if (suplemento.isEmpty())
                return;
        }
        else
        {
            // A versão 2 usa o DigestValue produzido pela assinatura do XML.
            const QString inicio = QStringLiteral("<DigestValue>");
            const QString fim = QStringLiteral("</DigestValue>");
            const QString tag = CppUtility::extractStr(QString::fromLocal8Bit(resultado), inicio, fim);
            const QString digest = tag.mid(inicio.size(), tag.size() - inicio.size() - fim.size());
            suplemento = XmlWrite::get_infNFeSupl_OFFLINE(get_chNFe(), versaoQr, ide.get_tpAmb(),
                                                          ide.get_dhEmi(), this->infNFe->total->ICMSTot->get_vNF(),
                                                          digest, config->get_IdCSC(), config->get_CSC(),
                                                          ide.get_cUF(), true);
        }

        QByteArray xmlComSuplemento;
        if (!libxml.addChildXML(xml, suplemento, QByteArray(), xmlComSuplemento) ||
            !libxml.addChildXML(xmlComSuplemento, resultado, QByteArray(), xml))
        {
            this->m_error = libxml.get_error();
            return;
        }
        resultado = xml;
    }

    this->m_xml_assinado = QString::fromLocal8Bit(resultado);
    if (nfce)
    {
        this->infNFeSupl->setFromNode(CppUtility::extractStr(this->m_xml_assinado,
                                                            QStringLiteral("<infNFeSupl>"),
                                                            QStringLiteral("</infNFeSupl>")));
    }
}

void Nfe::gerarXML(const ConfigNFe *config)
{
    QByteArray _output;
    //limpando dados caso o xml seja importado ou gerado novamente
    this->m_error.clear();
    this->m_xml_original.clear();
    this->m_xml_assinado.clear();
    this->m_xml_autorizado.clear();
    this->protNFe->clear();
    this->infNFeSupl->clear();

    this->gerarChaveAcesso();
    if (!this->m_error.isEmpty())
        return;

    std::unique_ptr<XmlWrite> xml(new XmlWrite(this->infNFe.get(), config));
    if (xml->gerarXML(&_output))
    {
        this->m_xml_original.append(_output.data());
    }
    else
    {
        this->m_error = "Erro ao gerar o XML da nota.";
    }
}

void Nfe::validarXML(const ConfigNFe *config)
{
    QByteArray _schemaPath(config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray _schemaName;
    _schemaName.append("nfe_v");
    _schemaName.append(ConvNF::versaoNFToStr(config->get_VersaoNF()).toLocal8Bit());
    _schemaName.append(".xsd");
    std::unique_ptr<CppXML> _libxml(new CppLibXml2(config->certificado->get_cryptoLib(), _schemaPath));

    this->m_error.clear(); //limpando erro
    if (!_libxml->parseXML(this->m_xml_assinado.toLocal8Bit(), _schemaName))
    {
       this->m_error = _libxml->get_error();
       //qWarning() << this->m_error; //remover isso
    }
}

QString Nfe::get_XMLAutorizado() const
{
    return this->m_xml_autorizado;
}

void Nfe::set_XMLAutorizado(const QString &xmlAutorizado)
{
    this->m_xml_autorizado = xmlAutorizado;
}

void Nfe::set_XMLOriginal(const QString &xmlOriginal)
{
    this->m_xml_original = xmlOriginal;
}

void Nfe::gerarChaveAcesso()
{
    this->m_error.clear();
    this->m_chaveacesso.clear();
    this->infNFe->set_Id(QString());
    this->infNFe->ide->set_cDV(QString());
    if (this->infNFe->ide->get_mod() == ModeloDF::NFCe)
    {
        this->m_error = validarEmissaoNFCe(*this->infNFe);
        if (!this->m_error.isEmpty())
            return;
    }

    QString cUF = QString::number(this->infNFe->ide->get_cUF());  //Código da UF do emitente do Documento Fiscal
    cUF = CppUtility::insZeroLeft(cUF, 2);
    QString dhEmi = this->infNFe->ide->get_dhEmi().toString("yyMM"); //AAMM - Ano e Mês de emissão da NF-e
    QString cnpj_cpf = (!this->infNFe->emite->get_CNPJ().isEmpty() ?
                        this->infNFe->emite->get_CNPJ() :
                        CppUtility::insZeroLeft(this->infNFe->emite->get_CPF(), 14)) ; //CNPJ ou CPF do emitente
    QString mod = ConvNF::modeloDFToStr(this->infNFe->ide->get_mod());//Modelo do Documento Fiscal
    QString serie = QString::number(this->infNFe->ide->get_serie());//Serie do Documento Fiscal
    serie = CppUtility::insZeroLeft(serie, 3);
    QString nf = QString::number(this->infNFe->ide->get_nNF()); //Numero do Documento Fiscal
    nf = CppUtility::insZeroLeft(nf, 9);
    QString tpemis = ConvNF::tpEmisToStr((this->infNFe->ide->get_tpEmis()));//forma de emissao da NF-e
    QString cNF =  QString::number(this->infNFe->ide->get_cNF());//Código Numérico que compõe a Chave de Acesso
    cNF = CppUtility::insZeroLeft(cNF, 8);
    QString chaveNF = QString("%1%2%3%4%5%6%7%8").arg(cUF, dhEmi, cnpj_cpf, mod, serie, nf,
                                                      tpemis, cNF);
    //a chave sem o DV deve ter 43 digitos numericos, caso contrario a SEFAZ rejeita a nota.
    static const QRegularExpression _re43(QStringLiteral("^\\d{43}$"));
    if (!_re43.match(chaveNF).hasMatch())
    {
        this->m_error = "Chave de acesso invalida (" + chaveNF + "): verifique cUF, dhEmi, CNPJ/CPF do emitente "
                        "(somente numeros), mod, serie, nNF, tpEmis e cNF.";
        return;
    }
    QString cdv = QString::number(CppUtility::dvModulo11(chaveNF)); //Digito Verificador da Chave de Acesso
    //inserindo cDV na chave de acesso
    chaveNF.append(cdv);
    //inserindo o cDV
    this->infNFe->ide->set_cDV(cdv);
    //salvando chave de acesso
    this->m_chaveacesso = chaveNF;
    //id - Informar a Chave de Acesso precedida do literal NFe
    chaveNF.insert(0,"NFe");
    this->infNFe->set_Id(chaveNF);
}
