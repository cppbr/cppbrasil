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


#include "cpplibxml2.h"
CppLibXml2::CppLibXml2(CppCrypto *cppcrypto, QByteArray &schemapath)
    :m_cppcrypto(cppcrypto), m_schemapath(schemapath)
{
}

CppLibXml2::~CppLibXml2()
{
}

bool CppLibXml2::addChildXML(const QByteArray &xml_doc, const QByteArray &xml_child,
                             const QByteArray &parent_node, QByteArray &output)
{
    char * _res = NULL;
    if ((xml_doc.isEmpty()) || (xml_child.isEmpty()))
    {
        set_error(QStringLiteral("Erro ao adichinar xml filho: documento xml vazio."));
        return false;
    }


    if (!addChildFromString(xml_doc.data(), xml_child.data(),
                            (parent_node.isEmpty() ? NULL : parent_node.data()), &_res))
    {
        set_error(QStringLiteral("Erro ao adichinar xml filho: documento xml com erros."));
        return false;
    }

    //copy result to output
    output.clear();
    int _len = strlen(_res);
    for(int i = 0; i < _len; i++)
    {
        //remove caracter new line
        if (_res[i] != '\n')
         output.append(_res[i]);
    }
    if (_res != NULL)
      free(_res);

    return true;
}

bool CppLibXml2::addChildXML(QByteArray &xml_doc, const QByteArray &xml_child, const QByteArray &parent_node)
{
    char * _res = NULL;
    if ((xml_doc.isEmpty()) || (xml_child.isEmpty()))
    {
        set_error(QStringLiteral("Erro ao adichinar xml filho: documento xml vazio."));
        return false;
    }


    if (!addChildFromString(xml_doc.data(), xml_child.data(),
                            (parent_node.isEmpty() ? NULL : parent_node.data()), &_res))
    {
        set_error(QStringLiteral("Erro ao adichinar xml filho: documento xml com erros."));
        return false;
    }

    //copy result
    xml_doc.clear();
    int _len = strlen(_res);
    for(int i = 0; i < _len; i++)
    {
        //remove caracter new line
        if (_res[i] != '\n')
         xml_doc.append(_res[i]);
    }
    if (_res != NULL)
      free(_res);

    return true;

}

bool CppLibXml2::parseSign(const QByteArray &xml_doc)
{
    //not implemented
    if (xml_doc.isEmpty())
        return false;
    return true;
}

bool CppLibXml2::parseXML(const QByteArray &xml_doc, const QByteArray &schema_name)
{
    QString _path;
    QByteArray _schemafile;
    xmlDoc *_xml, *_xsd;
    xmlSchemaParserCtxt *_parser_ctxt = NULL;
    xmlSchema *_schema = NULL;
    xmlSchemaValidCtxt *_valid_ctxt = NULL;
    xmlError *_schemError = NULL;
    _xml = NULL;
    _xsd = NULL;
    bool _ret = true;


    if (!CppUtility::parsePath(QString::fromLocal8Bit(m_schemapath.data(),m_schemapath.length()), _path))
    {
       set_error(QStringLiteral("Diretório do schema inválido ou sem permissão para leitura"));
       _ret =  false;
    }
    _schemafile.append(_path.toLocal8Bit());
    _schemafile.append(schema_name.data());

    _xml = xmlParseMemory(xml_doc.data(),xml_doc.length());
    _xsd = xmlParseFile(_schemafile.data());
    _parser_ctxt = xmlSchemaNewDocParserCtxt(_xsd);
    _schema = xmlSchemaParse(_parser_ctxt);
    _valid_ctxt = xmlSchemaNewValidCtxt(_schema);


    if (_ret)
    {
        if ((_xml == NULL) || (xmlDocGetRootElement(_xml) == NULL))
        {
            set_error(QStringLiteral("Erro: Falha ao interpretar o XML xmlParseDoc"));
            _ret = false;
        }
    }
    if (_ret)
    {
        if (_schema == NULL)
        {
            set_error(QStringLiteral("Erro: Schema inválido"));
            _ret = false;
        }
    }
    if (_ret)
    {
        if (_valid_ctxt == NULL)
        {
            set_error(QStringLiteral("Erro: Não foi possivel criar um contexto para o Schema"));
            _ret = false;
        }

    }
    if (_ret)
    {
        if (xmlSchemaValidateDoc(_valid_ctxt, _xml) != 0)
        {
            _schemError = xmlGetLastError();
            _ret = false;
            if (_schemError != NULL)
            {
                set_error(QString("%1%2%3%4").arg("Erro: ",QString::number(_schemError->code), " - ", _schemError->message));
            }else
            {
                set_error(QString("%1%2").arg(QStringLiteral("Erro indefinido ao validar XML com o Schema: "), schema_name.data()));
            }
        }
    }

    if (_xml != NULL)
      xmlFreeDoc(_xml);
    if (_xsd != NULL)
      xmlFreeDoc(_xsd);
    if (_parser_ctxt != NULL)
      xmlSchemaFreeParserCtxt(_parser_ctxt);
    if (_schema != NULL)
      xmlSchemaFree(_schema);
    if (_valid_ctxt != NULL)
      xmlSchemaFreeValidCtxt(_valid_ctxt);
    return _ret;
}

bool CppLibXml2::setNode(const QByteArray &xml_doc, const QByteArray &node_name,
                         const QByteArray &value, QByteArray &output)
{
    xmlDoc *_xml = NULL;
    bool _ret = true;
    int _len;
    unsigned char *_res = NULL;

    //criando o main document de string xml_doc
    _len = strlen(xml_doc);
    if ((_xml = xmlParseMemory(xml_doc, _len)) == NULL)
      _ret = false;

    //insira no _xml_sigin os valores _x509 e _signature
    if (_ret)
    {
        if (!nodeSetContent(_xml, node_name.data(), value.data()))
          _ret = false;
    }
    if (_ret)
    {
        xmlDocDumpMemoryEnc(_xml, &_res, &_len,"UTF-8");
        //se a saída não tiver memória, obtenha memória para a saída
        for(int i = 0; i < _len; i++)
        {
            //remove o caracter new line
            if (_res[i] != '\n')
             output.append(_res[i]);
        }
    }
    //if erro
    if (!_res)
      set_error(QStringLiteral("Erro: Error ao setar o nó ") + QString(node_name) + QStringLiteral(" do XML."));

    if (_res != NULL)
      free(_res);
    if (_xml != NULL)
      xmlFreeDoc(_xml);
    return _ret;
}

bool CppLibXml2::signXML(const QByteArray &xml_doc, const HashType &hashtype,
                         const QByteArray &node_doc, const QByteArray &node_sign,
                         const QByteArray &URI, const XMLReturnType &returntype,
                         QByteArray &output)
{
    xmlDoc *_xml, *_xml_sigin;
    char *_ret_xml_doc, *_ret_xml_sigin, *_ret_xml_complete;
    QByteArray _digestvalue, _signature, _c14n_sign, _x509;
    QString _str_xmlsign;
    HashType _sha(hashtype);
    bool _ret = true;
    int _len;
    _xml = NULL;
    _xml_sigin = NULL;
    _ret_xml_doc = NULL;
    _ret_xml_sigin = NULL;
    _ret_xml_complete = NULL;

    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    xmlSubstituteEntitiesDefault(1);


    _len = xml_doc.length();
    if ((_xml = xmlParseMemory(xml_doc.data(), _len)) == NULL)
      _ret = false;


    _x509.append(this->m_cppcrypto->get_certBase64().toLocal8Bit());

    if ((xml_doc.isEmpty()) || (node_doc.isEmpty()) || (node_sign.isEmpty()))
    {
        set_error(QStringLiteral("Erro: xmldoc ou nó do coumento vazio"));
        _ret = false;
    }

    if (_ret)
    {
        //digest
        if (!this->c14n(_xml, node_doc.data(), &_ret_xml_doc))
        {
            set_error(QStringLiteral("Erro c14n: falha na canonização do XML."));
            _ret = false;
        }
    }

    if (_ret)
    {
        if (!this->m_cppcrypto->digest(_sha, QByteArray::fromRawData(_ret_xml_doc,strlen(_ret_xml_doc)), true, _digestvalue))
        {
            //get error crypto class
            this->set_error(this->m_cppcrypto->get_error());
            _ret = false;
        }
    }

    if (_ret)
    {
        _str_xmlsign.append(this->get_signNode(_sha, URI, "", _digestvalue, ""));
        if (_str_xmlsign.isEmpty())
        {
            this->set_error(QStringLiteral("Erro ao gerar nó de assinatura."));
            _ret = false;
        }
    }
    if (_ret)
    {
        _len = _str_xmlsign.length();
        if ((_xml_sigin = xmlParseMemory(_str_xmlsign.toLocal8Bit(), _len)) == NULL)
          _ret = false;
    }

    if (_ret)
    {
        //sign
        if (!this->c14n(_xml_sigin, node_sign.data(), &_ret_xml_sigin))
        {
            set_error(QStringLiteral("Erro c14n: falha na canonização do XML."));
            _ret = false;
        }
    }
    if (_ret)
    {
        if (!this->m_cppcrypto->signature(_sha, _ret_xml_sigin, true, _signature))
        {
            //get error crypto class
            this->set_error(this->m_cppcrypto->get_error());
            _ret = false;
        }
    }

    if (_ret)
    {
        if (returntype == XMLReturnType::XMLSign)
        {
            //node sign
            //insira no _xml_sigin os valores _x509 e _signature
            if (!nodeSetContent(_xml_sigin, (char*)"SignatureValue", _signature.data()))
              _ret = false;
            if (!nodeSetContent(_xml_sigin, (char*)"X509Certificate", _x509.data()))
              _ret = false;

            //inserir xml filho com assinatura no xml pai
           if (addChildFromXMLDOC(_xml, _xml_sigin, NULL, &_ret_xml_complete))
           {

               _len = strlen(_ret_xml_complete);
               for(int i = 0; i < _len; i++)
               {
                   //remove caracter new line
                   if (_ret_xml_complete[i] != '\n')
                    output.append(_ret_xml_complete[i]);
               }
           }else
           {
               this->set_error(QStringLiteral("Erro ao gerar unir xml."));
               _ret = false;
           }

        }else
        {   //retorna apenas a assinatura
            _str_xmlsign.clear();
            _str_xmlsign.append(this->get_signNode(_sha, URI, _x509, _digestvalue, _signature));
            output.append(_str_xmlsign.toLocal8Bit());
        }
    }
    _digestvalue.clear();
    _signature.clear();
    _c14n_sign.clear();
    _x509.clear();
    _str_xmlsign.clear();


    if (_xml != NULL)
      xmlFreeDoc(_xml);
    if (_xml_sigin != NULL)
      xmlFreeDoc(_xml_sigin);
    if (_ret_xml_doc != NULL)
      free(_ret_xml_doc);
    if (_ret_xml_sigin != NULL)
      free(_ret_xml_sigin);
    if (_ret_xml_complete != NULL)
      free(_ret_xml_complete);

    return _ret;
}

bool CppLibXml2::c14n(const xmlDoc *xml_doc, const char *node_name,
                      char **output)
{
    xmlDoc *_new_xmldoc;
    xmlNode *_root_node, *_node_element, *_new_node;
    _new_xmldoc = NULL;
    _root_node = NULL;
    _node_element = NULL;
    _new_node = NULL;
    bool _ret = true;
    unsigned char *_res = NULL;
    int _len;


    if((_root_node = xmlDocGetRootElement(xml_doc)) == NULL) {
        _ret = false;
    }

    if (_ret)
    {
       //find element
       if((_node_element = find_node(_root_node, node_name)) == NULL)
       {
            xmlFreeNode(_root_node);
            _ret = false;
        }
    }

    if (_ret)
    {
        if((_new_node = xmlCopyNode(_node_element, 1)) == NULL)
        {
            _ret = false;
        }
    }
    if (_ret)
    {
        _new_xmldoc = xmlNewDoc((const xmlChar *) "1.0");
        xmlDocSetRootElement(_new_xmldoc, _new_node);

        if(_new_xmldoc == NULL)
        {
            _ret = false;
        }
    }
    _len = xmlC14NDocDumpMemory(_new_xmldoc, NULL, 0, NULL, 0, &_res);

    //copy result
    if(_len > 0)
    {
        //se a saída não tiver memória, obtenha memória para a saída
        if ((*output == NULL) || (*output[0] == '\0')) {
            *output = (char*) calloc(_len + 1, sizeof(char));
        }
        memcpy(*output,_res,_len);
    }
    else
    {
        _ret = false;
    }
    //ok
    if(_res != NULL)
      xmlFree(_res);
    if (_new_xmldoc != NULL)
      xmlFreeDoc(_new_xmldoc);

    return _ret;
}

xmlNode *CppLibXml2::find_node(xmlNode *node, const char *node_name)
{
    xmlNode * _result = NULL;

    if (node == NULL)
      return NULL;

    while(node)
    {
      if((node->type == XML_ELEMENT_NODE) && (strcmp((const char*)node->name, node_name) == 0))
      {
        return node;
      }
      _result = find_node(node->children, node_name);
      if (_result != NULL)
        return _result;

      node = node->next;
    }

    return NULL;
}

bool CppLibXml2::addChildFromXMLDOC(const xmlDoc *xml_doc, const xmlDoc *xml_child,
                                    const char *parent, char **output)
{
    xmlDoc *_newdoc;
    xmlNode *_root_node, *_parent_node, *_new_node, *_child_node;
    _newdoc = NULL;
    _root_node = NULL;
    _new_node = NULL;
    _parent_node = NULL;
    _child_node = NULL;
    unsigned char *_res = NULL;
    bool _ret = true;
    int _len;

    //novo documento de root node
    if ((_root_node = xmlDocGetRootElement(xml_doc)) != NULL)
    {
        _newdoc = xmlNewDoc((const xmlChar *) "1.0");
        xmlDocSetRootElement(_newdoc, _root_node);
        _root_node = xmlDocGetRootElement(_newdoc);
    }
    else
      _ret = false;

    //find node
    if (_ret)
    {
        //se o pai não for nulo, procure pai, senão, pai = nó raiz.
        if (parent != NULL)
        {
            //find parent
            if((_parent_node = find_node(_root_node, parent)) == NULL)
              _ret = false;
        }else //root node
          _parent_node = _root_node;
    }
    //create new node
    if (_ret)
    {
        _child_node = xmlCopyNode(xmlDocGetRootElement(xml_child)->children, 1);
        _new_node = xmlAddChildList(_parent_node, _child_node);

        if (_new_node == NULL)
          _ret = false;
    }

    if (_ret)
    {
        //resultado para string sem codificação UTF-8
        //xmlDocDumpMemory(_newdoc, &_res, &_len);
        //resultado para string com codificação UTF-8
        xmlDocDumpMemoryEnc(_newdoc, &_res, &_len,"UTF-8");
        //if output does not have memory, get memory for output
        if (*output == NULL)
        {
            *output = (char*) calloc(_len + 1, sizeof(char));
        }
        memcpy(*output,_res,_len);

    }
    //clear memory
    if (_res != NULL)
       xmlFree(_res);
    if (_newdoc != NULL)
       xmlFreeDoc(_newdoc);
    xmlCleanupParser();
    return _ret;
}

bool CppLibXml2::addChildFromString(const char *xml_doc, const char *xml_child,
                                    const char *parent, char **output)
{
    xmlDoc *_xml, *_xml_child, *_newdoc;
    xmlNode *_root_node, *_parent_node, *_new_node, *_child_node;
    _xml = NULL;
    _xml_child = NULL;
    _newdoc = NULL;
    _root_node = NULL;
    _parent_node = NULL;
    _new_node = NULL;
    unsigned char *_res = NULL;
    bool _ret = true;
    int _len;

    //main document de string xml_doc
    _len = strlen(xml_doc);
    if ((_xml = xmlParseMemory(xml_doc, _len)) == NULL)
      _ret = false;

    //cria xml child de string xml_child
    if (_ret)
    {
        _len = strlen(xml_child);
        if ((_xml_child = xmlParseMemory(xml_child, _len)) == NULL)
          _ret = false;
    }

    //root node de _xml
    if (_ret)
    {
        if ((_root_node = xmlDocGetRootElement(_xml)) != NULL)
        {
            _newdoc = xmlNewDoc((const xmlChar *) "1.0");
            xmlDocSetRootElement(_newdoc, _root_node);
            _root_node = xmlDocGetRootElement(_newdoc);
        }
        else
          _ret = false;
    }
    //procura node
    if (_ret)
    {
        //se o pai não for nulo, procure pai, senão, pai = nó raiz.
        if (parent != NULL)
        {
            //procura parent
            if((_parent_node = find_node(_root_node, parent)) == NULL)
              _ret = false;
        }else //root node
          _parent_node = _root_node;
    }
    //new node
    if (_ret)
    {
        _child_node = xmlCopyNode(xmlDocGetRootElement(_xml_child)->children, 1);
        _new_node = xmlAddChildList(_parent_node, _child_node);

        if (_new_node == NULL)
          _ret = false;
    }

    if (_ret)
    {
        // resultado para string sem codificação UTF-8
        //xmlDocDumpMemory(_newdoc, &_res, &_len);
        // resultado para string com codificação UTF-8
        xmlDocDumpMemoryEnc(_newdoc, &_res, &_len,"UTF-8");
        //se a saída não tiver memória, obtenha memória para a saída
        if (*output == NULL)
        {
            *output = (char*) calloc(_len + 1, sizeof(char));
        }
        //copy result to output
        memcpy(*output,_res,_len);
    }
    //clear memory
    if (_res != NULL)
        xmlFree(_res);
    if (_xml != NULL)
      xmlFreeDoc(_xml);
    if (_xml_child != NULL)
      xmlFreeDoc(_xml_child);
    if (_newdoc != NULL)
      xmlFreeDoc(_newdoc);

    return _ret;
}

bool CppLibXml2::nodeSetContent(const xmlDoc *xml_doc, const char *node_name,
                                const char *value)
{
    xmlNode * _node = NULL;

    _node = find_node(xmlDocGetRootElement(xml_doc), node_name);
    if (_node == NULL)
    {
       return false;
    }

    xmlNodeSetContent(_node, (unsigned char*)value);
    return true;
}
