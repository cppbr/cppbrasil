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


#ifndef NFE_H
#define NFE_H

#include <memory>
#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/NFe/infnfe.h>
#include <CppBrasil/NFe/xmlwrite.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/convnf.h>
#include <CppBrasil/NFe/retornonfe.h>
#include <CppBrasil/NFe/infnfesupl.h>


class CPPNFE_EXPORT Nfe
{
    friend class XmlRead;
public:
    Nfe();
    ~Nfe();
    void clear();
    QString get_error() const;
    QString get_chNFe() const;
    QString get_XMLOriginal() const;
    QString get_XMLAssinado() const;
    void salvarXML(const ConfigNFe *config);
    void salvarXML(const QString &caminho, const QString &nomeArquivo);

    void assinarXML(const ConfigNFe *config);
    void gerarXML(const ConfigNFe *config);
    void validarXML(const ConfigNFe *config);

    std::shared_ptr<InfNFe> infNFe;
    std::shared_ptr<ProtNFe> protNFe;
    std::shared_ptr<InfNFeSupl> infNFeSupl;

protected:
    void set_chNFe(const QString &chNFe);
private:
    QString m_chaveacesso;
    QString m_xml_original;
    QString m_xml_assinado;
    QString m_error;
    void gerarChaveAcesso();

};

#endif // NFE_H
