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

void TimelineScene::behaveOnLeftXtend()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
            if (rect->previousboxwidth+rect->previousxpos<selection->scenePos().x() && rect->x()<selection->previousxpos)
            {
                rect->setRect(0, 0, rect->previousboxwidth , 100);
                rect->modifiedOnXtend=false;
            }
            else if (selection->scenePos().x()>rect->x() && selection->scenePos().x()<= rect->x()+rect->previousboxwidth){
                rect->setRect(0, 0, selection->x() - rect->x() , 100);
            }
            else if (selection->scenePos().x()<rect->pos().x() && rect->x()<selection->previousxpos)
            {
                rect->setRect(0, 0, 0 , 100);
                rect->modifiedOnXtend=true;
            }
        }
    }
}

void TimelineScene::behaveOnRightXtend()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection)
        {
            if (selection->scenePos().x() + selection->rect().width()< rect->previousxpos && rect->scenePos().x() > selection->scenePos().x())
            {
                qDebug()<<"in complete restore"<< rect;
                rect->setX(rect->previousxpos);
                rect->setRect(0, 0, rect->previousboxwidth , 100);
                rect->modifiedOnXtend=false;
            }
            else if (selection->scenePos().x()+ selection->rect().width() > rect->previousxpos
                     &&  selection->scenePos().x()+ selection->rect().width() < rect->previousboxwidth+rect->previousxpos)

            {
                qDebug()<<"in interact with selection"<< rect;
                rect->setX(selection->scenePos().x()+ selection->rect().width());
                rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+ selection->rect().width()), 100);
            }
            else if (selection->scenePos().x()+ selection->rect().width() > rect->previousboxwidth+rect->previousxpos && rect->scenePos().x() > selection->scenePos().x())
            {
                rect->setRect(0, 0, 0 , 100);
                rect->modifiedOnXtend=true;
            }
        }
    }}

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
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
            if (rect->rect().width()==0 && rect->modifiedOnXtend){

                this->removeItem(rect);
                delete(rect);
            }
            else{
                rect->setPreviousToCurrent();
                rect->modifiedOnXtend =false;
            }
        }
    }
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
        behaveOnLeftXtend();
    }
    else if(rect->rightxtend)
    {
        if (e->scenePos().x()>(rect->previousxpos+rect->previousboxwidth)){

            rect->setRect(0, 0, (e->scenePos().x()- rect->previousxpos), 100);
            rect->setY(0);
            rect->setX(rect->previousxpos);
        }
        else if (e->scenePos().x() >rect->previousxpos &&  e->scenePos().x()<rect->previousboxwidth+rect->previousxpos){
            rect->setX(rect->previousxpos);
            rect->setRect(0, 0, (e->scenePos().x()-rect->previousxpos), 100);
            rect->setY(0);

        }
        else{
            rect->setRect(0, 0, 4, 100);
            rect->setY(0);
            rect->setX(rect->previousxpos);
        }
        behaveOnRightXtend();
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
    rect->previousboxwidth=100;
    rect->previousxpos=0;
    connect(rect->emitter, SIGNAL(leftxtndactivated()), this, SLOT(activatelxt()));
    connect(rect->emitter, SIGNAL(rightxtndactivated()), this, SLOT(activaterxt()));
    connect(rect->emitter, SIGNAL(xtendDeactived()), this, SLOT(deactivatext()));
    addItem(rect);
}

//if (e->pos().x()>selection->previousxpos && rect->previousxpos>selection->scenePos().x()+selection->rect().width() && rect->scenePos().x()>selection->previousxpos)
//{
//    if (selection == rect)
//        qDebug()<<"shit happened";
//    qDebug()<<"fullrecovery";
//    rect->setRect(0, 0, rect->previousboxwidth , 100);
//    rect->setX(rect->previousxpos);
//    rect->modifiedOnXtend=false;
//}
//else if (rect->scenePos().x()>selection->previousxpos &&
//         e->pos().x()>rect->previousxpos && e->pos().x()<(rect->previousxpos + rect->previousboxwidth)
//         ){
//    qDebug()<<"in recovery";
//    rect->setRect(0, 0, ((rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+selection->rect().width())) , 100);
//    rect->setX(selection->scenePos().x()+selection->rect().width());
//}
//else if (rect->scenePos().x()>selection->previousxpos && e->pos().x()>rect->previousboxwidth+rect->previousxpos)
//{
//    qDebug()<<"in deleteion";
//    rect->setRect(0, 0, 0 , 100);
//    rect->modifiedOnXtend=true;
//}
