#include "graphicsview.h"
#include <QMenuBar>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

GraphicView::GraphicView(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    this->setFixedHeight(16777215);
    this->setFixedWidth(16777215);
    scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scene);          // Устанавливаем сцену в виджет
    this->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    this->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene->setSceneRect(0,0,this->maximumHeight(),this->maximumWidth());
    points = new QList<QSharedPointer<Point>>(); // Инициализируем первую группу элементов
    lines = new QList<QSharedPointer<Line>>(); // Инициализируем вторую группу элементов
    i1 = new QGraphicsItemGroup();
    i2 = new QGraphicsItemGroup();
    scene->addItem(i1);
    scene->addItem(i2);
    time = new QTimer();
    point = new Point();
    line = new Line();
    myPat = new MyPath();
    previous = new QSharedPointer<Point>();// Инициализируем Таймер
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(time, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));

}

GraphicView::~GraphicView()
{

}

void GraphicView::mousePressEvent(QMouseEvent *event )
{
    if(scene->focusItem() != 0)
    {
        scene->focusItem()->setPos(mapToScene(event->pos()));
        scene->focusItem()->setX(event->pos().x());
        scene->focusItem()->setY(event->pos().y());
        scene->setFocusItem(NULL);
    }

   if(BuildMode == true)
       if(points->isEmpty())
       {
        //point = new Point();
        point->SetX(event->x());
        point->SetY(event->y());
        points->append(QSharedPointer<Point>(point));
        i1->addToGroup(scene->addRect(point->GetX(),point->GetY(),5,5,QPen(Qt::blue,7)));
        *previous = QSharedPointer<Point>(point);
        return;
       }
       else
       {
           point = new Point();
           point->SetX(event->x());
           point->SetY(event->y());
           points->append(QSharedPointer<Point>(point));
           i1->addToGroup(scene->addRect(point->GetX(),point->GetY(),5,5,QPen(Qt::blue,7)));
           line = new Line();
           line->b1 = NULL;
           line->b2 = NULL;
           connect(line,SIGNAL(signal1(Line *)),this, SLOT(Bezier(Line *)));

            line->SetE(point->GetX(),point->GetY());
            line->SetS((*previous)->GetX(),(*previous)->GetY());
            (*previous)->SetX(point->GetX());
            (*previous)->SetY(point->GetY());
           lines->append(QSharedPointer<Line>(line));
           scene->addItem(line);

           return;
       }
}

void GraphicView::focus(Point *p, bool move,bool dbl)
{
    if(BezierMode == false)
    {
        scene->removeItem(myPat);
        scene->removeItem(line->b1);
        scene->removeItem(line->b2);
        line->b1 = NULL;
        line->b2 = NULL;
        myPat = NULL;
      //  line->setVisible(true);
        scene->update();
        return;
    }
    scene->update();
    if(move == false)
    {
    scene->removeItem(myPat);
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
                           map[7]
                           ,map[9]);
   myPat->myPath2->cubicTo(map[8],
                           map[6],
                           map[3]);
   myPat->b1 = line->b1;
   myPat->b2 = line->b2;
    scene->addItem(myPat);
    myPat->update();
    scene->update();
}
}

void GraphicView::Bezier(Line *l)
{
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
          // this->itemAt(map[0].x(),map[0].y())->setVisible(false);
           myPat->myPath1 = new QPainterPath(map[0]);
           myPat->myPath2 = new QPainterPath(map[9]);
           myPat->myPath1->cubicTo(map[4],
                     map[7]
                    ,map[9]);
           myPat->myPath2->cubicTo(map[8],
                   map[6],
                    map[3]);
           myPat->b1 = l->b1;
           myPat->b2 = l->b2;
            scene->addItem(myPat);
            scene->addItem(l->b1);
            scene->addItem(l->b2);
            scene->update();
    }
    scene->update();
}

void GraphicView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void GraphicView::slotAlarmTimer()
{

}

void GraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
   /* Перебираем все элементы сцены, и если они принадлежат группе,
    * переданной в метод, то удаляем их
    * */
   foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
      if(item->group() == group ) {
         delete item;
      }
   }
}
