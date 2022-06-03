#include "barcode128.h"
#include <QPainter>
#include "code128.h"

//convert type BarType to TYPES
TYPES convType(const BarType &types){
    TYPES _res;
    int _t = static_cast<int>(types);
    _res = static_cast<TYPES>(_t);
    return _res;
}

BarCode128::BarCode128(): BarCode128(BarType::DYNAMIC)
{
}

BarCode128::BarCode128(const BarType &types): m_TextVisible(true),
    m_Width(200), m_Height(80), m_barType(types)
{
}

BarCode128::~BarCode128()
{
}

void BarCode128::setType(const BarType &types)
{
    this->m_barType = types;
}

void BarCode128::setText(const QString &text)
{
    Code128 code(text, convType(m_barType));
    this->m_barEncode = code.GetEncoding();
    this->m_Text = text;

}

void BarCode128::setWidth(float width)
{
    this->m_Width = width;
}

void BarCode128::setHeight(float height)
{
    this->m_Height = height;
}

void BarCode128::setTextVisible(bool visible)
{
    this->m_TextVisible = visible;
}

void BarCode128::setHighDPI(bool highDPI)
{
    this->m_HighDPI = highDPI;
}

QRectF BarCode128::boundingRect() const
{
    return QRectF(0,0, m_Width, m_Height);
}

void BarCode128::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    float lineWidth = m_Width / m_barEncode.length();


    /*
     * This code tries to fit the barcode right inside the width. If the code
     * is too long this means that the bar width goes below one pixel. Which in
     * turn means we get no barcode. On printers this is not a problem too fast
     * as they have 600 DPI usually. Screens with 96 DPI run out faster.
     *
     */

    if ( !m_HighDPI )
    {
        lineWidth = qRound(lineWidth);
        if ( lineWidth < 1 )
        {
            lineWidth = 1;
        }
    }


    float fontHeight = painter->fontMetrics().height();

    float left = 0;
    for (int i=0;i<m_barEncode.length();i++)
    {

        float width = 1 * lineWidth;

        if ( m_barEncode.at(i) == QChar('1') )
        {
            QRectF bar(left, 0, width, m_Height - fontHeight );
            painter->fillRect(bar, Qt::SolidPattern);
        }

        left+= width;
    }

    if ( m_TextVisible )
    {
        QRectF box(0, m_Height - fontHeight , left, fontHeight);
        painter->drawText(box, m_Text, Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

