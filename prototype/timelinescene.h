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
    bool leftxtend = false;
    bool rightxtend = false;
    float previousxpos;
    float previousboxwidth;
    TimelineScene(QObject* parent = nullptr);
    ~TimelineScene();

public slots:
    void activatelxt();
    void activaterxt();
    void deactivatext();
   void newRect();
   void debugItems();
   void clearItems();
protected:
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

#endif // TIMELINESCENE_H
