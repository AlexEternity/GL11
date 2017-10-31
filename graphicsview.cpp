#include "graphicsview.h"
#include <QMenuBar>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

GraphicView::GraphicView(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFixedHeight(16777215);
    this->setFixedWidth(16777215);

    scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scene);
    this->setRenderHint(QPainter::Antialiasing);
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene->setSceneRect(0,0,this->maximumHeight(),this->maximumWidth());

    points = new QList<Point*>();
    lines = new QList<Line*>();
    paths = new QList<MyPath*>();
    i1 = new QGraphicsItemGroup();
    i2 = new QGraphicsItemGroup();
    spline = new QList<MyPath*>();
    splines = new QList<QList<MyPath*>*>();
    scene->addItem(i1);
    scene->addItem(i2);
    point = NULL;
    line = NULL;
    myPat = new MyPath();
    previous = new QSharedPointer<Point>();
}

GraphicView::~GraphicView()
{

}

void GraphicView::mousePressEvent(QMouseEvent *event )
{

   if(BuildMode == true)
       if(point == NULL)
       {
           point = new Point();
           point->SetX(event->x());
           point->SetY(event->y());
           i1->addToGroup(scene->addRect(point->GetX(),point->GetY(),5,5,QPen(Qt::green,7)));
           *previous = QSharedPointer<Point>(point);
           return;
       }
       else
       {
           point = new Point();
           point->SetX(event->x());
           point->SetY(event->y());
           i1->addToGroup(scene->addRect(point->GetX(),point->GetY(),5,5,QPen(Qt::green,7)));
           line = new Line();
           line->b1 = NULL;
           line->b2 = NULL;

           connect(line,SIGNAL(signal1(Line *)),this, SLOT(Bezier(Line *)));

           line->SetE(point->GetX(),point->GetY());
           line->SetS((*previous)->GetX(),(*previous)->GetY());
           (*previous)->SetX(point->GetX());
           (*previous)->SetY(point->GetY());

           lines->append(line);
           scene->addItem(line);
           scene->update();
           return;
       }
   if(SplineMode == true)
   {
       point = new Point();
       point->SetX(event->x());
       point->SetY(event->y());
       Point *temp;
       line = NULL;
       for(int i = 0; i<lines->size();i++)
       {
           if(point->GetX()>lines->at(i)->end.x() ||
              point->GetX()<lines->at(i)->start.x())
               continue;
           else
           {
               line = lines->at(i);
           }
       }
       spline = NULL;
       for(int i = 0; i < splines->size();i++)
           if(splines->at(i)->at(0)->s1->GetX() == line->start.x())
               spline = splines->at(i);
       if(spline == NULL)
       {
           myPat = new MyPath();
           myPat->s1 = new Point();
           myPat->s2 = new Point();

           myPat->s1->SetX(line->start.x());
           myPat->s1->SetY(line->start.y());
           myPat->s2->SetX(line->end.x());
           myPat->s2->SetY(line->end.y());
           spline = new QList<MyPath*>();
           myPat->l = line;

           spline->append(myPat);
           splines->append(spline);
       }
       for(int i = 0; i < spline->size(); i++)
       {
           if(point->GetX() > spline->at(i)->s1->GetX() &&
              point->GetX() < spline->at(i)->s2->GetX())
           {
                    temp =  spline->at(i)->s2;
                    spline->at(i)->s2 = point;

                    connect(spline->at(i)->s2,SIGNAL(signal2(Point *,bool,bool)),this, SLOT(focus(Point *,bool,bool)));

                    MyPath *pat = new MyPath();
                    pat->s1 = point;
                    pat->s2 = temp;
                    pat->l = line;

                    spline->insert(i+1,pat);
                    break;
           }
       }
       for(int i = 0;i<splines->size();i++)
       {
           for(int j = 0; j < splines->at(i)->size();j++)
           {
           splines->at(i)->at(j)->myPath1 = new QPainterPath(QPointF(splines->at(i)->at(j)->s1->GetX(),
                                                                     splines->at(i)->at(j)->s1->GetY()));
           splines->at(i)->at(j)->myPath1->cubicTo(QPointF(splines->at(i)->at(j)->s1->GetX(),(splines->at(i)->at(j)->s1->GetY()+splines->at(i)->at(j)->s2->GetY())/2),
                                                   QPointF((splines->at(i)->at(j)->s1->GetX()+splines->at(i)->at(j)->s2->GetX())/2,splines->at(i)->at(j)->s1->GetY()),
                                                   QPointF(splines->at(i)->at(j)->s2->GetX(),splines->at(i)->at(j)->s2->GetY()));
           splines->at(i)->at(j)->myPath2 = NULL;

           if(j!=0)
              scene->addItem(splines->at(i)->at(j)->s1);
           if(j!=splines->at(i)->size()-1)
              scene->addItem(splines->at(i)->at(j)->s2);

           scene->addItem(splines->at(i)->at(j));
           scene->update();
           splines->at(i)->at(j)->update();

           line->setVisible(false);
           }
       }
   }
}

void GraphicView::focus(Point *p, bool move,bool dbl)
{
    for(int i = 0;i < lines->size();i++)
        if(lines->at(i)->b1 != NULL && lines->at(i)->b2 != NULL)
        {
            if(lines->at(i)->b1->GetX() == p->GetX())
                line = lines->at(i);
            else if(lines->at(i)->b2->GetX() == p->GetX())
                line = lines->at(i);
        }
        else
            continue;

    for(int i = 0; i< paths->size();i++)
        if(paths->at(i)->b1->GetX() == p->GetX())
            myPat = paths->at(i);
        else if(paths->at(i)->b2->GetX() == p->GetX())
            myPat = paths->at(i);
        else continue;

    if(BezierMode == true && dbl == true)
    {
        scene->removeItem(myPat);
        scene->removeItem(line->b1);
        scene->removeItem(line->b2);
        paths->removeOne(myPat);
        line->b1 = NULL;
        line->b2 = NULL;
        scene->update();
        return;
    }
    scene->update();
    if(BezierMode == true && move == false)
    {
        scene->removeItem(myPat);
        paths->removeOne(myPat);
        myPat = NULL;

        QMap<int,QPointF> map;
        map[0] = line->start;
        map[1] = QPointF(line->b1->GetX(),line->b1->GetY());
        map[2] = QPointF(line->b2->GetX(),line->b2->GetY());
        map[3] = line->end;
        map[4] = QPointF(((map[1].x()+map[0].x())/2),
                        ((map[1].y()+map[0].y())/2));
        map[5] = QPointF(((map[1].x()+map[2].x())/2),
                        ((map[1].y()+map[2].y())/2));
        map[6] = QPointF(((map[2].x()+map[3].x())/2),
                        ((map[2].y()+map[3].y())/2));
        map[7] = QPointF(((map[4].x()+map[5].x())/2),
                        ((map[4].y()+map[5].y())/2));
        map[8] = QPointF(((map[5].x()+map[6].x())/2),
                        ((map[5].y()+map[6].y())/2));
        map[9]= QPointF(((map[7].x()+map[8].x())/2),
                        ((map[7].y()+map[8].y())/2));

        myPat = new MyPath();
        myPat->myPath1 = new QPainterPath(map[0]);
        myPat->myPath2 = new QPainterPath(map[9]);
        myPat->myPath1->cubicTo(map[4],
                                map[7],
                                map[9]);
        myPat->myPath2->cubicTo(map[8],
                                map[6],
                                map[3]);
        myPat->b1 = line->b1;
        myPat->b2 = line->b2;

        paths->append(myPat);
        scene->addItem(myPat);
        myPat->update();
        scene->update();
    }

    if(SplineMode == false && BezierMode == false && move == false && BuildMode == false)
    {
        spline = NULL;
        for(int i = 0;i < splines->size();i++)
        {
            for(int j = 0; j < splines->at(i)->size();j++)
            {
                   if(splines->at(i)->at(j)->s1->GetX() == p->GetX() ||
                       splines->at(i)->at(j)->s2->GetX() == p->GetX())
                   {
                       splines->at(i)->at(j)->myPath1 = NULL;
                       QPainterPath *myPath1 = new QPainterPath(QPointF(splines->at(i)->at(j)->s1->GetX(),
                                                                        splines->at(i)->at(j)->s1->GetY()));
                       myPath1->cubicTo(QPointF(splines->at(i)->at(j)->s1->GetX(),(splines->at(i)->at(j)->s1->GetY()+splines->at(i)->at(j)->s2->GetY())/2),
                                        QPointF((splines->at(i)->at(j)->s1->GetX()+splines->at(i)->at(j)->s2->GetX())/2,splines->at(i)->at(j)->s1->GetY()),
                                        QPointF(splines->at(i)->at(j)->s2->GetX(),splines->at(i)->at(j)->s2->GetY()));

                       splines->at(i)->at(j)->myPath1 = myPath1;
                       scene->addItem(splines->at(i)->at(j));
                       scene->update();
                   }
            }
        }
    }
    if(SplineMode == false && dbl == true)
    {
        for(int i = 0;i < splines->size();i++)
        {
            for(int j = 0; j < splines->at(i)->size();j++)
            {
                   if(splines->at(i)->at(j)->s1->GetX() == p->GetX() ||
                      splines->at(i)->at(j)->s2->GetX() == p->GetX())
                   {
                       spline = splines->at(i);
                       break;
                   }
            }
        }

        line = spline->at(0)->l;
        for(int i = 0;i < spline->size();i++)
        {
            scene->removeItem(spline->at(i));
            if(i != 0)
                scene->removeItem(spline->at(i)->s1);
            if(i != spline->size()-1)
                scene->removeItem(spline->at(i)->s2);
        }

        scene->update();
        splines->removeOne(spline);
        line->setVisible(true);
    }
}

void GraphicView::Bezier(Line *l)
{
    line = l;
    if(BezierMode == true)
        if(l->b1 == NULL)
        {
           l->b1 = new Point();
           l->b2 = new Point();
           l->b1->SetX(((l->end.x()+l->start.x())/2));
           l->b1->SetY(l->start.y());
           l->b2->SetX(l->end.x());
           l->b2->SetY((l->end.y()+l->start.y())/2);

           connect(l->b1,SIGNAL(signal2(Point *,bool,bool)),this, SLOT(focus(Point *,bool,bool)));
           connect(l->b2,SIGNAL(signal2(Point *,bool,bool)),this, SLOT(focus(Point *,bool,bool)));

           QMap<int,QPointF> map;
           map[0] = l->start;
           map[1] = QPointF(l->b1->GetX(),l->b1->GetY());
           map[2] = QPointF(l->b2->GetX(),l->b2->GetY());
           map[3] = l->end;
           map[4] = QPointF(((map[1].x()+map[0].x())/2),
                            ((map[1].y()+map[0].y())/2));
           map[5] = QPointF(((map[1].x()+map[2].x())/2),
                            ((map[1].y()+map[2].y())/2));
           map[6] = QPointF(((map[2].x()+map[3].x())/2),
                            ((map[2].y()+map[3].y())/2));
           map[7] = QPointF(((map[4].x()+map[5].x())/2),
                            ((map[4].y()+map[5].y())/2));
           map[8] = QPointF(((map[5].x()+map[6].x())/2),
                            ((map[5].y()+map[6].y())/2));
           map[9]= QPointF(((map[7].x()+map[8].x())/2),
                            ((map[7].y()+map[8].y())/2));

           myPat = new MyPath();
           myPat->myPath1 = new QPainterPath(map[0]);
           myPat->myPath2 = new QPainterPath(map[9]);
           myPat->myPath1->cubicTo(map[4],
                     map[7],
                     map[9]);
           myPat->myPath2->cubicTo(map[8],
                     map[6],
                     map[3]);
           myPat->b1 = l->b1;
           myPat->b2 = l->b2;

           paths->append(myPat);
           scene->addItem(myPat);
           scene->addItem(l->b1);
           scene->addItem(l->b2);
           scene->update();
    }
    scene->update();
}
