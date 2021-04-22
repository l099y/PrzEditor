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

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    bool isPastLimit = false;

    TimelineScene(SequenceRegister*,QGraphicsView*, QObject* parent = nullptr);
    ~TimelineScene();

    QGraphicsView* view;
    Ruler ruler;
    float previousSceneWidth;

    QHash<Shot*, int> imageOfPositions;

    QJsonObject generateJson();

    void resetBoxStates();
    void resetToPrevious();
    void behaveOnSelectionSwitchPosMove();
    void behaveOnSelectionSwitchPosMoveFinal();
    void handleSelectionMoveFinal();
    void handleSelectionMove();
    void behaveOnSelectionDisplace();
    void behaveOnSelectionInsertionDisplace();
    void placeInsertedShotInTimeline();
    void resetShotDisplacedByInsertion();
    void resetShotDisplacedFinal();
    void generateImageOfPosition();
    float rectXAndWBefore(Shot *rect);
    QVector<Shot*> getMovedShots();
    Shot* dropRepresentation;
    SequenceRegister* przreg;

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();

   void moveAllFrom(float from, float distance);
   void ExtendSceneWidth(float f);
   void allign();
   void setdisp();
   void deleteSelection();
   void displaceSelection(int newPos);
   void changeSelectionSize(int newSize);
   void setSingleSelectionToLast();


signals:

   void scaleUp();
   void scaleDown();
   void deleteSelectionSignal();
   void createShot(QList<SequenceData*> seq, int xpos, int length, TimelineScene* timeline, QVector<Shot*> movedShot);
   void moveShotss(TimelineScene*, QVector<Shot*>, int, int);
   void clearTimeline(TimelineScene*, QVector<Shot*>, int);


protected:
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
   virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *e);
   virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *e);
   virtual void wheelEvent(QGraphicsSceneWheelEvent *e);
   virtual void keyPressEvent(QKeyEvent *keyEvent);
   virtual void dropEvent(QGraphicsSceneDragDropEvent*);
   virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* e);

};

#endif // TIMELINESCENE_H
