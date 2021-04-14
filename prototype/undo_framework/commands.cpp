#include "commands.h"
#include "sequence_elements/shot.h"
#include "sequence_elements/timelinescene.h"
#include "filesystem/sequencedata.h"
#include <QDebug>

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

AddCommand::AddCommand(SequenceData* seq, int xpos, int length, TimelineScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    timeline = scene;
    shot = new Shot();


    this->seq = seq;
    this->xpos = xpos;
    this->length = length;
    shot->setRect(0, 0, length, 100);
    shot->setX(xpos);
    scene->update();
    setText(QObject::tr("Add %1")
        .arg(seq->name));
}
//! [7]

AddCommand::~AddCommand()
{
    if (!shot->scene())
        delete shot;
}

//! [8]
void AddCommand::undo()
{
    timeline->removeItem(shot);
    timeline->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{

    timeline->addItem(shot);
    qDebug()<<timeline->items();
    timeline->przreg->usedSequences.insert(seq->name, seq);
    qDebug()<<timeline->przreg->usedSequences;
    shot->setXToFrame(xpos);
    shot->rect().setWidth(length);
    timeline->clearSelection();
    timeline->update();
}
