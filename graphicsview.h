#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <point.h>
#include <line.h>
#include <QMouseEvent>
#include <QSharedPointer>
#include <mypath.h>

class GraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicView(QWidget *parent = 0);
    ~GraphicView();
    bool BuildMode;
    bool BezierMode;
    bool SplineMode;
signals:

private slots:
    void slotAlarmTimer();
    void Bezier(Line *l);
    void focus(Point *p, bool move, bool dbl);


private:
    QGraphicsScene *scene;
    QList<QSharedPointer<Point>> *points;
    QList<QSharedPointer<Line>> *lines;
    QSharedPointer<Point> *previous;
    QGraphicsItemGroup *i1;
    QGraphicsItemGroup *i2;
    QTimer *time;
    double x,y;
void deleteItemsFromGroup(QGraphicsItemGroup *group);

private:
    void mousePressEvent(QMouseEvent *event );
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    Point *point;
    Line *line;
    MyPath *myPat;
};

#endif // GRAPHICVIEW_H
