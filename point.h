#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QDebug>

class Point : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Point(QObject *parent = nullptr);
    ~Point();

    void SetX(double x);
    void SetY(double x);
    double GetX();
    double GetY();

    bool move;
signals:
    void signal2(Point *p,bool move,bool dbl);

public slots:

private:
    double x,y;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent();
};
#endif // POINT_H
