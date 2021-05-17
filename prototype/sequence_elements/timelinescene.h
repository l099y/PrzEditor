#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include "shot.h"
#include "ruler.h"
#include "filesystem/sequenceregister.h"
#include <QJsonObject>
#include <sequence_elements/soundtrack.h>
#include <filesystem/tbesounddata.h>
#include <QModelIndex>

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    bool isPastLimit = false;
    QGraphicsItem* selection = nullptr;

    TimelineScene(SequenceRegister*,QGraphicsView*, QObject* parent = nullptr);
    ~TimelineScene();

    QGraphicsView* view;
    Ruler ruler;
    float previousSceneWidth;

    QHash<Shot*, int> imageOfShotsPositions;
    QHash<SoundTrack*, int> imageOfSoundPositions;
    SoundTrack* soundRemovedByInsertion = nullptr;


    QJsonObject generateJson();

    void resetBoxStates();
    void resetToPrevious(QString);

    //redirect to sound of shot displacement functions

    void handleSelectionMove(float mouseXPos, bool final);

    //shot displacement functions

    void behaveOnSelectionSwitchPosMove(float mouseXPos, bool final);
    void placeInsertedShotInTimeline(float e);
    void behaveOnSelectedShotDisplace();
    void resetShotsDisplacedByInsertion();
    void resetShotsDisplacedFinal();
    int findLastXWShot();
    int findLastXWSound();
    float ShotXAndWBefore(Shot *rect);
    QVector<Shot*> getMovedShots();
    //sound displacement functions

    void behaveOnSelectionSwitchPosMoveSound(float mouseXPos, bool final);
    void placeInsertedSoundTrackInTimeline(float e);
    void behaveOnSelectedSoundDisplace();
    void resetSoundsDisplacedByInsertion();
    void resetSoundsDisplacedFinal();
    float SoundXAndWBefore(SoundTrack *rect);
    QVector<SoundTrack*> getMovedSounds();
    float findClosestShot(float);

    void generateImageOfShotsPositions();
    void generateImageOfSoundsPositions();

    Shot* shotDropRepresentation = nullptr;
    SoundTrack* soundDropRepresentation = nullptr;
    SequenceRegister* przreg;
    void setPreviousToCurrent();
    void realignSelectionOn260();
    // check if the condition to assign selection to the the interface Panel

    bool validateParameterTargetChange();

    QRectF getVisibleRect();

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();

   void moveAllFrom(float from, float distance, QString);
   void ExtendSceneWidth(float f);
   void align();
   void setdisp();


   void deleteSelection();
   void displaceSelection(int newPos, QString);
   void changeSelectionSize(int newSize, QString);

   void insertShotAtEnd(QList<SequenceData*>);
   void insertSoundAtEnd(TbeSoundData*);

   void refreshRuler(int);
   void scaleViewToScene();

signals:
   void scaleToViewRequest();
   void scaleUp();
   void scaleDown();
   void deleteSelectionSignal();

   void createShot(QList<SequenceData*> seq, int xpos, int length, TimelineScene* timeline, QVector<Shot*> movedShot);
   void moveShots(TimelineScene*, QVector<Shot*>, int, int);
   void resizeShot(TimelineScene*, QVector<Shot*>,Shot*, int, int, int);

   void moveSoundtracks(TimelineScene*, QVector<SoundTrack*>, int, int);
   void resizeSound(TimelineScene*, QVector<SoundTrack*>,SoundTrack*, int, int, int);
   void babar(SoundTrack*, TbeSoundData*, int, int, TimelineScene*, QVector<SoundTrack*>); // int xpos, int length, TimelineScene* timeline,

   void displayError(QString, int);
   void selectionXChanged();

   void clearTimeline(TimelineScene*, QVector<Shot*>, int);

protected:
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

   virtual void wheelEvent(QGraphicsSceneWheelEvent *e);
   virtual void keyPressEvent(QKeyEvent *keyEvent);

   virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *e);
   virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *e);
   virtual void dropEvent(QGraphicsSceneDragDropEvent*);
   virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* e);
};

#endif // TIMELINESCENE_H
