#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <extendedqgri.h>

TimelineScene::TimelineScene(QObject* parent): QGraphicsScene(parent)
{
    setSceneRect(0,0, 1000, 100);
    //        for (int i = 0; i <1000; i++){
    //            addRect(i*10, 0, 10, 100);
    //        }
}

TimelineScene::~TimelineScene(){
}

void TimelineScene::activatelxt()
{
    if (selectedItems().size() == 1){
        auto *item = selectedItems().at(0);
        QGraphicsRectItem* rect = (QGraphicsRectItem*)item;
        previousxpos = selectedItems().at(0)->x();
        previousboxwidth = rect->rect().width();
    }

    leftxtend = true;
    qDebug()<<"leftxtend activated in timeline";
}

void TimelineScene::activaterxt()
{
    if (selectedItems().size() == 1)
    {
        auto *item = selectedItems().at(0);
        QGraphicsRectItem* rect = (QGraphicsRectItem*)item;
        previousxpos = selectedItems().at(0)->x();
        previousboxwidth = rect->rect().width();
    }
    rightxtend = true;
    qDebug()<<"leftxtend activated in timeline";
}

void TimelineScene::deactivatext()
{
    leftxtend = false;
    rightxtend = false;
    qDebug()<<"xtension mod déactivated";
    previousxpos = -1000;
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

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseMoveEvent (e);
    if (selectedItems().size() == 1){
        auto *item = selectedItems().at(0);
       ExtendedQGRI* rect = (ExtendedQGRI*)item;
        qDebug()<< previousxpos;
        if(leftxtend){
            if (previousxpos - e->scenePos().x()>=0  && previousxpos> -990){
                rect->setRect(0, 0, (previousxpos - e->scenePos().x())+previousboxwidth, 100);
                rect->setX(e->scenePos().x());
            }
            else if (previousxpos - e->scenePos().x()<=0 && previousxpos +previousboxwidth - e->scenePos().x()>=0  && previousxpos> -990){
                rect->setRect(0, 0, (previousxpos - e->scenePos().x())+previousboxwidth, 100);
                rect->setX(e->scenePos().x());
            }
            else{
                rect->setX(previousxpos + previousboxwidth - 4);
                rect->setRect(0, 0, 4, 100);
            }
        }
        else if(rightxtend)
        {
            if ((previousxpos+previousboxwidth)-e->scenePos().x()<=0 && previousxpos > -990){
                rect->setRect(0, 0, (e->scenePos().x()-previousxpos), 100);
                //rect->setX(e->scenePos().x());
                qDebug()<<"trying to stretch from right";
            }
            else if (previousxpos - e->scenePos().x()+previousboxwidth>=0 &&  e->scenePos().x() >= previousxpos && previousxpos > -990){
                rect->setRect(0, 0, (e->scenePos().x()-previousxpos), 100);
            }
            else{
                rect->setX(previousxpos);
                rect->setRect(0, 0, 4, 100);
            }
        }
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseReleaseEvent(e);
    leftxtend = false;
    rightxtend = false;
    previousxpos = -1000;

}

void TimelineScene :: newRect(){
    QPen pen (Qt::black);
    QBrush brush (Qt::transparent);
    ExtendedQGRI *rect = new ExtendedQGRI();
    rect->setPen(pen);
    rect->setBrush(brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    rect->setFlag(QGraphicsItem::ItemIsSelectable);
    rect->setRect(0,0,100,100);
    connect(rect->emitter, SIGNAL(leftxtndactivated()), this, SLOT(activatelxt()));
    connect(rect->emitter, SIGNAL(rightxtndactivated()), this, SLOT(activaterxt()));
    connect(rect->emitter, SIGNAL(xtendDeactived()), this, SLOT(deactivatext()));
    addItem(rect);
}


