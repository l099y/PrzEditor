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
    Q_INTERFACES(QGraphicsRectItem);
public:
    ExtendedQGRI();
    ~ExtendedQGRI();
    bool xtendleft = false;
    bool xtendright = false;
    Emitter *emitter = new Emitter(nullptr);

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
