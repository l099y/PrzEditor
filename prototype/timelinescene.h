#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QGraphicsRectItem>
#include "extendedqgri.h"

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    TimelineScene(QObject* parent = nullptr);
    ~TimelineScene();
    void behaveOnSelectionLeftXtend();
    void behaveOnSelectionRightXtend();
    void behaveOnSelectionMove();

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();
   void handleBoxResize(QGraphicsSceneMouseEvent *e);
   void moveAllToLeft(float f);
   void ExtendSceneWidth(float f);
protected:
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

#endif // TIMELINESCENE_H
