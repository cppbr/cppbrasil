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


#include "cppdanfebrreport.h"

#include <QDebug>
#include <QFile>
#include <brreportcore.h>
#include <brprinting.h>

namespace {

QFont fonte(const double &pt, const bool &negrito = false)
{
    QFont _f(QStringLiteral("Arial"));
    _f.setPointSizeF(pt);
    _f.setBold(negrito);
    return _f;
}

//----------formatação

QString dinheiro(const double &v)
{
    return CppUtility::formatMoney(v, 2);
}

//campos de exibição condicionada: sem valor no XML, ficam em branco.
QString dinheiroSeInformado(const double &v)
{
    return v > 0 ? dinheiro(v) : QString();
}

//número com no mínimo minDec e no máximo maxDec casas decimais.
QString numero(const double &v, const int &minDec, const int &maxDec)
{
    QString _s = CppUtility::formatMoney(v, maxDec);
    for (int _i = maxDec; _i > minDec && _s.endsWith(QLatin1Char('0')); --_i)
        _s.chop(1);
    if (_s.endsWith(QLatin1Char(',')))
        _s.chop(1);
    return _s;
}

QString percentual(const double &v)
{
    return numero(v, 2, 4) + QLatin1Char('%');
}

QString cep(const int &v)
{
    if (v <= 0)
        return QString();
    const QString _s = CppUtility::insZeroLeft(QString::number(v), 8);
    return _s.left(5) + QLatin1Char('-') + _s.mid(5);
}

QString telefone(const QString &v)
{
    QString _d;
    for (const QChar &_c : v)
        if (_c.isDigit())
            _d += _c;
    if (_d.length() == 10)
        return QStringLiteral("(%1) %2-%3").arg(_d.left(2), _d.mid(2, 4), _d.mid(6));
    if (_d.length() == 11)
        return QStringLiteral("(%1) %2-%3").arg(_d.left(2), _d.mid(2, 5), _d.mid(7));
    return v;
}

QString cnpjCpf(const QString &cnpj, const QString &cpf)
{
    if (!cnpj.isEmpty())
        return CppUtility::formatCNPJ(cnpj);
    if (!cpf.isEmpty())
        return CppUtility::formatCPF(cpf);
    return QString();
}

QString numeroEndereco(const int &nro)
{
    return nro > 0 ? QString::number(nro) : QStringLiteral("S/N");
}

QString crt(const Crt &v)
{
    switch (v)
    {
    case Crt::SimplesNacional: return QStringLiteral("1 - SIMPLES NACIONAL");
    case Crt::SimplesExcesso: return QStringLiteral("2 - SIMPLES NACIONAL, EXCESSO DE SUBLIMITE DE RECEITA BRUTA");
    case Crt::RegimeNormal: return QStringLiteral("3 - REGIME NORMAL");
    case Crt::SimplesMEI: return QStringLiteral("4 - SIMPLES NACIONAL - MEI");
    default: return QString();
    }
}

QString freteParaTexto(const ModFrete &v)
{
    switch (v)
    {
    case ModFrete::PorContaRemetente: return QStringLiteral("0 - Remetente (CIF)");
    case ModFrete::PorContaDestinatario: return QStringLiteral("1 - Destinatário (FOB)");
    case ModFrete::PorContaTerceiros: return QStringLiteral("2 - Terceiros");
    case ModFrete::ProprioContaRemetente: return QStringLiteral("3 - Próprio, remetente");
    case ModFrete::ProprioContaDestinatario: return QStringLiteral("4 - Próprio, destinatário");
    case ModFrete::SemFrete: return QStringLiteral("9 - Sem transporte");
    default: return QString();
    }
}

//o grupo gRed existe quando há redução de alíquota ou compra governamental.
template<class T>
double aliquotaImpressa(const T *grupo, const double &aliquota)
{
    const bool _reducao = grupo->gRed->get_pRedAliq() > 0 || grupo->gRed->get_pAliqEfet() > 0;
    return _reducao ? grupo->gRed->get_pAliqEfet() : aliquota;
}

template<class T>
QVariantMap localParaRegistro(const QString &titulo, const T *local)
{
    QString _endereco = local->get_xLgr() + QStringLiteral(", ") + numeroEndereco(local->get_nro());
    if (!local->get_xCpl().isEmpty())
        _endereco += QStringLiteral(" - ") + local->get_xCpl();

    QVariantMap _r;
    _r.insert(QStringLiteral("local_titulo"), titulo);
    _r.insert(QStringLiteral("local_xNome"), local->get_xNome());
    _r.insert(QStringLiteral("local_doc"), cnpjCpf(local->get_CNPJ(), local->get_CPF()));
    _r.insert(QStringLiteral("local_IE"), local->get_IE());
    _r.insert(QStringLiteral("local_endereco"), _endereco);
    _r.insert(QStringLiteral("local_bairro"), local->get_xBairro());
    _r.insert(QStringLiteral("local_CEP"), cep(local->get_CEP()));
    _r.insert(QStringLiteral("local_municipio"), local->get_xMun());
    _r.insert(QStringLiteral("local_UF"), local->get_UF());
    _r.insert(QStringLiteral("local_fone"), telefone(local->get_fone()));
    return _r;
}

} // namespace

CppDanfeBrReport::CppDanfeBrReport(const CppNFe *cppnfe, const int &recNo): CppDanfe(cppnfe, recNo),
    m_dsItens(new BrListDataSource), m_dsDuplicatas(new BrListDataSource),
    m_dsVolumes(new BrListDataSource), m_dsLocais(new BrListDataSource)
{
}

CppDanfeBrReport::CppDanfeBrReport(const CppNFe &cppnfe, const int &recNo): CppDanfeBrReport(&cppnfe, recNo)
{
}

CppDanfeBrReport::~CppDanfeBrReport()
{
}

bool CppDanfeBrReport::carregarLeiaute(BrReport &report)
{
    const QString &_arquivo = this->m_pathArquivo;
    if (_arquivo.isEmpty())
    {
        this->m_erro = QStringLiteral("Informe o modelo .brt do DANFE em caminhoArquivo().");
        return false;
    }
    if (!QFile::exists(_arquivo))
    {
        this->m_erro = QStringLiteral("Modelo do DANFE não encontrado: %1").arg(_arquivo);
        return false;
    }
    if (!report.load(_arquivo))
    {
        this->m_erro = QStringLiteral("Não foi possível carregar o modelo %1: %2")
                           .arg(_arquivo, report.errorString());
        return false;
    }
    return true;
}

void CppDanfeBrReport::caminhoArquivo(const QString &arquivo)
{
    this->m_pathArquivo = arquivo;
}

void CppDanfeBrReport::caminhoLogo(const QString &logo)
{
    this->m_pathLogo = logo;
}

void CppDanfeBrReport::exibirCanhoto(const bool &exibir)
{
    this->m_exibirCanhoto = exibir;
}

QString CppDanfeBrReport::erro() const
{
    return this->m_erro;
}

void CppDanfeBrReport::print()
{
    if (preparar())
        BrPrinting::preview(m_report.get());
    else
        qWarning() << "CppDanfeBrReport:" << this->m_erro;
}

bool CppDanfeBrReport::imprimir(const bool &exibirDialogo)
{
    if (!preparar())
        return false;
    return BrPrinting::print(m_report.get(), nullptr, exibirDialogo);
}

bool CppDanfeBrReport::exportarPDF(const QString &arquivo)
{
    if (!preparar())
        return false;
    if (!m_report->exportToPdf(arquivo))
    {
        this->m_erro = QStringLiteral("Não foi possível gravar o arquivo %1").arg(arquivo);
        return false;
    }
    return true;
}

bool CppDanfeBrReport::preparar()
{
    this->m_erro.clear();
    if (this->m_mod != ModeloDF::NFe)
    {
        this->m_erro = QStringLiteral("O modelo do DANFE com o BrReport atende somente a NF-e, modelo 55.");
        return false;
    }

    carregarDados();

    m_report.reset(new BrReport);
    if (!carregarLeiaute(*m_report))
        return false;

    vincular(*m_report);

    if (!m_report->prepare())
    {
        this->m_erro = m_report->errorString();
        return false;
    }
    return true;
}

void CppDanfeBrReport::carregarDados()
{
    const InfNFe *_inf = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe.get();

    //----------itens
    QList<QVariantMap> _itens;
    for (int _i = 0; _i < _inf->det->count(); ++_i)
    {
        const Det *_det = _inf->det->value(_i);
        const Prod *_prod = _det->prod.get();
        const Imposto *_imp = _det->imposto.get();
        const Icms *_icms = _imp->ICMS.get();
        const Ipi *_ipi = _imp->IPI.get();
        const Is *_is = _imp->IS.get();
        const IbsCbs *_ibscbs = _imp->IBSCBS.get();
        const GIBSCBS *_g = _ibscbs->gIBSCBS.get();

        //descrição, informações adicionais do item, código, NCM e classificação tributária.
        QString _descricao = _prod->get_xProd();
        if (!_det->get_infAdProd().isEmpty())
            _descricao += QLatin1Char('\n') + _det->get_infAdProd();
        QStringList _codigos;
        if (!_prod->get_cProd().isEmpty())
            _codigos << QStringLiteral("[Cód. %1]").arg(_prod->get_cProd());
        if (!_prod->get_NCM().isEmpty())
            _codigos << QStringLiteral("[NCM %1]").arg(_prod->get_NCM());
        if (!_ibscbs->get_cClassTrib().isEmpty())
            _codigos << QStringLiteral("[cClassTrib %1]").arg(_ibscbs->get_cClassTrib());
        if (!_codigos.isEmpty())
            _descricao += QLatin1Char('\n') + _codigos.join(QLatin1Char(' '));

        //CST: origem + CST do ICMS ou CSOSN.
        const bool _temICMS = _icms->get_CST() != CstICMS::None || _icms->get_CSOSN() != CsosnICMS::None;
        QString _cst = ConvNF::origToStr(_icms->get_orig());
        if (_icms->get_CST() != CstICMS::None)
            _cst += ConvNF::cstICMSToStr(_icms->get_CST());
        else
            _cst += ConvNF::csosnICMSToStr(_icms->get_CSOSN());

        //sem o grupo no XML, as linhas do tributo ficam em branco.
        const bool _temIPI = _ipi->get_CST() != CstIPI::None;
        const bool _temIS = !_is->get_CSTIS().isEmpty();
        const bool _temIBSCBS = !_ibscbs->get_CST().isEmpty()
                                && (_g->get_vBC() > 0 || _g->get_vIBS() > 0 || _g->gCBS->get_vCBS() > 0
                                    || _g->gIBSUF->get_pIBSUF() > 0 || _g->gIBSMun->get_pIBSMun() > 0
                                    || _g->gCBS->get_pCBS() > 0);

        QString _bICMS, _pICMS, _vICMS, _bIPI, _pIPI, _vIPI, _bIS, _pIS, _vIS;
        QString _bIBSCBS, _pIBSUF, _vIBSUF, _pIBSMun, _vIBSMun, _pCBS, _vCBS;
        if (_temICMS)
        {
            _bICMS = dinheiro(_icms->get_vBC());
            _pICMS = percentual(_icms->get_pICMS());
            _vICMS = dinheiro(_icms->get_vICMS());
        }
        if (_temIPI)
        {
            _bIPI = dinheiro(_ipi->get_vBC());
            _pIPI = percentual(_ipi->get_pIPI());
            _vIPI = dinheiro(_ipi->get_vIPI());
        }
        if (_temIS)
        {
            _bIS = dinheiro(_is->get_vBCIS());
            _pIS = percentual(_is->get_pIS());
            _vIS = dinheiro(_is->get_vIS());
        }
        if (_temIBSCBS)
        {
            //com redução de alíquota ou compra governamental, imprime a alíquota efetiva.
            _bIBSCBS = dinheiro(_g->get_vBC());
            _pIBSUF = percentual(aliquotaImpressa(_g->gIBSUF.get(), _g->gIBSUF->get_pIBSUF()));
            _vIBSUF = dinheiro(_g->gIBSUF->get_vIBSUF());
            _pIBSMun = percentual(aliquotaImpressa(_g->gIBSMun.get(), _g->gIBSMun->get_pIBSMun()));
            _vIBSMun = dinheiro(_g->gIBSMun->get_vIBSMun());
            _pCBS = percentual(aliquotaImpressa(_g->gCBS.get(), _g->gCBS->get_pCBS()));
            _vCBS = dinheiro(_g->gCBS->get_vCBS());
        }

        const QLatin1Char _nl('\n');
        QVariantMap _r;
        _r.insert(QStringLiteral("prod_nItem"), _prod->get_nItem());
        _r.insert(QStringLiteral("prod_descricao"), _descricao);
        _r.insert(QStringLiteral("prod_cst_cfop"), _cst + _nl + _prod->get_CFOP());
        _r.insert(QStringLiteral("prod_qtd"), numero(_prod->get_qCom(), 2, 4) + _nl + _prod->get_uCom());
        _r.insert(QStringLiteral("prod_vUnCom"), numero(_prod->get_vUnCom(), 2, 6));
        _r.insert(QStringLiteral("prod_vProd"), dinheiro(_prod->get_vProd()));
        _r.insert(QStringLiteral("prod_bases"), _bICMS + _nl + _bIBSCBS + _nl + _bIS + _nl + _bIPI);
        _r.insert(QStringLiteral("prod_aliq_esq"), _pICMS + _nl + _pIBSUF + _nl + _pIBSMun);
        _r.insert(QStringLiteral("prod_aliq_dir"), _pCBS + _nl + _pIPI + _nl + _pIS);
        _r.insert(QStringLiteral("prod_valor_esq"), _vICMS + _nl + _vIBSUF + _nl + _vIBSMun);
        _r.insert(QStringLiteral("prod_valor_dir"), _vCBS + _nl + _vIPI + _nl + _vIS);
        _itens.append(_r);
    }
    m_dsItens->setRows(_itens);

    //----------fatura e duplicatas
    QList<QVariantMap> _duplicatas;
    for (int _i = 0; _i < _inf->cobr->dup->count(); ++_i)
    {
        const Dup *_dup = _inf->cobr->dup->value(_i);
        QVariantMap _r;
        _r.insert(QStringLiteral("dup_numero"), _dup->get_nDup());
        _r.insert(QStringLiteral("dup_vencimento"), _dup->get_dVenc().toString(QStringLiteral("dd/MM/yyyy")));
        _r.insert(QStringLiteral("dup_valor"), dinheiro(_dup->get_vDup()));
        _duplicatas.append(_r);
    }
    //fatura sem duplicatas: imprime a fatura com o valor líquido.
    if (_duplicatas.isEmpty()
        && (!_inf->cobr->fat->get_nFat().isEmpty() || _inf->cobr->fat->get_vLiq() > 0))
    {
        QVariantMap _r;
        _r.insert(QStringLiteral("dup_numero"), _inf->cobr->fat->get_nFat());
        _r.insert(QStringLiteral("dup_vencimento"), QString());
        _r.insert(QStringLiteral("dup_valor"), dinheiro(_inf->cobr->fat->get_vLiq()));
        _duplicatas.append(_r);
    }
    m_dsDuplicatas->setRows(_duplicatas);

    //----------volumes
    QList<QVariantMap> _volumes;
    for (int _i = 0; _i < _inf->transp->vol->count(); ++_i)
    {
        const Vol *_vol = _inf->transp->vol->value(_i);
        QVariantMap _r;
        _r.insert(QStringLiteral("vol_qVol"), _vol->get_qVol() > 0 ? numero(_vol->get_qVol(), 0, 4) : QString());
        _r.insert(QStringLiteral("vol_esp"), _vol->get_esp());
        _r.insert(QStringLiteral("vol_marca"), _vol->get_marca());
        _r.insert(QStringLiteral("vol_nVol"), _vol->get_nVol());
        _r.insert(QStringLiteral("vol_pesoB"), _vol->get_pesoB() > 0
                  ? numero(_vol->get_pesoB(), 3, 3) + QStringLiteral(" kg") : QString());
        _r.insert(QStringLiteral("vol_pesoL"), _vol->get_pesoL() > 0
                  ? numero(_vol->get_pesoL(), 3, 3) + QStringLiteral(" kg") : QString());
        _volumes.append(_r);
    }
    m_dsVolumes->setRows(_volumes);

    //----------local de retirada e local de entrega
    QList<QVariantMap> _locais;
    if (!_inf->retirada->get_xNome().isEmpty() || !_inf->retirada->get_xLgr().isEmpty())
        _locais.append(localParaRegistro(QStringLiteral("INFORMAÇÕES DO LOCAL DE RETIRADA"),
                                         _inf->retirada.get()));
    if (!_inf->entrega->get_xNome().isEmpty() || !_inf->entrega->get_xLgr().isEmpty())
        _locais.append(localParaRegistro(QStringLiteral("INFORMAÇÕES DO LOCAL DE ENTREGA"),
                                         _inf->entrega.get()));
    m_dsLocais->setRows(_locais);
}

QVariantMap CppDanfeBrReport::variaveis() const
{
    const Nfe *_nfe = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo);
    const InfNFe *_inf = _nfe->infNFe.get();
    const Ide *_ide = _inf->ide.get();
    const Emit *_emit = _inf->emite.get();
    const EnderEmit *_endEmit = _emit->enderEmit.get();
    const Dest *_dest = _inf->dest.get();
    const EnderDest *_endDest = _dest->enderDest.get();
    const IcmsTot *_tot = _inf->total->ICMSTot.get();
    const IssQNtot *_iss = _inf->total->ISSQNtot.get();
    const IbsCbsTot *_ibscbs = _inf->total->IBSCBSTot.get();
    const Transp *_transp = _inf->transp.get();
    const bool _contingenciaFS = this->m_tpEmis == TpEmis::Contingencia_FSIA
                                 || this->m_tpEmis == TpEmis::Contingencia_FSDA;

    QVariantMap _v;

    //----------identificação
    _v.insert(QStringLiteral("ide_natOp"), _ide->get_natOp());
    _v.insert(QStringLiteral("ide_tpNF"), ConvNF::tpNFToStr(_ide->get_tpNF()));
    _v.insert(QStringLiteral("ide_numero"), QStringLiteral("Nº ") + this->m_nNF);
    _v.insert(QStringLiteral("ide_serie"), QStringLiteral("SÉRIE ") + this->m_serie);
    _v.insert(QStringLiteral("ide_dEmi"), _ide->get_dhEmi().toString(QStringLiteral("dd/MM/yyyy")));
    _v.insert(QStringLiteral("ide_dSaiEnt"), _ide->get_dhSaiEnt().toString(QStringLiteral("dd/MM/yyyy")));
    _v.insert(QStringLiteral("ide_hSaiEnt"), _ide->get_dhSaiEnt().toString(QStringLiteral("hh:mm:ss")));
    _v.insert(QStringLiteral("chave"), this->m_chave);
    _v.insert(QStringLiteral("consulta"),
              QStringLiteral("Consulte a autenticidade no portal nacional da NF-e "
                             "www.nfe.fazenda.gov.br/portal ou no site da SEFAZ autorizadora"));
    _v.insert(QStringLiteral("tem_contingencia_fs"), _contingenciaFS);

    //----------protocolo
    QString _protocoloTitulo = QStringLiteral("PROTOCOLO DE AUTORIZAÇÃO DE USO");
    QString _protocolo;
    if (_contingenciaFS)
    {
        _protocoloTitulo = QStringLiteral("DADOS DA NF-E");
        _protocolo = this->m_chaveCont;
    }
    else
    {
        if (this->m_tpEmis == TpEmis::Contingencia_EPEC)
            _protocoloTitulo = QStringLiteral("PROTOCOLO DE AUTORIZAÇÃO DO EPEC");
        if (!_nfe->protNFe->get_nProt().isEmpty())
            _protocolo = _nfe->protNFe->get_nProt() + QStringLiteral(" - ")
                         + _nfe->protNFe->get_dhRecbto().toString(QStringLiteral("dd/MM/yyyy hh:mm:ss"));
    }
    _v.insert(QStringLiteral("protocolo_titulo"), _protocoloTitulo);
    _v.insert(QStringLiteral("protocolo"), _protocolo);

    //----------emitente
    QString _endereco = _endEmit->get_xLgr() + QStringLiteral(", ") + numeroEndereco(_endEmit->get_nro());
    if (!_endEmit->get_xCpl().isEmpty())
        _endereco += QStringLiteral(" - ") + _endEmit->get_xCpl();
    _endereco += QStringLiteral(" - ") + _endEmit->get_xBairro() + QLatin1Char('\n')
                 + _endEmit->get_xMun() + QLatin1Char('/') + _endEmit->get_UF()
                 + QStringLiteral(" - CEP ") + cep(_endEmit->get_CEP());
    if (!_endEmit->get_fone().isEmpty())
        _endereco += QStringLiteral("\nFone: ") + telefone(_endEmit->get_fone());

    _v.insert(QStringLiteral("emit_xNome"), _emit->get_xNome());
    _v.insert(QStringLiteral("emit_endereco"), _endereco);
    _v.insert(QStringLiteral("emit_IE"), _emit->get_IE());
    _v.insert(QStringLiteral("emit_IEST"), _emit->get_IEST());
    _v.insert(QStringLiteral("emit_IM"), _emit->get_IM());
    _v.insert(QStringLiteral("emit_doc"), cnpjCpf(_emit->get_CNPJ(), _emit->get_CPF()));
    _v.insert(QStringLiteral("emit_CRT"), crt(_emit->get_CRT()));
    //campo reservado pela NT 2026.010: não imprimir conteúdo até a publicação da origem da informação.
    _v.insert(QStringLiteral("emit_regimeIBSCBS"), QString());
    _v.insert(QStringLiteral("tem_logo"),
              !this->m_pathLogo.isEmpty() && CppUtility::fileExists(this->m_pathLogo));

    //----------canhoto
    _v.insert(QStringLiteral("tem_canhoto"), this->m_exibirCanhoto);
    _v.insert(QStringLiteral("canhoto_texto"),
              QStringLiteral("RECEBEMOS DE %1 OS PRODUTOS/SERVIÇOS CONSTANTES DA NOTA FISCAL ELETRÔNICA "
                             "INDICADA AO LADO").arg(_emit->get_xNome()));

    //----------destinatário
    QString _docDest = cnpjCpf(_dest->get_CNPJ(), _dest->get_CPF());
    if (_docDest.isEmpty())
        _docDest = _dest->get_idEstrangeiro();
    QString _endDestino = _endDest->get_xLgr();
    if (!_endDestino.isEmpty())
    {
        _endDestino += QStringLiteral(", ") + numeroEndereco(_endDest->get_nro());
        if (!_endDest->get_xCpl().isEmpty())
            _endDestino += QStringLiteral(" - ") + _endDest->get_xCpl();
    }
    _v.insert(QStringLiteral("dest_xNome"), _dest->get_xNome());
    _v.insert(QStringLiteral("dest_doc"), _docDest);
    _v.insert(QStringLiteral("dest_endereco"), _endDestino);
    _v.insert(QStringLiteral("dest_bairro"), _endDest->get_xBairro());
    _v.insert(QStringLiteral("dest_CEP"), cep(_endDest->get_CEP()));
    _v.insert(QStringLiteral("dest_municipio"), _endDest->get_xMun());
    _v.insert(QStringLiteral("dest_fone"), telefone(_endDest->get_fone()));
    _v.insert(QStringLiteral("dest_UF"), _endDest->get_UF());
    _v.insert(QStringLiteral("dest_IE"), _dest->get_IE());

    //----------totais
    _v.insert(QStringLiteral("tot_vProd"), dinheiro(_tot->get_vProd()));
    _v.insert(QStringLiteral("tot_vFrete"), dinheiro(_tot->get_vFrete()));
    _v.insert(QStringLiteral("tot_vSeg"), dinheiro(_tot->get_vSeg()));
    _v.insert(QStringLiteral("tot_vDesc"), dinheiro(_tot->get_vDesc()));
    _v.insert(QStringLiteral("tot_vOutro"), dinheiro(_tot->get_vOutro()));
    _v.insert(QStringLiteral("tot_vNF"), dinheiro(_tot->get_vNF()));
    _v.insert(QStringLiteral("tot_vBC"), dinheiro(_tot->get_vBC()));
    _v.insert(QStringLiteral("tot_vICMS"), dinheiro(_tot->get_vICMS()));
    _v.insert(QStringLiteral("tot_vBCST"), dinheiro(_tot->get_vBCST()));
    _v.insert(QStringLiteral("tot_vST"), dinheiro(_tot->get_vST()));
    _v.insert(QStringLiteral("tot_vIPI"), dinheiro(_tot->get_vIPI()));

    //campos de exibição condicionada à existência da informação no XML.
    _v.insert(QStringLiteral("tot_vFCP"), dinheiroSeInformado(_tot->get_vFCP()));
    _v.insert(QStringLiteral("tot_vFCPST"), dinheiroSeInformado(_tot->get_vFCPST()));
    _v.insert(QStringLiteral("tot_vICMSUFDest"), dinheiroSeInformado(_tot->get_vICMSUFDest()));
    _v.insert(QStringLiteral("tot_vFCPUFDest"), dinheiroSeInformado(_tot->get_vFCPUFDest()));
    _v.insert(QStringLiteral("tem_fcp_difal"), _tot->get_vFCP() > 0 || _tot->get_vFCPST() > 0
              || _tot->get_vICMSUFDest() > 0 || _tot->get_vFCPUFDest() > 0);

    _v.insert(QStringLiteral("tot_qBCMono"), dinheiroSeInformado(_tot->get_qBCMono()));
    _v.insert(QStringLiteral("tot_vICMSMono"), dinheiroSeInformado(_tot->get_vICMSMono()));
    _v.insert(QStringLiteral("tot_qBCMonoReten"), dinheiroSeInformado(_tot->get_qBCMonoReten()));
    _v.insert(QStringLiteral("tot_vICMSMonoReten"), dinheiroSeInformado(_tot->get_vICMSMonoReten()));
    _v.insert(QStringLiteral("tem_icms_mono"), _tot->get_qBCMono() > 0 || _tot->get_vICMSMono() > 0
              || _tot->get_qBCMonoReten() > 0 || _tot->get_vICMSMonoReten() > 0);

    //IBS, CBS e IS: o quadro é fixo e os valores saem quando o grupo de totais existe no XML.
    const double _vCBS = _ibscbs->gCBS->get_vCBS();
    const double _vIBSUF = _ibscbs->gIBS->gIBSUF->get_vIBSUF();
    const double _vIBSMun = _ibscbs->gIBS->gIBSMun->get_vIBSMun();
    const bool _temIBSCBS = _ibscbs->get_vBCIBSCBS() > 0 || _vCBS > 0 || _vIBSUF > 0 || _vIBSMun > 0;
    _v.insert(QStringLiteral("tot_vCBS"), _temIBSCBS ? dinheiro(_vCBS) : QString());
    _v.insert(QStringLiteral("tot_vIBSUF"), _temIBSCBS ? dinheiro(_vIBSUF) : QString());
    _v.insert(QStringLiteral("tot_vIBSMun"), _temIBSCBS ? dinheiro(_vIBSMun) : QString());
    _v.insert(QStringLiteral("tot_vIS"), dinheiroSeInformado(_inf->total->ISTot->get_vIS()));

    const GMonoTot *_mono = _ibscbs->gMono.get();
    _v.insert(QStringLiteral("tot_vIBSMono"), dinheiroSeInformado(_mono->get_vIBSMono()));
    _v.insert(QStringLiteral("tot_vCBSMono"), dinheiroSeInformado(_mono->get_vCBSMono()));
    _v.insert(QStringLiteral("tot_vIBSMonoReten"), dinheiroSeInformado(_mono->get_vIBSMonoReten()));
    _v.insert(QStringLiteral("tot_vCBSMonoReten"), dinheiroSeInformado(_mono->get_vCBSMonoReten()));
    _v.insert(QStringLiteral("tem_ibscbs_mono"), _mono->get_vIBSMono() > 0 || _mono->get_vCBSMono() > 0
              || _mono->get_vIBSMonoReten() > 0 || _mono->get_vCBSMonoReten() > 0);

    _v.insert(QStringLiteral("tot_vServ"), dinheiro(_iss->get_vServ()));
    _v.insert(QStringLiteral("tot_vBCISS"), dinheiro(_iss->get_vBC()));
    _v.insert(QStringLiteral("tot_vISS"), dinheiro(_iss->get_vISS()));
    _v.insert(QStringLiteral("tem_issqn"), _iss->get_vServ() > 0 || _iss->get_vBC() > 0
              || _iss->get_vISS() > 0);

    //----------transportador
    const bool _temTransp = _transp->get_modFrete() != ModFrete::SemFrete
                            || !_transp->transporta->get_xNome().isEmpty()
                            || !_transp->transporta->get_CNPJ().isEmpty()
                            || !_transp->transporta->get_CPF().isEmpty()
                            || !_transp->veicTransp->get_placa().isEmpty()
                            || _transp->vol->count() > 0;
    _v.insert(QStringLiteral("tem_transp"), _temTransp && _transp->get_modFrete() != ModFrete::None);
    _v.insert(QStringLiteral("transp_xNome"), _transp->transporta->get_xNome());
    _v.insert(QStringLiteral("transp_modFrete"), freteParaTexto(_transp->get_modFrete()));
    _v.insert(QStringLiteral("transp_RNTC"), _transp->veicTransp->get_RNTC());
    _v.insert(QStringLiteral("transp_placa"), _transp->veicTransp->get_placa());
    _v.insert(QStringLiteral("transp_veicUF"), _transp->veicTransp->get_UF());
    _v.insert(QStringLiteral("transp_doc"),
              cnpjCpf(_transp->transporta->get_CNPJ(), _transp->transporta->get_CPF()));
    _v.insert(QStringLiteral("transp_endereco"), _transp->transporta->get_xEnder());
    _v.insert(QStringLiteral("transp_municipio"), _transp->transporta->get_xMun());
    _v.insert(QStringLiteral("transp_UF"), _transp->transporta->get_UF());
    _v.insert(QStringLiteral("transp_IE"), _transp->transporta->get_IE());

    //----------dados adicionais
    _v.insert(QStringLiteral("infCpl"), this->m_infCpl);
    _v.insert(QStringLiteral("tem_qrcode"), !_nfe->infNFeSupl->get_qrCode().trimmed().isEmpty());

    return _v;
}

void CppDanfeBrReport::vincular(BrReport &report)
{
    const Nfe *_nfe = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo);

    //----------fontes de dados
    report.addDataSource(QStringLiteral("itens"), m_dsItens.get());
    report.addDataSource(QStringLiteral("duplicatas"), m_dsDuplicatas.get());
    report.addDataSource(QStringLiteral("volumes"), m_dsVolumes.get());
    report.addDataSource(QStringLiteral("locais"), m_dsLocais.get());

    //----------variáveis
    const QVariantMap _variaveis = variaveis();
    for (auto _it = _variaveis.constBegin(); _it != _variaveis.constEnd(); ++_it)
        report.setVariable(_it.key(), _it.value());

    //o quadro do transportador sempre tem ao menos uma linha de volumes.
    if (_variaveis.value(QStringLiteral("tem_transp")).toBool() && m_dsVolumes->recordCount() == 0)
        m_dsVolumes->addRow({{QStringLiteral("vol_qVol"), QString()}, {QStringLiteral("vol_esp"), QString()},
                             {QStringLiteral("vol_marca"), QString()}, {QStringLiteral("vol_nVol"), QString()},
                             {QStringLiteral("vol_pesoB"), QString()}, {QStringLiteral("vol_pesoL"), QString()}});
    else if (!_variaveis.value(QStringLiteral("tem_transp")).toBool())
        m_dsVolumes->clear();

    //----------objetos preenchidos pelo código
    //chave de acesso: Code 128 com troca automática de subconjunto, para chaves numéricas e alfanuméricas.
    if (BrBarcode *_barras = report.object<BrBarcode>(QStringLiteral("barcodeChave")))
    {
        _barras->setText(_nfe->get_chNFe());
        _barras->setVisible(!_nfe->get_chNFe().isEmpty());
    }
    if (BrBarcode *_barras = report.object<BrBarcode>(QStringLiteral("barcodeContingencia")))
    {
        const QString _dados = QString(this->m_chaveCont).remove(QLatin1Char(' '));
        _barras->setText(_dados);
        _barras->setVisible(!_dados.isEmpty());
    }
    if (BrBarcode *_qr = report.object<BrBarcode>(QStringLiteral("qrCode")))
    {
        const QString _dados = _nfe->infNFeSupl->get_qrCode().trimmed();
        _qr->setText(_dados);
        _qr->setVisible(!_dados.isEmpty());
    }
    if (BrImage *_logo = report.object<BrImage>(QStringLiteral("logo")))
    {
        if (_variaveis.value(QStringLiteral("tem_logo")).toBool())
            _logo->load(this->m_pathLogo);
        else
            _logo->setVisible(false);
    }

    //----------marca d'água
    QString _marca;
    if (this->m_cStat == 101 || this->m_cStat == 151 || this->m_cStat == 155)
        _marca = QStringLiteral("NF-e CANCELADA");
    else if (_nfe->infNFe->ide->get_tpAmb() == TpAmb::Homologacao)
        _marca = QStringLiteral("SEM VALOR FISCAL");
    BrWatermark _wm;
    _wm.text = _marca;
    _wm.font = fonte(56, true);
    report.setWatermark(_wm);

    //----------dados adicionais: só na primeira página
    ajustarDadosAdicionais(report);
    if (BrBand *_rodape = report.object<BrBand>(QStringLiteral("dadosAdicionais")))
    {
        //fora da primeira página a banda fica invisível, para não reservar espaço no rodapé.
        report.setPageHandler([_rodape](int pagina) { _rodape->setVisible(pagina == 1); });
    }
}

//aumenta o quadro de dados adicionais quando o texto não cabe na altura padrão.
void CppDanfeBrReport::ajustarDadosAdicionais(BrReport &report) const
{
    const double _maxExtra = 60.0;
    BrBand *_banda = report.object<BrBand>(QStringLiteral("dadosAdicionais"));
    BrCustomText *_memo = report.object<BrCustomText>(QStringLiteral("infCpl"));
    if (!_banda || !_memo || this->m_infCpl.isEmpty())
        return;

    const double _largura = _memo->width() - 2 * _memo->padding();
    const QStringList _linhas = BrTextMetrics::wrap(_memo->font(), this->m_infCpl, _largura, true);
    const double _necessario = _linhas.size() * BrTextMetrics::lineHeight(_memo->font())
                               + 2 * _memo->padding();
    const double _extra = qMin(_necessario - _memo->height(), _maxExtra);
    if (_extra <= 0)
        return;

    //cresce a banda e os objetos que chegam até a sua base.
    const double _base = _banda->height();
    const QList<BrControl *> _controles = _banda->controls();
    for (BrControl *_c : _controles)
        if (qAbs(_c->y() + _c->height() - _base) < 1.0)
            _c->setHeight(_c->height() + _extra);
    _banda->setHeight(_base + _extra);
}
