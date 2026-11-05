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


#ifndef CPPLIBXML2_H
#define CPPLIBXML2_H

#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>
#include <libxml/c14n.h>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppxml.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/cppcrypto.h>
#include <CppBrasil/cppopenssl.h>

class CPPNFE_EXPORT CppLibXml2: public CppXML
{
public:
    CppLibXml2(CppCrypto *cppcrypto, const QByteArray &schemapath);
    ~CppLibXml2();

    // CppXML interface
    virtual bool addChildXML(const QByteArray &xml_doc, const QByteArray &xml_child,
                             const QByteArray &parent_node, QByteArray &output);
    virtual bool addChildXML(QByteArray &xml_doc, const QByteArray &xml_child,
                             const QByteArray &parent_node);
    virtual bool parseSign(const QByteArray &xml_doc);
    virtual bool parseXML(const QByteArray &xml_doc, const QByteArray &schema_name);
    virtual bool setNode(const QByteArray &xml_doc, const QByteArray &node_name,
                         const QByteArray &value, QByteArray &output);
    virtual bool signXML(const QByteArray &xml_doc, const HashType &hashtype,
                         const QByteArray &node_doc, const QByteArray &node_sign,
                         const QByteArray &URI, const XMLReturnType &returntype,
                         QByteArray &output);
private:
    CppCrypto *cppcrypto;
    QByteArray m_schemapath;

    bool c14n(const xmlDoc *xml_doc, const char *node_name,
              char **output);
    xmlNode * find_node(xmlNode * node, const char * node_name);
    bool addChildFromXMLDOC(const xmlDoc *xml_doc, const xmlDoc *xml_child,
                            const char *parent, char **output);
    bool addChildFromString(const char *xml_doc, const char *xml_child,
                             const char *parent, char **output);
    bool nodeSetContent(const xmlDoc *xml_doc, const char *node_name,
                        const char* value);

};

#endif // CPPLIBXML2_H
