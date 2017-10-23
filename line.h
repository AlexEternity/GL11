#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QSharedPointer>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <point.h>

class Line : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);
    ~Line();
    void SetS(double x,double y);
    void SetE(double x,double y);
    QPointF start;
    QPointF end;
    Point *b1;
    Point *b2;

signals:
    void signal1(Line *l);

public slots:

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event );

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // LINE_H
