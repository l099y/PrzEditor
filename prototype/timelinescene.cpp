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
}

TimelineScene::~TimelineScene(){
}

void TimelineScene::activatelxt()
{
    qDebug()<<"leftxtend activated in timeline";
}

void TimelineScene::activaterxt()
{
    qDebug()<<"right activated in timeline";
}

void TimelineScene::deactivatext()
{
    qDebug()<<"xtension mod deactivated";
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
        handleBoxResize(e);
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseReleaseEvent(e);
}
void TimelineScene::handleBoxResize(QGraphicsSceneMouseEvent *e){
    auto *item = selectedItems().at(0);
   ExtendedQGRI* rect = (ExtendedQGRI*)item;
    if(rect->leftxtend){
        if (rect->previousxpos - e->scenePos().x()>=0){
            rect->setRect(0, 0, (rect->previousxpos - e->scenePos().x())+rect->previousboxwidth, 100);
            rect->setX(e->scenePos().x());
            rect->setY(0);
        }
        else if (rect->previousxpos - e->scenePos().x()<=0 && rect->previousxpos +rect->previousboxwidth - e->scenePos().x()>=0){
            rect->setRect(0, 0, (rect->previousxpos - e->scenePos().x())+rect->previousboxwidth, 100);
            rect->setX(e->scenePos().x());
            rect->setY(0);
        }
        else{
            rect->setX(rect->previousxpos + rect->previousboxwidth - 4);
            rect->setY(0);
            rect->setRect(0, 0, 4, 100);
        }
    }
    else if(rect->rightxtend)
    {
        if ((rect->previousxpos+rect->previousboxwidth)-e->scenePos().x()<=0){
            rect->setRect(0, 0, (e->scenePos().x()- rect->previousxpos), 100);
            rect->setY(0);
            rect->setX(rect->previousxpos);
        }
        else if (rect->previousxpos - e->scenePos().x()+rect->previousboxwidth>=0 &&  e->scenePos().x() >= rect->previousxpos){
            rect->setRect(0, 0, (e->scenePos().x()-rect->previousxpos), 100);
            rect->setY(0);
            rect->setX(rect->previousxpos);
        }
        else{
            rect->setRect(0, 0, 4, 100);
            rect->setY(0);
            rect->setX(rect->previousxpos);
        }
    }

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


