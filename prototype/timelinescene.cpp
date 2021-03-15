#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <extendedqgri.h>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QList>
#include <QKeyEvent>
#include <ruler.h>

TimelineScene::TimelineScene(QObject* parent): QGraphicsScene(parent), ruler(0)
{

    ruler.setSize(200000);
    setSceneRect(0,0, 1300, 300);
    QBrush brush(QColor(50,150,200));
    setBackgroundBrush(brush);
    newRect();
    addItem(&ruler);
}

TimelineScene::~TimelineScene(){
}

void TimelineScene::handleBoxResize(){ //this functions roots to real resizing functions on the selection mod
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        switch (rect->mod){
//        case (BoxState :: STRETCH_LEFT):
//            rect->strechLeft();
//            behaveOnSelectionLeftXtend();
//            break;
//        case (BoxState :: STRETCH_RIGHT):
//            rect->strechRight();
//            behaveOnSelectionRightXtend();
//            break;
        case (BoxState :: REGULAR):
            behaveOnSelectionSwitchPosMove();
            break;
        case (BoxState ::SWITCHPOS_MOVE):
            behaveOnSelectionMove();
            break;
        case (BoxState ::DISPLACE):
            behaveOnSelectionDisplace();
        }
    }
}

void TimelineScene::behaveOnSelectionLeftXtend()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    float sX = selection->scenePos().x();
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect!= selection)
        {
            float rX = rect->scenePos().x();
            if (rect->previousboxwidth+rect->previousxpos < sX && rX < selection->previousxpos && rect->modified) //restore
            {
                rect->restore();
                rect->modified=false;
            }
            else if (sX > rX && sX <= rX+rect->previousboxwidth){
                rect->setModifyingcColorSignal();
                rect->setVisible(true);
                rect->setRect(0, 0, selection->x() - rect->x() , 100);
                rect->modified=true;
            }
            else if (sX < rX && rX <selection->previousxpos)
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
    float sX = selection->scenePos().x();
    float sW = selection->rect().width();
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection)
        {
            float rX = rect->scenePos().x();
            if (sX + sW < rect->previousxpos && rX > sX && rect->modified) // restore
            {
                rect->restore();
                rect->modified=false;
            }
            else if (sX+ sW > rect->previousxpos //resize if needed
                     &&  sX + sW < rect->previousboxwidth+rect->previousxpos)
            {
                rect->setModifyingcColorSignal();
                rect->setVisible(true);
                rect->setX(sX + sW);
                rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(sX + sW), 100);
                rect->modified=true;
            }
            else if (sX + sW > rect->previousboxwidth+rect->previousxpos && rX > sX) //prepare to erase
            {
                rect->setVisible(false);
                rect->modified=true;
            }
        }
    }
}

void TimelineScene::behaveOnSelectionMove()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    float sX = selection->scenePos().x();
    float sW = selection->rect().width();
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection)
        {
            if ((sX > rect->previousxpos && sX < rect->previousboxwidth+rect->previousxpos)||( // si le bord gauche du rectangle est plus grand que la position précédente du rectangle observer et et est plus
                                                                                               (sX + sW >rect->previousxpos && sX + sW < rect->previousboxwidth+rect->previousxpos)||
                                                                                               (sX < rect->previousxpos && sX + sW > rect->previousboxwidth+rect->previousxpos)))
            {
                rect->setVisible(true);
                rect->setModifyingcColorSignal();
                if ((sX + sW > rect->previousxpos && sX + sW < rect->previousboxwidth+rect->previousxpos) &&
                        (sX > rect->previousxpos && sX < rect->previousboxwidth+rect->previousxpos))
                {
                    if (sX - rect->previousxpos < (rect->previousboxwidth+rect->previousxpos) - (sX + sW)){
                        rect->setX(sX+sW);
                        rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(sX + sW),100);
                    }
                    else
                    {
                        rect->setX(rect->previousxpos);
                        rect->setRect(0,0,(sX - rect->previousxpos),100);
                    }

                }
                else if (sX + sW >rect->previousxpos && sX + sW < rect->previousboxwidth+rect->previousxpos)
                {
                    rect->setX(sX+sW);
                    rect->setRect(0,0,(rect->previousboxwidth+rect->previousxpos)-(sX + sW),100);
                }
                else if (sX > rect->previousxpos && sX <rect->previousboxwidth+rect->previousxpos)
                {
                    rect->setX(rect->previousxpos);
                    rect->setRect(0,0,(sX - rect->previousxpos),100);
                }
                else if (sX < rect->previousxpos && sX + sW >rect->previousboxwidth+rect->previousxpos)
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

void TimelineScene::behaveOnSelectionSwitchPosMove()
{
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        selection->setX(selection->roundedTo10(selection->scenePos().x()));
        float sX = selection->scenePos().x();
        float sW = selection->rect().width();
        foreach (QGraphicsItem* current, items()){
            ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
            if (rect && rect != selection)
            {
                if (selection->wasLeftOf(rect)){

                    if  (selection->rightSideIsInFirstHalfOf(rect))
                    {
                        qDebug()<<"rightSideIsInFirstHalfOf(rect)";
                        selection->setXToFrame(rect->previousxpos-sW) ;
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
                            rect->animatedMove(rect->previousxpos);
                    }
                    else if (selection->rightSideIsAfterSecondHalfOf(rect)){
                        qDebug()<<"selection->rightSideIsAfterSecondHalfOf(rect)";
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos-sW)
                            rect->animatedMove(rect->previousxpos-sW);

                        if (sW+sX<rect->previousboxwidth+rect->previousxpos)
                            selection->setXToFrame(rect->previousboxwidth+rect->previousxpos-sW);
                    }
                    else
                        if(!rect->animated)
                            rect->restore();
                }
                else
                {
                    if (selection->leftSideIsInSecondHalfOf(rect))
                    {
                        qDebug()<<"leftSideIsInSecondHalfOf(rect)";
                        selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
                            rect->animatedMove(rect->previousxpos);
                    }

                    else  if (selection->leftSideIsAfterFirstHalfOf(rect))
                    {
                        qDebug()<<"leftSideIsAfterFirstHalfOf(rect)";
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos+sW)
                            rect->animatedMove(rect->previousxpos+sW);
                        if (sX>=rect->previousxpos)
                            selection->setXToFrame(rect->previousxpos);
                    }
                    else
                        if(!rect->animated)
                            rect->restore();
                }
            }
        }
    }
}

//void TimelineScene::behaveOnSelectionSwitchPosMove(QGraphicsSceneMouseEvent *e)
//{
//    if (!selectedItems().isEmpty()){
//        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
//        selection->setX(selection->roundedTo10(selection->scenePos().x()));
//        float sX = selection->scenePos().x();
//        float sW = selection->rect().width();
//        foreach (QGraphicsItem* current, items()){
//            ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
//            if (rect && rect != selection)
//            {


//                if (selection->previousxpos<rect->previousxpos){

//                    if  (sX+sW > rect->previousxpos && sX+sW <= rect->previousxpos+(rect->previousboxwidth/2))
//                    {
//                        selection->setXToFrame(rect->previousxpos-sW) ;
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
//                            rect->animatedMove(rect->previousxpos);
//                    }
//                    else if ( sX+sW >rect->previousxpos+(rect->previousboxwidth/2)){
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos-sW)
//                            rect->animatedMove(rect->previousxpos-sW);

//                        if (sW+sX<rect->previousboxwidth+rect->previousxpos)
//                            selection->setXToFrame(rect->previousboxwidth+rect->previousxpos-sW);
//                    }
//                    else
//                        if(!rect->animated)
//                            rect->restore();
//                }
//                else
//                {
//                    if (sX <=rect->previousxpos+rect->previousboxwidth/2)
//                    {
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos+sW)
//                            rect->animatedMove(rect->previousxpos+sW);
//                        if (sX>=rect->previousxpos)
//                            selection->setXToFrame(rect->previousxpos);
//                    }
//                    else if (sX>rect->previousxpos+rect->previousboxwidth/2 && sX<rect->previousxpos+rect->previousboxwidth)
//                    {
//                        selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
//                            rect->animatedMove(rect->previousxpos);
//                    }
//                    else

//                        if(!rect->animated)
//                            rect->restore();
//                }
//            }
//        }
//    }
//}



void TimelineScene::behaveOnSelectionDisplace()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    if (selection->previousxpos != selection->scenePos().x()){
        foreach (QGraphicsItem* current, items()){
            ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
            if (rect && rect != selection && rect->previousxpos> selection->previousxpos)
                rect->setX(rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));
        }
        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}
bool compareX(const QGraphicsItem *r1, const QGraphicsItem *r2)
{
    return r1->pos().x() < r2->pos().x();
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
    allign();
}
void TimelineScene :: clearItems(){
    //    foreach (QGraphicsItem* current, items()){
    //        delete current;
    //    }
    if (items().contains(&ruler))
        removeItem(&ruler);
    qDeleteAll(items());
    setSceneRect(0,0, 1500, 300);


    ruler.setSize(20000);

    addItem(&ruler);
    update();
    newRect();
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseMoveEvent (e);
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        handleBoxResize();
        selectedItems().at(0)->setY(0);
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        if (selection)
        {
            if (!selection->collidingItems().isEmpty())
                handleBoxResize();
            resetBoxStates();
            QGraphicsScene :: mouseReleaseEvent(e);;
            previousSceneWidth = this->width();
        }

    }
}
void TimelineScene::resetBoxStates(){
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
            if (!rect->isVisible() && rect->modified){

                this->removeItem(rect);
                delete(rect);
            }
            else{

                if (rect->animated)
                    rect->prevposresetrequested = true;
                else{
                    rect->setPreviousToCurrent();
                }
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

void TimelineScene::wheelEvent(QGraphicsSceneWheelEvent *e)
{
    QGraphicsScene :: wheelEvent(e);
    if (e->delta()<0)
        emit(scaleDown());
    else
        emit(scaleUp());
}

void TimelineScene::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<e->key();
    if (e->key()==16777223)
    {
        deleteSelection();
    }

}


void TimelineScene::moveAllFrom(float from, float distance)
{
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect){
            if (rect->scenePos().x()>=from){
                rect->setX(rect->scenePos().x()+distance);
                rect->setPreviousToCurrent();
            }
        }
    }

}


void TimelineScene::ExtendSceneWidth(float f)
{
    this->setSceneRect(0,0,this->width()+f, 100);
    previousSceneWidth = this->width();
    ruler.xtand(f);
}


void TimelineScene::allign() // sadly n² still didnt find a way to sort that collection
{
    QVector<QGraphicsItem*> sortedlist;
    foreach (QGraphicsItem *current, items()){
        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(current);
        if (rec)
            sortedlist.append(current);
    }
    std::sort(sortedlist.begin(), sortedlist.end(), [](QGraphicsItem *a, QGraphicsItem *b){
        return a->scenePos().x()<b->scenePos().x();
    });
    float widthsum = 0;
    foreach (QGraphicsItem *current, sortedlist)
    {
        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(current);
        rec->animatedMove(widthsum);
        widthsum+=rec->rect().width();
    }
    resetBoxStates();
}

void TimelineScene::setmod2()
{

    if (!selectedItems().isEmpty()){

        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        if (rec){
            qDebug()<<"in setmod";
            rec->mod = BoxState::SWITCHPOS_MOVE;
        }

    }
}

void TimelineScene::setdisp()
{

    if (!selectedItems().isEmpty()){
        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        if (rec){
            qDebug()<<"in setdisp";
            rec->mod = BoxState::DISPLACE;
        }
    }

}

void TimelineScene::deleteSelection()
{
    qDebug()<<items();
    if(!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        if (!selectedItems().isEmpty())
        {
            foreach (QGraphicsItem *current, selectedItems()){
                this->removeItem(current);
            }
        }
    }
}

void TimelineScene::displaceSelection(int framePos)
{
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        selection->setX(framePos*10);
        handleBoxResize();
        resetBoxStates();
        update();
    }
}
void TimelineScene :: newRect(){
    QPen pen (Qt::gray);
    QBrush brush (QColor(200,240,200));
    ExtendedQGRI *rect = new ExtendedQGRI();
    rect->setPen(pen);
    rect->setBrush(brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    rect->setFlag(QGraphicsItem::ItemIsSelectable);
    rect->setRect(0,0,250,100);
    rect->setPreviousToCurrent();
    connect(rect->emitter, SIGNAL(leftxtndactivated()), this, SLOT(activatelxt()));
    connect(rect->emitter, SIGNAL(rightxtndactivated()), this, SLOT(activaterxt()));
    connect(rect->emitter, SIGNAL(xtendDeactived()), this, SLOT(deactivatext()));
    moveAllFrom(0,250);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(250);



    rect = new ExtendedQGRI();
    rect->setPen(pen);
    rect->setBrush(brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    rect->setFlag(QGraphicsItem::ItemIsSelectable);
    rect->setRect(0,0,750,100);
    rect->setPreviousToCurrent();
    connect(rect->emitter, SIGNAL(leftxtndactivated()), this, SLOT(activatelxt()));
    connect(rect->emitter, SIGNAL(rightxtndactivated()), this, SLOT(activaterxt()));
    connect(rect->emitter, SIGNAL(xtendDeactived()), this, SLOT(deactivatext()));
    moveAllFrom(0,750);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(750);

    rect = new ExtendedQGRI();
    rect->setPen(pen);
    rect->setBrush(brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    rect->setFlag(QGraphicsItem::ItemIsSelectable);
    rect->setRect(0,0,500,100);
    rect->setPreviousToCurrent();
    connect(rect->emitter, SIGNAL(leftxtndactivated()), this, SLOT(activatelxt()));
    connect(rect->emitter, SIGNAL(rightxtndactivated()), this, SLOT(activaterxt()));
    connect(rect->emitter, SIGNAL(xtendDeactived()), this, SLOT(deactivatext()));
    moveAllFrom(0,500);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(500);
}
