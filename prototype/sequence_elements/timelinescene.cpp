#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <sequence_elements/extendedqgri.h>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QList>
#include <QKeyEvent>
#include <sequence_elements/ruler.h>
#include <QModelIndex>
#include <QTreeView>
#include <filesystem/sequencedata.h>

TimelineScene::TimelineScene(SequenceRegister* reg, QObject* parent): QGraphicsScene(parent), ruler(0)
{
    przreg = reg;
    ruler.setSize(200000);
    setSceneRect(0,0, 20000, 300);
    QBrush brush(QColor(50,150,200));
    setBackgroundBrush(brush);
    newRect();
    addItem(&ruler);
    connect (this ,SIGNAL(selectionChanged()), this, SLOT(setSingleSelectionToLast()));
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
        selection->setX(selection->roundedTo10(selection->scenePos().x()<0?0:selection->scenePos().x()));
        float sX = selection->scenePos().x();
        float sW = selection->rect().width();
        foreach (QGraphicsItem* current, items()){
            ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
            if (rect && rect != selection)
            {
                if (selection->wasLeftOf(rect)){

                    if  (selection->rightSideIsInFirstHalfOf(rect))
                    {

                        selection->setXToFrame(rect->previousxpos-sW) ;
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos){
                            rect->animatedMove(rect->previousxpos);

                        }
                    }
                    else if (selection->rightSideIsAfterSecondHalfOf(rect)){

                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos-sW){
                            rect->animatedMove(rect->previousxpos-sW);

                        }

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
                        selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos){
                            rect->animatedMove(rect->previousxpos);

                        }
                    }

                    else  if (selection->leftSideIsAfterFirstHalfOf(rect))
                    {
                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos+sW){
                            rect->animatedMove(rect->previousxpos+sW);

                        }
                        if (sX>rect->previousxpos)
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
//void TimelineScene::behaveOnSelectionSwitchPosMove()
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
//                if (selection->wasLeftOf(rect)){

//                    if  (selection->rightSideIsInFirstHalfOf(rect))
//                    {

//                        selection->setXToFrame(rect->previousxpos-sW) ;
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
//                            rect->animatedMove(rect->previousxpos);
//                    }
//                    else if (selection->rightSideIsAfterSecondHalfOf(rect)){

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
//                    if (selection->leftSideIsInSecondHalfOf(rect))
//                    {
//                        selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos)
//                            rect->animatedMove(rect->previousxpos);
//                    }

//                    else  if (selection->leftSideIsAfterFirstHalfOf(rect))
//                    {
//                        if(!rect->animated && rect->scenePos().x() != rect->previousxpos+sW)
//                            rect->animatedMove(rect->previousxpos+sW);
//                        if (sX>=rect->previousxpos)
//                            selection->setXToFrame(rect->previousxpos);
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
    int limit = rectXAndWBefore(selection);
    selection->setXToFrame(selection->scenePos().x()<limit? limit : selection->scenePos().x());


    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection && rect->previousxpos>= selection->previousxpos){
            rect->setXToFrame(rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));

        }

        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}
void TimelineScene::behaveOnSelectionInsertionDisplace()
{
    ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
    int limit = rectXAndWBefore(selection);
    selection->setXToFrame(selection->scenePos().x()<limit? limit : selection->scenePos().x());


    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect && rect != selection && rect->previousxpos>= selection->previousxpos){
            rect->setXToFrame(selection->rect().width()+rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));

        }

        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}

float TimelineScene::rectXAndWBefore(ExtendedQGRI *rect)
{
    QVector<ExtendedQGRI*> sortedlist;
    foreach (QGraphicsItem *current, items()){
        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(current);
        if (rec)
            sortedlist.append(rec);
    }
    std::sort(sortedlist.begin(), sortedlist.end(), [](ExtendedQGRI *a, ExtendedQGRI *b){


        return a->previousxpos<b->previousxpos;
    });
    float ret= 0;
    foreach (ExtendedQGRI *current, sortedlist)
    {
        if (rect->previousxpos>current->previousxpos && current != rect )
            ret = current->previousboxwidth+current->previousxpos;
    }
    qDebug()<<ret;
    return ret;
}

float TimelineScene::positionOfInsertedShot(QGraphicsSceneDragDropEvent *e)
{
    foreach (QGraphicsItem *current, items()){
        ExtendedQGRI *rec = dynamic_cast<ExtendedQGRI *>(current);
        if (rec && rec != dropRepresentation)
        {
        if (dropRepresentation->scenePos().x() >= rec->scenePos().x() && dropRepresentation->scenePos().x() <= rec->scenePos().x()+rec->rect().width()){
                if (dropRepresentation->scenePos().x() < rec->scenePos().x()+(rec->rect().width()/2)){
                    return rec->scenePos().x();
                }
                else{
                    return rec->scenePos().x()+rec->rect().width();
                }
            }
        }
    }
    return e->scenePos().x();
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
    foreach (QGraphicsItem *current, items()){
        this->removeItem(current);
        delete(current);
    }
    setSceneRect(0,0, 20000, 300);


    ruler.setSize(20000);

    addItem(&ruler);
    update();
    newRect();
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseMoveEvent (e);
    if (!selectedItems().isEmpty() && e->buttons()==Qt::LeftButton){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        handleBoxResize();
        selectedItems().at(0)->setY(-10);
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouseReleaseEvent";
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        if (selection)
        {
            handleBoxResize();
            resetBoxStates();
            QGraphicsScene :: mouseReleaseEvent(e);
            previousSceneWidth = this->width();
        }

    }
}
void TimelineScene::resetBoxStates(){
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
            if (rect->animated)
                rect->prevposresetrequested = true;
            else{
                rect->setPreviousToCurrent();
            }
        }
    }
}

void TimelineScene::resetToPrevious()
{
    foreach (QGraphicsItem* current, items()){
        ExtendedQGRI* rect= dynamic_cast<ExtendedQGRI *>(current);
        if (rect)
        {
                rect->setX(rect->previousxpos);
        }
    }
}
void TimelineScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    auto parent = (QTreeView*)e->source();
    qDebug()<<"detected dragenter - "<< parent->selectionModel()->selectedIndexes()[0].data().toString();
    e->accept();
    auto file = parent->selectionModel()->selectedIndexes()[0].data().toString();
    auto path =  parent->model()->headerData(0, Qt::Horizontal, 0).toString();
    qDebug()<<przreg->currentExpandedFolderSequences->contains(path);
    auto list = przreg->currentExpandedFolderSequences->value(path);
    foreach (SequenceData* current, list){
        if(current->name == file){
            qDebug()<<"found the file";
            dropRepresentation = new ExtendedQGRI ();
            dropRepresentation->setRect(0,0,current->sequencelength()*100,100);
            dropRepresentation->setX(e->scenePos().x());
            if (!selectedItems().isEmpty()){
                 ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
                 selection->setSelected(false);
            }
            qDebug()<<selectedItems();
            dropRepresentation->setSelected(true);
            dropRepresentation->setBrush(QColor(100,255,200));
            dropRepresentation->setPreviousToCurrent();
            addItem(dropRepresentation);
        }
    }
    QGraphicsScene :: dragEnterEvent(e);

}

void TimelineScene::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
  if (e->scenePos().y()<-100 || e->scenePos().y()>100){
      if (dropRepresentation->inserted){
          dropRepresentation->inserted=false;
          resetToPrevious();
      }
      dropRepresentation->setX(e->scenePos().x());
      dropRepresentation->setY(e->scenePos().y());
  }
  else
  {
      if (!dropRepresentation->inserted){
          dropRepresentation->inserted=true;

            // gérer cette interaction, il est certainement nécessaire de réécire des fonctions appropriées à l'UC
          auto pos = positionOfInsertedShot(e);
          qDebug()<<pos <<"rect inserted @";
          dropRepresentation->setXToFrame(pos);
          dropRepresentation->setPreviousToCurrent();
//          moveAllFrom(dropRepresentation->previousxpos, dropRepresentation->rect().width());
          behaveOnSelectionInsertionDisplace();
      }
      else{
      dropRepresentation->setX(e->scenePos().x());
      dropRepresentation->setY(0);
      behaveOnSelectionInsertionDisplace();
      }
  }
  qDebug()<<"dragmove";
}

void TimelineScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    if (dropRepresentation->inserted)
    {
        this->resetBoxStates();
    }
    else
    {
        removeItem(dropRepresentation);
    }
    qDebug()<< "drop happened" ;
    QGraphicsScene :: dropEvent(e);
}

void TimelineScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *e)
{
    removeItem(dropRepresentation);
    resetToPrevious();
    qDebug()<<"drag left";
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


void TimelineScene::allign()
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
        resetBoxStates();
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
        if (!selectedItems().isEmpty())
        {
            foreach (QGraphicsItem *current, selectedItems()){
                this->removeItem(current);
                delete(current);
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

void TimelineScene::changeSelectionSize(int newSize)
{
    if (!selectedItems().isEmpty()){
        ExtendedQGRI* selection= dynamic_cast<ExtendedQGRI *>(selectedItems().at(0));
        selection->setSize(newSize *10);
        moveAllFrom(selection->previousxpos+1, newSize*10 - selection->previousboxwidth);
        selection->setPreviousToCurrent();
        ExtendSceneWidth(newSize*10 - selection->previousboxwidth);
    }
}

void TimelineScene::setSingleSelectionToLast()
{
    if (selectedItems().length()>1)
        {
        int i = 0;
        foreach (QGraphicsItem* current, selectedItems()){
            if (i!= selectedItems().length()-1)
                current->setSelected(false);
            current->setY(0);
            }
        }
}
void TimelineScene :: newRect(){

    ExtendedQGRI *rect = new ExtendedQGRI();
    rect->setRect(0,0,250,100);
    rect->setPreviousToCurrent();
    moveAllFrom(0,250);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(250);



    rect = new ExtendedQGRI();
    rect->setRect(0,0,750,100);
    rect->setPreviousToCurrent();
    moveAllFrom(0,750);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(750);

    rect = new ExtendedQGRI();

    rect->setRect(0,0,500,100);
    rect->setPreviousToCurrent();
    moveAllFrom(0,500);
    addItem(rect);
    resetBoxStates();
    ExtendSceneWidth(500);
}
