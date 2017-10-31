#ifndef MYPATH_H
#define MYPATH_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <line.h>

class MyPath : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MyPath(QObject *parent = nullptr);
    ~MyPath();
    QPainterPath *myPath1;
    QPainterPath *myPath2;
    Line *l;
    Point *b1;
    Point *b2;
    Point *s1;
    Point *s2;
signals:

private slots:
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event );
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // MYPATH_H
