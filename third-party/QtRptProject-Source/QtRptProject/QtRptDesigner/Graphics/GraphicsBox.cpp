/*
Name: QtRpt
Version: 3.0.0
Web-site: https://qtrpt.sourceforge.io
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2022 Aleksey Osipov

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

#include "GraphicsBox.h"
#include <QByteArray>
#include <QBrush>
#include <QGraphicsLayout>
#include <QLinearGradient>
#include <QApplication>
#include <QDebug>
#include "GraphicsScene.h"
#include <QAbstractTextDocumentLayout>
#include <QBuffer>

GraphicsBox::GraphicsBox()
{
    m_width = 200;
    m_height = 20;
    m_RenderingMode = 0;

    m_XcornerGrabBuffer = -3;
    m_YcornerGrabBuffer = -3;
    m_drawingWidth = (m_width - m_XcornerGrabBuffer);
    m_drawingHeight = (m_height - m_YcornerGrabBuffer);
    m_drawingOrigenX = (m_XcornerGrabBuffer);
    m_drawingOrigenY = (m_YcornerGrabBuffer);

    m_corners.resize(8);
    for (auto &corner : m_corners)
        corner = (SPtrCorner)nullptr;

    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(ItemSendsGeometryChanges,true);

    m_autoHeight = false;
    m_borderIsVisible = true;
    m_highlighting = "";
    m_formatString = "";
    m_inputFormatString = "";
    m_text = tr("New Label");
    m_textRotate = 0;
    m_radius = 6;
    m_barcode = (SPtrBarCode)nullptr;
    m_crossTab = (SPtrCrossTab)nullptr;
    m_chart = (SPtrQChart)nullptr;
    m_outterborderPen.setWidth(1);
    m_outterborderPen.setColor(m_outterborderColor);

    if (QApplication::layoutDirection() == Qt::RightToLeft) {
        m_textDirection = true;
        m_alignment = Qt::AlignRight | Qt::AlignVCenter;
    } else {
        m_textDirection = false;
        m_alignment = Qt::AlignLeft | Qt::AlignVCenter;
    }

    this->graphicsItem = this;
    this->adjustSize(0,0);
    this->setAcceptHoverEvents(true);
}

void GraphicsBox::setWidth(qreal value)
{
    m_width = value;
    adjustSize(0,0);
    setCornerPositions();
    if (this->scene() != nullptr)
        this->scene()->update();
}

void GraphicsBox::setHeight(qreal value)
{
    m_height = value;
    adjustSize(0,0);
    setCornerPositions();
    if (this->scene() != nullptr)
        this->scene()->update();
}

void GraphicsBox::adjustSize(int x, int y)
{
    m_width += x;
    m_height += y;

    m_drawingWidth  =  m_width + m_XcornerGrabBuffer;
    m_drawingHeight =  m_height + m_YcornerGrabBuffer;

    if (m_chart != nullptr) {
        m_chartView->resize(m_width, m_height);
        m_chart->resize(m_width, m_height);
    }
    if (m_crossTab != nullptr) {
        m_crossTab->rect.setHeight(m_height);
        m_crossTab->rect.setWidth(m_width);
    }
}

bool GraphicsBox::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    CornerGrabber *corner = dynamic_cast<CornerGrabber *>(watched);
    if (corner == nullptr) return false; // not expected to get here

    QGraphicsSceneMouseEvent *mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (mevent == nullptr) {
        // this is not one of the mouse events we are interrested in
        return false;
    }

    switch (event->type()) {
        // if the mouse went down, record the x,y coords of the press, record it inside the corner object
        case QEvent::GraphicsSceneMousePress: {
                corner->setMouseState(CornerGrabber::kMouseDown);
                corner->mouseDownX = mevent->pos().x();
                corner->mouseDownY = mevent->pos().y();

                //Посылаем сигнал в сцену для отслеживания Ундо при перемещении концов
                auto model = qobject_cast<GraphicsScene *>(scene());
                if (model)
                    model->itemMoving(this);
            }
            break;
        case QEvent::GraphicsSceneMouseRelease: {
                corner->setMouseState(CornerGrabber::kMouseReleased);
            }
            break;
        case QEvent::GraphicsSceneMouseMove: {
                corner->setMouseState(CornerGrabber::kMouseMoving);
            }
            break;
        default:
            // we dont care about the rest of the events
            return false;
            break;
    }

    if (corner->getMouseState() == CornerGrabber::kMouseMoving) {
        qreal x = mevent->pos().x(), y = mevent->pos().y();

        // depending on which corner has been grabbed, we want to move the position
        // of the item as it grows/shrinks accordingly. so we need to eitehr add
        // or subtract the offsets based on which corner this is.
        int XaxisSign = 0;
        int YaxisSign = 0;
        switch( corner->getCorner() ) {
        case 0:
            XaxisSign = +1;
            YaxisSign = +1;
            break;
        case 1:
            XaxisSign = -1;
            YaxisSign = +1;
            break;
        case 2:
            if (type() == ItemType::GBand) XaxisSign = 0;
            else XaxisSign = -1;
            YaxisSign = -1;
            break;
        case 3:
            if (type() == ItemType::GBand) XaxisSign = 0;
            else XaxisSign = +1;
            YaxisSign = -1;
            break;
        case 4:  //bottom-center
            XaxisSign = 0;
            YaxisSign = -1;
            break;
        case 5:  //top-center
            XaxisSign = 0;
            YaxisSign = +1;
            break;
        case 6:  //left-center
            XaxisSign = +1;
            YaxisSign = 0;
            break;
        case 7:  //right-center
            XaxisSign = -1;
            YaxisSign = 0;
            break;
        }

        // if the mouse is being dragged, calculate a new size and also re-position
        // the box to give the appearance of dragging the corner out/in to resize the box

        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int newWidth = m_width + ( XaxisSign * xMoved);
        int newHeight = m_height + (YaxisSign * yMoved) ;

        int deltaWidth  =   newWidth - m_width ;
        int deltaHeight =   newHeight - m_height ;

        adjustSize(  deltaWidth ,   deltaHeight);

        deltaWidth *= (-1);
        deltaHeight *= (-1);

        if ( corner->getCorner() == 0 ) {
            int newXpos = this->pos().x() + deltaWidth;
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(newXpos, newYpos);
        }
        if ( corner->getCorner() == 1 ) {
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(this->pos().x(), newYpos);
        }
        if ( corner->getCorner() == 3 ) {
            int newXpos = this->pos().x() + deltaWidth;
            this->setPos(newXpos,this->pos().y());
        }
        if ( corner->getCorner() == 5 ) {  //top-center
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(this->pos().x(), newYpos);
        }
        if ( corner->getCorner() == 6 ) {  //left-center
            int newXpos = this->pos().x() + deltaWidth;
            this->setPos(newXpos, this->pos().y());
        }
        auto m_scene = qobject_cast<GraphicsScene *>(scene());
        m_scene->itemResizing(this);
        setCornerPositions();
        this->scene()->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}

QPointF GraphicsBox::getPos()
{
    return m_location;
}

void GraphicsBox::setPos(QPointF pos)
{
    m_location = pos;
    QGraphicsItem::setPos(pos);
}

void GraphicsBox::setPos(qreal x, qreal y)
{
    setPos(QPoint(x,y));
}

void GraphicsBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    this->scene()->update();
}

void GraphicsBox::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    this->scene()->update();
}

// for supporting moving the box across the scene
void GraphicsBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    event->setAccepted(true);

    auto model = qobject_cast<GraphicsScene *>(scene());
    int numMoved = model->itemsMoved().size();

    if (numMoved > 1) {
        qDebug() << model->selectedItems().size();
        qDebug() << model->itemsSelected().size();
        return;
    }

    this->setSelected(this->isSelected());
}

// for supporting moving the box across the scene
void GraphicsBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    event->setAccepted(true);

    if (event->button() == Qt::LeftButton)
        m_dragStart = event->pos();

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);

    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
        this->setSelected(!this->isSelected());
    else {
        if (this->scene()->selectedItems().size() == 1)
            this->setSelected(true);
    }
}

// for supporting moving the box across the scene
void GraphicsBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->type() == ItemType::GBand)
        return;

    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_dragStart).manhattanLength();
        if (distance < QApplication::startDragDistance())
            return;
    }


    QGraphicsItem::mouseMoveEvent(event); // move the item...

    auto m_scene = qobject_cast<GraphicsScene *>(scene());

    auto band = qgraphicsitem_cast<ReportBand*>(this->parentItem());
    if (band != nullptr) {
        bool toBound = false;
        if (x() <= 0) {
            setPos(0, y());
            toBound = true;
        } else if (x() > band->getWidth() - this->getWidth()) {
            setPos(band->getWidth() - this->getWidth(), y());
            toBound = true;
        }

        if (y() <= band->titleHeight) {
            setPos(x(), band->titleHeight);
            toBound = true;
        } else if (y() > band->getHeight() - this->getHeight()) {
            setPos(x(), band->getHeight() - this->getHeight());
            toBound = true;
        }

        m_scene->itemResizing(this);
        if (toBound == true) return;
    }

    QPointF newPos = event->pos() ;
    m_location += (newPos - m_dragStart);
    this->setPos(m_location);
    this->scene()->update();
}

void GraphicsBox::setSelected(bool selected)
{
    QGraphicsItem::setSelected(selected);
    if (itemInTree != nullptr)
        itemInTree->setSelected(selected);

    if (selected) {
        createCorners();
        auto m_scene = qobject_cast<GraphicsScene *>(scene());
        m_scene->itemSelect(this);
    } else
        destroyCorners();
    this->scene()->update();
}

bool GraphicsBox::isSelected()
{
    if (itemInTree != nullptr)
        return itemInTree->isSelected();

    return false;
}

// create the corner grabbers
void GraphicsBox::createCorners()
{
    m_outterborderColor = m_borderColor;

    if (type() != ItemType::GBand) {
        if (m_corners[0] == nullptr) {
            m_corners[0] = SPtrCorner(new CornerGrabber(this,0));
            m_corners[0]->installSceneEventFilter(this);
        }
        if (m_corners[1] == nullptr) {
            m_corners[1] = SPtrCorner(new CornerGrabber(this,1));
            m_corners[1]->installSceneEventFilter(this);
        }
        if (m_corners[5] == nullptr) {  //top-center
            m_corners[5] = SPtrCorner(new CornerGrabber(this,5));
            m_corners[5]->installSceneEventFilter(this);
        }
        if (m_corners[6] == nullptr) {  //left-center
            m_corners[6] = SPtrCorner(new CornerGrabber(this,6));
            m_corners[6]->installSceneEventFilter(this);
        }
        if (m_corners[7] == nullptr) {  //rigth-center
            m_corners[7] = SPtrCorner(new CornerGrabber(this,7));
            m_corners[7]->installSceneEventFilter(this);
        }
    }
    if (m_corners[2] == nullptr) {
        m_corners[2] = SPtrCorner(new CornerGrabber(this,2));
        m_corners[2]->installSceneEventFilter(this);
    }
    if (m_corners[3] == nullptr) {
        m_corners[3] = SPtrCorner(new CornerGrabber(this,3));
        m_corners[3]->installSceneEventFilter(this);
    }
    if (m_corners[4] == nullptr) {  //bottom-center
        m_corners[4] = SPtrCorner(new CornerGrabber(this,4));
        m_corners[4]->installSceneEventFilter(this);
    }
    setCornerPositions();
}

void GraphicsBox::setCornerPositions()
{
    if (m_corners[0] != nullptr)  //top-left
        m_corners[0]->setPos(m_drawingOrigenX, m_drawingOrigenY);
    if (m_corners[1] != nullptr)
        m_corners[1]->setPos(m_drawingWidth,  m_drawingOrigenY);
    if (m_corners[2] != nullptr)
        m_corners[2]->setPos(m_drawingWidth , m_drawingHeight);
    if (m_corners[3] != nullptr)
        m_corners[3]->setPos(m_drawingOrigenX, m_drawingHeight);
    if (m_corners[4] != nullptr) //bottom-center
        m_corners[4]->setPos((m_drawingWidth - m_drawingOrigenX)/2, m_drawingHeight);
    if (m_corners[5] != nullptr) //top-center
        m_corners[5]->setPos((m_drawingWidth - m_drawingOrigenX)/2, m_drawingOrigenY);
    if (m_corners[6] != nullptr) //left-center
        m_corners[6]->setPos(m_drawingOrigenX, (m_drawingHeight + m_drawingOrigenY)/2);
    if (m_corners[7] != nullptr) //rigth-center
        m_corners[7]->setPos(m_drawingWidth, (m_drawingHeight + m_drawingOrigenY)/2);
}

QRectF GraphicsBox::boundingRect() const
{
    return QRectF(0, 0, m_width-1, m_height);
}

void GraphicsBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QBrush brush2(m_backgroundColor,Qt::SolidPattern);

    m_outterborderPen.setCapStyle(Qt::RoundCap);
    m_outterborderPen.setStyle(borderStyle());
    m_outterborderPen.setWidth(getBorderWidth());

    if (m_borderIsVisible)
        m_outterborderPen.setColor( m_borderColor );
    else
        m_outterborderPen.setColor(QColor(0,0,0,0));

    painter->setPen(m_outterborderPen);



    if (type() == ItemType::GBand) {
        auto band = qgraphicsitem_cast<ReportBand*>(this);
        auto titleHeight = band->titleHeight;

        QRectF rc (QPointF(0,0), QPointF(getWidth()-1, getHeight()));
        painter->drawRect(rc);

        QPointF p2R(getWidth(), titleHeight);
        QRectF textRect(QPointF(0,0), p2R);
        QRectF fillRect(QPointF(1,1), QPointF(p2R.x()-1,p2R.y()));
        painter->fillRect(fillRect, QBrush(QColor(232,183,98,255)));

        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);

        painter->drawLine(QPointF(0, titleHeight),  p2R);
        painter->drawText(textRect,Qt::AlignCenter,m_text);
        painter->drawPixmap(QRect(m_drawingWidth - titleHeight-2,
                                  1,
                                  titleHeight-2,
                                  titleHeight-2), m_bandPixmap);
    }
    if (type() == ItemType::GBox) {
        painter->save();

        QRectF rcT (QPointF(2,0), QPointF(getWidth(), getHeight()));

        QRectF rc (QPointF(0,0), QPointF(getWidth(), getHeight()));
        switch(this->getFieldType()) {
            case Text:
            case TextImage: {
                if (m_backgroundColor != Qt::white)
                    painter->fillRect(rc, brush2);

                if (borderIsCheck(FrameTop))
                    painter->drawLine(0,0, getWidth()-1,0);  //top
                if (borderIsCheck(FrameLeft))
                    painter->drawLine(0,0, 0,getHeight()); //left
                if (borderIsCheck(FrameRight))
                    painter->drawLine(getWidth()-1,0, getWidth()-1, getHeight()); //right
                if (borderIsCheck(FrameBottom))
                    painter->drawLine(0,getHeight(), getWidth()-1, getHeight()); //bottom

                int flags = m_alignment;
                if (m_textWrap == true)
                    flags = flags | Qt::TextWordWrap;

                m_outterborderPen.setColor( m_fontColor );
                painter->setPen(m_outterborderPen);
                painter->setFont(m_font);

                switch (m_textRotate) {
                case 1: {  //90 degres
                    painter->translate(getWidth(), 0);
                    painter->rotate(90);
                    drawText(painter, m_text, m_font, m_fontColor, rcT, flags, m_RenderingMode, m_textRotate);
                    break;
                }
                case 2: {  //180 degres
                    painter->translate(getWidth(), getHeight());
                    painter->rotate(180);
                    drawText(painter, m_text, m_font, m_fontColor, rcT, flags, m_RenderingMode, m_textRotate);
                    break;
                }
                case 3: {  //270 degres
                    painter->translate(0, getHeight());
                    painter->rotate(-90);
                    drawText(painter, m_text, m_font, m_fontColor, rcT, flags, m_RenderingMode, m_textRotate);
                    break;
                }
                default:
                    drawText(painter, m_text, m_font, m_fontColor, rcT, flags, m_RenderingMode, m_textRotate);
                }

                break;
            }
            case TextRich: {
                QTextDocument td;
                td.setHtml(m_text);
                td.setTextWidth(getWidth());
                QAbstractTextDocumentLayout::PaintContext ctx;
                ctx.clip = QRectF( 0, 0, getWidth(), getHeight() );
                td.documentLayout()->draw( painter, ctx );
                break;
            }
            case Image: {
                Qt::AspectRatioMode aspect = Qt::KeepAspectRatio;
                if (m_ignoreAspectRatio)
                    aspect = Qt::IgnoreAspectRatio;

                QSize pixSize = m_pixmap.size();
                pixSize.scale(rc.size().width(), rc.size().height(), aspect);

                QPixmap scaledPix = m_pixmap.scaled(pixSize, aspect, Qt::SmoothTransformation );
                painter->drawPixmap(0, 0,
                                    (int)pixSize.width(),
                                    (int)pixSize.height(), scaledPix);
                break;
            }
            case Circle: {
                painter->setBrush(brush2);
                painter->drawEllipse(0, 0, getWidth() - 1, getHeight() - 1);
                break;
            }
            case Triangle: {
                qreal pointX1 = 0;
                qreal pointY1 = getHeight()-1;

                qreal pointX2 = getWidth();
                qreal pointY2 = getHeight()-1;

                qreal pointX3 = getWidth()/2;
                qreal pointY3 = 0;

                QPainterPath path;
                path.moveTo (pointX1, pointY1);
                path.lineTo (pointX2, pointY2);
                path.lineTo (pointX3, pointY3);
                path.lineTo (pointX1, pointY1);
                painter->fillPath (path, brush2);
                painter->drawPath(path);
                break;
            }
            case Rhombus: {
                qreal pointX1 = getWidth()/2;
                qreal pointY1 = getHeight()-1;

                qreal pointX2 = getWidth();
                qreal pointY2 = getHeight()/2;

                qreal pointX3 = getWidth()/2;
                qreal pointY3 = 1;

                qreal pointX4 = 1;
                qreal pointY4 = getHeight()/2;

                QPainterPath path;
                path.moveTo (pointX1, pointY1);
                path.lineTo (pointX2, pointY2);
                path.lineTo (pointX3, pointY3);
                path.lineTo (pointX4, pointY4);
                path.lineTo (pointX1, pointY1);
                painter->fillPath (path, brush2);
                painter->drawPath(path);
                break;
            }
            case RoundedReactangle: {
                QRectF rect(1,1,getWidth()-2,getHeight()-2);
                painter->setBrush(brush2);
                painter->drawRoundedRect(rect, m_radius, m_radius);
                break;
            }
            case Reactangle: {
                QRectF rect(1,1,getWidth()-2,getHeight()-2);
                painter->setBrush(brush2);
                painter->drawRect(rect);
                break;
            }
            case Barcode: {
                if (m_barcode != nullptr) {
                    m_barcode->setValue(m_text);
                    m_barcode->drawBarcode(painter, 0, 0, this->getWidth(), this->getHeight());
                }
                break;
            }
            case Diagram: {
                if (m_chart != nullptr) {
                    QRectF rect = QRectF(0, 0, getWidth(), getHeight());
                    m_chartView.data()->render(painter, rect, m_chartView.data()->rect());
                }
                break;
            }
            case CrossTab: {
                painter->setPen(QPen( getColorValue(BorderColor), 1, Qt::SolidLine, Qt::RoundCap));

                int fieldWidth = m_crossTab->rect.width()/m_crossTab->colCount();
                int fieldheight = m_crossTab->rowHeight();
                //int posInCell_V = fieldheight/2+5;
                //int posInCell_H = 5;

                QVarLengthArray<QLineF, 100> lines;
                painter->drawRect(0, 0,
                                  m_crossTab->rect.width(),
                                  m_crossTab->rect.height());

                painter->setPen(QPen( getColorValue(BorderColor), 1, Qt::DashLine, Qt::RoundCap));

                //vertical lines
                for (qreal x = 0; x <= m_crossTab->rect.right(); x += fieldWidth)
                    lines.append(QLineF(x, 0,
                                        x, m_crossTab->rect.bottom()));

                //horizontal lines
                for (qreal y = 0; y <= m_crossTab->rect.bottom(); y += fieldheight)
                    lines.append(QLineF(m_crossTab->rect.left(), y,
                                        m_crossTab->rect.right(), y));

                painter->drawLines(lines.data(), lines.size());
                painter->setPen(QPen( getColorValue(BorderColor), 1, Qt::SolidLine, Qt::RoundCap));
                break;
            }
            default: {

            }
        }

        painter->restore();
    }
}

void GraphicsBox::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void GraphicsBox::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void GraphicsBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    edit();
    //QGraphicsItem::mouseDoubleClickEvent(event);
}

QVariant GraphicsBox::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        auto m_scene = qobject_cast<GraphicsScene *>(scene());
        if (m_scene)
            m_scene->itemMoving(this);
    }

    return QGraphicsItem::itemChange(change, value);
}

GraphicsBox* GraphicsBox::clone()
{
    QPoint newPos(this->x(),this->y());
    newPos.setY(newPos.y()+5);
    newPos.setX(newPos.x()+5);

    auto newContField  = new GraphicsBox();
    newContField->setBorderVisible(this->borderIsVisible());
    newContField->setColorValue(BackgroundColor, this->getColorValue(BackgroundColor));
    newContField->setColorValue(BorderColor, this->getColorValue(BorderColor));
    newContField->setColorValue(FontColor, this->getColorValue(FontColor));
    newContField->setColorValue(FrameTop, this->getColorValue(FrameTop));
    newContField->setColorValue(FrameBottom, this->getColorValue(FrameBottom));
    newContField->setColorValue(FrameLeft, this->getColorValue(FrameLeft));
    newContField->setColorValue(FrameRight, this->getColorValue(FrameRight));
    newContField->setBorderWidth(this->getBorderWidth());
    newContField->setBorderStyle(this->borderStyle());
    newContField->setFieldType(this->getFieldType());
    newContField->setText(this->getText());
    newContField->setFont(this->getFont());
    newContField->setAlignment(this->getAlignment());
    newContField->setFont(this->getFont());
    newContField->setWidth(this->getWidth());
    newContField->setHeight(this->getHeight());
    newContField->m_formatString = this->m_formatString;
    newContField->m_inputFormatString = this->m_inputFormatString;
    newContField->m_textWrap = this->m_textWrap;
    newContField->m_textRotate = this->m_textRotate;

    if (newContField->getFieldType() == Image) {
        newContField->setImgFromat(this->getImgFormat());
        newContField->setIgnoreAspectRatio(this->getIgnoreAspectRatio());
        newContField->setImage(this->getImage());
    }
    if (newContField->getFieldType() == Barcode) {
        newContField->setBarcodeType(this->getBarcodeType());
        newContField->setBarcodeFrameType(this->getBarcodeFrameType());
    }
    if (newContField->getFieldType() == CrossTab) {
        newContField->m_crossTab = this->m_crossTab;
    }
    newContField->setVisible(true);
    newContField->setPos(newPos);

    return newContField;
}

void GraphicsBox::loadParamFromXML(QDomElement e)
{
    GraphicsHelperClass::loadParamFromXML(e);
    this->setFieldType(m_type);

    this->setPos(e.attribute("left").toInt(), e.attribute("top").toInt()+ReportBand::titleHeight);
    this->setWidth(e.attribute("width").toInt());
    this->setHeight(e.attribute("height").toInt());

    if (this->m_type == Text) {
        this->m_formatString = e.attribute("format","");
        this->m_inputFormatString = e.attribute("input_format","");
        this->m_highlighting = e.attribute("highlighting","");
        m_textWrap = e.attribute("textWrap","1").toInt();
        m_textRotate = e.attribute("rotate","0").toInt();
    } else if (this->m_type == Image || e.attribute("picture","text") != "text") {
        //load picture into lable
        QByteArray byteArray = QByteArray::fromBase64(e.attribute("picture","text").toLatin1());
        m_imgFormat = e.attribute("imgFormat","PNG");
        m_pixmap = QPixmap::fromImage(QImage::fromData(byteArray, m_imgFormat.toLatin1().data()));
        m_ignoreAspectRatio = e.attribute("ignoreAspectRatio","1").toInt();
    } else if (this->m_type == Diagram) {
        QColor color;
        QBrush brush;

        m_chart->removeAllSeries();
        m_chart->setTitle(e.attribute("caption"));
        QFont fnt =  m_chart->titleFont();
        fnt.fromString(e.attribute("titleFont", m_chart->titleFont().toString()));
        m_chart->setTitleFont(fnt);
        m_chart->legend()->setVisible(e.attribute("showLegend", "1").toInt());
        fnt =  m_chart->legend()->font();
        fnt.fromString(e.attribute("legendFont", m_chart->legend()->font().toString()));
        m_chart->legend()->setFont(fnt);

        color = colorFromString(e.attribute("colorLegend"));
        brush = m_chart->legend()->labelBrush();
        brush.setColor(color);
        m_chart->legend()->setLabelBrush(brush);

        color = colorFromString(e.attribute("colorBackground"));
        brush = m_chart->backgroundBrush();
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        m_chart->setBackgroundBrush(brush);

        color = colorFromString(e.attribute("colorTitle"));
        brush = m_chart->titleBrush();
        brush.setColor(color);
        m_chart->setTitleBrush(brush);

        m_chart->setProperty("staticChart", e.attribute("staticChart", "1").toInt());

        Qt::Alignment alig;
        if (e.attribute("legendAligment").toInt() == 0) alig = Qt::AlignTop;
        if (e.attribute("legendAligment").toInt() == 1) alig = Qt::AlignBottom;
        if (e.attribute("legendAligment").toInt() == 2) alig = Qt::AlignLeft;
        if (e.attribute("legendAligment").toInt() == 3) alig = Qt::AlignRight;
        m_chart->legend()->setAlignment(alig);


        if (e.attribute("chartType").contains("SeriesTypeLine")) {
            QDomNode c = e.firstChild();
            while(!c.isNull()) {
                QDomElement graphElement = c.toElement();
                if (!graphElement.isNull()) {
                    auto series = new QLineSeries();
                    series->setName(graphElement.attribute("caption"));
                    series->setColor(colorFromString(graphElement.attribute("color")));
                    series->setProperty("graphDS", graphElement.attribute("graphDS"));

                    QDomNode v = graphElement.firstChild();
                    while(!v.isNull()) {
                        QDomElement valueElement = v.toElement();
                        series->append(valueElement.attribute("x").toDouble(),
                                       valueElement.attribute("y").toDouble());

                        v = v.nextSibling();
                    }

                    m_chart->addSeries(series);
                }

                c = c.nextSibling();
            }


        }

        if (e.attribute("chartType") == "SeriesTypeBar" ||
            e.attribute("chartType") == "SeriesTypeStackedBar") {

            QAbstractSeries *abstrSeries = nullptr;

            if (e.attribute("chartType") == "SeriesTypeBar") {
                auto series = new QBarSeries();
                abstrSeries = series;
            }
            if (e.attribute("chartType") == "SeriesTypeStackedBar") {
                auto series = new QStackedBarSeries();
                abstrSeries = series;
            }


            QDomNode c = e.firstChild();
            while(!c.isNull()) {
                QDomElement graphElement = c.toElement();
                if (!graphElement.isNull()) {
                    auto barSet = new QBarSet(graphElement.attribute("caption"));
                    barSet->setColor(colorFromString(graphElement.attribute("color")));
                    barSet->setProperty("graphDS", graphElement.attribute("graphDS"));

                    QDomNode v = graphElement.firstChild();
                    while(!v.isNull()) {
                        QDomElement valueElement = v.toElement();
                        barSet->append(valueElement.attribute("val").toDouble());

                        v = v.nextSibling();
                    }



                    if (abstrSeries->type() == QAbstractSeries::SeriesTypeStackedBar) {
                        auto series = qobject_cast<QStackedBarSeries*>(abstrSeries);
                        series->append(barSet);
                    }
                    if (abstrSeries->type() == QAbstractSeries::SeriesTypeBar) {
                        auto series = qobject_cast<QBarSeries*>(abstrSeries);
                        series->append(barSet);
                    }
                }

                c = c.nextSibling();
            }

            m_chart->addSeries(abstrSeries);
            m_chart->createDefaultAxes();
            m_chart->update();
        }

        if (e.attribute("chartType") == "SeriesTypePie") {
            auto series = new QPieSeries();
            series->setName(e.attribute("caption"));
            series->setHoleSize(e.attribute("holeSize", "0.00").toDouble());
            series->setProperty("graphDS", e.attribute("graphDS"));

            m_chart->addSeries(series);

            QDomNode c = e.firstChild();
            while(!c.isNull()) {
                QDomElement graphElement = c.toElement();
                if (!graphElement.isNull()) {
                    auto slice = new QPieSlice(graphElement.attribute("caption"),
                                               graphElement.attribute("value").toDouble(), series);
                    slice->setExploded(graphElement.attribute("sliceExploaded").toInt());
                    slice->setLabelVisible(graphElement.attribute("labelVisible").toInt());

                    series->append(slice);

                    QColor color = colorFromString(graphElement.attribute("color"));
                    slice->setColor(color);
                }
                c = c.nextSibling();
            }
        }

        m_chart->createDefaultAxes();
        //m_chart->setTheme(QChart::ChartThemeBrownSand);

    } else if (this->m_type == Barcode) {
        setBarcodeType( (BarCode::BarcodeTypes)e.attribute("barcodeType","13").toInt() );
        setBarcodeFrameType( (BarCode::FrameTypes)e.attribute("barcodeFrameType","0").toInt() );
        setBarcodeHeight(e.attribute("barcodeHeight","50").toInt() );
    } else if (this->m_type == CrossTab) {
        QDomElement elem = e;
        m_crossTab->loadParamFromXML(elem);
    }
    m_text = e.attribute("value");

    if (this->m_type != CrossTab)
        this->setColorValue(BackgroundColor, colorFromString(e.attribute("backgroundColor","rgba(255,255,255,0)")));
    this->setColorValue(FontColor, colorFromString(e.attribute("fontColor","rgba(0,0,0,255)")));
    this->setColorValue(FrameTop, colorFromString(e.attribute("borderTop","rgba(0,0,0,255)")));
    this->setColorValue(FrameBottom, colorFromString(e.attribute("borderBottom","rgba(0,0,0,255)")));
    this->setColorValue(FrameLeft, colorFromString(e.attribute("borderLeft","rgba(0,0,0,255)")));
    this->setColorValue(FrameRight, colorFromString(e.attribute("borderRight","rgba(0,0,0,255)")));

    m_font.setBold(e.attribute("fontBold").toInt());
    m_font.setItalic(e.attribute("fontItalic").toInt());
    m_font.setUnderline(e.attribute("fontUnderline").toInt());
    m_font.setStrikeOut(e.attribute("fontStrikeout").toInt());
    m_font.setFamily(e.attribute("fontFamily"));
    int fntSize = e.attribute("fontSize","8").toInt();
    if (fntSize == 0) fntSize = 8;
    m_font.setPointSize(fntSize);
    this->m_autoHeight = e.attribute("autoHeight","0").toInt();
    this->m_RenderingMode = e.attribute("renderingMode","0").toInt();

    Qt::Alignment hAl, vAl;
    if (e.attribute("aligmentH")== "hLeft")          hAl = Qt::AlignLeft;
    else if (e.attribute("aligmentH")== "hRight")    hAl = Qt::AlignRight;
    else if (e.attribute("aligmentH")== "hCenter")   hAl = Qt::AlignHCenter;
    else if (e.attribute("aligmentH") == "hJustify") hAl = Qt::AlignJustify;
    else                                             hAl = Qt::AlignLeft;

    if (e.attribute("aligmentV") == "vTop")          vAl = Qt::AlignTop;
    else if (e.attribute("aligmentV") == "vBottom")  vAl = Qt::AlignBottom;
    else if (e.attribute("aligmentV") == "vCenter")  vAl = Qt::AlignVCenter;
    else                                             vAl = Qt::AlignVCenter;

    m_alignment = hAl | vAl;
}

QDomElement GraphicsBox::saveParamToXML(QSharedPointer<QDomDocument> xmlDoc)
{
    QDomElement elem = GraphicsHelperClass::saveParamToXML(xmlDoc);

    elem.setAttribute("top",this->m_location.y() - ReportBand::titleHeight);
    elem.setAttribute("left",this->m_location.x());
    elem.setAttribute("width",this->m_width);
    elem.setAttribute("height",this->m_height);

    //---FROM TCONTAINERFIELD
    if (this->m_type == Text) {
        elem.setAttribute("format",this->m_formatString);
        elem.setAttribute("input_format",this->m_inputFormatString);
        elem.setAttribute("highlighting",this->m_highlighting);
        elem.setAttribute("textWrap",this->m_textWrap);
        elem.setAttribute("rotate",this->m_textRotate);
    }
    if (this->m_type == Image) {
        //Saving picture
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        if (m_imgFormat.isEmpty() || m_imgFormat.isNull())
            m_pixmap.save(&buffer, "PNG");
        else
            m_pixmap.save(&buffer, m_imgFormat.toLatin1().data());
        QString s = byteArray.toBase64();
        elem.setAttribute("picture",s);
        elem.setAttribute("imgFormat",m_imgFormat);
        elem.setAttribute("ignoreAspectRatio",m_ignoreAspectRatio);
    }
    if (this->m_type == Diagram) {
        if (m_chart->series().size() > 0) {
            QString chartType;
            QString strColor;

            if (m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypeLine) {
                chartType = "SeriesTypeLine";

                for (auto &absSeries : m_chart->series()) {
                    auto series = qobject_cast<QLineSeries*>(absSeries);

                    QDomElement graph = xmlDoc->createElement("graph");
                    graph.setAttribute("color", colorToString(series->color()));
                    graph.setAttribute("caption", series->name());
                    graph.setAttribute("graphDS", series->property("graphDS").toString());

                    for (int row = 0; row < series->count(); row++) {
                        auto point = series->at(row);

                        QDomElement valueElement = xmlDoc->createElement("value");
                        valueElement.setAttribute("x", QString::number(point.x()));
                        valueElement.setAttribute("y", QString::number(point.y()));
                        graph.appendChild(valueElement);
                    }

                    elem.appendChild(graph);
                }
            }
            if (m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypeBar ||
                m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypeStackedBar) {

                QList<QBarSet *> barSets;


                if (m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypeBar) {
                    chartType = "SeriesTypeBar";

                    auto series = qobject_cast<QBarSeries*>(m_chart->series().at(0));
                    barSets = series->barSets();
                }
                if (m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypeStackedBar) {
                    chartType = "SeriesTypeStackedBar";

                    auto series = qobject_cast<QStackedBarSeries*>(m_chart->series().at(0));
                    barSets = series->barSets();
                }

                for (auto &barSet : barSets) {
                    QString graphDS = barSet->property("graphDS").toString();

                    QDomElement graph = xmlDoc->createElement("graph");
                    graph.setAttribute("color", colorToString(barSet->color()));
                    graph.setAttribute("caption", barSet->label());
                    graph.setAttribute("graphDS", graphDS);

                    for (int row = 0; row < barSet->count(); row++) {
                        auto value = barSet->at(row);

                        QDomElement valueElement = xmlDoc->createElement("value");
                        valueElement.setAttribute("val", QString::number(value));
                        graph.appendChild(valueElement);
                    }

                    elem.appendChild(graph);
                }
            }
            if (m_chart->series().at(0)->type() == QAbstractSeries::SeriesTypePie) {
                chartType = "SeriesTypePie";

                auto series = qobject_cast<QPieSeries*>(m_chart->series().at(0));
                elem.setAttribute("graphDS", series->property("graphDS").toString());
                elem.setAttribute("holeSize", series->holeSize());


                for (auto &slice : series->slices()) {
                    QDomElement graph = xmlDoc->createElement("graph");
                    graph.setAttribute("color", colorToString(slice->color()));
                    graph.setAttribute("caption", slice->label());
                    graph.setAttribute("value", slice->value());
                    graph.setAttribute("labelVisible", slice->isLabelVisible());
                    graph.setAttribute("sliceExploaded", slice->isExploded());
                    elem.appendChild(graph);
                }
            }

            int currentIndex = 0;
            if (m_chart->legend()->alignment() == Qt::AlignTop) currentIndex = 0;
            if (m_chart->legend()->alignment() == Qt::AlignBottom) currentIndex = 1;
            if (m_chart->legend()->alignment() == Qt::AlignLeft) currentIndex = 2;
            if (m_chart->legend()->alignment() == Qt::AlignRight) currentIndex = 3;

            elem.setAttribute("legendAligment", currentIndex);
            elem.setAttribute("chartType", chartType);
            elem.setAttribute("showLegend", m_chart->legend()->isVisible());
            elem.setAttribute("legendFont", m_chart->legend()->font().toString());
            elem.setAttribute("caption", m_chart->title());
            elem.setAttribute("titleFont", m_chart->titleFont().toString());
            elem.setAttribute("staticChart", m_chart->property("staticChart").toInt());

            strColor = colorToString(m_chart->titleBrush().color());
            elem.setAttribute("colorTitle", strColor);

            strColor = colorToString(m_chart->backgroundBrush().color());
            elem.setAttribute("colorBackground", strColor);

            strColor = colorToString(m_chart->legend()->labelBrush().color());
            elem.setAttribute("colorLegend", strColor);
        }
    }
    if (this->m_type == Barcode) {
        elem.setAttribute("barcodeType",m_barcode->getBarcodeType());
        elem.setAttribute("barcodeFrameType",m_barcode->getFrameType());
        elem.setAttribute("barcodeHeight",m_barcode->getHeight());
    }
    if (this->m_type == CrossTab) {
        m_crossTab->saveParamToXML(xmlDoc, elem);
    }

    QString hAl, vAl;
    if (getAlignment() & Qt::AlignLeft)         hAl = "hLeft";
    else if (getAlignment() & Qt::AlignRight)   hAl = "hRight";
    else if (getAlignment() & Qt::AlignHCenter) hAl = "hCenter";
    else if (getAlignment() & Qt::AlignJustify) hAl = "hJustify";
    else                                        hAl = "hLeft";

    if (getAlignment() & Qt::AlignTop)          vAl = "vTop";
    else if (getAlignment() & Qt::AlignBottom)  vAl = "vBottom";
    else if (getAlignment() & Qt::AlignVCenter) vAl = "vCenter";
    else                                        vAl = "vCenter";

    elem.setAttribute("aligmentH",hAl);
    elem.setAttribute("aligmentV",vAl);
    elem.setAttribute("value",m_text);

    elem.setAttribute("fontBold",m_font.bold());
    elem.setAttribute("fontItalic",m_font.italic());
    elem.setAttribute("fontUnderline",m_font.underline());
    elem.setAttribute("fontStrikeout",m_font.strikeOut());

    if (m_font.family().isEmpty())
        elem.setAttribute("fontFamily","Arial");
    else
        elem.setAttribute("fontFamily",m_font.family());

    elem.setAttribute("fontSize",m_font.pointSize());

    QString fontColor = colorToString(getColorValue(FontColor));
    QString backgroundColor = colorToString(getColorValue(BackgroundColor));
    QString top = colorToString(getColorValue(FrameTop));
    QString bottom = colorToString(getColorValue(FrameBottom));
    QString left = colorToString(getColorValue(FrameLeft));
    QString right = colorToString(getColorValue(FrameRight));

    elem.setAttribute("fontColor",fontColor);
    elem.setAttribute("backgroundColor",backgroundColor);
    elem.setAttribute("borderTop",top);
    elem.setAttribute("borderBottom",bottom);
    elem.setAttribute("borderLeft",left);
    elem.setAttribute("borderRight",right);

    elem.setAttribute("autoHeight",this->m_autoHeight);
    elem.setAttribute("renderingMode",this->m_RenderingMode);

    return elem;
}

void GraphicsBox::setFieldType(FieldType value)
{
    GraphicsHelperClass::setFieldType(value);
    switch(value) {
        case TextRich: {
            setAlignment(Qt::AlignTop);
            break;
        }
        case Reactangle:
        case RoundedReactangle:
        case Rhombus:
        case Triangle:
        case Circle: {
            break;
        }
        case Image: {
            setText(tr("New image"));
            setIgnoreAspectRatio(true);
            setImage(QPixmap(QString::fromUtf8(":/new/prefix1/images/picture.png")));
            break;
        }
        case Diagram: {
            setText(tr("New diagram"));
            setIgnoreAspectRatio(true);
            setImage(QPixmap(QString::fromUtf8(":/new/prefix1/images/diagram.png")));

            this->setWidth(300);
            this->setHeight(300);

            m_chart = SPtrQChart(new QChart());
            m_chart->layout()->setContentsMargins(0, 0, 0, 0);
            m_chart->setContentsMargins(0,0,0,0);
            m_chart->setMargins(QMargins());
            m_chart->setBackgroundRoundness(0);

            //---------------------------------
            auto set0 = new QBarSet("Jane");
            auto set1 = new QBarSet("John");
            auto set2 = new QBarSet("Axel");
            auto set3 = new QBarSet("Mary");
            auto set4 = new QBarSet("Samantha");

            *set0 << 1 << 2 << 3 << 4 << 5 << 6;
            *set1 << 5 << 0 << 0 << 4 << 0 << 7;
            *set2 << 3 << 5 << 8 << 13 << 8 << 5;
            *set3 << 5 << 6 << 7 << 3 << 4 << 5;
            *set4 << 9 << 7 << 5 << 3 << 1 << 2;

            auto series = new QStackedBarSeries();
            series->setName("QStackedBarSeries");
            series->append(set0);
            series->append(set1);
            series->append(set2);
            series->append(set3);
            series->append(set4);

            m_chart->addSeries(series);
            m_chart->setTitle("Simple stackedbarchart example");

            QStringList categories;
            categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";

            auto axis = new QBarCategoryAxis();
            axis->append(categories);
            m_chart->addAxis(axis, Qt::AlignBottom);
            series->attachAxis(axis);
            m_chart->legend()->setVisible(true);
            m_chart->legend()->setAlignment(Qt::AlignBottom);

            QBrush brush = m_chart->backgroundBrush();
            brush.setColor(Qt::white);
            m_chart->setBackgroundBrush(brush);
            //---------------------------------

            m_chartView = SPtrQChartView(new QChartView(m_chart.data()));
            m_chartView->setRenderHint(QPainter::Antialiasing);
            m_chartView->setVisible(false);
            m_chartView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            m_chartView->resize(m_width, m_height);
            m_chart->resize(m_width, m_height);
            break;
        }
        case Barcode: {
            m_barcode = SPtrBarCode(new BarCode(nullptr));
            this->setWidth(200);
            this->setHeight(100);
            break;
        }
        case CrossTab: {
            this->setWidth(400);
            this->setHeight(300);
            m_crossTab = SPtrCrossTab(new RptCrossTabObject());
            m_crossTab->rect.setHeight(this->getHeight());
            m_crossTab->rect.setWidth(this->getWidth());
            break;
        }
        default:
            break;
    }
    graphicsItem->setZValue(11);
}

void GraphicsBox::setText(QString value)
{
    m_text = value;
}

QString GraphicsBox::getText()
{
    return m_text;
}

void GraphicsBox::setBorderVisible(bool value)
{
    m_borderIsVisible = value;
}

bool GraphicsBox::borderIsVisible()
{
    return m_borderIsVisible;
}

void GraphicsBox::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    m_menu->popup(event->screenPos());
}

void GraphicsBox::setMenu(QMenu *menu)
{
    if (this->type() == ItemType::GBand)
        return;

    QIcon icon;
    auto actContEdit = new QAction(tr("Edit"),this);
    actContEdit->setObjectName("actContEdit");
    QObject::connect(actContEdit, SIGNAL(triggered()), this, SLOT(edit()));

    auto actContDel = new QAction(tr("Delete"),this);
    icon.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/delete.png")), QIcon::Normal, QIcon::On);
    actContDel->setObjectName("actContDel");
    actContDel->setIcon(icon);
    QObject::connect(actContDel, SIGNAL(triggered()), this, SIGNAL(itemRemoving()));

    auto actContMoveForward = new QAction(tr("Move forward"),this);
    actContMoveForward->setObjectName("actContMoveForward");
    icon.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/moveForward.png")), QIcon::Normal, QIcon::On);
    actContMoveForward->setIcon(icon);
    QObject::connect(actContMoveForward, SIGNAL(triggered()), this, SLOT(moveForward()));

    auto actContMoveBack = new QAction(tr("Move back"),this);
    actContMoveBack->setObjectName("actContMoveBack");
    icon.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/moveBack.png")), QIcon::Normal, QIcon::On);
    actContMoveBack->setIcon(icon);
    QObject::connect(actContMoveBack, SIGNAL(triggered()), this, SLOT(moveBack()));

    m_menu->clear();
    m_menu->insertActions(nullptr, menu->actions());
    m_menu->addAction(actContEdit);
    m_menu->addAction(actContDel);
    m_menu->addSeparator();
    m_menu->addAction(actContMoveForward);
    m_menu->addAction(actContMoveBack);
}

BarCode::BarcodeTypes GraphicsBox::getBarcodeType()
{
    return m_barcode->getBarcodeType();
}

void GraphicsBox::setBarcodeType(BarCode::BarcodeTypes value)
{
    m_barcode->setBarcodeType(value);
    update();
}

BarCode::FrameTypes GraphicsBox::getBarcodeFrameType()
{
    return m_barcode->getFrameType();
}

void GraphicsBox::setBarcodeFrameType(BarCode::FrameTypes value)
{
    m_barcode->setFrameType(value);
    update();
}

int GraphicsBox::getBarcodeHeight()
{
    return m_barcode->getHeight();
}

void GraphicsBox::setBarcodeHeight(int value)
{
    m_barcode->setHeight(value);
    update();
}

void GraphicsBox::setImage(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

QPixmap GraphicsBox::getImage()
{
    return m_pixmap;
}

QString GraphicsBox::getImgFormat()
{
    return m_imgFormat;
}

void GraphicsBox::setImgFromat(QString value)
{
    m_imgFormat = value;
}

SPtrCrossTab GraphicsBox::getCrossTab()
{
    return m_crossTab;
}

SPtrQChart GraphicsBox::getChart()
{
    return m_chart;
}

SPtrBarCode GraphicsBox::getBarCode()
{
    return m_barcode;
}

GraphicsBox::~GraphicsBox()
{}
