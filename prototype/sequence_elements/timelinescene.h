#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include "extendedqgri.h"
#include "ruler.h"
#include "filesystem/sequenceregister.h"

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    bool isPastLimit = false;
    TimelineScene(SequenceRegister*, QObject* parent = nullptr);
    ~TimelineScene();
    ruler ruler;
    float previousSceneWidth;
    void resetBoxStates();
    void resetToPrevious();
    void behaveOnSelectionLeftXtend();
    void behaveOnSelectionRightXtend();
    void behaveOnSelectionMove();
    void behaveOnSelectionSwitchPosMove();
    void behaveOnSelectionDisplace();
    void behaveOnSelectionInsertionDisplace();
    float rectXAndWBefore(ExtendedQGRI *rect);
    float positionOfInsertedShot(QGraphicsSceneDragDropEvent* e);
    ExtendedQGRI* dropRepresentation;
    SequenceRegister* przreg;

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();
   void handleBoxResize();
   void moveAllFrom(float from, float distance);
   void ExtendSceneWidth(float f);
   void allign();
   void setmod2();
   void setdisp();
   void deleteSelection();
   void displaceSelection(int newPos);
   void changeSelectionSize(int newSize);
   void setSingleSelectionToLast();


signals:
   void scaleUp();
   void scaleDown();

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
