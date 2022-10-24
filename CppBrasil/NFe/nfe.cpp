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
    HashType _sha1 = HashType::sha1;
    QByteArray _xmldoc, _schema, _nodedoc, _nodesign, _uri;
    QByteArray _xml_online, _xml_offline, _xml_infNFeSupl, _xml_signature;
    QString _digestvalue;
    XMLReturnType _returntype = XMLReturnType::XMLSign; //retornar xml assinado

    _xmldoc.append(this->m_xml_original.toLocal8Bit());
    _schema.append(config->arquivos->get_caminhoSchema().toLocal8Bit());
    _nodedoc.append("infNFe");
    _nodesign.append("SignedInfo");
    _uri.append("NFe"); _uri.append(this->m_chaveacesso.toLocal8Bit());

    CppXML * _libxml = new CppLibXml2(config->certificado->get_cryptoLib(), _schema);

    this->m_error.clear(); //limpando erro

    //caso emissao normal ou modelo NFe a nota retorna assinada e completa.
    if ((this->infNFe->ide->get_tpEmis() == TpEmis::Normal) ||
       (this->infNFe->ide->get_mod() == ModeloDF::NFe))
    {
        //caso normal apenas assina, ja que o documento está completo
        if (_libxml->signXML(_xmldoc, _sha1, _nodedoc, _nodesign, _uri,
                             _returntype, _xml_online))
        {
            this->m_xml_assinado.clear();
            this->m_xml_assinado.append(_xml_online);
            //caso NFCe passa a tag infNFeSupl para a classe InfNFeSupl
            if (this->infNFe->ide->get_mod() == ModeloDF::NFCe)
            {
                this->infNFeSupl->setFromNode(CppUtility::extractStr(_xml_online, QStringLiteral("<infNFeSupl>"), QStringLiteral("</infNFeSupl>")));
            }
        }else
        {
            this->m_error = _libxml->get_error();
        }
    }else
    {
        //caso NFCe contingência, deverá assinar e gerar a tag infNFeSupl após a assinatura,
        //Esta tag necessita do DigestValue para completar suas informações
        if (this->infNFe->ide->get_mod() == ModeloDF::NFCe)
        {
            //retorna somente o nó de assinatura
            _returntype = XMLReturnType::SignNode;
            if (_libxml->signXML(_xmldoc, _sha1, _nodedoc, _nodesign, _uri,
                                 _returntype, _xml_signature))
            {
                //copiando o digestvalue da assinatura
                _digestvalue = CppUtility::extractStr(QString::fromLocal8Bit(_xml_signature),
                                                      QStringLiteral("<DigestValue>"), QStringLiteral("</DigestValue>"));
                _digestvalue = _digestvalue.mid(QStringLiteral("<DigestValue>").length(), _digestvalue.length() - QStringLiteral("<DigestValue></DigestValue>").length());

                //criando o nó infNFeSupl
                _xml_infNFeSupl = XmlWrite::get_infNFeSupl_OFFLINE(get_chNFe(),
                                                                 config->get_VersaoQrCodeNF(),
                                                                 this->infNFe->ide->get_tpAmb(),
                                                                 this->infNFe->ide->get_dhEmi(),
                                                                 this->infNFe->total->ICMSTot->get_vNF(),
                                                                 _digestvalue,
                                                                 config->get_IdCSC(),
                                                                 config->get_CSC(),
                                                                 this->infNFe->ide->get_cUF(),
                                                                 true);

                //primeiro adiciona infNFeSupl ao nó raiz xml.
                //Quando parent_node é "" vazio a função adiciona ao nó raiz
                if (_libxml->addChildXML(_xmldoc, _xml_infNFeSupl, QByteArray(""), _xml_offline))
                {
                    _xmldoc.clear();
                    _xmldoc.append(_xml_offline);
                    _xml_offline.clear();
                    //passando _xml_infNFeSupl para a classe InfNFeSupl
                    this->infNFeSupl->setFromNode(QString::fromLocal8Bit(_xml_infNFeSupl));
                }else
                {
                    this->m_error = _libxml->get_error();
                }

                //segundo adiciona a assinatura ao nó raiz xml
                if (_libxml->addChildXML(_xmldoc, _xml_signature, QByteArray(""), _xml_offline))
                {
                    this->m_xml_assinado.clear();
                    this->m_xml_assinado.append(_xml_offline);
                }else
                {
                    this->m_error = _libxml->get_error();
                }
            }else
            {
                this->m_error = _libxml->get_error();
            }
        }
    }

    //limpando tudo
    _xmldoc.clear();
    _schema.clear();
    _nodedoc.clear();
    _nodesign.clear();
    _uri.clear();
    _xml_online.clear();
    _xml_offline.clear();
    _xml_infNFeSupl.clear();
    _xml_signature.clear();
    _digestvalue.clear();
    delete _libxml;

}

void Nfe::gerarXML(const ConfigNFe *config)
{
    QByteArray _output;
    //limpando dados caso o xml seja importado
    this->m_xml_autorizado.clear();
    this->protNFe->clear();

    this->gerarChaveAcesso();
    std::unique_ptr<XmlWrite> xml(new XmlWrite(this->infNFe.get(), config));
    if (xml->gerarXML(&_output))
    {
        this->m_xml_original.append(_output.data());
    }
}

void Nfe::validarXML(const ConfigNFe *config)
{
    QByteArray _schemaPath(config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray _schemaName;
    _schemaName.append("nfe_v");
    _schemaName.append(ConvNF::versaoNFToStr(config->get_VersaoNF()).toLocal8Bit());
    _schemaName.append(".xsd");
    CppXML * _libxml = new CppLibXml2(config->certificado->get_cryptoLib(), _schemaPath);

    this->m_error.clear(); //limpando erro
    if (!_libxml->parseXML(this->m_xml_assinado.toLocal8Bit(), _schemaName))
    {
       this->m_error = _libxml->get_error();
       //qWarning() << this->m_error; //remover isso
    }

    delete _libxml;
}

QString Nfe::get_XMLAutorizado() const
{
    return this->m_xml_autorizado;
}

void Nfe::set_XMLAutorizado(const QString &xmlAutorizado)
{
    this->m_xml_autorizado = xmlAutorizado;
}

void Nfe::set_chNFe(const QString &chNFe)
{
    this->m_chaveacesso = chNFe;
}

void Nfe::gerarChaveAcesso()
{
    QString cUF = QString::number(this->infNFe->ide->get_cUF());  //Código da UF do emitente do Documento Fiscal
    cUF = CppUtility::insZeroLeft(cUF, 2);
    QString dhEmi = this->infNFe->ide->get_dhEmi().toString("yyMM"); //AAMM - Ano e Mês de emissão da NF-e
    QString cnpj = this->infNFe->emite->get_CNPJ(); //CNPJ do emitente
    QString mod = ConvNF::modeloDFToStr(this->infNFe->ide->get_mod());//Modelo do Documento Fiscal
    QString serie = QString::number(this->infNFe->ide->get_serie());//Serie do Documento Fiscal
    serie = CppUtility::insZeroLeft(serie, 3);
    QString nf = QString::number(this->infNFe->ide->get_nNF()); //Numero do Documento Fiscal
    nf = CppUtility::insZeroLeft(nf, 9);
    QString tpemis = ConvNF::tpEmisToStr((this->infNFe->ide->get_tpEmis()));//forma de emissao da NF-e
    QString cNF =  QString::number(this->infNFe->ide->get_cNF());//Código Numérico que compõe a Chave de Acesso
    cNF = CppUtility::insZeroLeft(cNF, 8);
    QString chaveNF = QString("%1%2%3%4%5%6%7%8").arg(cUF, dhEmi, cnpj, mod, serie, nf,
                                                      tpemis, cNF);
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
