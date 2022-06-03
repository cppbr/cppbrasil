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

void CppDanfe::genBarcode()
{
    BarCode128 _barcode(BarType::C);
    _barcode.setWidth( 554 );
    _barcode.setHeight( 80 );
    _barcode.setPos(0,0);
    _barcode.setHighDPI(true);
    _barcode.setTextVisible(false);
    //barcode chave
    _barcode.setText(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->get_chNFe());
    _barcode.update();
    QPainter paint(&m_imBarcode);
    m_imBarcode.fill( Qt::white );
    _barcode.paint(&paint,nullptr, nullptr);
    //barcode contingência FS ou FS-DA
    if (this->m_tpEmis == TpEmis::Contingencia_FSIA || this->m_tpEmis == TpEmis::Contingencia_FSDA)
    {
        //cUF, tpEmis, CNPJ
        QString _barCt = QString::number(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_cUF())
                      + ConvNF::tpEmisToStr(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_tpEmis())
                      + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->emite->get_CNPJ();

        //vNF
        QString _vNF = CppUtility::insZeroLeft(QString::number((this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vNF() * 100)), 14);
        _barCt += _vNF;

        //ICMSp
        double _vICMS = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vICMS();
        _barCt += (_vICMS > 0 ? "1" : "2");

        //ICMSs
        double _vST = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->total->ICMSTot->get_vST();
        _barCt += (_vST > 0 ? "1" : "2");

        //DD
        QString _dd = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_dhEmi().toString("dd");
        _barCt += _dd;

        //DV
        int _dv = CppUtility::dvModulo11(_barCt);
        _barCt += QString::number(_dv);

        this->m_chaveCont.clear();
        this->m_chaveCont = formatChave(_barCt);
        BarCode128 _barcodeCt(BarType::C);
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

void CppDanfe::getParam()
{
    m_mod = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_mod();
    m_tpEmis = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_tpEmis();

    //modelo frete
    if (this->m_mod == ModeloDF::NFe)
    {
        int _val = ConvNF::modFreteToStr(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->transp->get_modFrete()).toInt();

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
    if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->get_chNFe().isEmpty())
    {
        this->m_chave = formatChave(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->get_chNFe());
    }

    //protocolo
    this->m_protocolo = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->protNFe->get_nProt() + " "
                      + this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->protNFe->get_dhRecbto().toString("dd/MM/yyyy hh:mm:ss");

    //infCpl
    if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->infAdic->get_infAdFisco().isEmpty())
        this->m_infCpl = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->infAdic->get_infAdFisco();
    if (!this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->infAdic->get_infCpl().isEmpty())
    {
        if (!this->m_infCpl.isEmpty())
            this->m_infCpl += QString("\n");
        this->m_infCpl += this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->infAdic->get_infCpl();
    }

    //formato do número da nf
    //se for NFe
    if (this->m_mod == ModeloDF::NFe)
    {
        //999.999.999
        QString _n = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_nNF()), 9);
        this->m_nNF = _n.mid(0, 3) + QStringLiteral(".") + _n.mid(3, 3) + QStringLiteral(".") + _n.mid(6, 3);
    }
    else //se for NFCe
    {
        //999999999
        this->m_nNF = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_nNF()), 9);
    }

    //formato do número da série da nf
    this->m_serie = CppUtility::insZeroLeft(QString::number(this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->infNFe->ide->get_serie()), 3);

    //cStat
    this->m_cStat = this->m_cppnfe->notafiscal->NFe->items->value(this->m_recNo)->protNFe->get_cStat();

    //gerando codigo de barras
    genBarcode();
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
