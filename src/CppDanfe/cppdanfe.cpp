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


#include "cppdanfe.h"

CppDanfe::CppDanfe(const CppNFe *cppnfe, const int &recNo): m_cppnfe(cppnfe), m_recNo(recNo),
    m_imBarcode(554, 80, QImage::Format_RGB16), m_imBarcodeCont(554, 80, QImage::Format_RGB16)
{
    getParam();
}

CppDanfe::~CppDanfe()
{
}

void CppDanfe::caminhoArquivo(const QString &arquivo)
{
    Q_UNUSED(arquivo);
}

void CppDanfe::caminhoLogo(const QString &logo)
{
    Q_UNUSED(logo);
}

void CppDanfe::print()
{

}

QString CppDanfe::tPagToNome(const TPag &tPag)
{
    switch (tPag) {
        case TPag::None:
            return "Nenhum";
            break;
        case TPag::Dinheiro:
            return "Dinheiro";
            break;
        case TPag::Cheque:
            return "Cheque";
            break;
        case TPag::CartaoCredito:
            return "Cartao de Credito";
            break;
        case TPag::CartaoDebito:
            return "Cartao de Debito";
            break;
        case TPag::CreditoLoja:
            return "Credito da Loja";
            break;
        case TPag::ValeAlimentacao:
            return "Vale Alimentacao";
            break;
        case TPag::ValeRefeicao:
            return "Vale Refeicao";
            break;
        case TPag::ValePresente:
            return "Vale Presente";
            break;
        case TPag::ValeCombustivel:
            return "Vale Combustivel";
            break;
        case TPag::BoletoBancario:
            return "Boleto Bancario";
            break;
        case TPag::DepositoBancario:
            return "Deposito Bancario";
            break;
        case TPag::Pix:
            return "Pix";
            break;
        case TPag::TranferenciaBancaria:
            return "Tranferencia Bancaria";
            break;
        case TPag::ProgramaFidelidade:
            return "Programa de Fidelidade";
            break;
        case TPag::SemPagamento:
            return "Sem Pagamento";
            break;
        case TPag::Outros:
            return "Outros";
            break;
        default:
            return "Nenhum";
            break;
    }
}

void CppDanfe::genBarcode()
{
    CppBarCode _barcode(BarType::Bar128C);
    _barcode.setWidth( 554 );
    _barcode.setHeight( 80 );
    _barcode.setPos(0,0);
    _barcode.setHighDPI(true);
    _barcode.setTextVisible(false);
    //barcode chave
    _barcode.setText(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->get_chNFe());
    _barcode.update();
    QPainter painter(&m_imBarcode);
    m_imBarcode.fill( Qt::white );
    _barcode.paint(&painter,nullptr, nullptr);
    //barcode contingência FS ou FS-DA
    if (this->m_tpEmis == TpEmis::Contingencia_FSIA || this->m_tpEmis == TpEmis::Contingencia_FSDA)
    {
        //cUF, tpEmis, CNPJ
        QString _barCt = QString::number(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_cUF())
                      + ConvNF::tpEmisToStr(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_tpEmis())
                      + this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->emite->get_CNPJ();

        //vNF
        QString _vNF = CppUtility::insZeroLeft(QString::number(qRound64(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->total->ICMSTot->get_vNF() * 100)), 14); //qRound64: inteiro em centavos, sem notacao cientifica
        _barCt += _vNF;

        //ICMSp
        double _vICMS = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->total->ICMSTot->get_vICMS();
        _barCt += (_vICMS > 0 ? "1" : "2");

        //ICMSs
        double _vST = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->total->ICMSTot->get_vST();
        _barCt += (_vST > 0 ? "1" : "2");

        //DD
        QString _dd = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_dhEmi().toString("dd");
        _barCt += _dd;

        //DV
        int _dv = CppUtility::dvModulo11(_barCt);
        _barCt += QString::number(_dv);

        this->m_chaveCont.clear();
        this->m_chaveCont = formatChave(_barCt);
        CppBarCode _barcodeCt(BarType::Bar128C);
        _barcodeCt.setWidth( 466 );
        _barcodeCt.setHeight( 80 );
        _barcodeCt.setPos(0,0);
        _barcodeCt.setHighDPI(true);
        _barcodeCt.setTextVisible(false);
        //barcode chave contingência
        _barcodeCt.setText(_barCt);
        _barcodeCt.update();
        QPainter paintCont(&m_imBarcodeCont);
        m_imBarcodeCont.fill( Qt::white );
        _barcodeCt.paint(&paintCont,nullptr, nullptr);
    }

}

void CppDanfe::genQrCode()
{
    QString _http = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFeSupl->get_qrCode();
    QrCode _qr = QrCode::encodeText(_http.toLocal8Bit(), QrCode::Ecc::HIGH);
    //desenha os módulos direto na imagem, com margem de 2 módulos, sem depender do Qt SVG.
    const int _border = 2;
    const int _total = _qr.getSize() + _border * 2;
    //tamanho do QrCode
    const int _tamanho = 100;
    m_imQrCode = QImage(_tamanho, _tamanho, QImage::Format_ARGB32);
    m_imQrCode.fill(Qt::white);
    QPainter _painter(&m_imQrCode);
    _painter.scale(double(_tamanho) / _total, double(_tamanho) / _total);
    _painter.setPen(Qt::NoPen);
    _painter.setBrush(Qt::black);
    for (int _y = 0; _y < _qr.getSize(); _y++)
    {
        for (int _x = 0; _x < _qr.getSize(); _x++)
        {
            if (_qr.getModule(_x, _y))
                _painter.drawRect(QRectF(_x + _border, _y + _border, 1, 1));
        }
    }
}

void CppDanfe::getParam()
{
    m_mod = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_mod();
    m_tpEmis = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_tpEmis();

    //modelo frete
    if (this->m_mod == ModeloDF::NFe)
    {
        int _val = ConvNF::modFreteToStr(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->transp->get_modFrete()).toInt();

        switch (_val)
        {
        case 0:  this->m_modFrete = QString::number(_val) + " - " + "REMETENTE";
            break;
        case 1: this->m_modFrete = QString::number(_val) + " - " + "DESTINATÁRIO";
            break;
        case 2:  this->m_modFrete = QString::number(_val) + " - " + "TERCEIROS";
            break;
        case 3: this->m_modFrete = QString::number(_val) + " - " + "REMETENTE";
            break;
        case 4: this->m_modFrete = QString::number(_val) + " - " + "DESTINATÁRIO";
            break;
        case 9: this->m_modFrete = QString::number(_val) + " - " + "SEM TRANSPORTE";
            break;
        default: this->m_modFrete.clear();
            break;
        }
    }

    //chave deve ser separada por espaço
    if (!this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->get_chNFe().isEmpty())
    {
        this->m_chave = formatChave(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->get_chNFe());
    }

    //protocolo
    this->m_protocolo = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->protNFe->get_nProt() + " "
                      + this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->protNFe->get_dhRecbto().toString("dd/MM/yyyy hh:mm:ss");

    //infCpl
    if (!this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->infAdic->get_infAdFisco().isEmpty())
        this->m_infCpl = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->infAdic->get_infAdFisco();
    if (!this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->infAdic->get_infCpl().isEmpty())
    {
        if (!this->m_infCpl.isEmpty())
            this->m_infCpl += QString("\n");
        this->m_infCpl += this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->infAdic->get_infCpl();
    }

    //formato do número da nf
    //se for NFe
    if (this->m_mod == ModeloDF::NFe)
    {
        //999.999.999
        QString _n = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_nNF()), 9);
        this->m_nNF = _n.mid(0, 3) + QStringLiteral(".") + _n.mid(3, 3) + QStringLiteral(".") + _n.mid(6, 3);
    }
    else //se for NFCe
    {
        //999999
        this->m_nNF = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_nNF()), 9);
    }

    //formato do número da série da nf
    this->m_serie = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->infNFe->ide->get_serie()), 3);

    //cStat
    this->m_cStat = this->m_cppnfe->notafiscal->NFe->value(this->m_recNo)->protNFe->get_cStat();

    //gerar codigo de barras
    genBarcode();
    //gerar QrCode
    genQrCode();
}

QString CppDanfe::formatChave(const QString &chave)
{
    QString _v = chave.mid(0, 4)  + QStringLiteral(" ") + chave.mid(4, 4)  + QStringLiteral(" ")
               + chave.mid(8, 4)  + QStringLiteral(" ") + chave.mid(12, 4) + QStringLiteral(" ")
               + chave.mid(16, 4) + QStringLiteral(" ") + chave.mid(20, 4) + QStringLiteral(" ")
               + chave.mid(24, 4) + QStringLiteral(" ") + chave.mid(28, 4) + QStringLiteral(" ")
               + chave.mid(32, 4) + QStringLiteral(" ") + chave.mid(36, 4) + QStringLiteral(" ")
               + chave.mid(40, 4);
    return _v;
}
