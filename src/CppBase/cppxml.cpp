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


#include "cppxml.h"

CppXML::CppXML()
{

}

CppXML::~CppXML()
{

}

QString CppXML::get_error() const
{
    return this->m_error;
}

bool CppXML::addChildXML(const QByteArray &xml_doc, const QByteArray &xml_child,
                         const QByteArray &parent_node, QByteArray &output)
{
    Q_UNUSED(xml_doc);
    Q_UNUSED(xml_child);
    Q_UNUSED(parent_node);
    Q_UNUSED(output);
    return false;
}

bool CppXML::addChildXML(QByteArray &xml_doc, const QByteArray &xml_child,
                         const QByteArray &parent_node)
{
    Q_UNUSED(xml_doc);
    Q_UNUSED(xml_child);
    Q_UNUSED(parent_node);
    return false;

}

bool CppXML::parseSign(const QByteArray &xml_doc)
{
    Q_UNUSED(xml_doc);
    return false;
}

bool CppXML::parseXML(const QByteArray &xml_doc, const QByteArray &schema_name)
{
    Q_UNUSED(xml_doc);
    Q_UNUSED(schema_name);
    return false;
}

bool CppXML::setNode(const QByteArray &xml_doc, const QByteArray &node_name,
                     const QByteArray &value, QByteArray &output)
{
    Q_UNUSED(xml_doc);
    Q_UNUSED(node_name);
    Q_UNUSED(value);
    Q_UNUSED(output);
    return false;
}

bool CppXML::signXML(const QByteArray &xml_doc, const HashType &hashtype,
                     const QByteArray &node_doc, const QByteArray &node_sign,
                     const QByteArray &URI, const XMLReturnType &returntype,
                     QByteArray &output)
{
    Q_UNUSED(xml_doc);
    Q_UNUSED(hashtype);
    Q_UNUSED(node_doc);
    Q_UNUSED(node_sign);
    Q_UNUSED(URI);
    Q_UNUSED(returntype);
    Q_UNUSED(output);
    return false;
}

void CppXML::set_error(const QString &error)
{
    this->m_error = error;
    emit errorOccurred(error);
}

QString CppXML::get_signNode(const HashType &hashType, const QByteArray &URI,
                          const QByteArray &X509cert, const QByteArray &digest,
                          const QByteArray &signature)
{
    QString _xmlSigin;
    QString _hash = CppCrypto::hashTypeToStr(hashType);
    try{
        _xmlSigin += QStringLiteral("<a>");
        _xmlSigin += QStringLiteral("<Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\">");
        _xmlSigin +=  QStringLiteral("<SignedInfo>");
        _xmlSigin +=   QStringLiteral("<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/>");
        _xmlSigin +=   QStringLiteral("<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-"); _xmlSigin += _hash; _xmlSigin += QStringLiteral("\"/>");
        _xmlSigin +=   QStringLiteral("<Reference URI=\"#"); _xmlSigin += QString(URI); _xmlSigin += QStringLiteral("\">");
        _xmlSigin +=   QStringLiteral("<Transforms>");
        _xmlSigin +=    QStringLiteral("<Transform Algorithm=\"http://www.w3.org/2000/09/xmldsig#enveloped-signature\"/>");
        _xmlSigin +=    QStringLiteral("<Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/>");
        _xmlSigin +=   QStringLiteral("</Transforms>");
        _xmlSigin +=   QStringLiteral("<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#"); _xmlSigin += _hash; _xmlSigin += QStringLiteral("\"/>");
        _xmlSigin +=   QStringLiteral("<DigestValue>"); _xmlSigin += QString(digest); _xmlSigin += QStringLiteral("</DigestValue>");
        _xmlSigin +=  QStringLiteral("</Reference>");
        _xmlSigin +=  QStringLiteral("</SignedInfo>");
        _xmlSigin +=  QStringLiteral("<SignatureValue>"); _xmlSigin += (signature.isEmpty() ? "" : QString(signature)); _xmlSigin += QStringLiteral("</SignatureValue>");
        _xmlSigin +=  QStringLiteral("<KeyInfo>");
          _xmlSigin += QStringLiteral("<X509Data>");
          _xmlSigin +=  QStringLiteral("<X509Certificate>"); _xmlSigin += (X509cert.isEmpty() ? "" : QString(X509cert)); _xmlSigin += QStringLiteral("</X509Certificate>");
          _xmlSigin += QStringLiteral("</X509Data>");
        _xmlSigin +=  QStringLiteral("</KeyInfo>");
        _xmlSigin += QStringLiteral("</Signature>");
        _xmlSigin += QStringLiteral("</a>");

    } catch (std::exception const& e) {
         this->set_error(QStringLiteral("Erro ao gerar nó de assinatura: ") + e.what());
    }
    return _xmlSigin;
}
