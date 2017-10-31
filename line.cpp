#include "line.h"

Line::Line(QObject *parent) : QObject(parent)
{

}

Line::~Line()
{

}

void Line::SetE(double x, double y)
{
   end.setX(x);
   end.setY(y);
}

void Line::SetS(double x, double y)
{
    start.setX(x);
    start.setY(y);
}

QRectF Line:: boundingRect() const
{
    return QRectF(start,end).normalized();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if(b1 == NULL)
    painter->setPen(QPen(Qt::black,6));
    else
    painter->setPen(QPen( Qt::red, 6, Qt::DashLine ));
    painter->drawLine(start,end);
        Q_UNUSED(option);
        Q_UNUSED(widget);
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event )
{
    emit signal1(this);
    Q_UNUSED(event);
}
