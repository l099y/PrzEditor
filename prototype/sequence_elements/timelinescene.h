#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include "extendedqgri.h"
#include "ruler.h"

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    bool isPastLimit = false;
    TimelineScene(QObject* parent = nullptr);
    ~TimelineScene();
    ruler ruler;
    float previousSceneWidth;
    void resetBoxStates();
    void behaveOnSelectionLeftXtend();
    void behaveOnSelectionRightXtend();
    void behaveOnSelectionMove();
    void behaveOnSelectionSwitchPosMove();
    void behaveOnSelectionDisplace();
    float rectXAndWBefore(ExtendedQGRI *rect);

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

signals:
   void scaleUp();
   void scaleDown();

protected:
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
   virtual void dragEnterEven(QGraphicsSceneDragDropEvent *e);
   virtual void dropEvent(QGraphicsSceneDragDropEvent *e);
   virtual void wheelEvent(QGraphicsSceneWheelEvent *e);
   virtual void keyPressEvent(QKeyEvent *keyEvent);
};

#endif // TIMELINESCENE_H
