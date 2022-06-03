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


#include "retornonfe.h"

//class ProtNFe------------------------------------------------

ProtNFe::ProtNFe() : m_tpAmb(TpAmb::None), m_cStat(0), m_cMsg(0)
{
}

ProtNFe::~ProtNFe()
{
}

void ProtNFe::clear()
{
    this->m_versao.clear();
    this->m_Id.clear();
    this->m_tpAmb = TpAmb::None;
    this->m_verAplic.clear();
    this->m_chNFe.clear();
    CppUtility::clearDateTime(this->m_dhRecbto);
    this->m_nProt.clear();
    this->m_digVal.clear();
    this->m_cStat = 0;
    this->m_xMotivo.clear();
    this->m_cMsg = 0;
    this->m_xMsg.clear();
}

QString ProtNFe::get_versao() const
{
    return this->m_versao;
}

QString ProtNFe::get_Id() const
{
    return this->m_Id;
}

TpAmb ProtNFe::get_tpAmb() const
{
    return this->m_tpAmb;
}

QString ProtNFe::get_verAplic() const
{
    return this->m_verAplic;
}

QString ProtNFe::get_chNFe() const
{
    return this->m_chNFe;
}

QDateTime ProtNFe::get_dhRecbto() const
{
    return this->m_dhRecbto;
}

QString ProtNFe::get_nProt() const
{
    return this->m_nProt;
}

QString ProtNFe::get_digVal() const
{
    return this->m_digVal;
}

int ProtNFe::get_cStat() const
{
    return this->m_cStat;
}

QString ProtNFe::get_xMotivo() const
{
    return this->m_xMotivo;
}

int ProtNFe::get_cMsg() const
{
    return this->m_cMsg;
}

QString ProtNFe::get_xMsg() const
{
    return this->m_xMsg;
}

QString ProtNFe::get_xml() const
{
    return this->m_xml;
}

void ProtNFe::salvarXML(const ConfigNFe *config)
{
    QString path = config->get_caminhoNF();
    QString nome = get_chNFe() + "-nfe";
    CppUtility::saveFile(path, nome, TipoArquivo::XML, m_xml.toLocal8Bit());
}

void ProtNFe::salvarXML(const QString &caminho, const QString &nomeArquivo)
{
    QString nome;
    if (nomeArquivo.isEmpty())
        nome = get_chNFe() + "-nfe";
    else
        nome = nomeArquivo;
    CppUtility::saveFile(caminho, nome, TipoArquivo::XML, m_xml.toLocal8Bit());
}

void ProtNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

void ProtNFe::set_Id(const QString &Id)
{
    this->m_Id = Id;
}

void ProtNFe::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

void ProtNFe::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

void ProtNFe::set_chNFe(const QString &chNFe)
{
    this->m_chNFe = chNFe;
}

void ProtNFe::set_dhRecbto(const QDateTime &dhRecbto)
{
    this->m_dhRecbto = dhRecbto;
}

void ProtNFe::set_nProt(const QString &nProt)
{
    this->m_nProt = nProt;
}

void ProtNFe::set_digVal(const QString &digVal)
{
    this->m_digVal = digVal;
}

void ProtNFe::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

void ProtNFe::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

void ProtNFe::set_cMsg(const int &cMsg)
{
    this->m_cMsg = cMsg;
}

void ProtNFe::set_xMsg(const QString &xMsg)
{
    this->m_xMsg = xMsg;
}

void ProtNFe::set_xml(const QString &xml)
{
    this->m_xml = xml;
}

//class RetConsReciNFe------------------------------------------------

RetConsReciNFe::RetConsReciNFe(): protNFe(new Container<ProtNFe>), m_tpAmb(TpAmb::None),
    m_cStat(0), m_cUF(0), m_cMsg(0)
{
}

RetConsReciNFe::~RetConsReciNFe()
{
}

void RetConsReciNFe::clear()
{
    this->m_versao.clear();
    this->m_tpAmb = TpAmb::None;
    this->m_verAplic.clear();
    this->m_nRec.clear();
    this->m_cStat = 0;
    this->m_xMotivo.clear();
    this->m_cUF = 0;
    CppUtility::clearDateTime(this->m_dhRecbto);
    this->m_cMsg = 0;
    this->m_xMsg.clear();
    //protNFe
    this->protNFe->clear();
}

QString RetConsReciNFe::get_versao() const
{
    return this->m_versao;
}

TpAmb RetConsReciNFe::get_tpAmb() const
{
    return this->m_tpAmb;
}

QString RetConsReciNFe::get_verAplic() const
{
    return this->m_verAplic;
}

QString RetConsReciNFe::get_nRec() const
{
    return this->m_nRec;
}

int RetConsReciNFe::get_cStat() const
{
    return this->m_cStat;
}

QString RetConsReciNFe::get_xMotivo() const
{
    return this->m_xMotivo;
}

int RetConsReciNFe::get_cUF() const
{
    return this->m_cUF;
}

QDateTime RetConsReciNFe::get_dhRecbto() const
{
    return this->m_dhRecbto;
}

int RetConsReciNFe::get_cMsg() const
{
    return this->m_cMsg;
}

QString RetConsReciNFe::get_xMsg() const
{
    return this->m_xMsg;
}

void RetConsReciNFe::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

void RetConsReciNFe::set_tpAmb(const TpAmb &tpAmb)
{
    this->m_tpAmb = tpAmb;
}

void RetConsReciNFe::set_verAplic(const QString &verAplic)
{
    this->m_verAplic = verAplic;
}

void RetConsReciNFe::set_nRec(const QString &nRec)
{
    this->m_nRec = nRec;
}

void RetConsReciNFe::set_cStat(const int &cStat)
{
    this->m_cStat = cStat;
}

void RetConsReciNFe::set_xMotivo(const QString &xMotivo)
{
    this->m_xMotivo = xMotivo;
}

void RetConsReciNFe::set_cUF(const int &cUF)
{
    this->m_cUF = cUF;
}

void RetConsReciNFe::set_dhRecbto(const QDateTime &dhRecbto)
{
    this->m_dhRecbto = dhRecbto;
}

void RetConsReciNFe::set_cMsg(const int &cMsg)
{
    this->m_cMsg = cMsg;
}

void RetConsReciNFe::set_xMsg(const QString &xMsg)
{
    this->m_xMsg = xMsg;
}

void RetConsReciNFe::finalizarXML()
{

}
