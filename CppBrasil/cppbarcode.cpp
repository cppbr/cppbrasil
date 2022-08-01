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


#include "cppbarcode.h"

//convert type BarType to TYPES
TYPES convType(const BarType &types){
    TYPES _res;
    int _t = static_cast<int>(types);
    _res = static_cast<TYPES>(_t);
    return _res;
}

CppBarCode::CppBarCode(): CppBarCode(BarType::DYNAMIC)
{
}

CppBarCode::CppBarCode(const BarType &types): m_TextVisible(true),
    m_Width(200), m_Height(80), m_barType(types)
{
}

CppBarCode::~CppBarCode()
{
}

void CppBarCode::setType(const BarType &types)
{
    this->m_barType = types;
}

void CppBarCode::setText(const QString &text)
{
    Code128 code(text, convType(m_barType));
    this->m_barEncode = code.GetEncoding();
    this->m_Text = text;

}

void CppBarCode::setWidth(float width)
{
    this->m_Width = width;
}

void CppBarCode::setHeight(float height)
{
    this->m_Height = height;
}

void CppBarCode::setTextVisible(bool visible)
{
    this->m_TextVisible = visible;
}

void CppBarCode::setHighDPI(bool highDPI)
{
    this->m_HighDPI = highDPI;
}

QRectF CppBarCode::boundingRect() const
{
    return QRectF(0,0, m_Width, m_Height);
}

void CppBarCode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

