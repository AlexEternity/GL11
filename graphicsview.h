#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QMouseEvent>
#include <QSharedPointer>
#include <mypath.h>
#include <point.h>
#include <line.h>

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
    void Bezier(Line *l);
    void focus(Point *p, bool move, bool dbl);


private:
    QGraphicsScene *scene;
    QList<Point*> *points;
    QList<Line*> *lines;
    QList<MyPath*> *paths;
    QList<QList<MyPath*>*> *splines;
    QList<MyPath*> *spline;
    QSharedPointer<Point> *previous;
    QGraphicsItemGroup *i1;
    QGraphicsItemGroup *i2;
    double x,y;

private:
    void mousePressEvent(QMouseEvent *event );
    Point *point;
    Line *line;
    MyPath *myPat;
};

#endif // GRAPHICVIEW_H
