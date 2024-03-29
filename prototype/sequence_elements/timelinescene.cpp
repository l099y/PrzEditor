﻿#include "timelinescene.h"
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
#include <filesystem/tbesounddata.h>
#include <sequence_elements/ruler.h>
#include <QTime>
#include <QCoreApplication>
#include <QHash>
#include <QJsonArray>
#include <QtMath>
#include <QGraphicsView>
#include <QScrollBar>
#include <QApplication>
#include <QTransform>
#include <QGraphicsEllipseItem>
//#include <QtTest/QTestEventList>


TimelineScene::TimelineScene(SequenceRegister* reg, QGraphicsView* vview, QObject* parent): QGraphicsScene(parent), ruler(0)
{
    ruler.view = vview;
    przreg = reg;
    view = vview;
    ruler.setSize(10400000);
    timerr = new QTimer(this);
    timerr->start(20);
    connect (timerr, SIGNAL(timeout()), this,SLOT( handleViewMoveWithMouse()));

    setSceneRect(0,0, 10000, 400);
    setPreviousToCurrent();
    setItemIndexMethod(QGraphicsScene::ItemIndexMethod(NoIndex));
    QBrush brush(QColor(50,150,200));
    setBackgroundBrush(brush);
    newRect();
    addItem(&ruler);
    QRect c (0, 150,20000000, 10);
    QGraphicsRectItem*  marginbackgrounds = addRect(c, QColor(150,255,180,50), QColor(150,255,180,20));
    QRect a (0, 160,20000000, 100);
    QGraphicsRectItem*  shotsbackground = addRect(a, QColor(255,255,180,50), QColor(255,255,180,50));
    shotsbackground->setZValue(-1);
    QRect b (0, 260,20000000, 20);
    QGraphicsRectItem*  soundbackgrounds = addRect(b, QColor(150,255,180,50), QColor(150,255,180,50));

    connect(this, (SIGNAL(selectionChanged())), this, SLOT(realignSelectionOn260()));
}

TimelineScene::~TimelineScene(){
}

QJsonObject TimelineScene::generateJson()
{
    QJsonArray array;
    QJsonArray soundArray;
    foreach (QGraphicsItem* current, items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        if (sh)
        {
            array.push_back(sh->generateJson());
        }
        else{
            SoundTrack* sound = dynamic_cast<SoundTrack*>(current);
            if (sound){
                soundArray.push_back(sound->generateJson());
            }
        }
    }
    QJsonObject ret;
    ret.insert("shots", array);
    ret.insert("soundtracks", soundArray);
    ret.insert("size", sceneRect().width());
    return ret;
}

//this functions roots to real resizing functions on the selection mod on mouse move

void TimelineScene::handleSelectionMove(float e, bool final){
    if (selectedItems().length()==1 && !isTryingToMultiselect && !MultiSelectingByCtrl){

        Shot* shot= dynamic_cast<Shot*>(selectedItems().at(0));
        SoundTrack* sound = dynamic_cast<SoundTrack*>(selectedItems().at(0));
        // this should be another design pattern to not duplicate all of those... it is noticed but later fixed

        if (shot){
            switch (shot->mod){
            case (BoxState :: REGULAR):
                behaveOnSelectionSwitchPosMove(e, final);
                break;
            case (BoxState ::DISPLACE):
                behaveOnSelectedShotDisplace();
            }
        }
        else if (sound){
            switch (sound->mod){
            case (BoxState :: REGULAR):
                behaveOnSelectionSwitchPosMoveSound(e, final);
                break;
            case (BoxState ::DISPLACE):
                behaveOnSelectedSoundDisplace();
            }
        }

    }
    else if (!MultiSelectingByCtrl){
        foreach(QGraphicsItem* current, selectedItems()){
            auto shot = dynamic_cast<Shot*>(current);
            if (shot)
                shot->restore();
        }
    }
}

void TimelineScene::behaveOnSelectionSwitchPosMove(float e, bool final)
{
    if (selectedItems().length()==1){


        Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));


        selection->setXToFrame(selection->scenePos().x()+selection->rect().width()<sceneRect().width()?
                                   selection->roundedTo10(selection->scenePos().x()<0?0:selection->scenePos().x()):sceneRect().width()-selection->rect().width());
        float sW = selection->rect().width();
        foreach (QGraphicsItem* current, items()){
            Shot* rect= dynamic_cast<Shot*>(current);
            if (rect && rect != selection)
            {
                if (final){
                    rect->timer->stop();
                    rect->setAnimatedFalse();
                    rect->restore();
                    rect->update();
                }
                if (selection->wasLeftOf(rect)){

                    if  (!rect->isMyMiddBefore(e+selection->previousboxwidth-selection->mousePosXonClick))
                    {
                        if (rect->isInMyFirstHalf(e+selection->previousboxwidth-selection->mousePosXonClick)){
                            if (final)
                                selection->setXToFrame(rect->previousxpos-sW) ;
                        }
                        if (!rect->animated && Shot::roundedTo10(rect->scenePos().x())!=rect->previousxpos){
                            final? rect->setXToFrame(rect->previousxpos):rect->animatedMove(rect->previousxpos);
                        }
                    }
                    else {

                        if(!rect->animated && Shot::roundedTo10(rect->scenePos().x())!=rect->previousxpos-sW)
                        {
                            if (rect->previousxpos-sW > 0){
                                final? rect->setXToFrame(rect->previousxpos-sW):rect->animatedMove(rect->previousxpos-sW);
                            }
                            else{
                                final? rect->setXToFrame(0):rect->animatedMove(0);
                            }
                        }
                        if (rect->isInMySecondHalf(e+selection->previousboxwidth-selection->mousePosXonClick)){
                            if (final)
                                selection->setXToFrame(rect->previousboxwidth+rect->previousxpos-sW);
                        }
                    }
                }
                else
                {
                    if (rect->isMyMiddBefore(e-selection->mousePosXonClick))
                    {
                        if (rect->isInMySecondHalf(e-selection->mousePosXonClick)){
                            if (final)
                                selection->setXToFrame(rect->previousboxwidth+rect->previousxpos);
                        }
                        if (!rect->animated && Shot::roundedTo10(rect->scenePos().x())!=rect->previousxpos)
                            final? rect->setXToFrame(rect->previousxpos):rect->animatedMove(rect->previousxpos);
                    }

                    else
                    {
                        if(!rect->animated && Shot::roundedTo10(rect->scenePos().x())!=rect->previousxpos+sW){
                            final? rect->setXToFrame(rect->previousxpos+sW):rect->animatedMove(rect->previousxpos+sW);

                        }
                        if (rect->isInMyFirstHalf(e-selection->mousePosXonClick))
                            if (final)
                                selection->setXToFrame(rect->previousxpos);
                    }
                }
            }
        }
        if (selection != shotDropRepresentation){
            selection->setY(150);
            emit(selectionXChanged());
        }
        if (final)
            if (sceneRect().width() < selection->scenePos().x())
                this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
    }
    else if (selectedItems().length()>1){
        realignSelectionOn260();
    }
}

void TimelineScene::behaveOnSelectionSwitchPosMoveSound(float e, bool final)
{
    SoundTrack* selection = dynamic_cast<SoundTrack*>(selectedItems().at(0));
    int pos = 200000000;
    selection->setY(260);
    foreach (QGraphicsItem* current, items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        if (sh){
            if (e < sh->scenePos().x()+sh->rect().width() && e >=  sh->scenePos().x()){
                pos=sh->scenePos().x();

            }
        }
    }
    if (pos==200000000){
        pos = findClosestShot(e);
    }
    selection->setXToFrame(pos);
}

void TimelineScene::behaveOnSelectedShotDisplace()
{
    Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
    int limit = ShotXAndWBefore(selection);

    selection->setXToFrame(selection->scenePos().x()<limit? limit :
                                                            selection->scenePos().x()>1000000?
                                                                1000000 : selection->scenePos().x());
    selection->setY(160);
    emit(selectionXChanged());


    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot*>(current);
        if (rect && rect != selection && rect->previousxpos>= selection->previousxpos){
            rect->setXToFrame(rect->previousxpos+(selection->scenePos().x()-selection->previousxpos));
        }
    }
    this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),400);
    scaleViewToScene();
}
void TimelineScene::behaveOnSelectedSoundDisplace()
{
    SoundTrack* selection= dynamic_cast<SoundTrack*>(selectedItems().at(0));
    int limit = SoundXAndWBefore(selection);
    selection->setXToFrame(selection->scenePos().x()<limit? limit : selection->scenePos().x());
    selection->setY(260);

    foreach (QGraphicsItem* current, items()){
        SoundTrack* sound = dynamic_cast<SoundTrack*>(current);
        if (sound  && sound  != selection && sound ->previousxpos>= selection->previousxpos){
            sound ->setXToFrame(sound ->previousxpos+(selection->scenePos().x()-selection->previousxpos));
        }

    }
    this->setSceneRect(0,0,previousSceneWidth+(selection->scenePos().x()-selection->previousxpos),100);
}



void TimelineScene::placeInsertedShotInTimeline(float e)
{
    if (shotDropRepresentation->collidingItems().length()!=0){

        // i want to find the information allowing me to place the droprepresentation, and move the appriated sshots in the timeline.

        int maxbeforeInsertMiddle = 0;
        int minafterInsertMiddle = 2000000000;

        foreach (QGraphicsItem* current, items()){
            Shot * sh = dynamic_cast<Shot*>(current);
            if (sh && sh!= shotDropRepresentation)
            {
                if (sh->isMyMiddBefore(e))
                {
                    if (sh->previousxpos+sh->previousboxwidth>maxbeforeInsertMiddle)
                        maxbeforeInsertMiddle=sh->previousxpos+sh->previousboxwidth;
                }
                else
                {
                    if (sh->collidesWithItem(shotDropRepresentation) && sh->previousxpos<minafterInsertMiddle)
                        minafterInsertMiddle=sh->previousxpos;
                }
            }
        }
        float decalage = maxbeforeInsertMiddle-shotDropRepresentation->scenePos().x() ;

        foreach (QGraphicsItem* current, items()){
            Shot * sh = dynamic_cast<Shot*>(current);
            if (sh && sh!= shotDropRepresentation && !sh->isMyMiddBefore(e)){
                sh->setXToFrame( decalage>0? sh->previousxpos+shotDropRepresentation->rect().width(): sh->previousxpos+shotDropRepresentation->rect().width()-decalage);
                sh->setPreviousToCurrent();
            }
        }
    }
}

void TimelineScene::placeInsertedSoundTrackInTimeline(float e)
{
    foreach (QGraphicsItem* current, items()){
        SoundTrack * st = dynamic_cast<SoundTrack*>(current);
        Shot* sh = dynamic_cast<Shot*>(current);
        if (st && st!= soundDropRepresentation)
        {
            soundRemovedByInsertion=st;
            removeItem(st);
        }
        else if (sh){
            if (e < sh->scenePos().x()+sh->rect().width() && e>=  sh->scenePos().x()+sh->rect().width()){

                soundDropRepresentation->setXToFrame(sh->scenePos().x());
            }
        }
    }

    //    if (soundDropRepresentation->collidingItems().length()!=0){

    //        // i want to find the information allowing me to place the droprepresentation, and move the appriated sshots in the timeline.

    //        int maxbeforeInsertMiddle = 0;
    //        int minafterInsertMiddle = 2000000000;

    //        foreach (QGraphicsItem* current, items()){
    //            SoundTrack * sh = dynamic_cast<SoundTrack*>(current);
    //            if (sh && sh!= soundDropRepresentation)
    //            {
    //                if (sh->isMyMiddlePastOrEqual(e))
    //                {
    //                    if (sh->collidesWithItem(soundDropRepresentation) && sh->previousxpos+sh->previousboxwidth>maxbeforeInsertMiddle)
    //                        maxbeforeInsertMiddle=sh->previousxpos+sh->previousboxwidth;
    //                }
    //                else
    //                {
    //                    if (sh->collidesWithItem(soundDropRepresentation) && sh->previousxpos<minafterInsertMiddle)
    //                        minafterInsertMiddle=sh->previousxpos;
    //                }
    //            }
    //        }

    //        // if droprepresentation has to be moved to the right because it colliding with a shot that will remain in its current pos

    //        if (soundDropRepresentation->scenePos().x()<maxbeforeInsertMiddle){

    //            foreach (QGraphicsItem* current, items()){
    //                SoundTrack * sh = dynamic_cast<SoundTrack*>(current);
    //                if (minafterInsertMiddle == 2000000000){


    //                    //if the half of droprepresentation is before the half of current inspected shot

    //                    if (sh && sh!= soundDropRepresentation && !sh->isMyMiddlePastOrEqual(e))
    //                    {
    //                        sh->setXToFrame(sh->previousxpos+shotDropRepresentation->rect().width());
    //                        sh->setPreviousToCurrent();
    //                    }
    //                }
    //                else{
    //                    int decalage = (soundDropRepresentation->previousboxwidth+maxbeforeInsertMiddle)-minafterInsertMiddle;
    //                    soundDropRepresentation->setXToFrame(maxbeforeInsertMiddle);
    //                    if (sh && sh!= soundDropRepresentation && sh->posOfMidd()>=soundDropRepresentation->posOfMidd())
    //                    {
    //                        sh->setXToFrame(sh->previousxpos+decalage);
    //                        sh->setPreviousToCurrent();
    //                    }

    //                }

    //            }


    //        }
    //        else if (soundDropRepresentation->previousxpos+soundDropRepresentation->previousboxwidth>minafterInsertMiddle){
    //            int decalage = minafterInsertMiddle-(soundDropRepresentation->previousboxwidth+soundDropRepresentation->previousxpos);
    //            foreach (QGraphicsItem* current, items()){
    //                SoundTrack * sh = dynamic_cast<SoundTrack*>(current);

    //                //if the half of droprepresentation is before the half of current inspected shot

    //                if (sh && sh!= soundDropRepresentation && sh->posOfMidd()>=soundDropRepresentation->posOfMidd())
    //                {
    //                    sh->setXToFrame(sh->previousxpos-decalage);
    //                    sh->setPreviousToCurrent();
    //                }
    //            }

    //        }
    //        soundDropRepresentation->setPreviousToCurrent();
    //    }
}



void TimelineScene::resetShotsDisplacedByInsertion()
{

    QHash<Shot*, int>::const_iterator i = imageOfShotsPositions.constBegin();
    while (i != imageOfShotsPositions.constEnd()) {
        auto  sh =i.key();
        sh->previousxpos = i.value();
        sh->timer->stop();
        sh->setXToFrame(sh->previousxpos);
        sh->setPreviousToCurrent();
        sh->setAnimatedFalse();
        i++;
    }
    imageOfShotsPositions.clear();
}
void TimelineScene::resetSoundsDisplacedByInsertion()
{
    if (soundRemovedByInsertion!=nullptr)
        addItem(soundRemovedByInsertion);
    soundRemovedByInsertion=nullptr;
    //    QHash<SoundTrack*, int>::const_iterator i = imageOfSoundPositions.constBegin();
    //    while (i != imageOfSoundPositions.constEnd()) {
    //        auto  sh =i.key();
    //        sh->previousxpos = i.value();
    //        sh->timer->stop();
    //        sh->setXToFrame(sh->previousxpos);
    //        sh->setPreviousToCurrent();
    //        sh->setAnimatedFalse();
    //        i++;
    //    }
    //    imageOfSoundPositions.clear();
}

void TimelineScene::resetShotsDisplacedFinal()
{
    QHash<Shot*, int>::const_iterator i = imageOfShotsPositions.constBegin();
    while (i != imageOfShotsPositions.constEnd()) {
        auto  sh =i.key();
        sh->previousxpos = i.value();
        i++;
    }
    imageOfShotsPositions.clear();
}

void TimelineScene::loadJSONfromSave(QJsonObject obj)
{
    setSceneRect(0,0,obj.value("size").toInt(), 400);

    QJsonArray shots = obj.value("shots").toArray();
    foreach (QJsonValue current, shots){
        auto obj =  current.toObject();
        Shot* shotToBeInsert = new Shot(obj);
        foreach (QJsonValue currentseq, obj.value("sequences").toArray()){
            auto curseq = currentseq.toObject();

            SequenceData* sq = new SequenceData(curseq, nullptr);
            shotToBeInsert->seqs.append(sq);
        }
        addItem(shotToBeInsert);
    }

    QJsonArray sounds = obj.value("soundtracks").toArray();
    foreach (QJsonValue current, sounds){
        auto obj =  current.toObject();
        SoundTrack* soundToBeInsert = new SoundTrack(obj);
        auto currentsoundfile = obj.value("soundfile").toObject();
        auto sf = new TbeSoundData(currentsoundfile);
        soundToBeInsert->soundfile = sf;
        przreg->usedSoundFiles.insert(sf->filename, sf);
        addItem(soundToBeInsert);
    }
    validateDataIntegrity();
    scaleViewToScene();
}

int TimelineScene::findLastXWShot()
{
    int min = 0;
    foreach (QGraphicsItem* current, items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        if (sh && sh->scenePos().x()+sh->rect().width() > min)
            min =  sh->scenePos().x()+sh->rect().width();
    }
    return min;
}

int TimelineScene::findLastXWSound()
{
    int min = 0;
    foreach (QGraphicsItem* current, items()){
        SoundTrack* sh = dynamic_cast<SoundTrack*>(current);
        if (sh && sh->scenePos().x()+sh->rect().width() > min)
            min =  sh->scenePos().x()+sh->rect().width();
    }
    return min;
}

void TimelineScene::resetSoundsDisplacedFinal()
{
    QHash<SoundTrack*, int>::const_iterator i = imageOfSoundPositions.constBegin();
    while (i != imageOfSoundPositions.constEnd()) {
        auto  sh =i.key();
        sh->previousxpos = i.value();
        i++;
    }
    imageOfSoundPositions.clear();
}

void TimelineScene::generateImageOfShotsPositions()
{
    foreach (QGraphicsItem *current, items()){
        Shot *shot = dynamic_cast<Shot*>(current);
        if (shot && shot!= shotDropRepresentation)
            imageOfShotsPositions.insert(shot, shot->previousxpos);
    }
}

void TimelineScene::generateImageOfSoundsPositions()
{
    foreach (QGraphicsItem *current, items()){
        SoundTrack* sound = dynamic_cast<SoundTrack*>(current);
        if (sound && sound!= soundDropRepresentation)
            imageOfSoundPositions.insert(sound, sound->previousxpos);
    }
}



void TimelineScene::setPreviousToCurrent()
{
    previousSceneWidth = sceneRect().width();
}

void TimelineScene::realignSelectionOn260()
{
    foreach (QGraphicsItem *current, selectedItems()){
        Shot *shot = dynamic_cast<Shot*>(current);
        if (shot)
            shot->setY(160);
    }
}

void TimelineScene::displaceView(bool toLeft)
{
    if (this->view[0].horizontalScrollBar()->isEnabled())
    {
        if (toLeft)
        this->view[0].horizontalScrollBar()->setValue(this->view[0].horizontalScrollBar()->value()-(600/(1/ruler.scale)));
        else
        this->view[0].horizontalScrollBar()->setValue(this->view[0].horizontalScrollBar()->value()+(600/(1/ruler.scale)));
    }
//    this->view[0].setTransformationAnchor(QGraphicsView::NoAnchor);
//    if (visible_scene_rect.x()-100>=0)
//        this->view[0].translate(100, 0);
//    else
//       {
//        this->view[0].translate(visible_scene_rect.x(), 0);
//    }
//    this->view[0].setTransformationAnchor(QGraphicsView::AnchorViewCenter);

}

void TimelineScene::handleViewMoveWithMouse()
{
    if (moveToLeftView)
    displaceView(true);
    else if (moveToRightView)
    displaceView(false);
    //qDebug()<<"in handleViewMove()";
}

float TimelineScene::ShotXAndWBefore(Shot *rect)
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
float TimelineScene::SoundXAndWBefore(SoundTrack *rect)
{
    QVector<SoundTrack*> sortedlist;
    foreach (QGraphicsItem *current, items()){
        SoundTrack *rec = dynamic_cast<SoundTrack*>(current);
        if (rec)
            sortedlist.append(rec);
    }
    std::sort(sortedlist.begin(), sortedlist.end(), [](SoundTrack *a, SoundTrack *b){


        return a->previousxpos<b->previousxpos;
    });
    float ret= 0;
    foreach (SoundTrack *current, sortedlist)
    {
        if (rect->previousxpos>current->previousxpos && current != rect )
            ret = current->previousboxwidth+current->previousxpos;
    }
    return ret;
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
    align();
}
void TimelineScene :: clearItems(){
    QVector<Shot*> ret;
    foreach (QGraphicsItem* current, items()){
        Shot* shot= dynamic_cast<Shot*>(current);
        {
            if (shot){
                ret.append(shot);
            }
        }
    }
    emit (clearTimeline(this, ret, sceneRect().width()));
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QRect viewport_rect(0, 0, view->viewport()->width(), view->viewport()->height());
    QRectF visible_scene_rect = view->mapToScene(viewport_rect).boundingRect();
    if (e->scenePos().x()>= visible_scene_rect.x() && e->scenePos().x()<visible_scene_rect.x()+visible_scene_rect.width()/20){
        moveToLeftView = true;

    }
    else if (e->scenePos().x()< visible_scene_rect.x()+visible_scene_rect.width() && e->scenePos().x()>= (visible_scene_rect.x()+visible_scene_rect.width())-visible_scene_rect.width()/20){
        moveToRightView = true;
    }
    else
    {
        moveToLeftView = false;
        moveToRightView = false;
    }

    if (selectedItems().length()==1 && e->buttons()==Qt::LeftButton && !MultiSelectingByCtrl){
        QGraphicsScene :: mouseMoveEvent (e);
        handleSelectionMove(e->scenePos().x(), false);
    }
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouseReleaseEvent";
    if (selectedItems().size()==1){
        Shot* selection= dynamic_cast<Shot*>(selectedItems().at(0));
        SoundTrack* soundtrack = dynamic_cast<SoundTrack*>(selectedItems().at(0));
        if (selection)
        {
            handleSelectionMove(e->scenePos().x(), true);
            //resetBoxStates();
            if (selection->scenePos().x()!=selection->previousxpos){
                emit (moveShots(this,  getMovedShots(), previousSceneWidth, this->sceneRect().width()));
                previousSceneWidth = this->width();
            }

        }
        else if (soundtrack){
            handleSelectionMove(e->scenePos().x(), true);
            //resetBoxStates();
            if (soundtrack->scenePos().x()!=soundtrack->previousxpos){
                emit (moveSoundtracks(this,  getMovedSounds(), previousSceneWidth, this->sceneRect().width()));
                previousSceneWidth = this->width();
            }
        }
    }
    isTryingToMultiselect=false;
    QGraphicsScene :: mouseReleaseEvent(e);
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

void TimelineScene::resetToPrevious(QString type)
{
    if (type=="shot")
        foreach (QGraphicsItem* current, items()){
            Shot* rect= dynamic_cast<Shot *>(current);
            if (rect)
            {
                rect->setX(rect->previousxpos);
            }
        }
    else if (type=="sound")
        foreach (QGraphicsItem* current, items()){
            SoundTrack* sound= dynamic_cast<SoundTrack *>(current);
            if (sound)
            {
                sound->setX(sound->previousxpos);
            }
        }
}
void TimelineScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    auto parent = (QTreeView*)e->source();
    QApplication::restoreOverrideCursor();
    if (parent){
        e->accept();
        qDebug()<<"accepted draenter";
    }
    else
    {
        qDebug()<<"ignore draenter";
        e->ignore();
    }
    auto file = parent->selectionModel()->selectedIndexes()[0].data().toString();
    auto path =  parent->model()->headerData(0, Qt::Horizontal, 0).toString();

    if (file.right(3)=="tbe") // this evaluation should be done differently
    {
            clearSelection();
        auto list = przreg->currentExpandedFolderSounds->value(path);
        foreach (TbeSoundData* current, list){
            if(current->filename == file){
                soundDropRepresentation = new SoundTrack ();
                soundDropRepresentation->setRect(0,0,100000000,20);
                soundDropRepresentation->setX(e->scenePos().x());
                if (!selectedItems().isEmpty()){
                    QGraphicsItem* selection= selectedItems().at(0);
                    selection->setSelected(false);
                }
                soundDropRepresentation->setSelected(true);
                soundDropRepresentation->setPreviousToCurrent();
                soundDropRepresentation->soundfile = current;
                qDebug()<<current->filename << "inserted in dragenter sound" << current->path;
                addItem(soundDropRepresentation);
            }
        }
    }
    else if (file.right(3)=="prz"){
        auto list = przreg->currentExpandedFolderBackground->value(path);
        foreach (BackgroundPrz* current, list){
            if(current->filename == file){
                backgroundDropRepresentation = current;
                qDebug()<<current->filename << "inserted in dragenter sound" << current->path;
            }
        }
        qDebug()<<"trying to insert background";
    }
    else{
            clearSelection();
        auto list = przreg->currentExpandedFolderSequences->value(path);
        foreach (SequenceData* current, list){
            if(current->name == file){
                if (validateInsertionSizeCap(current->sequencelength()*10)){
                    shotDropRepresentation = new Shot ();
                    shotDropRepresentation->setRect(0,0,current->sequencelength()*10,100);
                    shotDropRepresentation->setX(e->scenePos().x());
                    if (!selectedItems().isEmpty()){
                        clearSelection();
                    }
                    shotDropRepresentation->setSelected(true);
                    shotDropRepresentation->setPreviousToCurrent();
                    shotDropRepresentation->mousePosXonClick=0;
                    shotDropRepresentation->seqs.append( current );
                    qDebug()<<current->name << "inserted in dragenter shot";
                    addItem(shotDropRepresentation);
                }

                else{
                    e->ignore();
                    emit(displayError("this insertion exceeds the limit of frames", 3000));
                }
            }
        }

    }

    QGraphicsScene :: dragEnterEvent(e);

}

void TimelineScene::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
    if (shotDropRepresentation != nullptr){
        if (e->scenePos().y()<60 || e->scenePos().y()>260){
            if (shotDropRepresentation->inserted){
                shotDropRepresentation->inserted=false;
                resetShotsDisplacedByInsertion();
                ExtendSceneWidth(-shotDropRepresentation->rect().width());

            }
            shotDropRepresentation->setXToFrame(e->scenePos().x());
            shotDropRepresentation->setY(e->scenePos().y());
        }
        else
        {
            if (!shotDropRepresentation->inserted){
                generateImageOfShotsPositions();
                shotDropRepresentation->setXToFrame(e->scenePos().x());
                shotDropRepresentation->scenePos().setY(160);
                shotDropRepresentation->setPreviousToCurrent();
                shotDropRepresentation->inserted=true;
                placeInsertedShotInTimeline(e->scenePos().x());
                ExtendSceneWidth(shotDropRepresentation->rect().width());
            }
            else{
                shotDropRepresentation->setXToFrame(e->scenePos().x());
                shotDropRepresentation->scenePos().setY(160);
                behaveOnSelectionSwitchPosMove(e->scenePos().x(), false);
            }
        }
    }
    else if (soundDropRepresentation != nullptr)
    {
        if (e->scenePos().y()<220| e->scenePos().y()>300){
            if (soundDropRepresentation->inserted){
                soundDropRepresentation->setXToFrame(e->scenePos().x());
                soundDropRepresentation->scenePos().setY(360);
                soundDropRepresentation->inserted=false;
                resetSoundsDisplacedByInsertion();
            }
            soundDropRepresentation->setXToFrame(e->scenePos().x());
            soundDropRepresentation->setY(e->scenePos().y());
        }
        else
        {
            if (!soundDropRepresentation->inserted){
                generateImageOfSoundsPositions();
                soundDropRepresentation->setXToFrame(e->scenePos().x());
                soundDropRepresentation->setY(260);
                soundDropRepresentation->setPreviousToCurrent();
                soundDropRepresentation->inserted=true;
                placeInsertedSoundTrackInTimeline(e->scenePos().x());
            }
            else{
                soundDropRepresentation->setXToFrame(e->scenePos().x());
                soundDropRepresentation->setY(260);
                behaveOnSelectionSwitchPosMoveSound(e->scenePos().x(), false);
            }
        }
    }
    else if (backgroundDropRepresentation != nullptr)
    {
        handleBackgroundDropMouvement(e, false);
    }

}

void TimelineScene::handleBackgroundDropMouvement(QGraphicsSceneDragDropEvent *e, bool final)
{
    bool isSelected = false;
    if (e->scenePos().y()>160 && e->scenePos().y()<260)
    {
        foreach (QGraphicsItem* current, items()){
            Shot* sh = dynamic_cast<Shot*>(current);
            if (sh){
                if (e->scenePos().x() > sh->scenePos().x() && e->scenePos().x() < sh->scenePos().x()+sh->rect().width()){
                    if (final){
                         if (sh->isSelected()){
                             isSelected = true;
                         }
                         else
                        emit(addBackgroundToShot({sh}, backgroundDropRepresentation));
                    }
                    else
                    {
                        if (sh->isSelected()){
                            isSelected = true;
                        }
                        else
                        sh->tempBackground = backgroundDropRepresentation;
                    }

                }
                else {
                    sh->tempBackground = nullptr;
                }
                 sh->update();
            }
        }
        if (isSelected){
            if (final){
                QList<Shot*> changedBgShots;
                foreach (QGraphicsItem* current, selectedItems())
                {
                    Shot* sh = dynamic_cast<Shot*>(current);
                    if (sh){
                        changedBgShots.append(sh);
                    }
                }
                emit (addBackgroundToShot(changedBgShots, backgroundDropRepresentation));
            }
            else{
                foreach (QGraphicsItem* current, selectedItems())
                {
                    Shot* sh = dynamic_cast<Shot*>(current);
                    if (sh){
                        sh->tempBackground=backgroundDropRepresentation;
                    }
                }
            }
        }
    }
    else{
        foreach (QGraphicsItem* current, items()){
            Shot* sh = dynamic_cast<Shot*>(current);
            if (sh){
                 sh->tempBackground = nullptr;
                 sh->update();

            }
        }
    }
    if (final)
        backgroundDropRepresentation = nullptr;
}


QVector<Shot *> TimelineScene::getMovedShots()
{
    QVector<Shot*> ret;
    foreach (QGraphicsItem* current, items()){
        Shot* rect= dynamic_cast<Shot*>(current);
        if (rect && rect != shotDropRepresentation)
        {
            if (rect->scenePos().x() != rect->previousxpos)
                ret.append(rect);
        }
    }
    return ret;
}
QVector<SoundTrack *> TimelineScene::getMovedSounds()
{
    QVector<SoundTrack*> ret;
    foreach (QGraphicsItem* current, items()){
        SoundTrack* sound= dynamic_cast<SoundTrack*>(current);
        if (sound && sound != soundDropRepresentation)
        {
            if (sound->scenePos().x() != sound->previousxpos)
                ret.append(sound);
        }
    }
    return ret;
}

float TimelineScene::findClosestShot(float e)
{
    int dist = 20000000;
    int xpos = 0;
    foreach (QGraphicsItem* current, items()){
        Shot* sh= dynamic_cast<Shot*>(current);
        if (sh)
        {
            if (sh->scenePos().x()>e){
                if (qFabs(sh->scenePos().x()-e)<dist){
                    dist = sh->scenePos().x()-e;
                    xpos = sh->scenePos().x();
                }
            }
            else{
                if (qFabs(e-(sh->scenePos().x()+sh->rect().width()))<dist){
                    dist = e-(sh->scenePos().x()+sh->rect().width());
                    xpos = sh->scenePos().x()+sh->rect().width();
                }
            }
        }
    }
    if (dist ==  20000000)
        return 0;
    else
        return xpos;
}


bool TimelineScene::validateParameterTargetChange()
{
    return (selectedItems().length() != 0 && selectedItems().at(0)!= shotDropRepresentation && selectedItems().at(0)!= soundDropRepresentation);
}

bool TimelineScene::validateInsertionSizeCap(int newRectWidth)
{
    int sizecap = newRectWidth;
    foreach (QGraphicsItem* current, items())
    {
        auto sh = dynamic_cast<Shot*>(current);
        if(sh)
        {
            sizecap+= sh->rect().width();
        }
    }
    return sizecap<800000;
}

QList<Shot *> TimelineScene::getShotsPastSelection()
{
    int max = 0;
    QList<Shot*> ret;
    foreach (QGraphicsItem* current, selectedItems())
    {
        auto sh = dynamic_cast<Shot*>(current);
        if (sh)
        {
            if (sh->scenePos().x()>max)
                max = sh->scenePos().x();
        }
    }
    foreach (QGraphicsItem* current, items())
    {
        auto sh = dynamic_cast<Shot*>(current);
        if (sh && !sh->isSelected())
        {
            if (sh->scenePos().x()>max)
                ret.append(sh);
        }
    }
    return ret;
}

QRectF TimelineScene::getVisibleRect()
{

    QPointF A = view->mapToScene( QPoint(0, 0) );
    QPointF B = view->mapToScene( QPoint(
                                      view->viewport()->width(),
                                      view->viewport()->height() ));
    return QRectF( A, B );

}


bool TimelineScene::validateDataIntegrity()
{
    qDebug()<<"PARSSINNNG";
    bool ret = true;
    foreach(QGraphicsItem* current, items()){
        auto sh = dynamic_cast<Shot*>(current);
        auto sound = dynamic_cast<SoundTrack*>(current);
        if (sh){
            if (!sh->seqs[0]->checkIntegrity())
                ret = false;
        }
        else if (sound){
            if (!sound->soundfile->checkIntegrity())
                ret=false;
        }
    }
    ExtendSceneWidth(0);
    return ret;
}

void TimelineScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{

    if (shotDropRepresentation != nullptr){
        if (shotDropRepresentation->inserted)
        {
            behaveOnSelectionSwitchPosMove(e->scenePos().x(), true);
            removeItem(shotDropRepresentation);
            qDebug()<<"should be created";
            resetShotsDisplacedFinal();
            auto list = getMovedShots();
            //resetBoxStates();
            ExtendSceneWidth(-shotDropRepresentation->rect().width());
            emit (createShot(shotDropRepresentation->seqs, shotDropRepresentation->scenePos().x(), shotDropRepresentation->rect().width(), this, list));
            shotDropRepresentation =  nullptr;
        }
        else
        {
            removeItem(shotDropRepresentation);
            resetShotsDisplacedByInsertion();
            resetBoxStates();
            shotDropRepresentation =  nullptr;

        }
    }
    else if (soundDropRepresentation != nullptr){
        if (soundDropRepresentation->inserted)
        {
            behaveOnSelectionSwitchPosMoveSound(e->scenePos().x(), true);
            removeItem(soundDropRepresentation);
            resetSoundsDisplacedFinal();
            auto list = getMovedSounds();
            //resetBoxStates();
            qDebug()<<soundDropRepresentation->soundfile->filename << "inserted in dragenter sound" << soundDropRepresentation->soundfile->path;
            emit (babar(soundRemovedByInsertion, soundDropRepresentation->soundfile,soundDropRepresentation->scenePos().x(), soundDropRepresentation->rect().width(), this, list));//,
            //
            soundRemovedByInsertion = nullptr;
            soundDropRepresentation =  nullptr;
        }
        else
        {
            removeItem(soundDropRepresentation);
            resetSoundsDisplacedByInsertion();
            resetBoxStates();
            soundDropRepresentation =  nullptr;

        }
    }
    else if (backgroundDropRepresentation != nullptr){
        handleBackgroundDropMouvement(e, true);
    }
    QGraphicsScene :: dropEvent(e);
}

void TimelineScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *e)
{
    if (soundDropRepresentation!=nullptr){
        resetSoundsDisplacedByInsertion();
        removeItem(soundDropRepresentation);
        soundDropRepresentation=nullptr;
        resetToPrevious("sound");
    }
    else if (shotDropRepresentation != nullptr){
        resetShotsDisplacedByInsertion();
        removeItem(shotDropRepresentation);
        if (shotDropRepresentation->inserted)
            setSceneRect(0,0, sceneRect().width() - shotDropRepresentation->rect().width(), 400);
        shotDropRepresentation=nullptr;
        resetToPrevious("shot");
    }
    else if (backgroundDropRepresentation != nullptr){
        backgroundDropRepresentation = nullptr;
    }
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
    else if(e->key()==16777249){
        MultiSelectingByCtrl=true;
        qDebug()<<"ctrl pressed";
    }
    else if(e->key()==16777221){
        foreach (SequenceData* seq, przreg->usedSequences)
            qDebug()<<seq->name<< "name "<<seq->padding;
        foreach (TbeSoundData* seq, przreg->usedSoundFiles)
            qDebug()<<seq->filename<< "name "<<seq->path;
    }
    else if (e->key()==16777236){
        displaceView(false);
    }
    else if (e->key()==16777234)
        displaceView(true);

}

void TimelineScene::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==16777249){
        MultiSelectingByCtrl=false;
        qDebug()<<"ctrl released";
    }
}


void TimelineScene::moveAllFrom(float from, float distance, QString type)
{
    foreach (QGraphicsItem* current, items()){
        Shot* shot= dynamic_cast<Shot *>(current);
        SoundTrack* sound = dynamic_cast<SoundTrack*>(current);
        if ((shot && type == "shot")||(sound && type == "sound")){
            if (shot->scenePos().x()>=from){
                shot->setX(shot->scenePos().x()+distance);
            }
        }
    }
}


void TimelineScene::ExtendSceneWidth(float f)
{
    this->setSceneRect(0,0,this->width()+f, 100);
    previousSceneWidth = this->width();
}


void TimelineScene::align()
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
    QTime dieTime= QTime::currentTime().addMSecs(250);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    emit (moveShots(this, getMovedShots(), previousSceneWidth, previousSceneWidth));
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
}

void TimelineScene::displaceSelection(int framePos, QString type)
{
    if (!selectedItems().isEmpty()){

        Shot* shot= dynamic_cast<Shot *>(selectedItems().at(0));
        SoundTrack* sound = dynamic_cast<SoundTrack*>(selectedItems().at(0));

        if (shot){
            if (shot->validatePosChange(framePos)){
                shot->setPreviousToCurrent();
                shot->setX(framePos);
                QVector<Shot*> moved;
                moved.append(shot);
                emit (moveShots(this,  moved, previousSceneWidth, this->sceneRect().width()));
            }
            else{
                emit (displayError("Invalid position, this shot will collide with other shots", 3000));
            }
        }

        else if (sound){
            if (sound->validatePosChange(framePos)){
                sound->setPreviousToCurrent();
                sound->setX(framePos*10);
                QVector<SoundTrack*> moved;
                moved.append(sound);
                emit (moveSoundtracks(this,moved, previousSceneWidth, this->sceneRect().width()));
            }
            else{
                emit (displayError("Invalid position, this sounds will collide with other sounds", 10000));
            }
        }
    }
}

void TimelineScene::changeSelectionSize(int newSize, QString type)
{
    if (!selectedItems().isEmpty()){
        Shot* shot= dynamic_cast<Shot *>(selectedItems().at(0));
        SoundTrack* sound = dynamic_cast<SoundTrack*>(selectedItems().at(0));

        if (shot){
            shot->setSize(newSize*10);
            moveAllFrom(shot->previousxpos+1, newSize*10 - shot->previousboxwidth, type);
            emit (resizeShot(this, getMovedShots(),shot, shot->rect().width(), this->sceneRect().width()));

        }
        else if (sound){
            if (sound->validateSizeChange(newSize *10)){
                sound->setSize(newSize *10);
                moveAllFrom(shot->previousxpos+1, newSize*10 - sound->previousboxwidth, type);
                emit (resizeSound(this, getMovedSounds(),sound, sound->rect().width(), this->sceneRect().width()));

            }
            else{
                emit (displayError("Invalid size, this shot contains shorter soundfile", 10000));
            }
        }
    }
}

void TimelineScene::insertShotAtEnd(QList<SequenceData*> list)
{
    qDebug()<<"insertshot at the end";
    if (validateInsertionSizeCap(list[0]->sequencelength()*10)){
        emit (createShot(list, findLastXWShot(), list[0]->sequencelength()*10, this, {}));
    }
    else{
        emit(displayError("this insertion exceeds the limit of frames", 3000));
    }
}
void TimelineScene::insertSoundAtEnd(TbeSoundData* soundfile)
{
    SoundTrack* suppressedSound = nullptr;
    foreach (QGraphicsItem* current, items()){
        SoundTrack* sh = dynamic_cast<SoundTrack*>(current);
        if (sh){
            suppressedSound = sh;
        }
    }
    emit (babar(suppressedSound, soundfile, 0, 1000000, this, {}));
}

void TimelineScene::refreshRuler(int)
{



    auto timelineView = views()[0];

    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());
    QRectF visible_scene_rect = timelineView->mapToScene(viewport_rect).boundingRect();


    qDebug()<< (getVisibleRect().x() + visible_scene_rect.width());
    ruler.update();
}

void TimelineScene::scaleViewToScene()
{
    emit (scaleToViewRequest());
}

void TimelineScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    //qDebug()<<"mousepressed";e->ignore();
    QGraphicsScene::mousePressEvent(e);
    if (selectedItems().length()==0){
        qDebug()<<"trying to multiselect";
        isTryingToMultiselect = true;
    }
}
void TimelineScene :: newRect(){
    ExtendSceneWidth(0);
    qDebug()<<generateJson();
    qDebug()<<"sequences"<<endl<<przreg->usedSequences;
}

