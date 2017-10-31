#include "mypath.h"

MyPath::MyPath(QObject *parent) : QObject(parent)
{

}

MyPath::~MyPath()
{

}

void MyPath::mousePressEvent(QGraphicsSceneMouseEvent *event )
{
    this->setVisible(false);
    this->l->setVisible(true);
    l->signal1(l);
    Q_UNUSED(event)
}

QRectF MyPath::boundingRect() const
{
    return QRectF();
}
void MyPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black,6));
    painter->drawPath(*myPath1);
    if(myPath2 != NULL)
    painter->drawPath(*myPath2);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
