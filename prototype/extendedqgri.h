#ifndef EXTENDEDQGRI_H
#define EXTENDEDQGRI_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "emitter.h"
enum class BoxState {REGULAR, STRETCH_LEFT, STRETCH_RIGHT, SWITCHPOS_MOVE, DISPLACE};
class ExtendedQGRI : public QGraphicsRectItem
{
    //Q_INTERFACES(QGraphicsRectItem);
public:
    ExtendedQGRI();
    ~ExtendedQGRI();
    BoxState mod = BoxState::REGULAR;
    bool modified = false;
    float previousxpos;
    float previousboxwidth;
    Emitter *emitter = new Emitter(nullptr);
    void setPreviousToCurrent();
    void restore();
    void strechLeft(QGraphicsSceneMouseEvent *e);
    void strechRight(QGraphicsSceneMouseEvent *e);
    void setModifyingcColorSignal();
    void setRegularColor();

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
