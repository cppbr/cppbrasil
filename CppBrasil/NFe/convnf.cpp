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


#include "convnf.h"

ConvNF::ConvNF()
{

}

QString ConvNF::webServicesNFToMetodo(const WebServicesNF &value)
{
    switch (value) {
        case WebServicesNF::NFeAutorizacao :
            return "NFeAutorizacao";
            break;
        case WebServicesNF::NFeConsultaCadastro :
            return "CadConsultaCadastro";
            break;
        case WebServicesNF::NFeConsultaProtocolo :
            return "NFeConsultaProtocolo";
            break;
        case WebServicesNF::NFeInutilizacao :
            return "NFeInutilizacao";
            break;
        case WebServicesNF::NFeRecepcaoEvento :
            return "NFeRecepcaoEvento";
            break;
        case WebServicesNF::NFeRetAutorizacao :
            return "NFeRetAutorizacao";
            break;
        case WebServicesNF::NFeStatusServico :
            return "NFeStatusServico";
            break;
        default: return "";
    }
}

QString ConvNF::versaoLayout(const WebServicesNF &value)
{ //retorna a ultima versão do layout de cada serviço

    switch (value) {
        case WebServicesNF::NFeAutorizacao :
            return "4.00";
            break;
        case WebServicesNF::NFeConsultaCadastro :
            return "2.00";
            break;
        case WebServicesNF::NFeConsultaProtocolo :
            return "4.00";
            break;
        case WebServicesNF::NFeInutilizacao :
            return "4.00";
            break;
        case WebServicesNF::NFeRecepcaoEvento :
            return "1.00";
            break;
        case WebServicesNF::NFeRetAutorizacao :
            return "4.00";
            break;
        case WebServicesNF::NFeStatusServico :
            return "4.00";
            break;
        case WebServicesNF::NFeDistribuicaoDFe :
            return "9.99";
            break;
        default: return "";
    }
}

QString ConvNF::versaoNFToStr(const VersaoNF &value)
{
    if (value == VersaoNF::v400)
        return "4.00";

    //padrão
    return "";
}

VersaoNF ConvNF::strToVersaoNF(const QString &value)
{
    if (value == "4.00")
        return VersaoNF::v400;
    //padrão
    return VersaoNF::None;
}

QString ConvNF::versaoQrCodeNFToStr(const VersaoQrCodeNF &value)
{
    if (value == VersaoQrCodeNF::v200)
        return "2.00";
    //padrão
    return "";
}

VersaoQrCodeNF ConvNF::strToVersaoQrCodeNF(const QString &value)
{
    if ((value == "2") || (value == "2.00"))
        return VersaoQrCodeNF::v200;
    //padrão
    return VersaoQrCodeNF::None;
}

QString ConvNF::indSincToStr(const IndSinc &value)
{
    if (value == IndSinc::Nao)
        return "0";
    else
        return "1";
}

IndSinc ConvNF::strToIndSinc(const QString &value)
{
    if (value == "0")
        return IndSinc::Nao;
    else
        return IndSinc::Sim;
}

QString ConvNF::tpNFToStr(const TpNF &value)
{
    if (value == TpNF::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpNF ConvNF::strToTpNF(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpNF::None;
    else
        return static_cast<TpNF>(_r[_i].toInt());
}

QString ConvNF::idDestToStr(const IdDest &value)
{
    if (value == IdDest::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IdDest ConvNF::strToIdDest(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IdDest::None;
    else
        return static_cast<IdDest>(_r[_i].toInt());
}

QString ConvNF::tpImpToStr(const TpImp &value)
{
    if (value == TpImp::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpImp ConvNF::strToTpImp(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","5"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpImp::None;
    else
        return static_cast<TpImp>(_r[_i].toInt());
}

QString ConvNF::tpEmisToStr(const TpEmis &value)
{
    if (value == TpEmis::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpEmis ConvNF::strToTpEmis(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3","4","5","6","7","8","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpEmis::None;
    else
        return static_cast<TpEmis>(_r[_i].toInt());
}

QString ConvNF::finNFeToStr(const FinNFe &value)
{
    if (value == FinNFe::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

FinNFe ConvNF::strToFinNFe(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3","4"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return FinNFe::None;
    else
        return static_cast<FinNFe>(_r[_i].toInt());
}

QString ConvNF::indFinalToStr(const IndFinal &value)
{
    if (value == IndFinal::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndFinal ConvNF::strToIndFinal(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndFinal::None;
    else
        return static_cast<IndFinal>(_r[_i].toInt());

}

QString ConvNF::indPresToStr(const IndPres &value)
{
    if (value == IndPres::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndPres ConvNF::strToIndPres(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","5","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndPres::None;
    else
        return static_cast<IndPres>(_r[_i].toInt());
}

QString ConvNF::indIntermedToStr(const IndIntermed &value)
{
    if (value == IndIntermed::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndIntermed ConvNF::strToIndIntermed(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndIntermed::None;
    else
        return static_cast<IndIntermed>(_r[_i].toInt());
}

QString ConvNF::procEmiToStr(const ProcEmi &value)
{
    if (value == ProcEmi::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

ProcEmi ConvNF::strToProcEmi(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return ProcEmi::None;
    else
        return static_cast<ProcEmi>(_r[_i].toInt());
}

QString ConvNF::crtToStr(const Crt &value)
{
    if (value == Crt::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

Crt ConvNF::strToCrt(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return Crt::None;
    else
        return static_cast<Crt>(_r[_i].toInt());
}

QString ConvNF::indIEDestToStr(const IndIEDest &value)
{
    if (value == IndIEDest::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndIEDest ConvNF::strToIndIEDest(const QString &value)
{
    QList<QString> _r = {"-1","1","2","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndIEDest::None;
    else
        return static_cast<IndIEDest>(_r[_i].toInt());
}

QString ConvNF::indTotToStr(const IndTot &value)
{
    if (value == IndTot::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndTot ConvNF::strToIndTot(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndTot::None;
    else
        return static_cast<IndTot>(_r[_i].toInt());
}

QString ConvNF::indEscalaToStr(const IndEscala &value)
{
    if (value == IndEscala::EscalaRelevante)
        return "S";
    else if (value == IndEscala::EscalaNaoRelevante)
        return "N";
    else
        return QString();
}

IndEscala ConvNF::strToIndEscala(const QString &value)
{
    if (value == "S")
        return IndEscala::EscalaRelevante;
    else if (value == "N")
        return IndEscala::EscalaNaoRelevante;
    else
        return IndEscala::None;
}

QString ConvNF::tpViaTranspToStr(const TpViaTransp &value)
{
    if (value == TpViaTransp::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpViaTransp ConvNF::strToTpViaTransp(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3","4","5","6","7", "8", "9", "10", "11", "12", "13"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpViaTransp::None;
    else
        return static_cast<TpViaTransp>(_r[_i].toInt());
}

QString ConvNF::tpIntermedioToStr(const TpIntermedio &value)
{
    if (value == TpIntermedio::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpIntermedio ConvNF::strToTpIntermedio(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpIntermedio::None;
    else
        return static_cast<TpIntermedio>(_r[_i].toInt());
}

QString ConvNF::tpOpToStr(const TpOp &value)
{
    if (value == TpOp::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpOp ConvNF::strToTpOp(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpOp::None;
    else
        return static_cast<TpOp>(_r[_i].toInt());
}

QString ConvNF::condVeicToStr(const CondVeic &value)
{
    if (value == CondVeic::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

CondVeic ConvNF::strToCondVeic(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CondVeic::None;
    else
        return static_cast<CondVeic>(_r[_i].toInt());
}

QString ConvNF::tpRestToStr(const TpRest &value)
{
    if (value == TpRest::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpRest ConvNF::strToTpRest(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpRest::None;
    else
        return static_cast<TpRest>(_r[_i].toInt());

}

QString ConvNF::tpArmaToStr(const TpArma &value)
{
    if (value == TpArma::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpArma ConvNF::strToTpArma(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpArma::None;
    else
        return static_cast<TpArma>(_r[_i].toInt());
}

QString ConvNF::origToStr(const Orig &value)
{
    if (value == Orig::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

Orig ConvNF::strToOrig(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","5","6","7","8"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return Orig::None;
    else
        return static_cast<Orig>(_r[_i].toInt());
}

QString ConvNF::cstICMSToStr(const CstICMS &value)
{
    /*
     * Obs: Como alguns CST são alfanumérico, a conversão de CST para STR do ICMS
     * será diferente. Não tem como converter para inteiro como nos demais. Nesse caso,
     * pegará o número da posição para fazer o cast. Caso seja None já retorna
     * string vazio, caso não, fará o cast pegando a posição a partir do zero sem a preseça
     * do -1 na QList, se não, ficaria errado.
     *
     * Obs2: Os demais serão no mesmo padrão anterior, já que são apenas números.
    */
    if (value == CstICMS::None)
        return QString();
    else
    {
        QList<QString>_r = {"00","10","20","30","40","41","50","51","60",
                           "70","90","Part10", "Part90", "ST41", "ST60"};
        return _r[static_cast<int>(value)];
    }
}

CstICMS ConvNF::strToCstICMS(const QString &value)
{
    QList<QString>_r = {"-1","00","10","20","30","40","41","50","51","60",
                            "70","90","Part10", "Part90", "ST41", "ST60"};

    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CstICMS::None;
    else
        return static_cast<CstICMS>(_r[_i].toInt());
}

QString ConvNF::csosnICMSToStr(const CsosnICMS &value)
{
    if (value == CsosnICMS::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

CsosnICMS ConvNF::strToCsosnICMS(const QString &value)
{
    QList<QString>_r = {"-1","101","102","103","201","202","203","300","400","500","900"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CsosnICMS::None;
    else
        return static_cast<CsosnICMS>(_r[_i].toInt());
}

QString ConvNF::modBCToStr(const ModBC &value)
{
    if (value == ModBC::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

ModBC ConvNF::strToModBC(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return ModBC::None;
    else
        return static_cast<ModBC>(_r[_i].toInt());
}

QString ConvNF::modBCSTToStr(const ModBCST &value)
{
    if (value == ModBCST::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

ModBCST ConvNF::strToModBCST(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","5","6"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return ModBCST::None;
    else
        return static_cast<ModBCST>(_r[_i].toInt());
}

QString ConvNF::motDesICMSToStr(const MotDesICMS &value)
{
    if (value == MotDesICMS::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

MotDesICMS ConvNF::strToMotDesICMS(const QString &value)
{
    QList<QString> _r = {"-1","1","3","4","5","6","7","8","9","10","11", "12","16","90"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return MotDesICMS::None;
    else
        return static_cast<MotDesICMS>(_r[_i].toInt());
}

QString ConvNF::cstIPIToStr(const CstIPI &value)
{
    //Esse deve retornar o string com o zero a esquerda, se menor que 10
    if (value == CstIPI::None)
        return QString();
    else
        return QString(QString("%1%2").arg("0", QString::number(static_cast<int>(value)))).right(2);

}

CstIPI ConvNF::strToCstIPI(const QString &value)
{
    QList<QString>_r = {"-1","00","01","02","03","04","05","49",
                           "50","51","52","53","54","55","99"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CstIPI::None;
    else
        return static_cast<CstIPI>(_r[_i].toInt());
}

QString ConvNF::cstPISToStr(const CstPIS &value)
{
    //Esse deve retornar o string com o zero a esquerda, se menor que 10
    if (value == CstPIS::None)
        return QString();
    else
        return QString(QString("%1%2").arg("0", QString::number(static_cast<int>(value)))).right(2);
}

CstPIS ConvNF::strToCstPIS(const QString &value)
{
    QList<QString>_r = {"-1","01","02","03","04","05","06","07","08","09","49","50",
                            "51","52","53","54","55","56","60","61","62","63","64",
                            "65","66","67","70","71","72","73","74","75","98","99"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CstPIS::None;
    else
        return static_cast<CstPIS>(_r[_i].toInt());
}

QString ConvNF::cstCOFINSToStr(const CstCOFINS &value)
{
    //Esse deve retornar o string com o zero a esquerda, se menor que 10
    if (value == CstCOFINS::None)
        return QString();
    else
        return QString(QString("%1%2").arg("0", QString::number(static_cast<int>(value)))).right(2);
}

CstCOFINS ConvNF::strToCstCOFINS(const QString &value)
{
    QList<QString>_r = {"-1","01","02","03","04","05","06","07","08","09","49","50",
                            "51","52","53","54","55","56","60","61","62","63","64",
                            "65","66","67","70","71","72","73","74","75","98","99"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CstCOFINS::None;
    else
        return static_cast<CstCOFINS>(_r[_i].toInt());
}

QString ConvNF::indISSToStr(const IndISS &value)
{
    if (value == IndISS::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndISS ConvNF::strToIndISS(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3","4","5","6","7"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndISS::None;
    else
        return static_cast<IndISS>(_r[_i].toInt());
}

QString ConvNF::indIncentivoToStr(const IndIncentivo &value)
{
    if (value == IndIncentivo::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndIncentivo ConvNF::strToIndIncentivo(const QString &value)
{
    QList<QString> _r = {"-1","1","2"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndIncentivo::None;
    else
        return static_cast<IndIncentivo>(_r[_i].toInt());
}

QString ConvNF::cRegTribToStr(const CRegTrib &value)
{
    if (value == CRegTrib::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

CRegTrib ConvNF::strToCRegTrib(const QString &value)
{
    QList<QString> _r = {"-1","1","2","3","4","5","6"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return CRegTrib::None;
    else
        return static_cast<CRegTrib>(_r[_i].toInt());
}

QString ConvNF::modFreteToStr(const ModFrete &value)
{
    if (value == ModFrete::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

ModFrete ConvNF::strToModFrete(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","4","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return ModFrete::None;
    else
        return static_cast<ModFrete>(_r[_i].toInt());
}

QString ConvNF::indPagToStr(const IndPag &value)
{
    if (value == IndPag::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndPag ConvNF::strToIndPag(const QString &value)
{
    QList<QString> _r = {"-1","0","1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndPag::None;
    else
        return static_cast<IndPag>(_r[_i].toInt());
}

QString ConvNF::tPagToStr(const TPag &value)
{
    //Esse deve retornar o string com o zero a esquerda, se menor que 10
    if (value == TPag::None)
        return QString();
    else
        return QString(QString("%1%2").arg("0", QString::number(static_cast<int>(value)))).right(2);
}

TPag ConvNF::strToTPag(const QString &value)
{
    QList<QString> _r = {"-1","01","02","03","04","05","10","11","12",
                         "13","14","15","16","17","18","19","90","99"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TPag::None;
    else
        return static_cast<TPag>(_r[_i].toInt());
}

QString ConvNF::tpIntegraToStr(const TpIntegra &value)
{
    if (value == TpIntegra::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpIntegra ConvNF::strToTpIntegra(const QString &value)
{
    QList<QString> _r = {"-1","1","2"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpIntegra::None;
    else
        return static_cast<TpIntegra>(_r[_i].toInt());
}

QString ConvNF::tBandToStr(const TBand &value)
{
    //Esse deve retornar o string com o zero a esquerda, se menor que 10
    if (value == TBand::None)
        return QString();
    else
        return QString(QString("%1%2").arg("0", QString::number(static_cast<int>(value)))).right(2);
}

TBand ConvNF::strToTBand(const QString &value)
{
    QList<QString> _r = {"-1","01","02","03","04","05","06","07","08","09","10",
                         "11","12","13","14","15","16","17","18","19","20","21",
                         "22","23","24","25","26","27","99"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TBand::None;
    else
        return static_cast<TBand>(_r[_i].toInt());
}

QString ConvNF::indProcToStr(const IndProc &value)
{
    if (value == IndProc::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

IndProc ConvNF::strToIndProc(const QString &value)
{
    QList<QString> _r = {"-1","0","1","2","3","9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return IndProc::None;
    else
        return static_cast<IndProc>(_r[_i].toInt());
}

QString ConvNF::tpAtoToStr(const TpAto &value)
{
    if (value == TpAto::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpAto ConvNF::strToTpAto(const QString &value)
{
    QList<QString> _r = {"-1","8","10","12"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpAto::None;
    else
        return static_cast<TpAto>(_r[_i].toInt());
}

QString ConvNF::tpEventoToStr(const TpEvento &value)
{
    if(value == TpEvento::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));
}

TpEvento ConvNF::strToTpEvento(const QString &value)
{
    QList<QString> _r = {"-1", "110110", "110111", "110112", "110130", "110131", "110140", "111500",
                         "111501", "111502", "111503", "110150", "210200", "210210", "210220", "210240"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpEvento::None;
    else
        return static_cast<TpEvento>(_r[_i].toInt());

}

QString ConvNF::tpAutorToStr(const TpAutor &value)
{
    if(value == TpAutor::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));

}

TpAutor ConvNF::strToTpAutor(const QString &value)
{
    QList<QString> _r = {"-1", "1", "2", "3", "5", "6", "9"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpAutor::None;
    else
        return static_cast<TpAutor>(_r[_i].toInt());

}

QString ConvNF::tpAutorizacaoToStr(const TpAutorizacao &value)
{
    if(value == TpAutorizacao::None)
        return QString();
    else
        return QString::number(static_cast<int>(value));

}

TpAutorizacao ConvNF::strToTpAutorizacao(const QString &value)
{
    QList<QString> _r = {"-1", "0", "1"};
    int _i = _r.indexOf(value);
    if (_i == -1 || value.isEmpty())
        return TpAutorizacao::None;
    else
        return static_cast<TpAutorizacao>(_r[_i].toInt());

}
