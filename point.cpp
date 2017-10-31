#include "point.h"
#include <QCursor>

Point::Point(QObject *parent) : QObject(parent)
{
    move = false;
}

Point::~Point()
{

}

void Point::SetX(double x)
{
    this->x = x;
}

void Point::SetY(double x)
{
    this->y = x;
}

double Point::GetX()
{
    return x;
}

double Point::GetY()
{
    return y;
}

QRectF Point:: boundingRect() const
{
    return QRectF(x,y,7,7);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::blue);
    painter->drawEllipse(QRectF(x,y, 10,10));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->update();
    this->SetX(event->scenePos().x());
    this->SetY(event->scenePos().y());
    this->update();
    emit signal2(this,true,false);
}

void Point::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));
        Q_UNUSED(event);
    if(event->button() == Qt::RightButton)
        emit signal2(this,false,true);
}

void Point::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
    emit signal2(this,false,false);
}

void Point::mouseDoubleClickEvent()
{
    emit signal2(this,false,true);
}
