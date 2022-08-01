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


#include "cppdanfeqtrpt.h"

struct RetEnt{
    QString m_descricao;
    QString m_CNPJ;
    QString m_CPF;
    QString m_xNome;
    QString m_xLgr;
    int m_nro;
    QString m_xCpl;
    QString m_xBairro;
    int m_cMun;
    QString m_xMun;
    QString m_UF;
    int m_CEP;
    int m_cPais;
    QString m_xPais;
    QString m_fone;
    QString m_email;
    QString m_IE;

    RetEnt(){};

    RetEnt(QString descricao, QString CNPJ, QString CPF, QString xNome, QString xLgr, int nro, QString xCpl,
           QString xBairro, int cMun, QString xMun, QString UF, int CEP, int cPais, QString xPais, QString fone,
           QString email, QString IE): m_descricao(descricao), m_CNPJ(CNPJ), m_CPF(CPF), m_xNome(xNome),
                                       m_xLgr(xLgr), m_nro(nro), m_xCpl(xCpl), m_xBairro(xBairro), m_cMun(cMun),
                                       m_xMun(xMun), m_UF(UF), m_CEP(CEP), m_cPais(cPais), m_xPais(xPais),
                                       m_fone(fone), m_email(email), m_IE(IE) {};
};
QList<RetEnt> RetEntList;

CppDanfeQtRPT::CppDanfeQtRPT(const CppNFe *cppnfe, const int &recNo): CppDanfe(cppnfe, recNo),
    m_report(new QtRPT(this))
{
}

CppDanfeQtRPT::~CppDanfeQtRPT()
{
}

void CppDanfeQtRPT::caminhoArquivo(const QString &arquivo)
{
    this->m_pathArquivo = arquivo;
}

void CppDanfeQtRPT::caminhoLogo(const QString &logo)
{
    this->m_pathLogo = logo;
}

void CppDanfeQtRPT::print()
{
    QString _file = this->m_pathArquivo;
    loadRetEnt();

    //background não está funcionando
    //m_report->setBackgroundImage(QPixmap(""));

    if (!m_report->loadReport(_file))
        qDebug() << "Report file not found";

    QObject::connect(&*m_report, &QtRPT::setValue, this, &CppDanfeQtRPT::setValue);
    QObject::connect(&*m_report, &QtRPT::setField, this, &CppDanfeQtRPT::setField);
    QObject::connect(&*m_report, &QtRPT::setValueImage, this, &CppDanfeQtRPT::setValueImage);
    QObject::connect(&*m_report, &QtRPT::setDSInfo, this, &CppDanfeQtRPT::setDSInfo);

    m_report->printExec(true);
}

void CppDanfeQtRPT::loadRetEnt()
{
    RetEntList.clear();
    //se existir local de retirada
    if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xNome().isEmpty() ||
        !this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xLgr().isEmpty())
    {
         RetEntList.append(RetEnt("INFORMAÇÕES DO LOCAL DE RETIRADA",
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_CNPJ(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_CPF(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xNome(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xLgr(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_nro(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xCpl(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xBairro(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_cMun(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xMun(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_UF(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_CEP(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_cPais(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_xPais(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_fone(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_email(),
                                  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->retirada->get_IE()));
    }
    //se existir local de entrega
    if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xNome().isEmpty() ||
        !this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xLgr().isEmpty())
    {
        RetEntList.append(RetEnt("INFORMAÇÕES DO LOCAL DE ENTREGA",
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_CNPJ(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_CPF(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xNome(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xLgr(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_nro(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xCpl(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xBairro(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_cMun(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xMun(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_UF(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_CEP(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_cPais(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_xPais(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_fone(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_email(),
                                 this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->entrega->get_IE()));
    }
}

void CppDanfeQtRPT::setDSInfo(DataSetInfo &dsInfo)
{
    //retirada/entrega
    if (dsInfo.dsName == "dsRetEnt")
       dsInfo.recordCount = RetEntList.count();

    //fatura
    if (dsInfo.dsName == "dsFatura")
    {
        if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_nFat().isEmpty() ||
           this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_vLiq() > 0)
        {
           dsInfo.recordCount = 1;

        }
        else
           dsInfo.recordCount = 0;
    }

    //duplicata
    if (dsInfo.dsName == "dsDup")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->count() > 0)
        {
            //obs: como não existe horizontalBand nesse relatório, deve ser feito um cálculo colocando 5 registros por linha.
            int _count, _tReg;
            _count = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->count();

            if (_count <= 5)
            {
                _tReg = 1;
            }
            else
            {
                //caso o resto da divisão por 5 seja maior que zero, acrescenta um ao total de registros.
                _tReg = (_count / 5);
                if ((_count % 5) > 0)
                    _tReg += 1;
            }

           dsInfo.recordCount = _tReg;
        }
        else
           dsInfo.recordCount = 0;
    }

    //transporte/volume
    if (dsInfo.dsName == "dsVolume")
    {
        //deve ter no mínimo 1
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
           dsInfo.recordCount = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count();
        else
           dsInfo.recordCount = 1;
    }
    //produto
    if (dsInfo.dsName == "dsProd")
       dsInfo.recordCount = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->count();

    //rodapé da nfce
    if (dsInfo.dsName == "dsFooterNFCe")
       dsInfo.recordCount = 1;

    //pagamento
    if (dsInfo.dsName == "dsPagamento")
       dsInfo.recordCount = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->pag->detPag->items->count();

}

void CppDanfeQtRPT::setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage)
{
    Q_UNUSED(reportPage);


    //----------canhoto
    if (paramName == "canhoto.vNF")
       paramValue =  CppUtility::formatMoney(this->m_cppnfe->notafiscal->NFe->items->
                                             value(this->m_recNo)->infNFe->total->ICMSTot->get_vNF(), 2);

    //----------info geral
    if (paramName == "siteConsulta")
    {
        if (this->m_mod == ModeloDF::NFe)
        {
            if (this->m_tpEmis == TpEmis::Normal)
            {
                paramValue = QStringLiteral("Consulta de autenticidade no portal nacional da NF-e ")
                           + QStringLiteral("www.nfe.fazenda.gov.br/portal ou no site da Sefaz Autorizadora");
            }
            else
                if (this->m_tpEmis == TpEmis::Contingencia_EPEC)
                {
                    paramValue = QStringLiteral("Consulta de autenticidade no portal nacional da NF-e ")
                               + QStringLiteral("www.nfe.fazenda.gov.br/portal");

                }
        } else
        {   //nfce pega somente o endereço para consulta por chave
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFeSupl->get_urlChave();
        }
    }

    if (paramName == "geral.msg")
    {
        QString _msg;
        //homologação
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_tpAmb() == TpAmb::Homologacao)
        {
            _msg = QStringLiteral("SEM VALOR FISCAL") + QString("\n") + QStringLiteral("HOMOLOGAÇÃO");
        }
        //cancelamento
        if (this->m_cStat == 101 || this->m_cStat == 151)
        {
            if (!_msg.isEmpty())
               _msg += QString("\n");

            _msg += QStringLiteral("NFe CANCELADA");
        }
        if (!_msg.isEmpty())
           paramValue = _msg;
    }
    if (paramName == "tpnf")
       paramValue =  ConvNF::tpNFToStr(this->m_cppnfe->notafiscal->NFe->items->
                                       value(this->m_recNo)->infNFe->ide->get_tpNF());
    if (paramName == "nNF")
       paramValue = this->m_nNF;
    if (paramName == "serie")
       paramValue =  this->m_serie;
    if (paramName == "chave")
       paramValue = this->m_chave;

    //protocolo
    if (paramName == "labelProtocolo")
    {
        switch (this->m_tpEmis)
        {
            case TpEmis::Normal :
                paramValue = "PROTOCOLO DE AUTORIZAÇÃO DE USO";
                break;
            case TpEmis::Contingencia_FSIA :
            case TpEmis::Contingencia_FSDA :
                paramValue = "DADOS DA NF-E";
                break;
            case TpEmis::Contingencia_EPEC :
                paramValue = "PROTOCOLO DE AUTORIZAÇÃO DO EPEC";
                break;
            default:
                paramValue = "";
                break;
        }
    }
    if (paramName == "protocolo")
    {
        switch (this->m_tpEmis)
        {
            case TpEmis::Normal :
            case TpEmis::Contingencia_EPEC :
                paramValue = this->m_protocolo;
                break;
            case TpEmis::Contingencia_FSIA :
            case TpEmis::Contingencia_FSDA :
                paramValue = this->m_chaveCont;
                break;
            default:
                paramValue = "";
                break;
        }
    }

    if (paramName == "natOp")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_natOp();
    if (paramName == "natOp")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_natOp();

    //----------emit
    if (paramName == "emit.xNome")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_xNome();
    if (paramName == "emit.ender")
    {
        paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->enderEmit->get_xLgr() + ", "
                    + QString::number(this->m_cppnfe->notafiscal->NFe->items->
                                      value(this->m_recNo)->infNFe->emite->enderEmit->get_nro())
                    + " - " + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->enderEmit->get_xBairro()
                    + " - " + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->enderEmit->get_xMun()
                    + " - " + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->enderEmit->get_UF()
                    + " - CEP: " + QString::number(this->m_cppnfe->notafiscal->NFe->items->
                                                   value(this->m_recNo)->infNFe->emite->enderEmit->get_CEP());
    }
    if (paramName == "emit.fone")
       paramValue = "fone: " + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->enderEmit->get_fone();
    if (paramName == "emit.ie")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_IE();
    if (paramName == "emit.iest")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_IEST();
    if (paramName == "emit.cnpj")
    {
        if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_CNPJ().isEmpty())
        {
            paramValue = CppUtility::formatCNPJ(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_CNPJ());
        }
        else
        {
            paramValue = CppUtility::formatCPF(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_CPF());
        }
    }

    //----------dest
    if (paramName == "dest.xNome")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_xNome();
    if (paramName == "dest.cnpj")
    {
        QString _cpfCnpj;
        //se vazio
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_CNPJ().isEmpty() &&
            this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_CPF().isEmpty())
        {
            _cpfCnpj = "CONSUMIDOR NAO IDENTIFICADO";
        } else
        {
            QString _numDoc, _nomeDoc;
            if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_CNPJ().isEmpty())
            {
                _numDoc = CppUtility::formatCNPJ(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_CNPJ());
                _nomeDoc = "CNPJ do Consumidor: ";
            }
            else
            {
                _numDoc = CppUtility::formatCPF(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_CPF());
                _nomeDoc = "CPF do Consumidor: ";
            }
            //se NFCe, já deve ir formatado com o nome do documento. NFe somente número.
            if (this->m_mod == ModeloDF::NFCe)
               _cpfCnpj = _nomeDoc + _numDoc;
            else
               _cpfCnpj = _numDoc;
        }

        paramValue = _cpfCnpj;
    }
    if (paramName == "dest.ender")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_xLgr();
    if (paramName == "dest.xBairro")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_xBairro();
    if (paramName == "dest.cep")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_CEP();
    if (paramName == "dest.xMun")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_xMun();
    if (paramName == "dest.uf")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_UF();
    if (paramName == "dest.telefone")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->enderDest->get_fone();
    if (paramName == "dest.ie")
       paramValue =  this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->dest->get_IE();
    if (paramName == "dhEmi")
    {
        if (this->m_mod == ModeloDF::NFe)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_dhEmi().toString("dd/MM/yyyy");
        else //nfce
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_dhEmi().toString("dd/MM/yyyy hh:mm:ss");
    }
    if (paramName == "dhSaiEnt")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_dhSaiEnt().toString("dd/MM/yyyy");
    if (paramName == "hSaiEnt")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_dhSaiEnt().toString("hh:mm:ss");

    //retirada/entrega
    if ((paramName == "retent.descricao"))
       paramValue =  RetEntList.value(recNo).m_descricao;
    if ((paramName == "retent.xNome"))
       paramValue =  RetEntList.value(recNo).m_xNome;
    if ((paramName == "retent.cnpj"))
    {
        if (!RetEntList.value(recNo).m_CNPJ.isEmpty())
           paramValue =  RetEntList.value(recNo).m_CNPJ;
        else
           paramValue =  RetEntList.value(recNo).m_CPF;
    }
    if (paramName == "retent.IE")
       paramValue =  RetEntList.value(recNo).m_IE;
    if (paramName == "retent.xLgr")
       paramValue =  RetEntList.value(recNo).m_xLgr;
    if (paramName == "retent.nro")
       paramValue =  RetEntList.value(recNo).m_nro;
    if (paramName == "retent.xCpl")
       paramValue =  RetEntList.value(recNo).m_xCpl;
    if (paramName == "retent.xBairro")
       paramValue =  RetEntList.value(recNo).m_xBairro;
    if (paramName == "retent.CEP")
       paramValue =  RetEntList.value(recNo).m_CEP;
    if (paramName == "retent.xMun")
       paramValue =  RetEntList.value(recNo).m_xMun;
    if (paramName == "retent.UF")
       paramValue =  RetEntList.value(recNo).m_UF;
    if (paramName == "retent.fone")
       paramValue =  RetEntList.value(recNo).m_fone;

    //fatura
    if (paramName == "fatura.nFat")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_nFat();
    if (paramName == "fatura.vOrig")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_vOrig();
    if (paramName == "fatura.vDesc")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_vDesc();
    if (paramName == "fatura.vLiq")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->fat->get_vLiq();

    //duplicata
    if ((paramName == "dup.descricao") && (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->count() > 0))
    {
        //só aparece a descrição no primeiro registro
        if (recNo == 0)
           paramValue =  "DUPLICATA";
    }

    if ((paramName == "dup.reg0") || (paramName == "dup.reg1") || (paramName == "dup.reg2") || (paramName == "dup.reg3") ||
        (paramName == "dup.reg4"))
    {
        int _iColumn = 0;
        if (paramName == "dup.reg1")
           _iColumn = 1;
        else
            if (paramName == "dup.reg2")
               _iColumn = 2;
            else
                if (paramName == "dup.reg3")
                   _iColumn = 3;
                else
                    if (paramName == "dup.reg4")
                       _iColumn = 4;

        //obs: como são 5 registro por linha, o cálculo será: (registro atual * (total de colunas que é 5)) + numero da coluna.
        int _rec = (recNo * 5) + _iColumn;
        //verifica se há registros. Obs: Deve sempre ser maior, já que o recNo começa do zero.
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->count() > _rec)
        {
            QString _v =
              QStringLiteral("Parcela:           ")
            + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->value(_rec)->get_nDup()
            + QString("\n")
            + QStringLiteral("Vencimento:    ")
            + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->value(_rec)->get_dVenc().toString("dd/MM/yyyy")
            + QString("\n")
            + QStringLiteral("Valor:             ")
            + CppUtility::formatMoney(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->cobr->dup->items->value(_rec)->get_vDup(), 2);

            paramValue = _v;
        }
    }


    //----------total
    if (paramName == "imposto.vBC")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vBC();
    if (paramName == "imposto.vICMS")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vICMS();
    if (paramName == "imposto.vBCST")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vBCST();
    if (paramName == "imposto.vST")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vST();
    if (paramName == "imposto.vTotTrib")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vTotTrib();
    if (paramName == "imposto.vProd")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vProd();
    if (paramName == "imposto.vFrete")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vFrete();
    if (paramName == "imposto.vSeg")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vSeg();
    if (paramName == "imposto.vDesc")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vDesc();
    if (paramName == "imposto.vOutro")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vOutro();
    if (paramName == "imposto.vIPI")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vIPI();
    if (paramName == "imposto.vNF")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vNF();
    if (paramName == "imposto.qtdItens") //somente NFCe
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->count();

    //----------pagamentos - somente NFCe
    if (paramName == "pagamento.vTroco")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->pag->get_vTroco();
    if (paramName == "pagamento.tPag")
       paramValue = tPagToNome(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->pag->detPag->items->value(recNo)->get_tPag());
    if (paramName == "pagamento.vPag")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->pag->detPag->items->value(recNo)->get_vPag();

    //----------transportador
    if (paramName == "transporte.xNome")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_xNome();
    if (paramName == "transporte.modFrete")
       paramValue = this->m_modFrete;
    if (paramName == "transporte.RNTC")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->veicTransp->get_RNTC();
    if (paramName == "transporte.placa")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->veicTransp->get_placa();
    if (paramName == "transporte.veicTranspUF")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->veicTransp->get_UF();
    if (paramName == "transporte.cnpj")
    {
        if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_CNPJ().isEmpty())
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_CNPJ();
        else
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_CPF();
    }
    if (paramName == "transporte.xEnder")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_xEnder();
    if (paramName == "transporte.xEnder")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_xEnder();
    if (paramName == "transporte.xMun")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_xMun();
    if (paramName == "transporte.UF")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_UF();
    if (paramName == "transporte.IE")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->transporta->get_IE();

    //----------transportador/volume
    if (paramName == "volume.qVol")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_qVol();
        else//se for nota de saída, deve haver ao menos 1 volume
            if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_tpNF() == TpNF::Saida)
                paramValue = 1;
    }
    if (paramName == "volume.esp")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_esp();
    }
    if (paramName == "volume.marca")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_marca();
    }
    if (paramName == "volume.nVol")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
           paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_nVol();
    }
    if (paramName == "volume.pesoB")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_pesoB();
    }

    if (paramName == "volume.pesoL")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->count() > 0)
            paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->vol->items->value(recNo)->get_pesoL();
    }
    //----------prod
    if (paramName == "prod.nItem")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_nItem();
    if (paramName == "prod.cProd")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_cProd();
    if (paramName == "prod.xProd")
    {
        QString _descOrig = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_xProd();
        //caso o seja maior que 35 caracters, desce uma linha para NFe e Truncar para NFCe, já que o field do relatório não faz isso.
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_xProd().length() > 35)
        {
            QString _descModif;
            _descModif = _descOrig.mid(0, 35);
            //caso NFe, desce uma linha
            if (this->m_mod == ModeloDF::NFe)
            {
                _descModif += QString("\n");
                _descModif += _descOrig.mid(35, (_descOrig.length() - 35));
                //se o total, mesmo com o caractere de nova linha, ficar maior que 71, tem que truncar para não invadir o outro campo
                if (_descModif.length() > 71)
                {
                   QString _tmp = _descModif.mid(0, 71);
                   _descModif = _tmp;
                }
            }
            paramValue = _descModif;
        }
        else
            paramValue = _descOrig;

    }
    if (paramName == "prod.NCM")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_NCM();
    if (paramName == "prod.CST")
    {
        if (this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_CST() !=
            CstICMS::None)
        {
            paramValue = ConvNF::cstICMSToStr(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_CST());
        }
        else
        {
            paramValue = ConvNF::csosnICMSToStr(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_CSOSN());
        }
    }
    if (paramName == "prod.CFOP")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_CFOP();
    if (paramName == "prod.uCom")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_uCom();
    if (paramName == "prod.qCom")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_qCom();
    if (paramName == "prod.vUnCom")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_vUnCom();
    if (paramName == "prod.vDesc")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_vDesc();
    if (paramName == "prod.vProd")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->prod->get_vProd();
    if (paramName == "prod.vBC")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_vBC();
    if (paramName == "prod.vICMS")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_vICMS();
    if (paramName == "prod.vIPI")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->IPI->get_vIPI();
    if (paramName == "prod.pICMS")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->ICMS->get_pICMS();
    if (paramName == "prod.pIPI")
       paramValue = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->det->items->value(recNo)->imposto->IPI->get_pIPI();

    //informações complementares
    if (paramName == "infCpl")
       paramValue = this->m_infCpl;
}

void CppDanfeQtRPT::setField(RptFieldObject &fieldObject)
{
    //reduz campo endereço para exibir imagem
    if (!this->m_pathLogo.isEmpty())
    {
        if (CppUtility::fileExists(this->m_pathLogo))
        {
            if (fieldObject.name == "fieldenderEmit")
            {
                if (this->m_mod == ModeloDF::NFe)
                {
                    fieldObject.setHeight(76);
                    fieldObject.setWidth(202);
                    fieldObject.setLeft(116);
                } else //NFCe
                {
                    fieldObject.setHeight(47);
                    fieldObject.setWidth(207);
                    fieldObject.setLeft(79);
                }
            }
        }
    }
    //canhoto
    if (this->m_tpEmis == TpEmis::Contingencia_FSIA || this->m_tpEmis == TpEmis::Contingencia_FSDA)
    {
        if (fieldObject.name == "fieldIdentificacao")
        {
            fieldObject.setHeight(47);
            fieldObject.setWidth(320);
            fieldObject.setLeft(122);
            fieldObject.setTop(48);
        }
        if (fieldObject.name == "fieldNumCanhoto")
        {
            fieldObject.setHeight(47);
            fieldObject.setWidth(153);
            fieldObject.setLeft(637);
            fieldObject.setTop(1);
        }
        if (fieldObject.name == "boxBarrasCanhoto")
        {
            fieldObject.setHeight(47);
            fieldObject.setWidth(349);
            fieldObject.setLeft(441);
            fieldObject.setTop(48);
        }
    }
}

void CppDanfeQtRPT::setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage)
{
    Q_UNUSED(recNo);
    Q_UNUSED(reportPage);
    //logotipo empresa
    if (!this->m_pathLogo.isEmpty())
    {
        if (CppUtility::fileExists(this->m_pathLogo))
        {
            if (paramName == "imgLogo")
            {
                QImage _image(this->m_pathLogo);
                paramValue = _image;
            }
        }
    }
    //codigo de barras chave de acesso
    if (paramName == "barcodeChave")
    {
        paramValue = this->m_imBarcode;
    }
    //codigo de barras contingência FSIA e FSDA
    if (paramName == "barcodeCont")
    {
        if (this->m_tpEmis == TpEmis::Contingencia_FSIA || this->m_tpEmis == TpEmis::Contingencia_FSDA)
        {
            paramValue = this->m_imBarcodeCont;
        }
    }
    //QrCode
    if (paramName == "imgQrCode")
    {
        paramValue = this->m_imQrCode;
    }

}
