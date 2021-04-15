#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include "shot.h"
#include "ruler.h"
#include "filesystem/sequenceregister.h"

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

    void resetBoxStates();
    void resetToPrevious();
    void behaveOnSelectionSwitchPosMove();
    void behaveOnSelectionDisplace();
    void behaveOnSelectionInsertionDisplace();
    float rectXAndWBefore(Shot *rect);
    QVector<Shot*> getMovedShots();
    float positionOfInsertedShot(QGraphicsSceneDragDropEvent* e);
    Shot* dropRepresentation;
    SequenceRegister* przreg;

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();
   void handleSelectionMove();
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
   void createShot(SequenceData* seq, int xpos, int length, TimelineScene* timeline, QVector<Shot*> movedShot);
   void moveShotss(QVector<Shot*>, int, int);


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
