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
    shot = static_cast<Shot *>(list.first());
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
    setText(QObject::tr("shots movements"));
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
    timeline->setSceneRect(0,0,prevscenesize,100);
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
    timeline->setSceneRect(0,0,currentscenesize,100);
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
    timeline->setSceneRect(0,0,prevtimelinesize,100);
    timeline->ruler.setSize(prevtimelinesize*10);
}


void ClearCommand::redo()
{
    foreach (Shot* current, removedShots)
    {
        timeline->removeItem(current);
    }
    timeline->setSceneRect(0,0, 200000, 300);
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
    timeline->setSceneRect(0,0, 200000, 300);
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
