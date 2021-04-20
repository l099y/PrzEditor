#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "sequence_elements/shot.h"
#include "filesystem/sequencedata.h"
#include "sequence_elements/timelinescene.h"
#include <QHash>

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(TimelineScene *timeline, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    Shot *shot;
    TimelineScene *timeline;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(SequenceData* seq, int xpos, int length, TimelineScene* timeline, QVector<Shot*> movedShots,
               QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    Shot *shot;
    QHash<Shot*, int> movedShotOldPos;
    QHash<Shot*, int> movedShotNewPos;
    SequenceData* seq;
    TimelineScene *timeline;
    int xpos;
    int length;
};

class MoveCommand : public QUndoCommand
{
public:
    MoveCommand(TimelineScene* timeline, QVector<Shot*> movedShots, int previoustimelineWidth, int timelineWidth, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineScene* timeline;
    QHash<Shot*, int> movedShotOldPos;
    QHash<Shot*, int> movedShotNewPos;
    int prevscenesize;
    int currentscenesize;
};
class ClearCommand : public QUndoCommand
{
public:
    ClearCommand(TimelineScene*, QVector<Shot*> removedshots, int prevtimelinesize, QUndoCommand *parent= nullptr);

    void undo() override;
    void redo() override;
private:
    TimelineScene* timeline;
    QVector<Shot*> removedShots;
    int prevtimelinesize;
};
#endif // COMMANDS_H
