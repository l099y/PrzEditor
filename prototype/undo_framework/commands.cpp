#include "commands.h"
#include "sequence_elements/shot.h"
#include "sequence_elements/timelinescene.h"
#include "filesystem/sequencedata.h"
#include <QDebug>
#include <QDataStream>
#include <QHash>
#include <mainwindow.h>

DeleteCommand::DeleteCommand(TimelineScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    timeline = scene;
    QList<QGraphicsItem *> list = timeline->selectedItems();
    list.first()->setSelected(false);
    shot = list.first();
    setText(QObject::tr("Delete %1")
            .arg(" created "));
}

void DeleteCommand::undo()
{
    timeline->addItem(shot);
    timeline->update();
}

void DeleteCommand::redo()
{
    timeline->removeItem(shot);
}


AddCommand::AddCommand(QList<SequenceData*> seq, int xpos, int length, TimelineScene *scene, QVector<Shot*> movedShots ,QUndoCommand *parent)
    : QUndoCommand(parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->previousxpos);
        movedShotNewPos.insert(current, current->scenePos().x());
    }
    timeline = scene;
    shot = new Shot();

    this->seqs = seq;
    this->xpos = xpos;
    this->length = length;
    shot->seqs.append(seqs[0]);

    QJsonObject newparam = shot->templateParams.value("Shot designation");
    newparam.insert("value", seqs[0]->name);
    shot->templateParams.insert("Shot designation", newparam);


    scene->update();
    setText(QObject::tr("Add %1")
            .arg(seqs[0]->name));
}

AddCommand::~AddCommand()
{
    if (!shot->scene())
        delete shot;
}

void AddCommand::undo()
{
    timeline->removeItem(shot);
    timeline->update();
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
}

void AddCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->addItem(shot);
    timeline->przreg->usedSequences.insert(seqs[0]->name, seqs[0]);
    shot->setXToFrame(xpos);
    shot->setRect(0, 0, length, 100);
    shot->setY(160);
    qDebug()<<shot->rect() << "in redo"<< shot->scenePos().y();;
    shot->setPreviousToCurrent();
    timeline->clearSelection();
    shot->setSelected(true);
    timeline->update();
}

MoveCommand::MoveCommand(TimelineScene* timeline, QVector<Shot *> movedShots, int prevscenepos, int currentscenepos, QUndoCommand *parent) : QUndoCommand(parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->scenePos().x());
        movedShotNewPos.insert(current, current->previousxpos);
    }
    this->timeline = timeline;
    this->prevscenesize = prevscenepos;
    this->currentscenesize = currentscenepos;
    setText(QObject::tr("shots mouvements"));
}

void MoveCommand::undo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,prevscenesize,400);
}

void MoveCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,currentscenesize,400);
}



ClearCommand::ClearCommand(TimelineScene* timeline, QVector<Shot *> removedshots, int prevtimelinewidth, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->timeline = timeline;
    this->removedShots = removedshots;
    this->prevtimelinesize = prevtimelinewidth;
    setText(QObject::tr("clear command"));
}

void ClearCommand::undo()
{
    foreach (Shot* current, removedShots){
        timeline->addItem(current);
    }
    timeline->setSceneRect(0,0,prevtimelinesize,400);
    timeline->ruler.setSize(prevtimelinesize*10);
}


void ClearCommand::redo()
{
    foreach (Shot* current, removedShots)
    {
        timeline->removeItem(current);
    }
    timeline->setSceneRect(0,0, 200000, 400);
    timeline->ruler.setSize(2000000);
    timeline->newRect();
}

LoadCommand::LoadCommand(TimelineScene* timeline, QString path, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->timeline = timeline;
    timelinesize = this->timeline->sceneRect().width();
    foreach (QGraphicsItem* current, this->timeline->items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        if (sh){
            shots.append(sh);
        }
    }
    setText(QObject::tr("load %1").arg(path));
}

void LoadCommand::undo()
{
    foreach (QGraphicsItem* current, timeline->items())
    {
        auto sh = dynamic_cast<Shot*>(current);
        if (sh){
            timeline->removeItem(sh);
        }
    }
    timeline->setSceneRect(0,0, 200000, 400);
    timeline->ruler.setSize(2000000);
    timeline->przreg->usedSequences.clear();
    timeline->newRect();
}
void LoadCommand::redo(){

    foreach (Shot* current, shots){
        timeline->addItem(current);
    }
}

ChangeParameterInShotCommand::ChangeParameterInShotCommand(MainWindow* app, Shot * shot, QJsonObject obj, QUndoCommand *parent) : QUndoCommand(parent)
{
   this->app = app;
   sh = shot;
   QString name = obj.value("name").toString();
   oldconfig = shot->templateParams.value(name);
   newconfig = obj;
   setText(QObject::tr("change parameter %1").arg(name));
}

void ChangeParameterInShotCommand::undo()
{
   QString name = oldconfig.value("name").toString();
   sh->templateParams.insert(name, oldconfig);
   sh->scene()->clearSelection();
   sh->setSelected(true);
   sh->update();
}

void ChangeParameterInShotCommand::redo()
{
    QString name = oldconfig.value("name").toString();
    sh->templateParams.insert(name, newconfig);
    sh->scene()->clearSelection();
    sh->setSelected(true);
    sh->update();
}



AddSoundCommand::AddSoundCommand(TbeSoundData * sounddata, int xpos, int length, TimelineScene *timeline, QVector<SoundTrack * > movedSounds, QUndoCommand *parent)
{
    foreach(SoundTrack* current, movedSounds){
        movedSoundsOldPos.insert(current, current->previousxpos);
        movedSoundsNewPos.insert(current, current->scenePos().x());
    }
    this->timeline = timeline;
    sound = new SoundTrack();
    this->sounddata = sounddata;
    this->xpos = xpos;
    this->length = length;

    setText(QObject::tr("Add %1")
            .arg(sounddata->filename));
}

AddSoundCommand::~AddSoundCommand()
{
    if (!sound->scene())
        delete sound;
}

void AddSoundCommand::undo()
{
    timeline->removeItem(sound);
    timeline->update();
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsOldPos.constBegin();
    while (i != movedSoundsOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
}

void AddSoundCommand::redo()
{

    QHash<SoundTrack*, int>::const_iterator i = movedSoundsNewPos.constBegin();
    while (i != movedSoundsNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->addItem(sound);
    sound->setXToFrame(xpos);
    sound->setRect(0, 0, length, 20);
    sound->setY(260);
    sound->setPreviousToCurrent();
    timeline->clearSelection();
    sound->setSelected(true);
    timeline->update();
}

MoveSoundsCommand::MoveSoundsCommand(TimelineScene* timeline, QVector<SoundTrack *> movedShots, int prevscenepos, int currentscenepos, QUndoCommand *parent) : QUndoCommand(parent)
{
    foreach(SoundTrack* current, movedShots){
        movedSoundsOldPos.insert(current, current->scenePos().x());
        movedSoundsNewPos.insert(current, current->previousxpos);
    }
    this->timeline = timeline;
    this->prevscenesize = prevscenepos;
    this->currentscenesize = currentscenepos;
    setText(QObject::tr("soundtracks mouvements"));
}

void MoveSoundsCommand::undo()
{
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsNewPos.constBegin();
    while (i != movedSoundsNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,prevscenesize,100);
}

void MoveSoundsCommand::redo()
{
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsOldPos.constBegin();
    while (i != movedSoundsOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,currentscenesize,100);
}

ResizeShotCommand::ResizeShotCommand(TimelineScene *timeline, QVector<Shot *> movedShots, Shot* shot,int newShotSize, int previoustimelineWidth, int timelineWidth, QUndoCommand *parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->scenePos().x());
        movedShotNewPos.insert(current, current->previousxpos);
    }
    this->resizedShot = shot;
    this->timeline = timeline;
    this->prevscenesize = previoustimelineWidth;
    this->currentscenesize = timelineWidth;
    this->previousShotWidth = shot->previousboxwidth;
    this->newShotWidth = newShotSize;
    setText(QObject::tr("shot resize"));

}

void ResizeShotCommand::undo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    resizedShot->setSize(previousShotWidth);
    resizedShot->setPreviousToCurrent();
    timeline->setSceneRect(0,0,prevscenesize,400);
}

void ResizeShotCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    resizedShot->setSize(newShotWidth);
    resizedShot->setPreviousToCurrent();
    timeline->setSceneRect(0,0,currentscenesize,400);
}
