#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "sequence_elements/shot.h"
#include "filesystem/sequencedata.h"
#include "sequence_elements/timelinescene.h"
#include <QHash>
#include <QJsonObject>
#include <mainwindow.h>
#include <sequence_elements/soundtrack.h>

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(TimelineScene *timeline, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem *shot;
    TimelineScene *timeline;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(QList<SequenceData*> seq, int xpos, int length, TimelineScene* timeline, QVector<Shot*> movedShots,
               QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    Shot *shot;
    QHash<Shot*, int> movedShotOldPos;
    QHash<Shot*, int> movedShotNewPos;
    QList<SequenceData*> seqs;
    TimelineScene *timeline;
    int xpos;
    int length;
};

class AddSoundCommand : public QUndoCommand
{
public:
    AddSoundCommand(SoundTrack*, TbeSoundData*, int, int, TimelineScene*, QVector<SoundTrack*>,
               QUndoCommand *parent = nullptr);
    ~AddSoundCommand();

    void undo() override;
    void redo() override;

private:
    SoundTrack *sound;
    SoundTrack* suppressedSound;
    QHash<SoundTrack*, int> movedSoundsOldPos;
    QHash<SoundTrack*, int> movedSoundsNewPos;
    TimelineScene *timeline;
    TbeSoundData* sounddata;
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

class MoveSoundsCommand : public QUndoCommand
{
public:
    MoveSoundsCommand(TimelineScene* timeline, QVector<SoundTrack*> movedShots, int previoustimelineWidth, int timelineWidth, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineScene* timeline;
    QHash<SoundTrack*, int> movedSoundsOldPos;
    QHash<SoundTrack*, int> movedSoundsNewPos;
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

class LoadCommand : public QUndoCommand{
public:
    LoadCommand(TimelineScene*, QString,  QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    TimelineScene* timeline;
    float timelinesize;
    QList<Shot*> shots;
};

class ChangeParameterInShotCommand : public QUndoCommand{
public:
    ChangeParameterInShotCommand(MainWindow*, Shot*, QJsonObject,  QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    MainWindow* app;
    Shot* sh;
    QJsonObject oldconfig;
    QJsonObject newconfig;
};

class ResizeShotCommand: public QUndoCommand
{
public:
    ResizeShotCommand(TimelineScene* timeline, QVector<Shot*> movedShots,Shot* resizedShot, int newShotSize, int previoustimelineWidth, int timelineWidth, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    TimelineScene* timeline;
    QHash<Shot*, int> movedShotOldPos;
    QHash<Shot*, int> movedShotNewPos;

    Shot* resizedShot;
    int previousShotWidth;
    int newShotWidth;

    int prevscenesize;
    int currentscenesize;
};


#endif // COMMANDS_H
