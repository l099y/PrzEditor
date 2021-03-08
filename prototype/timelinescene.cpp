#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <extendedqgri.h>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>

TimelineScene::TimelineScene(QObject* parent): QGraphicsScene(parent)
{
    setSceneRect(0,0, 1000, 100);
    this->
    setBackgroundBrush(Qt::gray);
}

TimelineScene::~TimelineScene(){
}

void TimelineScene::behaveOnSelectionLeftXtend()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect!= selection)
        {
            if (rect->previousboxwidth+rect->previousxpos<selection->scenePos().x() && rect->x()<selection->previousxpos && rect->modified) //restore
            {
                rect->restore();
                rect->modified=false;
            }
            else if (selection->scenePos().x()>rect->x() && selection->scenePos().x()<= rect->x()+rect->previousboxwidth){
                rect->setRect(0, 0, selection->x() - rect->x() , 100);
                rect->modified=true;
            }
            else if (selection->scenePos().x()<rect->pos().x() && rect->x()<selection->previousxpos)
            {
                rect->setRect(0, 0, 0 , 100);
                rect->modified=true;
            }
        }
    }
}

void TimelineScene::behaveOnSelectionRightXtend()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection)
        {
            if (selection->scenePos().x() + selection->rect().width()< rect->previousxpos && rect->scenePos().x() > selection->scenePos().x() && rect->modified) // restore
            {
                rect->restore();
                rect->modified=false;
            }
            else if (selection->scenePos().x()+ selection->rect().width() > rect->previousxpos //resize if needed
                     &&  selection->scenePos().x()+ selection->rect().width() < rect->previousboxwidth+rect->previousxpos)

            {
                rect->setX(selection->scenePos().x()+ selection->rect().width());
                rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+ selection->rect().width()), 100);
                rect->modified=true;
            }
            else if (selection->scenePos().x()+ selection->rect().width() > rect->previousboxwidth+rect->previousxpos && rect->scenePos().x() > selection->scenePos().x()) //prepare to erase
            {
                rect->setRect(0, 0, 0 , 100);
                rect->modified=true;
            }
        }
    }}

void TimelineScene::behaveOnSelectionMove(QGraphicsSceneMouseEvent *e)
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect != selection)
        {
            if ((selection->scenePos().x()>rect->previousxpos && selection->scenePos().x()<rect->previousboxwidth+rect->previousxpos)||(
                        (selection->scenePos().x()+selection->rect().width()>rect->previousxpos && selection->scenePos().x()+selection->rect().width()<rect->previousboxwidth+rect->previousxpos)||
                        (selection->scenePos().x()<rect->previousxpos && selection->scenePos().x()+selection->rect().width()>rect->previousboxwidth+rect->previousxpos)))
            {
                if ((selection->scenePos().x()+selection->rect().width()>rect->previousxpos && selection->scenePos().x()+selection->rect().width()<rect->previousboxwidth+rect->previousxpos)&&
                        (selection->scenePos().x()>rect->previousxpos && selection->scenePos().x()<rect->previousboxwidth+rect->previousxpos)){
                    if (selection->scenePos().x()-rect->previousxpos < (rect->previousboxwidth+rect->previousxpos) - (selection->rect().width()+selection->scenePos().x())){
                        qDebug()<<selection->scenePos().x()-rect->previousxpos;
                        qDebug()<<"in the case smaller selection than current";
                        rect->setX(selection->scenePos().x()+selection->rect().width());
                        rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+selection->rect().width()),100);
                    }
                    else
                    {
                        rect->setX(rect->previousxpos);
                        rect->setRect(0,0,(selection->scenePos().x()-rect->previousxpos),100);
                    }

                }
                else if (selection->scenePos().x()+selection->rect().width()>rect->previousxpos && selection->scenePos().x()+selection->rect().width()<rect->previousboxwidth+rect->previousxpos)
                {
                    rect->setX(selection->scenePos().x()+selection->rect().width());
                    rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+selection->rect().width()),100);
                }
                else if (selection->scenePos().x()>rect->previousxpos && selection->scenePos().x()<rect->previousboxwidth+rect->previousxpos)
                {
                    rect->setX(rect->previousxpos);
                    rect->setRect(0,0,(selection->scenePos().x()-rect->previousxpos),100);
                }
                rect->modified=true;
                qDebug()<<"in the right usecase";
            }
            else
            {
                rect->restore();
            }
        }
    }
}

void TimelineScene::behaveOnSelectionMove2(QGraphicsSceneMouseEvent *e)
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    qDebug()<<"selection previous pos ssssssssssssss"<< selection->previousxpos;
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect != selection)
        {

            if (selection->previousxpos<rect->previousxpos){
                if (selection->scenePos().x()+selection->rect().width()>rect->scenePos().x()+(rect->rect().width()/2)){
                    rect->setX(rect->previousxpos-selection->rect().width());
                    rect->setPreviousToCurrent();
                    selection->setPreviousToCurrent();

                }
            }
                else
                {
                if (selection->scenePos().x()<rect->scenePos().x()+(rect->rect().width()/2))
                {
                    rect->setX(rect->previousxpos+selection->rect().width());
                    rect->setPreviousToCurrent();
                    selection->setPreviousToCurrent();

                }

            }
        }
    }
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
    qDebug()<<e->lastScenePos().x();
    qDebug()<<e->scenePos().x();
    if (e->lastScenePos().x()>e->scenePos().x()){
        qDebug()<<"move to left";
    }
    else
    {
        qDebug()<<"move to right";

    }
    if (selectedItems().size() == 1){
        handleBoxResize(e);
        selectedItems().at(0)->setY(0);
    }

}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseReleaseEvent(e);
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
            if (rect->rect().width()==0 && rect->modified){

                this->removeItem(rect);
                delete(rect);
            }
            else{
                rect->setPreviousToCurrent();
                rect->modified =false;
            }
        }
    }
}

void TimelineScene::dragEnterEven(QGraphicsSceneDragDropEvent *e)
{
    QGraphicsScene :: dragEnterEvent(e);
    if(e->mimeData()->hasUrls())
        qDebug()<<"alo";

}

void TimelineScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    qDebug()<<e->mimeData();
    QGraphicsScene :: dropEvent(e);
    qDebug()<<"in graphicsScene dropevent";
}
void TimelineScene::handleBoxResize(QGraphicsSceneMouseEvent *e){
    auto *item = selectedItems().at(0);
    ExtendedQGRI* rect = (ExtendedQGRI*)item;
    if(rect->leftxtend){
        if (rect->previousxpos - e->scenePos().x()>=0){
            rect->setRect(0, 0, (rect->previousxpos - e->scenePos().x())+rect->previousboxwidth, 100);
            rect->setX(e->scenePos().x());
        }
        else if (rect->previousxpos - e->scenePos().x()<=0 && rect->previousxpos +rect->previousboxwidth - e->scenePos().x()>=0){
            rect->setRect(0, 0, (rect->previousxpos - e->scenePos().x())+rect->previousboxwidth, 100);
            rect->setX(e->scenePos().x());

        }
        else{
            rect->setX(rect->previousxpos + rect->previousboxwidth - 4);
            rect->setRect(0, 0, 4, 100);
        }
        behaveOnSelectionLeftXtend();
    }
    else if(rect->rightxtend)
    {
        if (e->scenePos().x()>(rect->previousxpos+rect->previousboxwidth)){
            rect->setRect(0, 0, (e->scenePos().x()- rect->previousxpos), 100);
            rect->setX(rect->previousxpos);
        }
        else if (e->scenePos().x() >rect->previousxpos &&  e->scenePos().x()<rect->previousboxwidth+rect->previousxpos){
            rect->setX(rect->previousxpos);
            rect->setRect(0, 0, (e->scenePos().x()-rect->previousxpos), 100);

        }
        else{
            rect->setRect(0, 0, 4, 100);
            rect->setX(rect->previousxpos);
        }
        behaveOnSelectionRightXtend();
    }
    else
    {
        behaveOnSelectionMove(e);
        //rect->setPreviousToCurrent();
    }

}

void TimelineScene::moveAllToLeft(float f)
{
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        rect->setX(rect->scenePos().x()+f);
        rect->setPreviousToCurrent();

    }

}

void TimelineScene::ExtendSceneWidth(float f)
{
    this->setSceneRect(0,0,this->width()+f, 100);
}
void TimelineScene :: newRect(){
    QPen pen (Qt::black);
    QColor blue70 = Qt::blue;
    blue70.setAlphaF( 0.4 );
    QBrush brush (blue70);
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
    moveAllToLeft(100);
    ExtendSceneWidth(100);
    addItem(rect);
}
