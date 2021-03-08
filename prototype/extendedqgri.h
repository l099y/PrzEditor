#ifndef EXTENDEDQGRI_H
#define EXTENDEDQGRI_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "emitter.h"

class ExtendedQGRI : public QGraphicsRectItem
{
    //Q_INTERFACES(QGraphicsRectItem);
public:
    ExtendedQGRI();
    ~ExtendedQGRI();
    bool leftxtend = false;
    bool rightxtend = false;
    bool modified = false;
    float previousxpos;
    float previousboxwidth;
    Emitter *emitter = new Emitter(nullptr);
    void setPreviousToCurrent();
    void restore();

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *e);
};
#endif // EXTENDEDQGRI_H
