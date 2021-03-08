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
                rect->setModifyingcColorSignal();
                rect->setVisible(true);
                rect->setRect(0, 0, selection->x() - rect->x() , 100);
                rect->modified=true;
            }
            else if (selection->scenePos().x()<rect->pos().x() && rect->x()<selection->previousxpos)
            {
                rect->setVisible(false);
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
                rect->setModifyingcColorSignal();
                rect->setVisible(true);
                rect->setX(selection->scenePos().x()+ selection->rect().width());
                rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(selection->scenePos().x()+ selection->rect().width()), 100);
                rect->modified=true;
            }
            else if (selection->scenePos().x()+ selection->rect().width() > rect->previousboxwidth+rect->previousxpos && rect->scenePos().x() > selection->scenePos().x()) //prepare to erase
            {
                rect->setVisible(false);
                rect->modified=true;
            }
        }
    }
}

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
                rect->setVisible(true);
                rect->setModifyingcColorSignal();
                if ((selection->scenePos().x()+selection->rect().width()>rect->previousxpos && selection->scenePos().x()+selection->rect().width()<rect->previousboxwidth+rect->previousxpos)&&
                        (selection->scenePos().x()>rect->previousxpos && selection->scenePos().x()<rect->previousboxwidth+rect->previousxpos)){
                    if (selection->scenePos().x()-rect->previousxpos < (rect->previousboxwidth+rect->previousxpos) - (selection->rect().width()+selection->scenePos().x())){
                        qDebug()<<selection->scenePos().x()-rect->previousxpos;
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
                else if (selection->scenePos().x()<rect->previousxpos && selection->scenePos().x()+selection->rect().width()>rect->previousboxwidth+rect->previousxpos)
                {
                    rect->setVisible(false);
                }
                rect->modified=true;
            }
            else
            {
                rect->restore();

            }
        }
    }
}

void TimelineScene::behaveOnSelectionSwitchPosMove(QGraphicsSceneMouseEvent *e)
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect != selection)
        {


            if (selection->previousxpos<rect->previousxpos){
                if (selection->scenePos().x()+selection->rect().width()>rect->previousxpos+(rect->previousboxwidth/2)){

                    rect->setX(rect->previousxpos-selection->rect().width());
                }
                else
                    rect->restore();
            }
            else
            {
                if (selection->scenePos().x()<rect->previousxpos+rect->previousboxwidth-(rect->rect().width()/2))
                {

                    rect->setX(rect->previousxpos+selection->rect().width());
                }
                else
                    rect->restore();

            }
            selection->collidesWithItem(rect)?
                rect->setModifyingcColorSignal():rect->setRegularColor();
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
            if (!rect->isVisible() && rect->modified){

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

}

void TimelineScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    QGraphicsScene :: dropEvent(e);
}
void TimelineScene::handleBoxResize(QGraphicsSceneMouseEvent *e){ //this functions roots to real resizing functions on the selection mod
    ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    switch (rect->mod){
    case (BoxState :: STRETCH_LEFT):
        rect->strechLeft(e);
        behaveOnSelectionLeftXtend();
        break;

    case (BoxState :: STRETCH_RIGHT):
        rect->strechRight(e);
        behaveOnSelectionRightXtend();
        break;

    case (BoxState :: REGULAR):
        behaveOnSelectionSwitchPosMove(e);
        break;
    case (BoxState ::SWITCHPOS_MOVE):
        behaveOnSelectionSwitchPosMove(e);
        break;
    }
}

void TimelineScene::moveAllToLeft(float from, float distance)
{
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect->scenePos().x()>=from){
            rect->setX(rect->scenePos().x()+distance);
            rect->setPreviousToCurrent();
        }
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
    moveAllToLeft(0,100);
    ExtendSceneWidth(100);
    addItem(rect);
}
