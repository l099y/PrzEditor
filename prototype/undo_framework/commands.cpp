#include "commands.h"
#include "sequence_elements/shot.h"
#include "sequence_elements/timelinescene.h"
#include "filesystem/sequencedata.h"
#include <QDebug>
#include <QDataStream>
#include <QHash>

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

AddCommand::AddCommand(SequenceData* seq, int xpos, int length, TimelineScene *scene, QVector<Shot*> movedShots ,QUndoCommand *parent)
    : QUndoCommand(parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->previousxpos);
        qDebug()<< current->previousxpos << "saved previous pos - "<< current->scenePos().x() << "saved current pos";
        movedShotNewPos.insert(current, current->scenePos().x());
    }
    qDebug()<<movedShotNewPos.size();
    timeline = scene;
    shot = new Shot();
    this->seq = seq;
    this->xpos = xpos;
    this->length = length;


    scene->update();
    setText(QObject::tr("Add %1")
        .arg(seq->name));
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
    timeline->przreg->usedSequences.insert(seq->name, seq);
    shot->setXToFrame(xpos);
    shot->setRect(0, 0, length, 100);
    shot->setPreviousToCurrent();
    timeline->clearSelection();
    timeline->update();
}
