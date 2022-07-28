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


#ifndef CPPXML_H
#define CPPXML_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cppcrypto.h>

enum class XMLReturnType {XMLSign, SignNode};

class CPPBRASIL_EXPORT CppXML : public QObject
{
    Q_OBJECT

public:
    CppXML();
    virtual ~CppXML();
    QString get_error() const;
    //add child e retorna em output
    virtual bool addChildXML(const QByteArray &xml_doc, const QByteArray &xml_child,
                                    const QByteArray &parent_node, QByteArray &output);
    //add child e retorna no memos xml_doc
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
protected:
    void set_error(const QString &error);
    QString get_signNode(const HashType &hashType, const QByteArray &URI,
                      const QByteArray &X509cert, const QByteArray &digest,
                      const QByteArray &signature);

signals:
    void errorOccurred(const QString &error);

private:
    QString m_error;
};

#endif // CPPXML_H
