#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>

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


//    auto pos = e->lastScenePos();
//    foreach (QGraphicsItem* current, this->items())
//    {
//        if (((pos.x() >= current->boundingRect().x()) && (pos.x() <= current->boundingRect().x()+current->boundingRect().height()))|| ((pos.x() >= current->boundingRect().x() - 100) && (pos.x() <= current->boundingRect().x())))
//        {
//            qDebug()<<"i clicked on"<<current;
//            return;
//        }
//    }
//        auto *rect = new QGraphicsRectItem();
//        rect->setBrush(Qt::blue);
//        rect->setRect(pos.x()-((int)pos.x()%10), 0, 100, 100);
//        rect->setFlag(QGraphicsItem::ItemIsMovable);

//        foreach (QGraphicsItem* current, this->items())
//        {
//            if (pos.x() <= current->boundingRect().x()){
//                qDebug()<<" rect after the one set"<< "x: "<<current->boundingRect().x()<< ", y: "<<current->boundingRect().y();
//                current->setX(current->x()+100);
//            }
//        }

void TimelineScene :: debugItems(){
    qDebug()<<"reached diplayItems";
    foreach (const QGraphicsItem* item, items()){
        qDebug()<<item<<" x: "<<item->x()<< ", y: "<<item->y();
    }

}
void TimelineScene :: clearItems(){

}
void TimelineScene :: newRect(){
    QPen pen (Qt::black);
    QBrush brush (Qt::transparent);
    auto *rect = addRect(0,0,100,100, pen, brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    rect->setFlag(QGraphicsItem::ItemIsSelectable);
}

