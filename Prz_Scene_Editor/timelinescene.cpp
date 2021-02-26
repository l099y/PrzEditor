#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <przbox.h>
#include <extendedbutton.h>

TimelineScene::TimelineScene(QObject* parent): QGraphicsScene(parent)
{
    setSceneRect(0,0, 1000, 100);
    //        for (int i = 0; i <1000; i++){
    //            addRect(i*10, 0, 10, 100);
    //        }
}

TimelineScene::~TimelineScene(){
    delete(this);
}

void TimelineScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    auto pos = e->lastScenePos();
    foreach (const QGraphicsItem* current, this->items())
    {
        if ((pos.x() >= current->boundingRect().x()) && (pos.x() <= current->boundingRect().x()+current->boundingRect().height()))
        {
            qDebug()<<"i clicked on"<<current;
            return;
        }
    }
        PrzBox *rect = new PrzBox(this);
        rect->setBrush(Qt::blue);
        rect->setRect(pos.x()-((int)pos.x()%10), 0, 100, 100);
        addItem(rect);
}

void TimelineScene :: displayItems(){
    qDebug()<<"reached diplayItems";
    foreach (const QGraphicsItem* item, items()){
        qDebug()<<item<<" x: "<<item->boundingRect().x()<< ", y: "<<item->boundingRect().y();
    }
}
