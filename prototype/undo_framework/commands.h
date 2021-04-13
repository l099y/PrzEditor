#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "sequence_elements/shot.h"
#include "filesystem/sequencedata.h"

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    Shot *shot;
    QGraphicsScene *myGraphicsScene;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(SequenceData sequence, int xpos, int length, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    Shot *shot;
    QGraphicsScene *myGraphicsScene;
    int xpos;
    int length;
};

#endif // COMMANDS_H
