#include "commands.h"
#include "sequence_elements/shot.h"
#include "sequence_elements/timelinescene.h"

DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent), myGraphicsScene(scene)
{
    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
    list.first()->setSelected(false);
    shot = static_cast<Shot *>(list.first());
    setText(QObject::tr("Delete %1")
        .arg(" created "));
}
//! [4]

//! [5]
void DeleteCommand::undo()
{
    myGraphicsScene->addItem(shot);
    myGraphicsScene->update();
}
//! [5]

//! [6]
void DeleteCommand::redo()
{
    myGraphicsScene->removeItem(shot);
}
//! [6]

//! [7]
AddCommand::AddCommand(SequenceData seq, int xpos, int length,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent), myGraphicsScene(scene)
{
    static int itemCount = 0;

    shot = new Shot();
    this->xpos = xpos;
    this->length = length;
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg("created"));
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
    myGraphicsScene->removeItem(shot);
    myGraphicsScene->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{
    myGraphicsScene->addItem(shot);
    shot->setXToFrame(xpos);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}
