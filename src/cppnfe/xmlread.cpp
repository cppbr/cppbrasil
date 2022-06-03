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


#include "xmlread.h"

XmlRead::XmlRead(Nfe *NFe, const QByteArray &xml): m_nfe(NFe), m_xml(xml)
{
}

bool XmlRead::import() const
{
    return nfe();
}

bool XmlRead::nfe() const
{
    QXmlStreamReader _xml(this->m_xml);
    bool _ret = false;
    QXmlStreamAttributes attributes;
    QString _valor, _endEle;
    //TAG raiz da NF-e
    //while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == "NFe"))
    while (!_xml.atEnd())
    {
        if (_xml.tokenType() == QXmlStreamReader::StartElement && _xml.name() == QStringLiteral("NFe"))
        {
            while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("NFe")))
            {
                _ret = true; //encontrou o grupo NFe _ret será true
                 attributes = _xml.attributes();
                 if (attributes.hasAttribute("versao")) //A02
                 {
                     _valor = attributes.value("versao").toString();
                     if (!_valor.isEmpty())
                        m_nfe->infNFe->set_versao(_valor);
                 }
                 if (attributes.hasAttribute("Id")) //A03
                 {
                     _valor = attributes.value("Id").toString();
                     if (!_valor.isEmpty())
                     {
                        m_nfe->infNFe->set_Id(_valor);
                        //O campo chNFe não tem no layout, mas é necessário caso o user deseje pegar a chave
                        m_nfe->set_chNFe(_valor.right(_valor.length() - 3));
                     }
                 }

                 //Grupo A. Dados da Nota Fiscal eletrônica
                 if (_xml.name() == QStringLiteral("infNFe")) //A01
                   set_infNFe(_xml);

                 //Grupo ZX. Informações Suplementares da Nota Fiscal
                 if (_xml.name() == QStringLiteral("infNFeSupl")) //ZX01
                   set_infNFeSupl(_xml);

                 _xml.readNext();
            }
        }
        //se o xml já foi enviado
        if (_xml.tokenType() == QXmlStreamReader::StartElement && _xml.name() == QStringLiteral("protNFe"))
        {
            while(!(_xml.tokenType() == QXmlStreamReader::EndElement && _xml.name() == QStringLiteral("protNFe")))
            {
                attributes = _xml.attributes();
                if (attributes.hasAttribute("versao"))
                {
                    _valor = attributes.value("versao").toString();
                    if (!_valor.isEmpty())
                       m_nfe->protNFe->set_versao(_valor);
                }
                if (_xml.name() == QStringLiteral("tpAmb"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_tpAmb(ConvNF::strToTpAmb(_valor));
                }
                if (_xml.name() == QStringLiteral("verAplic"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_verAplic(_valor);
                }
                if (_xml.name() == QStringLiteral("chNFe"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_chNFe(_valor);
                }
                if (_xml.name() == QStringLiteral("dhRecbto"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_dhRecbto(QDateTime::fromString(_valor, Qt::ISODate));
                }
                if (_xml.name() == QStringLiteral("nProt"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_nProt(_valor);
                }
                if (_xml.name() == QStringLiteral("digVal"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_digVal(_valor);
                }
                if (_xml.name() == QStringLiteral("cStat"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_cStat(_valor.toInt());
                }
                if (_xml.name() == QStringLiteral("xMotivo"))
                {
                    _valor = _xml.readElementText();
                     if (!_valor.isEmpty())
                        m_nfe->protNFe->set_xMotivo(_valor);
                }
                _xml.readNext();
            }
        }
         _xml.readNext();
     }

    return _ret;
}

void XmlRead::set_infNFe(QXmlStreamReader &xml) const
{
    //A01 - infNFe
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("infNFe")))
    {
        //Grupo B. Identificação da Nota Fiscal eletrônica
        if (xml.name() == QStringLiteral("ide")) //B01
          set_ide(xml);

        //Grupo C. Identificação do Emitente da Nota Fiscal eletrônica
        if (xml.name() == QStringLiteral("emit")) //C01
          set_emite(xml);

        //Grupo D. Identificação do Fisco Emitente da NF-e
        if (xml.name() == QStringLiteral("avulsa")) //D01
          set_avulsa(xml);

        //Grupo E. Identificação do Destinatário da Nota Fiscal eletrônica
        if (xml.name() == QStringLiteral("dest")) //E01
          set_dest(xml);

        //Grupo F. Identificação do Local de Retirada
        if (xml.name() == QStringLiteral("retirada")) //F01
          set_retirada(xml);

        //Grupo G. Identificação do Local de Entrega
        if (xml.name() == QStringLiteral("entrega")) //G01
          set_entrega(xml);

        //Grupo GA. Autorização para obter XML
        if (xml.name() == QStringLiteral("autXML")) //G01
          set_autXML(xml);

        //Grupo H. Detalhamento de Produtos e Serviços da NF-e
        if (xml.name() == QStringLiteral("det")) //H01
          set_det(xml);

        //Grupo W. Total da NF-e
        if (xml.name() == QStringLiteral("total")) //W01
          set_total(xml);

        //Grupo X. Informações do Transporte da NF-e
        if (xml.name() == QStringLiteral("transp")) //X01
          set_transp(xml);

        //Grupo Y. Dados da Cobrança
        if (xml.name() == QStringLiteral("cobr")) //Y01
          set_cobr(xml);

        //Grupo YA. Informações de Pagamento
        if (xml.name() == QStringLiteral("pag")) //YA01
          set_pag(xml);

        //Grupo YB. Informações do Intermediador da Transação
        if (xml.name() == QStringLiteral("infIntermed")) //YB01
          set_infIntermed(xml);

        //Grupo Z. Informações Adicionais da NF-e
        if (xml.name() == QStringLiteral("infAdic")) //Z01
          set_infAdic(xml);

        //Grupo ZA. Informações de Comércio Exterior
        if (xml.name() == QStringLiteral("exporta")) //ZA01
          set_exporta(xml);

        //Grupo ZB. Informações de Compras
        if (xml.name() == QStringLiteral("compra")) //ZB01
          set_compra(xml);

        //Grupo ZC. Informações do Registro de Aquisição de Cana
        if (xml.name() == QStringLiteral("cana")) //ZC01
          set_cana(xml);

        //Grupo ZD. Informações do Responsável Técnico
        if (xml.name() == QStringLiteral("infRespTec")) //ZD01
          set_infRespTec(xml);

        xml.readNext();
    }
}

void XmlRead::set_ide(QXmlStreamReader &xml) const
{
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ide")))
    {
        if (xml.name() == QStringLiteral("cUF")) //B02
        {
            _valor = xml.readElementText();
             if (!_valor.isEmpty())
                m_nfe->infNFe->ide->set_cUF(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cNF")) //B03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_cNF(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("natOp")) //B04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_natOp(_valor);
        }
        if (xml.name() == QStringLiteral("mod")) //B06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_mod(ConvNF::strToModeloDF(_valor));
        }
        if (xml.name() == QStringLiteral("serie")) //B07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_serie(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nNF")) //B08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_nNF(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("dhEmi")) //B09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_dhEmi(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("dhSaiEnt")) //B10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_dhSaiEnt(QDateTime::fromString(_valor, Qt::ISODate));

        }
        if (xml.name() == QStringLiteral("tpNF")) //B11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_tpNF(ConvNF::strToTpNF(_valor));
        }
        if (xml.name() == QStringLiteral("idDest")) //B11a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_idDest(ConvNF::strToIdDest(_valor));
        }
        if (xml.name() == QStringLiteral("cMunFG")) //B12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_cMunFG(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("tpImp")) //B21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_tpImp(ConvNF::strToTpImp(_valor));
        }
        if (xml.name() == QStringLiteral("tpEmis")) //B22
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_tpEmis(ConvNF::strToTpEmis(_valor));
        }
        if (xml.name() == QStringLiteral("cDV")) //B23 - esse será gerado novamente caso execute o método gerar
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_cDV(_valor);
        }
        if (xml.name() == QStringLiteral("tpAmb")) //B24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_tpAmb(ConvNF::strToTpAmb(_valor));
        }
        if (xml.name() == QStringLiteral("finNFe")) //B25
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_finNFe(ConvNF::strToFinNFe(_valor));
        }
        if (xml.name() == QStringLiteral("indFinal")) //B25a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_indFinal(ConvNF::strToIndFinal(_valor));
        }
        if (xml.name() == QStringLiteral("indPres")) //B25b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_indPres(ConvNF::strToIndPres(_valor));
        }
        if (xml.name() == QStringLiteral("indIntermed")) //B25c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_indIntermed(ConvNF::strToIndIntermed(_valor));
        }
        if (xml.name() == QStringLiteral("procEmi")) //B26
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_procEmi(ConvNF::strToProcEmi(_valor));
        }
        if (xml.name() == QStringLiteral("verProc")) //B27
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_verProc(_valor);
        }
        if (xml.name() == QStringLiteral("dhCont")) //B28
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_dhCont(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("xJust")) //B29
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->set_xJust(_valor);
        }
        //Grupo BA. Documento Fiscal Referenciado
        if (xml.name() == QStringLiteral("NFref")) //BA01 - NFref
          set_ide_NFref(xml);

        xml.readNext();
    }
}

void XmlRead::set_ide_NFref(QXmlStreamReader &xml) const
{
    //BA01 - NFref
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("NFref")))
    {
        if (xml.name() == QStringLiteral("refNFe")) //BA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->set_refNFe(_valor);
        }

        if (xml.name() == QStringLiteral("refNF")) //BA03 - refNF
          set_ide_NFref_refNF(xml);

        if (xml.name() == QStringLiteral("refNFP")) //BA10 - refNFP
          set_ide_NFref_refNFP(xml);

        if (xml.name() == QStringLiteral("refCTe")) //BA19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->set_refCTe(_valor);
        }

        if (xml.name() == QStringLiteral("refECF")) //BA20 - refECF
          set_ide_NFref_refECF(xml);

        xml.readNext();
    }
    m_nfe->infNFe->ide->NFref->add();

}

void XmlRead::set_ide_NFref_refNF(QXmlStreamReader &xml) const
{
    //BA03 - refNF
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("refNF")))
    {
        if (xml.name() == QStringLiteral("cUF")) //BA04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_cUF(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("AAMM")) //BA05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_AAMM(_valor);
        }
        if (xml.name() == QStringLiteral("CNPJ")) //BA06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("mod")) //BA07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_mod(_valor);
        }
        if (xml.name() == QStringLiteral("serie")) //BA08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_serie(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nNF")) //BA09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNF->set_nNF(_valor.toInt());
        }

        xml.readNext();
    }

}

void XmlRead::set_ide_NFref_refNFP(QXmlStreamReader &xml) const
{
    //BA10 - refNFP
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("refNFP")))
    {
        if (xml.name() == QStringLiteral("cUF")) //BA11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_cUF(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("AAMM")) //BA12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_AAMM(_valor);
        }
        if (xml.name() == QStringLiteral("CNPJ")) //BA13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //BA14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("IE")) //BA15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_IE(_valor);
        }
        if (xml.name() == QStringLiteral("mod")) //BA16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_mod(_valor);
        }
        if (xml.name() == QStringLiteral("serie")) //BA17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_serie(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nNF")) //BA18
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refNFP->set_nNF(_valor.toInt());
        }

        xml.readNext();
    }

}

void XmlRead::set_ide_NFref_refECF(QXmlStreamReader &xml) const
{
    //BA20 - refECF
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("refECF")))
    {
        if (xml.name() == QStringLiteral("mod")) //BA21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refECF->set_mod(_valor);
        }
        if (xml.name() == QStringLiteral("nECF")) //BA22
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refECF->set_nECF(_valor);
        }
        if (xml.name() == QStringLiteral("nCOO")) //BA23
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->ide->NFref->obj->refECF->set_nCOO(_valor);
        }

        xml.readNext();
    }

}

void XmlRead::set_emite(QXmlStreamReader &xml) const
{
    //C01 - emit
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("emit")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //C02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //C02a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("xNome")) //C03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_xNome(_valor);
        }
        if (xml.name() == QStringLiteral("xFant")) //C04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_xFant(_valor);
        }
        //Endereço do emitente
        if (xml.name() == QStringLiteral("enderEmit")) //C05
          set_emite_enderEmit(xml);

        if (xml.name() == QStringLiteral("IE")) //C17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_IE(_valor);
        }
        if (xml.name() == QStringLiteral("IEST")) //C18
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_IEST(_valor);
        }
        if (xml.name() == QStringLiteral("IM")) //C19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_IM(_valor);
        }
        if (xml.name() == QStringLiteral("CNAE")) //C20
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_CNAE(_valor);
        }
        if (xml.name() == QStringLiteral("CRT")) //C21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->set_CRT(ConvNF::strToCrt(_valor));
        }

        xml.readNext();
    }
}

void XmlRead::set_emite_enderEmit(QXmlStreamReader &xml) const
{
    //C05 - enderEmit
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("enderEmit")))
    {
        if (xml.name() == QStringLiteral("xLgr")) //C06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_xLgr(_valor);
        }
        if (xml.name() == QStringLiteral("nro")) //C07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_nro(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xCpl")) //C08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_xCpl(_valor);
        }
        if (xml.name() == QStringLiteral("xBairro")) //C09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_xBairro(_valor);
        }
        if (xml.name() == QStringLiteral("cMun")) //C10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_cMun(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xMun")) //C11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_xMun(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //C12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("CEP")) //C13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_CEP(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cPais")) //C14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_cPais(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xPais")) //C15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_xPais(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //C16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->emite->enderEmit->set_fone(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_avulsa(QXmlStreamReader &xml) const
{
    //D01 - avulsa
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("avulsa")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //D02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("xOrgao")) //D03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_xOrgao(_valor);
        }
        if (xml.name() == QStringLiteral("matr")) //D04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_matr(_valor);
        }
        if (xml.name() == QStringLiteral("xAgente")) //D05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_xAgente(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //D06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_fone(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //D07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("nDAR")) //D08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_nDAR(_valor);
        }
        if (xml.name() == QStringLiteral("dEmi")) //D09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_dEmi(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("vDAR")) //D010
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_vDAR(QString(_valor).toDouble());
        }
        if (xml.name() == QStringLiteral("repEmi")) //D011
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_repEmi(_valor);
        }
        if (xml.name() == QStringLiteral("dPag")) //D012
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->avulsa->set_dPag(QDateTime::fromString(_valor, Qt::ISODate));
        }

        xml.readNext();
    }
}

void XmlRead::set_dest(QXmlStreamReader &xml) const
{
    //E01 - dest
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("dest")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //E02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //E03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("idEstrangeiro")) //E03a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_idEstrangeiro(_valor);
        }
        if (xml.name() == QStringLiteral("xNome")) //E04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_xNome(_valor);
        }

        if (xml.name() == QStringLiteral("enderDest")) //E05 - Grupo Endereço do Destinatário da NF-e
          set_dest_enderDest(xml);

        if (xml.name() == QStringLiteral("indIEDest")) //E16a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_indIEDest(ConvNF::strToIndIEDest(_valor));
        }
        if (xml.name() == QStringLiteral("IE")) //E17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_IE(_valor);
        }
        if (xml.name() == QStringLiteral("ISUF")) //E18
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_ISUF(_valor);
        }
        if (xml.name() == QStringLiteral("IM")) //E18a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_IM(_valor);
        }
        if (xml.name() == QStringLiteral("email")) //E19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->set_email(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_dest_enderDest(QXmlStreamReader &xml) const
{
    //enderDest - E05
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("enderDest")))
    {
        if (xml.name() == QStringLiteral("xLgr")) //E06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_xLgr(_valor);
        }
        if (xml.name() == QStringLiteral("nro")) //E07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_nro(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xCpl")) //E08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_xCpl(_valor);
        }
        if (xml.name() == QStringLiteral("xBairro")) //E09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_xBairro(_valor);
        }
        if (xml.name() == QStringLiteral("cMun")) //E10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_cMun(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xMun")) //E11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_xMun(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //E12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("CEP")) //E13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_CEP(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cPais")) //E14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_cPais(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xPais")) //E15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_xPais(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //E16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->dest->enderDest->set_fone(_valor);
        }

        xml.readNext();
    }

}

void XmlRead::set_retirada(QXmlStreamReader &xml) const
{
    //retirada - F01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("retirada")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //F02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //F02a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("xNome")) //F02b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xNome(_valor);
        }
        if (xml.name() == QStringLiteral("xLgr")) //F02b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xLgr(_valor);
        }
        if (xml.name() == QStringLiteral("nro")) //F04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_nro(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xCpl")) //F05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xCpl(_valor);
        }
        if (xml.name() == QStringLiteral("xBairro")) //F06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xBairro(_valor);
        }
        if (xml.name() == QStringLiteral("cMun")) //F07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_cMun(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xMun")) //F08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xMun(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //F09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("CEP")) //F10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_CEP(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cPais")) //F11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_cPais(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xPais")) //F12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_xPais(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //F13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_fone(_valor);
        }
        if (xml.name() == QStringLiteral("email")) //F14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_email(_valor);
        }
        if (xml.name() == QStringLiteral("IE")) //F15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->retirada->set_IE(_valor);
        }

        xml.readNext();
    }

}

void XmlRead::set_entrega(QXmlStreamReader &xml) const
{
    //entrega - G01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("entrega")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //G02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //G02a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("xNome")) //G02b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xNome(_valor);
        }
        if (xml.name() == QStringLiteral("xLgr")) //G03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xLgr(_valor);
        }
        if (xml.name() == QStringLiteral("nro")) //G04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_nro(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xCpl")) //G05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xCpl(_valor);
        }
        if (xml.name() == QStringLiteral("xBairro")) //G06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xBairro(_valor);
        }
        if (xml.name() == QStringLiteral("cMun")) //G07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_cMun(_valor.toUInt());
        }
        if (xml.name() == QStringLiteral("xMun")) //G08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xMun(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //G09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("CEP")) //G10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_CEP(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cPais")) //G11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_cPais(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("xPais")) //G12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_xPais(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //G13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_fone(_valor);
        }
        if (xml.name() == QStringLiteral("email")) //G14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_email(_valor);
        }
        if (xml.name() == QStringLiteral("IE")) //G15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->entrega->set_IE(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_autXML(QXmlStreamReader &xml) const
{
    //autXML - GA01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("retirada")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //GA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->autXML->obj->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //GA03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->autXML->obj->set_CPF(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->autXML->add();
}

void XmlRead::set_det(QXmlStreamReader &xml) const
{
    //det - H01
    QString _valor;
    QXmlStreamAttributes attributes;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("det")))
    {
        attributes = xml.attributes();
        if (attributes.hasAttribute("nItem")) //H02 - esse campo fica dentro da classe prod, mas no xml fica em det
        {
            _valor = attributes.value("nItem").toString();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_nItem(_valor.toInt());
        }

        //Grupo I. Produtos e Serviços da NF-e
        if (xml.name() == QStringLiteral("prod")) //I01
          set_prod(xml);

        //Grupo M. Tributos incidentes no Produto ou Serviço
        if (xml.name() == QStringLiteral("imposto")) //M01
          set_imposto(xml);

        //Grupo UA. Tributos Devolvidos (para o item da NF-e)
        if (xml.name() == QStringLiteral("impostoDevol")) //UA01
          set_impostoDevol(xml);

        if (xml.name() == QStringLiteral("infAdProd")) //V01
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->set_infAdProd(_valor);
        }

        //Grupo VA. Observações de uso livre (para o item da NF-e)
        if (xml.name() == QStringLiteral("obsItem")) //UA01
          set_obsItem(xml);

        xml.readNext();
    }
    m_nfe->infNFe->det->add();

}

void XmlRead::set_prod(QXmlStreamReader &xml) const
{
    //prod - I01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("prod")))
    {
        if (xml.name() == QStringLiteral("cProd")) //I02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cProd(_valor);
        }
        if (xml.name() == QStringLiteral("cEAN")) //I03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cEAN(_valor);
        }
        if (xml.name() == QStringLiteral("cBarra")) //I03a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cBarra(_valor);
        }
        if (xml.name() == QStringLiteral("xProd")) //I04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_xProd(_valor);
        }
        if (xml.name() == QStringLiteral("NCM")) //I05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_NCM(_valor);
        }
        if (xml.name() == QStringLiteral("NVE")) //I05a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_NVE(_valor);
        }
        if (xml.name() == QStringLiteral("CEST")) //I05c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_CEST(_valor);
        }
        if (xml.name() == QStringLiteral("indEscala")) //I05d
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_indEscala(ConvNF::strToIndEscala(_valor));
        }
        if (xml.name() == QStringLiteral("CNPJFab")) //I05e
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_CNPJFab(_valor);
        }
        if (xml.name() == QStringLiteral("cBenef")) //I05f
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cBenef(_valor);
        }
        if (xml.name() == QStringLiteral("EXTIPI")) //I06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_EXTIPI(_valor);
        }
        if (xml.name() == QStringLiteral("CFOP")) //I08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_CFOP(_valor);
        }
        if (xml.name() == QStringLiteral("uCom")) //I09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_uCom(_valor);
        }
        if (xml.name() == QStringLiteral("qCom")) //I10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_qCom(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vUnCom")) //I10a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vUnCom(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vProd")) //I11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("cEANTrib")) //I12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cEANTrib(_valor);
        }
        if (xml.name() == QStringLiteral("cBarraTrib")) //I12a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_cBarraTrib(_valor);
        }
        if (xml.name() == QStringLiteral("uTrib")) //I13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_uTrib(_valor);
        }
        if (xml.name() == QStringLiteral("qTrib")) //I14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_qTrib(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vUnTrib")) //I14a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vUnTrib(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFrete")) //I15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vFrete(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vSeg")) //I16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vSeg(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDesc")) //I17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vDesc(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vOutro")) //I17a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_vOutro(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("indTot")) //I17b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_indTot(ConvNF::strToIndTot(_valor));
        }
        //Grupo I01. Produtos e Serviços / Declaração de Importação
        if (xml.name() == QStringLiteral("DI")) //I18
          set_DI(xml);

        //Grupo I03. Produtos e Serviços / Grupo de Exportação
        if (xml.name() == QStringLiteral("detExport")) //I50
          set_detExport(xml);

        //Grupo I05. Produtos e Serviços / Pedido de Compra
        if (xml.name() == QStringLiteral("xPed")) //I60
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_xPed(_valor);
        }
        if (xml.name() == QStringLiteral("nItemPed")) //I61
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_nItemPed(_valor.toInt());
        }
        //Grupo I07. Produtos e Serviços / Grupo Diversos
        if (xml.name() == QStringLiteral("nFCI")) //I70
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_nFCI(_valor);
        }

        //Grupo I80. Rastreabilidade de produto
        if (xml.name() == QStringLiteral("rastro")) //I70
          set_rastro(xml);

        //Grupo J. Produto Específico
        //I90 -x- Sequência XML
        //Grupo JA. Detalhamento Específico de Veículos novos
        if (xml.name() == QStringLiteral("veicProd")) //J01
          set_veicProd(xml);

        //Grupo K. Detalhamento Específico de Medicamento e de matérias-primas farmacêuticas
        if (xml.name() == QStringLiteral("med")) //K01
          set_med(xml);

        //Grupo L. Detalhamento Específico de Armamentos
        if (xml.name() == QStringLiteral("arma")) //L01
          set_arma(xml);

        //Grupo LA. Detalhamento Específico de Combustíveis
        if (xml.name() == QStringLiteral("comb")) //LA01
          set_comb(xml);

        //Grupo LB. Detalhamento Específico para Operação com Papel Imune
        if (xml.name() == QStringLiteral("nRECOPI")) //I70
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->set_nRECOPI(_valor);
        }

        xml.readNext();
    }

}

void XmlRead::set_DI(QXmlStreamReader &xml) const
{
    //DI - I18
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("DI")))
    {
        if (xml.name() == QStringLiteral("nDI")) //I19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_nDI(_valor);
        }
        if (xml.name() == QStringLiteral("dDI")) //I20
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_dDI(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("xLocDesemb")) //I21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_xLocDesemb(_valor);
        }
        if (xml.name() == QStringLiteral("UFDesemb")) //I22
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_UFDesemb(_valor);
        }
        if (xml.name() == QStringLiteral("dDesemb")) //I23
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_dDesemb(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("tpViaTransp")) //I24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_tpViaTransp(ConvNF::strToTpViaTransp(_valor));
        }
        if (xml.name() == QStringLiteral("vAFRMM")) //I23b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_vAFRMM(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("tpIntermedio")) //I23c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_tpIntermedio(ConvNF::strToTpIntermedio(_valor));
        }
        if (xml.name() == QStringLiteral("CNPJ")) //I23d
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("UFTerceiro")) //I23e
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_UFTerceiro(_valor);
        }
        if (xml.name() == QStringLiteral("cExportador")) //I24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->set_cExportador(_valor);
        }
        //Grupo Adições - adi
        if (xml.name() == QStringLiteral("adi")) //I25
          set_DI_adi(xml);

        xml.readNext();
    }
    m_nfe->infNFe->det->obj->prod->DI->add();
}

void XmlRead::set_DI_adi(QXmlStreamReader &xml) const
{
    //adi - I25
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("adi")))
    {
        if (xml.name() == QStringLiteral("nAdicao")) //I26
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->adi->obj->set_nAdicao(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nSeqAdic")) //I27
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->adi->obj->set_nSeqAdic(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cFabricante")) //I28
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->adi->obj->set_cFabricante(_valor);
        }
        if (xml.name() == QStringLiteral("vDescDI")) //I29
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->adi->obj->set_vDescDI(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("nDraw")) //I29
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->DI->obj->adi->obj->set_nDraw(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->det->obj->prod->DI->obj->adi->add();
}

void XmlRead::set_detExport(QXmlStreamReader &xml) const
{
    //detExport - I50
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("detExport")))
    {
        if (xml.name() == QStringLiteral("nDraw")) //I51
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->detExport->obj->set_nDraw(_valor);
        }
        //Grupo sobre exportação indireta
        if (xml.name() == QStringLiteral("exportInd")) //I52
          set_detExport_exportInd(xml);

        xml.readNext();
    }
    m_nfe->infNFe->det->obj->prod->detExport->add();
}

void XmlRead::set_detExport_exportInd(QXmlStreamReader &xml) const
{
    //exportInd - I52
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("exportInd")))
    {
        if (xml.name() == QStringLiteral("nRE")) //I53
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->detExport->obj->exportInd->set_nRE(_valor);
        }
        if (xml.name() == QStringLiteral("chNFe")) //I54
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->detExport->obj->exportInd->set_chNFe(_valor);
        }
        if (xml.name() == QStringLiteral("qExport")) //I55
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->detExport->obj->exportInd->set_qExport(_valor.toDouble());
        }
        xml.readNext();
    }

}

void XmlRead::set_rastro(QXmlStreamReader &xml) const
{
    //rastro - I80
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("rastro")))
    {
        if (xml.name() == QStringLiteral("nLote")) //I81
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->rastro->obj->set_nLote(_valor);
        }
        if (xml.name() == QStringLiteral("qLote")) //I82
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->rastro->obj->set_qLote(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("dFab")) //I83
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->rastro->obj->set_dFab(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("dVal")) //I84
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->rastro->obj->set_dVal(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("cAgreg")) //I85
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->rastro->obj->set_cAgreg(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->det->obj->prod->rastro->add();
}

void XmlRead::set_veicProd(QXmlStreamReader &xml) const
{
    //veicProd - J01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("veicProd")))
    {
        if (xml.name() == QStringLiteral("tpOp")) //J02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_tpOp(ConvNF::strToTpOp(_valor));
        }
        if (xml.name() == QStringLiteral("chassi")) //J03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_chassi(_valor);
        }
        if (xml.name() == QStringLiteral("cCor")) //J04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_cCor(_valor);
        }
        if (xml.name() == QStringLiteral("xCor")) //J05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_xCor(_valor);
        }
        if (xml.name() == QStringLiteral("pot")) //J06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_pot(_valor);
        }
        if (xml.name() == QStringLiteral("cilin")) //J07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_cilin(_valor);
        }
        if (xml.name() == QStringLiteral("pesoL")) //J08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_pesoL(_valor);
        }
        if (xml.name() == QStringLiteral("pesoB")) //J09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_pesoB(_valor);
        }
        if (xml.name() == QStringLiteral("nSerie")) //J10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_nSerie(_valor);
        }
        if (xml.name() == QStringLiteral("tpComb")) //J11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_tpComb(_valor);
        }
        if (xml.name() == QStringLiteral("nMotor")) //J12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_nMotor(_valor);
        }
        if (xml.name() == QStringLiteral("CMT")) //J13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_CMT(_valor);
        }
        if (xml.name() == QStringLiteral("dist")) //J14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_dist(_valor);
        }
        if (xml.name() == QStringLiteral("anoMod")) //J16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_anoMod(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("anoFab")) //J17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_anoFab(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("tpPint")) //J18
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_tpPint(_valor);
        }
        if (xml.name() == QStringLiteral("tpVeic")) //J19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_tpVeic(_valor);
        }
        if (xml.name() == QStringLiteral("espVeic")) //J20
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_espVeic(_valor);
        }
        if (xml.name() == QStringLiteral("VIN")) //J21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_VIN(_valor);
        }
        if (xml.name() == QStringLiteral("condVeic")) //J22
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_condVeic(ConvNF::strToCondVeic(_valor));
        }
        if (xml.name() == QStringLiteral("cMod")) //J23
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_cMod(_valor);
        }
        if (xml.name() == QStringLiteral("cCorDENATRAN")) //J24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_cCorDENATRAN(_valor);
        }
        if (xml.name() == QStringLiteral("lota")) //J25
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_lota(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("tpRest")) //J26
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->veicProd->set_tpRest(ConvNF::strToTpRest(_valor));
        }

        xml.readNext();
    }

}

void XmlRead::set_med(QXmlStreamReader &xml) const
{
    //med - K01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("med")))
    {
        if (xml.name() == QStringLiteral("cProdANVISA")) //K01a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->med->set_cProdANVISA(_valor);
        }
        if (xml.name() == QStringLiteral("xMotivoIsencao")) //K01b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->med->set_xMotivoIsencao(_valor);
        }
        if (xml.name() == QStringLiteral("vPMC")) //K06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->med->set_vPMC(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_arma(QXmlStreamReader &xml) const
{
    //arma - L01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("arma")))
    {
        if (xml.name() == QStringLiteral("tpArma")) //L02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->arma->obj->set_tpArma(ConvNF::strToTpArma(_valor));
        }
        if (xml.name() == QStringLiteral("nSerie")) //L03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->arma->obj->set_nSerie(_valor);
        }
        if (xml.name() == QStringLiteral("nCano")) //L04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->arma->obj->set_nCano(_valor);
        }
        if (xml.name() == QStringLiteral("descr")) //L05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->arma->obj->set_descr(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->det->obj->prod->arma->add();
}

void XmlRead::set_comb(QXmlStreamReader &xml) const
{
    //comb - LA01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("comb")))
    {
        if (xml.name() == QStringLiteral("cProdANP")) //LA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_cProdANP(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("descANP")) //LA03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_descANP(_valor);
        }
        if (xml.name() == QStringLiteral("pGLP")) //LA03a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_pGLP(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pGNn")) //LA03b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_pGNn(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pGNi")) //LA03c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_pGNn(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vPart")) //LA03d
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_vPart(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("CODIF")) //LA04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_CODIF(_valor);
        }
        if (xml.name() == QStringLiteral("qTemp")) //LA05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_qTemp(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("UFCons")) //LA06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->set_UFCons(_valor);
        }
        //Grupo Informações da CIDE
        if (xml.name() == QStringLiteral("CIDE")) //LA06
          set_comb_CIDE(xml);
        //Grupo Informações do grupo de encerrante
        if (xml.name() == QStringLiteral("encerrante")) //LA11
          set_comb_encerrante(xml);

        xml.readNext();
    }
}

void XmlRead::set_comb_CIDE(QXmlStreamReader &xml) const
{
    //CIDE - LA07
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("CIDE")))
    {
        if (xml.name() == QStringLiteral("qBCProd")) //LA08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->CIDE->set_qBCProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliqProd")) //LA09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->CIDE->set_vAliqProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCIDE")) //LA10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->CIDE->set_vCIDE(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_comb_encerrante(QXmlStreamReader &xml) const
{
    //encerrante - LA11
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("encerrante")))
    {
        if (xml.name() == QStringLiteral("nBico")) //LA12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->encerrante->set_nBico(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nBomba")) //LA13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->encerrante->set_nBico(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nTanque")) //LA14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->encerrante->set_nTanque(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("vEncIni")) //LA15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->encerrante->set_vEncIni(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vEncFin")) //LA16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->prod->comb->encerrante->set_vEncIni(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_imposto(QXmlStreamReader &xml) const
{
    //imposto - M01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("imposto")))
    {
        if (xml.name() == QStringLiteral("vTotTrib")) //M02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->set_vTotTrib(_valor.toDouble());
        }
        //Grupo N01. ICMS Normal e ST
        if (xml.name() == QStringLiteral("ICMS")) //N01
          set_ICMS(xml);

        //Grupo NA. ICMS para a UF de destino
        if (xml.name() == QStringLiteral("ICMSUFDest")) //NA01
          set_ICMSUFDest(xml);

        //Grupo O. Imposto sobre Produtos Industrializados
        if (xml.name() == QStringLiteral("IPI")) //O01
          set_IPI(xml);

        //Grupo P. Imposto de Importação
        if (xml.name() == QStringLiteral("II")) //P01
          set_II(xml);

        //Grupo Q. PIS
        if (xml.name() == QStringLiteral("PIS")) //Q01
          set_PIS(xml);

        //Grupo R. PIS ST
        if (xml.name() == QStringLiteral("PISST")) //R01
          set_PISST(xml);

        //Grupo S. COFINS
        if (xml.name() == QStringLiteral("COFINS")) //S01
          set_COFINS(xml);

        //Grupo T. COFINS ST
        if (xml.name() == QStringLiteral("COFINSST")) //T01
          set_COFINSST(xml);

        //Grupo U. ISSQN
        if (xml.name() == QStringLiteral("ISSQN")) //U01
          set_ISSQN(xml);

        xml.readNext();
    }
}

void XmlRead::set_ICMS(QXmlStreamReader &xml) const
{
    //ICMS - N01
    //tudo que está dentro de ICMS está aqui
    QString _valor;
    bool _part = false;
    bool _st = false;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ICMS")))
    {
        //verifica se é Grupo de Partilha do ICMS
        if ((xml.name() == QStringLiteral("ICMSPart")))
            _part = true;
        //verifica se é Grupo de Repasse de ICMS ST
        if ((xml.name() == QStringLiteral("ICMSST")))
            _st = true;


        if (xml.name() == QStringLiteral("orig"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_orig(ConvNF::strToOrig(_valor));
        }

        if (xml.name() == QStringLiteral("CST"))
        {
            _valor = xml.readElementText();
            //se for partilha do icms
            if (_part)
            {
                if (_valor == "10"){
                  _valor  = "Part10";
                 } else if (_valor == "90"){
                  _valor  = "Part90";
                }
                //desativando partilha
                _part = false;
            }
            //se for ICMS ST
            if (_st)
            {
                if (_valor == "41"){
                  _valor  = "ST41";
                } else if (_valor == "60"){
                    _valor  = "ST60";
                }
                //desativando ICMS ST
                _st = false;
            }
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_CST(ConvNF::strToCstICMS(_valor));
        }

        if (xml.name() == QStringLiteral("CSOSN"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_CSOSN(ConvNF::strToCsosnICMS(_valor));
        }

        if (xml.name() == QStringLiteral("modBC"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_modBC(ConvNF::strToModBC(_valor));
        }
        if (xml.name() == QStringLiteral("vBC"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMS"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pICMS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMS"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pFCP"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pFCP(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCP"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vFCP(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCFCP"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCFCP(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("modBCST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_modBCST(ConvNF::strToModBCST(_valor));
        }
        if (xml.name() == QStringLiteral("pMVAST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pMVAST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pRedBCST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pRedBCST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pICMSST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCFCPST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCFCPST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pFCPST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pFCPST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vFCPST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSDeson"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSDeson(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("motDesICMS"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_motDesICMS(ConvNF::strToMotDesICMS(_valor));
        }
        if (xml.name() == QStringLiteral("vICMSOp"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSOp(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pDif"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pDif(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSDif"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSDif(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSSubstituto"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSSubstituto(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCFCPSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCFCPSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pFCPSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pFCPSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPSTRet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vFCPSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCSTDest"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCSTDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSSTDest"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSSTDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pRedBCEfet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pRedBCEfet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCEfet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vBCEfet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSEfet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pICMSEfet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSEfet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSEfet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pBCOp"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pBCOp(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("UFST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_UFST(_valor);
        }
        if (xml.name() == QStringLiteral("pRedBC"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pRedBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pCredSN"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pCredSN(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCredICMSSN"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vCredICMSSN(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSSTDeson"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vICMSSTDeson(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("motDesICMSST"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_motDesICMSST(ConvNF::strToMotDesICMS(_valor));
        }
        if (xml.name() == QStringLiteral("pFCPDif"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_pFCPDif(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPDif"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vFCPDif(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPEfet"))
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMS->set_vFCPEfet(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_ICMSUFDest(QXmlStreamReader &xml) const
{
    //ICMSUFDest - NA01
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ICMSUFDest")))
    {
        if (xml.name() == QStringLiteral("vFCPEfet")) //NA03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_vBCUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCFCPUFDest")) //NA04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_vBCFCPUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pFCPUFDest")) //NA05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_pFCPUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSUFDest")) //NA07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSInter")) //NA09

        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSInter(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSInterPart")) //NA11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_pICMSInterPart(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPUFDest")) //NA13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_vFCPUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSUFDest")) //NA15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_vICMSUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSUFRemet")) //NA17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ICMSUFDest->set_vICMSUFRemet(_valor.toDouble());
        }

        xml.readNext();
    }

}

void XmlRead::set_IPI(QXmlStreamReader &xml) const
{
    //O01 - IPI
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("IPI")))
    {
        if (xml.name() == QStringLiteral("CNPJProd")) //O03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_CNPJProd(_valor);
        }
        if (xml.name() == QStringLiteral("cSelo")) //O04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_cSelo(_valor);
        }
        if (xml.name() == QStringLiteral("qSelo")) //O05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_qSelo(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cEnq")) //O06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_cEnq(_valor);
        }
        //O07 - IPITrib / O08 - IPINT
        if (xml.name() == QStringLiteral("CST")) //O09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_CST(ConvNF::strToCstIPI(_valor));
        }
        if (xml.name() == QStringLiteral("vBC")) //O10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pIPI")) //O13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_pIPI(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qUnid")) //O11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_qUnid(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vUnid")) //O12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_vUnid(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIPI")) //O14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->IPI->set_vIPI(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_II(QXmlStreamReader &xml) const
{
    //P01 - II
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("II")))
    {
        if (xml.name() == QStringLiteral("vBC")) //P02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->II->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDespAdu")) //P03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->II->set_vDespAdu(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vII")) //P04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->II->set_vII(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIOF")) //P05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->II->set_vIOF(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_PIS(QXmlStreamReader &xml) const
{
    //Q01 - PIS / Q02 - PISAliq / Q03 - PISQtde / Q04 - PISNT / Q05 - PISOutr
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("PIS")))
    {
        if (xml.name() == QStringLiteral("CST")) //Q06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_CST(ConvNF::strToCstPIS(_valor));
        }
        if (xml.name() == QStringLiteral("vBC")) //Q07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pPIS")) //Q08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_pPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vPIS")) //Q09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_vPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qBCProd")) //Q10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_qBCProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliqProd")) //Q10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PIS->set_vAliqProd(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_PISST(QXmlStreamReader &xml) const
{
    //R01 - PISST
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("PISST")))
    {
        if (xml.name() == QStringLiteral("vBC")) //R02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pPIS")) //R03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_pPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qBCProd")) //R04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_qBCProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliqProd")) //R05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_vAliqProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vPIS")) //R06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_vPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("indSomaPISST")) //R07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->PISST->set_indSomaPISST(ConvNF::strToIndTot(_valor));
        }

        xml.readNext();
    }

}

void XmlRead::set_COFINS(QXmlStreamReader &xml) const
{
    //S01 - COFINS / S02 - COFINSAliq / S03 - COFINSQtde / S04 - COFINSNT / S05 - COFINSOutr
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("COFINS")))
    {
        if (xml.name() == QStringLiteral("CST")) //S06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_CST(ConvNF::strToCstCOFINS(_valor));
        }
        if (xml.name() == QStringLiteral("vBC")) //S07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pCOFINS")) //S08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_pCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCOFINS")) //S11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_vCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qBCProd")) //S09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_qBCProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliqProd")) //S10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINS->set_vAliqProd(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_COFINSST(QXmlStreamReader &xml) const
{
    //T01 - PISST
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("COFINSST")))
    {
        if (xml.name() == QStringLiteral("vBC")) //T02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pCOFINS")) //T03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_pCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qBCProd")) //T04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_qBCProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliqProd")) //T05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_vAliqProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCOFINS")) //T06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_vCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("indSomaCOFINSST")) //T07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->COFINSST->set_indSomaCOFINSST(ConvNF::strToIndTot(_valor));
        }

        xml.readNext();
    }
}

void XmlRead::set_ISSQN(QXmlStreamReader &xml) const
{
    //U01 - ISSQN
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ISSQN")))
    {
        if (xml.name() == QStringLiteral("vBC")) //U02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vAliq")) //U03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vAliq(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vISSQN")) //U04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vISSQN(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("cMunFG")) //U05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_cMunFG(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cListServ")) //U06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_cListServ(_valor);
        }
        if (xml.name() == QStringLiteral("vDeducao")) //U07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vDeducao(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vOutro")) //U08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vOutro(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDescIncond")) //U09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vDescIncond(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDescCond")) //U10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vDescCond(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vISSRet")) //U11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_vISSRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("indISS")) //U12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_indISS(ConvNF::strToIndISS(_valor));
        }
        if (xml.name() == QStringLiteral("cServico")) //U13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_cServico(_valor);
        }
        if (xml.name() == QStringLiteral("cMun")) //U14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_cMun(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("cPais")) //U15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_cPais(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("nProcesso")) //U16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_nProcesso(_valor);
        }
        if (xml.name() == QStringLiteral("indIncentivo")) //U17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->imposto->ISSQN->set_indIncentivo(ConvNF::strToIndIncentivo(_valor));
        }

        xml.readNext();
    }
}

void XmlRead::set_impostoDevol(QXmlStreamReader &xml) const
{
    //UA01 - impostoDevol
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("impostoDevol")))
    {
        if (xml.name() == QStringLiteral("pDevol")) //UA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->impostoDevol->set_pDevol(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIPIDevol")) //UA04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->det->obj->impostoDevol->set_vIPIDevol(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_obsItem(QXmlStreamReader &xml) const
{
    //Grupo VA. Observações de uso livre (para o item da NF-e)
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("obsItem")))
    {
        //VA02 - obsCont
        if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == QStringLiteral("obsCont"))
        {
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("obsCont")))
            {
                if (xml.name() == QStringLiteral("xCampo")) //VA03
                {
                    _valor = xml.readElementText();
                    if (!_valor.isEmpty())
                       m_nfe->infNFe->det->obj->obsItem->obsCont->set_xCampo(_valor);
                }
                if (xml.name() == QStringLiteral("xTexto")) //VA04
                {
                    _valor = xml.readElementText();
                    if (!_valor.isEmpty())
                       m_nfe->infNFe->det->obj->obsItem->obsCont->set_xTexto(_valor);
                }

                xml.readNext();
            }
        }

        //VA05 - obsFisco
        if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == QStringLiteral("obsFisco"))
        {
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("obsFisco")))
            {
                if (xml.name() == QStringLiteral("xCampo")) //VA06
                {
                    _valor = xml.readElementText();
                    if (!_valor.isEmpty())
                       m_nfe->infNFe->det->obj->obsItem->obsFisco->set_xCampo(_valor);
                }
                if (xml.name() == QStringLiteral("xTexto")) //VA07
                {
                    _valor = xml.readElementText();
                    if (!_valor.isEmpty())
                       m_nfe->infNFe->det->obj->obsItem->obsFisco->set_xTexto(_valor);
                }

                xml.readNext();
            }
        }

        xml.readNext();
    }
}

void XmlRead::set_total(QXmlStreamReader &xml) const
{
    //W01 - total
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("total")))
    {
        if (xml.name() == QStringLiteral("ICMSTot")) //W02
          set_ICMSTot(xml);

        if (xml.name() == QStringLiteral("ISSQNtot")) //W17
          set_ISSQNtot(xml);

        if (xml.name() == QStringLiteral("retTrib")) //W23
          set_retTrib(xml);

        xml.readNext();
    }
}

void XmlRead::set_ICMSTot(QXmlStreamReader &xml) const
{
    //W02 - ICMSTot
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ICMSTot")))
    {
        if (xml.name() == QStringLiteral("vBC")) //W03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMS")) //W04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vICMS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSDeson")) //W04a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vICMSDeson(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPUFDest")) //W04c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vFCPUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSUFDest")) //W04e
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vICMSUFDest(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSUFRemet")) //W04g
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vICMSUFRemet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCP")) //W04h
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vFCP(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCST")) //W05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vBCST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vST")) //W06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPST")) //W06a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vFCPST(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFCPSTRet")) //W06b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vFCPSTRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vProd")) //W07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vProd(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vFrete")) //W08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vFrete(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vSeg")) //W09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vSeg(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDesc")) //W010
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vDesc(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vII")) //W11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vII(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIPI")) //W12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vIPI(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIPIDevol")) //W12a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vIPIDevol(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vPIS")) //W13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCOFINS")) //W14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vOutro")) //W15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vOutro(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vNF")) //W16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vNF(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vTotTrib")) //W16a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ICMSTot->set_vTotTrib(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_ISSQNtot(QXmlStreamReader &xml) const
{
    //W17 - ISSQNtot
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("ISSQNtot")))
    {
        if (xml.name() == QStringLiteral("vServ")) //W18
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vServ(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBC")) //W19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vBC(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vISS")) //W20
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vISS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vPIS")) //W21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vCOFINS")) //22
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("dCompet")) //W22a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_dCompet(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("vDeducao")) //W22b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vDeducao(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vOutro")) //W22c
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vOutro(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDescIncond")) //W22d
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vDescIncond(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDescCond")) //W22e
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vDescCond(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vISSRet")) //W22f
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_vISSRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("cRegTrib")) //W22g
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->ISSQNtot->set_cRegTrib(ConvNF::strToCRegTrib(_valor));
        }

        xml.readNext();
    }
}

void XmlRead::set_retTrib(QXmlStreamReader &xml) const
{
    //W23 - retTrib
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("retTrib")))
    {
        if (xml.name() == QStringLiteral("vRetPIS")) //W24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vRetPIS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vRetCOFINS")) //W25
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vRetCOFINS(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vRetCSLL")) //W26
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vRetCSLL(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCIRRF")) //W27
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vBCIRRF(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vIRRF")) //W28
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vIRRF(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCRetPrev")) //W29
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vBCRetPrev(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vRetPrev")) //W30
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->total->retTrib->set_vRetPrev(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_transp(QXmlStreamReader &xml) const
{
    //X01 - transp
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("transp")))
    {
        if (xml.name() == QStringLiteral("modFrete")) //X02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->set_modFrete(ConvNF::strToModFrete(_valor));
        }

        //Grupo Transportador
        if (xml.name() == QStringLiteral("transporta")) //X03
          set_transp_transporta(xml);

        //Grupo Retenção ICMS transporte
        if (xml.name() == QStringLiteral("retTransp")) //X11
          set_transp_retTransp(xml);

        //Grupo Veículo Transporte
        if (xml.name() == QStringLiteral("veicTransp")) //X18
          set_transp_veicTransp(xml);

        //Grupo Reboque
        if (xml.name() == QStringLiteral("reboque")) //X22
          set_transp_reboque(xml);

        if (xml.name() == QStringLiteral("vagao")) //X25a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->set_vagao(_valor);
        }
        if (xml.name() == QStringLiteral("balsa")) //X25b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->set_balsa(_valor);
        }

        //Grupo Volumes
        if (xml.name() == QStringLiteral("vol")) //X26
          set_transp_vol(xml);

        xml.readNext();
    }
}

void XmlRead::set_transp_transporta(QXmlStreamReader &xml) const
{
    //X03 - transporta
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("transporta")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //X04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("CPF")) //X05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_CPF(_valor);
        }
        if (xml.name() == QStringLiteral("xNome")) //X06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_xNome(_valor);
        }
        if (xml.name() == QStringLiteral("IE")) //X07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_IE(_valor);
        }
        if (xml.name() == QStringLiteral("xEnder")) //X08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_xEnder(_valor);
        }
        if (xml.name() == QStringLiteral("xMun")) //X09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_xMun(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //X10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->transporta->set_UF(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_transp_retTransp(QXmlStreamReader &xml) const
{
    //X11 - retTransp
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("retTransp")))
    {
        if (xml.name() == QStringLiteral("vServ")) //X12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_vServ(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vBCRet")) //X13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_vBCRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pICMSRet")) //X14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_pICMSRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vICMSRet")) //X15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_vICMSRet(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("CFOP")) //X16
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_CFOP(_valor);
        }
        if (xml.name() == QStringLiteral("cMunFG")) //X17
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->retTransp->set_cMunFG(_valor.toInt());
        }

        xml.readNext();
    }
}

void XmlRead::set_transp_veicTransp(QXmlStreamReader &xml) const
{
    //X18 - veicTransp
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("veicTransp")))
    {
        if (xml.name() == QStringLiteral("placa")) //X19
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->veicTransp->set_placa(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //X20
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->veicTransp->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("RNTC")) //X21
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->veicTransp->set_RNTC(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_transp_reboque(QXmlStreamReader &xml) const
{
    //X22 - reboque
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("reboque")))
    {
        if (xml.name() == QStringLiteral("placa")) //X23
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->reboque->obj->set_placa(_valor);
        }
        if (xml.name() == QStringLiteral("UF")) //X24
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->reboque->obj->set_UF(_valor);
        }
        if (xml.name() == QStringLiteral("RNTC")) //X25
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->reboque->obj->set_RNTC(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->transp->reboque->add();
}

void XmlRead::set_transp_vol(QXmlStreamReader &xml) const
{
    //X26 - vol
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("vol")))
    {
        if (xml.name() == QStringLiteral("qVol")) //X27
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_qVol(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("esp")) //X28
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_esp(_valor);
        }
        if (xml.name() == QStringLiteral("marca")) //X29
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_marca(_valor);
        }
        if (xml.name() == QStringLiteral("nVol")) //X30
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_nVol(_valor);
        }
        if (xml.name() == QStringLiteral("pesoL")) //X31
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_pesoL(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("pesoB")) //X32
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->set_pesoB(_valor.toDouble());
        }

        //Grupo Lacres
        if (xml.name() == QStringLiteral("lacres")) //X33
          set_transp_vol_lacres(xml);

        xml.readNext();
    }
    m_nfe->infNFe->transp->vol->add();
}

void XmlRead::set_transp_vol_lacres(QXmlStreamReader &xml) const
{
    //X33 - lacres
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("lacres")))
    {
        if (xml.name() == QStringLiteral("nLacre")) //X34
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->transp->vol->obj->lacres->obj->set_nLacre(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->transp->vol->obj->lacres->add();
}

void XmlRead::set_cobr(QXmlStreamReader &xml) const
{
    //Y01 - cobr
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("cobr")))
    {
        //Grupo Fatura
        if (xml.name() == QStringLiteral("fat"))//Y02
          set_cobr_fat(xml);

        //Grupo Parcelas
        if (xml.name() == QStringLiteral("dup"))//Y07
          set_cobr_dup(xml);

        xml.readNext();
    }
}

void XmlRead::set_cobr_fat(QXmlStreamReader &xml) const
{
    //Y02 - fat
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("fat")))
    {

        if (xml.name() == QStringLiteral("nFat")) //Y03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->fat->set_nFat(_valor);
        }
        if (xml.name() == QStringLiteral("vOrig")) //Y04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->fat->set_vOrig(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vDesc")) //Y03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->fat->set_vDesc(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vLiq")) //Y04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->fat->set_vLiq(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_cobr_dup(QXmlStreamReader &xml) const
{
    //Y07 - dup
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("dup")))
    {
        if (xml.name() == QStringLiteral("nDup")) //Y08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->dup->obj->set_nDup(_valor);
        }
        if (xml.name() == QStringLiteral("dVenc")) //Y09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->dup->obj->set_dVenc(QDateTime::fromString(_valor, Qt::ISODate));
        }
        if (xml.name() == QStringLiteral("vDup")) //Y10
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cobr->dup->obj->set_vDup(_valor.toDouble());
        }

        xml.readNext();
    }
    m_nfe->infNFe->cobr->dup->add();
}

void XmlRead::set_pag(QXmlStreamReader &xml) const
{
    //YA01 - pag
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("pag")))
    {
        //Grupo Detalhamento do Pagamento
        if (xml.name() == QStringLiteral("detPag")) //YA01a
          set_pag_detPag(xml);

        if (xml.name() == QStringLiteral("vTroco")) //YA09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->set_vTroco(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_pag_detPag(QXmlStreamReader &xml) const
{
    //YA01a - detPag
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("detPag")))
    {
        if (xml.name() == QStringLiteral("indPag")) //YA01b
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->set_indPag(ConvNF::strToIndPag(_valor));
        }
        if (xml.name() == QStringLiteral("tPag")) //YA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->set_tPag(ConvNF::strToTPag(_valor));
        }
        if (xml.name() == QStringLiteral("xPag")) //YA02a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->set_xPag(_valor);
        }
        if (xml.name() == QStringLiteral("vPag")) //YA03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->set_vPag(_valor.toDouble());
        }

        //Grupo de Cartões
        if (xml.name() == QStringLiteral("card")) //YA04
          set_pag_detPag_card(xml);

        xml.readNext();
    }
    m_nfe->infNFe->pag->detPag->add();
}

void XmlRead::set_pag_detPag_card(QXmlStreamReader &xml) const
{
    //YA04 - card
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("card")))
    {
        if (xml.name() == QStringLiteral("tpIntegra")) //YA04a
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->card->set_tpIntegra(ConvNF::strToTpIntegra(_valor));
        }
        if (xml.name() == QStringLiteral("CNPJ")) //YA05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->card->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("tBand")) //YA06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->card->set_tBand(ConvNF::strToTBand(_valor));
        }
        if (xml.name() == QStringLiteral("cAut")) //YA07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->pag->detPag->obj->card->set_cAut(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_infIntermed(QXmlStreamReader &xml) const
{
    //YB01 - infIntermed
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("infIntermed")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //YB02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infIntermed->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("idCadIntTran")) //YB03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infIntermed->set_idCadIntTran(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_infAdic(QXmlStreamReader &xml) const
{
    //Z01 - infAdic
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("infAdic")))
    {
        if (xml.name() == QStringLiteral("infAdFisco")) //Z02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->set_infAdFisco(_valor);
        }
        if (xml.name() == QStringLiteral("infCpl")) //Z03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->set_infCpl(_valor);
        }

        //Grupo Campo de uso livre do contribuinte
        if (xml.name() == QStringLiteral("obsCont")) //Z04
          set_infAdic_obsCont(xml);

        //Grupo Campo de uso livre do Fisco
        if (xml.name() == QStringLiteral("obsFisco")) //Z07
          set_infAdic_obsFisco(xml);

        //Grupo Processo referenciado
        if (xml.name() == QStringLiteral("procRef")) //Z10
          set_infAdic_procRef(xml);

        xml.readNext();
    }
}

void XmlRead::set_infAdic_obsCont(QXmlStreamReader &xml) const
{
    //Z04 - obsCont
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("obsCont")))
    {
        if (xml.name() == QStringLiteral("xCampo")) //Z05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->obsCont->obj->set_xCampo(_valor);
        }
        if (xml.name() == QStringLiteral("xTexto")) //Z06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->obsCont->obj->set_xTexto(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->infAdic->obsCont->add();
}

void XmlRead::set_infAdic_obsFisco(QXmlStreamReader &xml) const
{
    //Z07 - obsFisco
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("obsFisco")))
    {
        if (xml.name() == QStringLiteral("xCampo")) //Z08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->obsFisco->obj->set_xCampo(_valor);
        }
        if (xml.name() == QStringLiteral("xTexto")) //Z09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->obsFisco->obj->set_xTexto(_valor);
        }

        xml.readNext();
    }
    m_nfe->infNFe->infAdic->obsFisco->add();
}

void XmlRead::set_infAdic_procRef(QXmlStreamReader &xml) const
{
    //Z10 - procRef
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("procRef")))
    {
        if (xml.name() == QStringLiteral("nProc")) //Z11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->procRef->obj->set_nProc(_valor);
        }
        if (xml.name() == QStringLiteral("indProc")) //Z12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->procRef->obj->set_indProc(ConvNF::strToIndProc(_valor));
        }
        if (xml.name() == QStringLiteral("tpAto")) //Z13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infAdic->procRef->obj->set_tpAto(ConvNF::strToTpAto(_valor));
        }

        xml.readNext();
    }
    m_nfe->infNFe->infAdic->procRef->add();
}

void XmlRead::set_exporta(QXmlStreamReader &xml) const
{
    //ZA01 - exporta
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("exporta")))
    {
        if (xml.name() == QStringLiteral("UFSaidaPais")) //ZA02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->exporta->set_UFSaidaPais(_valor);
        }
        if (xml.name() == QStringLiteral("xLocExporta")) //ZA03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->exporta->set_xLocExporta(_valor);
        }
        if (xml.name() == QStringLiteral("xLocDespacho")) //ZA04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->exporta->set_xLocDespacho(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_compra(QXmlStreamReader &xml) const
{
    //ZB01 - compra
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("compra")))
    {
        if (xml.name() == QStringLiteral("xNEmp")) //ZB02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->compra->set_xNEmp(_valor);
        }
        if (xml.name() == QStringLiteral("xPed")) //ZB03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->compra->set_xPed(_valor);
        }
        if (xml.name() == QStringLiteral("xCont")) //ZB04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->compra->set_xCont(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_cana(QXmlStreamReader &xml) const
{
    //ZC01 - canal
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("compra")))
    {
        if (xml.name() == QStringLiteral("safra")) //ZC02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_safra(_valor);
        }
        if (xml.name() == QStringLiteral("ref")) //ZC03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_ref(_valor);
        }

        //Grupo Fornecimento diário de cana
        if (xml.name() == QStringLiteral("forDia")) //ZC04
          set_cana_forDia(xml);

        if (xml.name() == QStringLiteral("qTotMes")) //ZC07
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_qTotMes(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qTotAnt")) //ZC08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_qTotAnt(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("qTotGer")) //ZC09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_qTotGer(_valor.toDouble());
        }

        //Grupo Deduções – Taxas e Contribuições
        if (xml.name() == QStringLiteral("deduc")) //ZC010
          set_cana_deduc(xml);

        if (xml.name() == QStringLiteral("vFor")) //ZC13
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_vFor(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vTotDed")) //ZC14
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_vTotDed(_valor.toDouble());
        }
        if (xml.name() == QStringLiteral("vLiqFor")) //ZC15
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->set_vLiqFor(_valor.toDouble());
        }

        xml.readNext();
    }
}

void XmlRead::set_cana_forDia(QXmlStreamReader &xml) const
{
    //ZC04 - forDia
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("forDia")))
    {
        if (xml.name() == QStringLiteral("dia")) //ZC05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->forDia->obj->set_dia(_valor.toInt());
        }
        if (xml.name() == QStringLiteral("qtde")) //ZC06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->forDia->obj->set_qtde(_valor.toInt());
        }

        xml.readNext();
    }
    m_nfe->infNFe->cana->forDia->add();
}

void XmlRead::set_cana_deduc(QXmlStreamReader &xml) const
{
    //ZC10 - deduc
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("deduc")))
    {
        if (xml.name() == QStringLiteral("xDed")) //ZC11
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->deduc->obj->set_xDed(_valor);
        }
        if (xml.name() == QStringLiteral("qtde")) //ZC12
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->cana->deduc->obj->set_vDed(_valor.toDouble());
        }

        xml.readNext();
    }
    m_nfe->infNFe->cana->deduc->add();
}

void XmlRead::set_infRespTec(QXmlStreamReader &xml) const
{
    //ZD01 - infRespTec
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("infRespTec")))
    {
        if (xml.name() == QStringLiteral("CNPJ")) //ZD02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("xContato")) //ZD04
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_xContato(_valor);
        }
        if (xml.name() == QStringLiteral("email")) //ZD05
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_email(_valor);
        }
        if (xml.name() == QStringLiteral("fone")) //ZD06
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_fone(_valor);
        }
        if (xml.name() == QStringLiteral("idCSRT")) //ZD08
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_CNPJ(_valor);
        }
        if (xml.name() == QStringLiteral("hashCSRT")) //ZD09
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFe->infRespTec->set_hashCSRT(_valor);
        }

        xml.readNext();
    }
}

void XmlRead::set_infNFeSupl(QXmlStreamReader &xml) const
{
    //ZX01 - infNFeSupl
    QString _valor;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QStringLiteral("infNFeSupl")))
    {
        if (xml.name() == QStringLiteral("qrCode")) //ZX02
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFeSupl->set_qrCode(_valor);
        }
        if (xml.name() == QStringLiteral("urlChave")) //ZX03
        {
            _valor = xml.readElementText();
            if (!_valor.isEmpty())
               m_nfe->infNFeSupl->set_urlChave(_valor);
        }

        xml.readNext();
    }
}



