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


#ifndef CPPBARCODE_H
#define CPPBARCODE_H
#include <QPainter>
#include <QGraphicsItem>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/code128.h>

//enum class BarType { DYNAMIC, A, B, C };
enum class BarType {DYNAMIC, Bar128A, Bar128B, Bar128C };
class CPPDANFE_EXPORT CppBarCode : public QGraphicsItem
{
public:
    CppBarCode();
    CppBarCode( const BarType &types );
    ~CppBarCode();
    void setType ( const BarType &types);
    void setText ( const QString & text );
    void setWidth( float width );
    void setHeight( float height );
    void setTextVisible( bool visible );
    void setHighDPI(bool highDPI);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
private:
    bool m_TextVisible;
    float m_Width;
    float m_Height;
    QString m_Text;
    bool m_HighDPI;
    BarType m_barType;
    QString m_barEncode;
};
#endif // CPPBARCODE_H
