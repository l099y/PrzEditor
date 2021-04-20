#include "timelinescene.h"
#include <QGraphicsScene>
#include <qdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsRectItem>
#include <sequence_elements/shot.h>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QList>
#include <QKeyEvent>
#include <sequence_elements/ruler.h>
#include <QModelIndex>
#include <QTreeView>
#include <filesystem/sequencedata.h>
#include <sequence_elements/ruler.h>

TimelineScene::TimelineScene(SequenceRegister* reg, QGraphicsView* vview, QObject* parent): QGraphicsScene(parent), ruler(0)
{
    ruler.view = vview;
    przreg = reg;
    view = vview;
    ruler.setSize(10400000);
    setSceneRect(0,0, 200000, 300);
    QBrush brush(QColor(50,150,200));
    setBackgroundBrush(brush);
    newRect();
    addItem(&ruler);
    connect (this ,SIGNAL(selectionChanged()), this, SLOT(setSingleSelectionToLast()));
}

TimelineScene::~TimelineScene(){
}

//this functions roots to real resizing functions on the selection mod on mouse move

void TimelineScene::handleSelectionMove(){
    if (!selectedItems().isEmpty()){
        Shot* rect= dynamic_cast<Shot*>(selectedItems().at(0));
        switch (rect->mod){
        case (BoxState :: REGULAR):
            behaveOnSelectionSwitchPosMove();
            break;
        case (BoxState ::DISPLACE):
            behaveOnSelectionDisplace();
        }
    }
}

// on mouse release, another treatment is nessessary to settle the proper positions ( sometime the animated move creates wrong evaluation

void TimelineScene::handleSelectionMoveFinal()
{
    if (!selectedItems().isEmpty()){
        Shot* rect= dynamic_cast<Shot*>(selectedItems().at(0));
        switch (rect->mod){
        case (BoxState :: REGULAR):
            behaveOnSelectionSwitchPosMoveFinal();
            break;
        case (BoxState ::DISPLACE):
            behaveOnSelectionDisplace();
            break;
        }
    }

}

void TimelineScene::behaveOnSelectionSwitchPosMove()
{
    if (!selectedItems().isEmpty()){
        Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
        selection->setX(selection->roundedTo10(selection->scenePos().x()<0?0:selection->scenePos().x()));
        float sX = selection->scenePos().x();
        float sW = selection->rect().width();
        foreach (QGraphicsItem* current, items()){
            Shot* rect= dynamic_cast<Shot*>(current);
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

void TimelineScene::behaveOnSelectionSwitchPosMoveFinal()
{
    qDebug()<<"rechaed posemovefinal";
    if (!selectedItems().isEmpty()){
        Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
        selection->setX(selection->roundedTo10(selection->scenePos().x()<0?0:selection->scenePos().x()));
        float sX = selection->scenePos().x();
        float sW = selection->rect().width();
        foreach (QGraphicsItem* current, items()){
            Shot* rect= dynamic_cast<Shot*>(current);

            if (rect && rect != selection)
            {
                rect->timer->stop();
                rect->setAnimatedFalse();
                rect->restore();
                if (selection->wasLeftOf(rect)){

                    if  (selection->rightSideIsInFirstHalfOf(rect))
                    {

                        selection->setXToFrame(rect->previousxpos-sW) ;
                        if(rect->scenePos().x() != rect->previousxpos){
                            rect->setXToFrame(rect->previousxpos);

                        }
                    }
                    else if (selection->rightSideIsAfterSecondHalfOf(rect)){

                        if(rect->scenePos().x() != rect->previousxpos-sW){
                            rect->setXToFrame(rect->previousxpos-sW);

                        }

                        if (sW+sX<rect->previousboxwidth+rect->previousxpos)
                            selection->setXToFrame(rect->previousboxwidth+rect->previousxpos-sW);
                    }
                    else
                            rect->restore();
                }
                else
                {
                    if (selection->leftSideIsInSecondHalfOf(rect))
                    {
                        selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
                        if(rect->scenePos().x() != rect->previousxpos){
                            rect->setXToFrame(rect->previousxpos);

                        }
                    }

                    else  if (selection->leftSideIsAfterFirstHalfOf(rect))
                    {
                        if(rect->scenePos().x() != rect->previousxpos+sW){
                            rect->setXToFrame(rect->previousxpos+sW);

                        }
                        if (sX>rect->previousxpos)
                            selection->setXToFrame(rect->previousxpos);
                    }
                    else
                            rect->restore();
                }
            }
        }
        if (selection->previousxpos<selection->scenePos().x())
        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}



void TimelineScene::behaveOnSelectionDisplace()
{
    Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
    int limit = rectXAndWBefore(selection);
    selection->setXToFrame(selection->scenePos().x()<limit? limit : selection->scenePos().x());


    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot*>(current);
        if (rect && rect != selection && rect->previousxpos>= selection->previousxpos){
            rect->setXToFrame(rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));

        }

        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}
void TimelineScene::behaveOnSelectionInsertionDisplace()
{
    Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
    int limit = rectXAndWBefore(selection);
    selection->setXToFrame(selection->scenePos().x()<limit? limit : selection->scenePos().x());


    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot *>(current);
        if (rect && rect != selection && rect->previousxpos>= selection->previousxpos){
            rect->setXToFrame(selection->rect().width()+rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));

        }
        if (previousSceneWidth<previousSceneWidth+(selection->scenePos().x()-selection->previousxpos))
        this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
}

float TimelineScene::rectXAndWBefore(Shot *rect)
{
    QVector<Shot*> sortedlist;
    foreach (QGraphicsItem *current, items()){
        Shot *rec = dynamic_cast<Shot*>(current);
        if (rec)
            sortedlist.append(rec);
    }
    std::sort(sortedlist.begin(), sortedlist.end(), [](Shot *a, Shot *b){


        return a->previousxpos<b->previousxpos;
    });
    float ret= 0;
    foreach (Shot *current, sortedlist)
    {
        if (rect->previousxpos>current->previousxpos && current != rect )
            ret = current->previousboxwidth+current->previousxpos;
    }
    return ret;
}



float TimelineScene::positionOfInsertedShot(QGraphicsSceneDragDropEvent *e)
{
    foreach (QGraphicsItem *current, items()){
        Shot *rec = dynamic_cast<Shot*>(current);
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
    setSceneRect(0,0, 200000, 300);
    ruler.setSize(2000000);
    addItem(&ruler);
    newRect();

}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene :: mouseMoveEvent (e);
    if (!selectedItems().isEmpty() && e->buttons()==Qt::LeftButton){
        Shot* rect= dynamic_cast<Shot*>(selectedItems().at(0));
        handleSelectionMove();
        selectedItems().at(0)->setY(-10);
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouseReleaseEvent";
    if (selectedItems().size()==1){
        Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
        if (selection)
        {
            handleSelectionMoveFinal();
            //resetBoxStates();
            emit (moveShotss(getMovedShots(), previousSceneWidth, this->sceneRect().width()));
            QGraphicsScene :: mouseReleaseEvent(e);
            previousSceneWidth = this->width();
        }

    }
}

void TimelineScene::resetBoxStates(){
    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot*>(current);
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
        Shot* rect= dynamic_cast<Shot *>(current);
        if (rect)
        {
                rect->setX(rect->previousxpos);
        }
    }
}
void TimelineScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    auto parent = (QTreeView*)e->source();
    e->accept();
    auto file = parent->selectionModel()->selectedIndexes()[0].data().toString();
    auto path =  parent->model()->headerData(0, Qt::Horizontal, 0).toString();
    auto list = przreg->currentExpandedFolderSequences->value(path);
    foreach (SequenceData* current, list){
        if(current->name == file){
            dropRepresentation = new Shot ();
            dropRepresentation->setRect(0,0,current->sequencelength()*10,100);
            dropRepresentation->setX(e->scenePos().x());
            if (!selectedItems().isEmpty()){
                 Shot* selection= dynamic_cast<Shot *>(selectedItems().at(0));
                 selection->setSelected(false);
            }
            dropRepresentation->setSelected(true);
            dropRepresentation->setBrush(QColor(100,255,200));
            dropRepresentation->setPreviousToCurrent();
            dropRepresentation->seq= current;
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
}
QVector<Shot *> TimelineScene::getMovedShots()
{
    QVector<Shot*> ret;
    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot*>(current);
        if (rect && rect != dropRepresentation)
        {
            if (rect->scenePos().x() != rect->previousxpos)
                ret.append(rect);
        }
    }
    return ret;
}

void TimelineScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    if (dropRepresentation->inserted)
    {
        removeItem(dropRepresentation);
        qDebug()<<"should be created";
        QList<Shot*> param;
        auto print = getMovedShots();

        emit (createShot(dropRepresentation->seq, dropRepresentation->scenePos().x(), dropRepresentation->rect().width(), this, print));
        dropRepresentation =  nullptr;
    }
    else
    {
        removeItem(dropRepresentation);
        dropRepresentation =  nullptr;
    }
    QGraphicsScene :: dropEvent(e);
}

void TimelineScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *e)
{
    removeItem(dropRepresentation);
    dropRepresentation=nullptr;
    resetToPrevious();
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
    if (e->key()==16777223)
    {
        deleteSelection();
    }

}


void TimelineScene::moveAllFrom(float from, float distance)
{
    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot *>(current);
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
        Shot *rec = dynamic_cast<Shot *>(current);
        if (rec)
            sortedlist.append(current);
    }
    std::sort(sortedlist.begin(), sortedlist.end(), [](QGraphicsItem *a, QGraphicsItem *b){
        return a->scenePos().x()<b->scenePos().x();
    });
    float widthsum = 0;
    foreach (QGraphicsItem *current, sortedlist)
    {
        Shot *rec = dynamic_cast<Shot *>(current);
        rec->animatedMove(widthsum);
        widthsum+=rec->rect().width();
    }
    resetBoxStates();
}


void TimelineScene::setdisp()
{

    if (!selectedItems().isEmpty()){
        resetBoxStates();
        Shot *rec = dynamic_cast<Shot *>(selectedItems().at(0));
        if (rec){
            qDebug()<<"in setdisp";
            rec->mod = BoxState::DISPLACE;
        }
    }

}

void TimelineScene::deleteSelection()
{
    qDebug()<<items();
    emit (deleteSelectionSignal());
//    if(!selectedItems().isEmpty()){
//        if (!selectedItems().isEmpty())
//        {
//            foreach (QGraphicsItem *current, selectedItems()){


////                this->removeItem(current);
////                delete(current);
//            }
//        }
//    }
}

void TimelineScene::displaceSelection(int framePos)
{
    if (!selectedItems().isEmpty()){
        Shot* selection= dynamic_cast<Shot *>(selectedItems().at(0));
        selection->setX(framePos*10);
        handleSelectionMove();
        resetBoxStates();
        update();
    }
}

void TimelineScene::changeSelectionSize(int newSize)
{
    if (!selectedItems().isEmpty()){
        Shot* selection= dynamic_cast<Shot *>(selectedItems().at(0));
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
    ExtendSceneWidth(0);
}
