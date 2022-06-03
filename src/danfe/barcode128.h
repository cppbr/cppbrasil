#ifndef BARCODE128_H
#define BARCODE128_H

#include <QGraphicsItem>
enum class BarType { DYNAMIC, A, B, C };

class BarCode128 : public QGraphicsItem
{
public:
    BarCode128();
    BarCode128( const BarType &types );
    ~BarCode128();
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
#endif // BARCODE128_H
