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
}

QRectF MyPath::boundingRect() const
{
    return QRectF();
}
void MyPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black,6));
    painter->drawPath(*myPath1);
    painter->drawPath(*myPath2);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MyPath::ReDrawBezier(Point *p)
{
    QMap<int,QPointF> map;
    map[0] = l->start;
    if(this->b1 == p)
    {
        map[1] = QPointF(p->GetX(),p->GetY());
        map[2] = QPointF(l->b2->GetX(),l->b2->GetY());
    }
    else
    {
        map[1] = QPointF(l->b1->GetX(),l->b1->GetY());
        map[2] = QPointF(p->GetX(),p->GetY());
    }
    map[3] = l->end;
    QMap<int,QPointF>::iterator it = (map.end()-1);
    for(;it != map.end(); it = it.key() == 6 ? (it=map.end()) : (it=(map.end()-1)))
     {
         map[it.key()+1]=QPointF(((map[it.key()-2].x() + map[it.key()-3].x())/2),
                 ((map[it.key()-2].y() + map[it.key()-3].y())/2));
        if(it.key()<7)
           map[it.key()+2]=QPointF(((map[it.key()-1].x() + map[it.key()-2].x())/2),
                 ((map[it.key()-1].y() + map[it.key()-2].y())/2));
        if(it.key()<6)
             map[it.key()+3]=QPointF(((map[it.key()].x() + map[it.key()-1].x())/2),
                 ((map[it.key()].y() + map[it.key()-1].y()/2)));
     }
     map[9]= QPointF(((map[7].x()+map[8].x())/2),
                     ((map[7].y()+map[8].y())/2));
    myPath1 = new QPainterPath(map[0]);
    myPath2 = new QPainterPath(map[9]);
    myPath1->cubicTo(map[4],
              map[7]
             ,map[9]);
    myPath2->cubicTo(map[8],
            map[6],
             map[3]);
}
