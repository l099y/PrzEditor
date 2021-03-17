#ifndef EXTENDEDQGRI_H
#define EXTENDEDQGRI_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "emitter.h"
#include <QTimeLine>
#include <QGraphicsItemAnimation>
enum class BoxState {REGULAR, STRETCH_LEFT, STRETCH_RIGHT, SWITCHPOS_MOVE, DISPLACE};
class ExtendedQGRI : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    ExtendedQGRI();
    ~ExtendedQGRI();
    QTimeLine *timer = new QTimeLine(150);
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    BoxState mod = BoxState::REGULAR;
    bool modified = false;
    bool animated = false;
    bool prevposresetrequested = false;
    float previousxpos;
    float previousboxwidth;
    float mousePosXonClick;
    Emitter *emitter = new Emitter(this);

    void setXToFrame(float x);
    void setPreviousToCurrent();
    void restore();
    void strechLeft(QGraphicsSceneMouseEvent *e);
    void strechRight(QGraphicsSceneMouseEvent *e);
    void setModifyingcColorSignal();
    void setRegularColor();
    void animatedMove(float pos);
    void setSize(int realsize);
inline int roundedTo10(float x);

inline bool wasLeftOf(ExtendedQGRI* OtherCube)
    {
        return  previousxpos<OtherCube->previousxpos;
    }
inline bool rightSideIsInFirstHalfOf (ExtendedQGRI* OtherCube)
    {
        return  scenePos().x() + rect().width() >= OtherCube->previousxpos &&
                scenePos().x() + rect().width() < OtherCube->previousxpos+(OtherCube->previousboxwidth/2);
    }
inline bool rightSideIsAfterSecondHalfOf (ExtendedQGRI* OtherCube)
    {
        return  scenePos().x() + rect().width() >= OtherCube->previousxpos+(OtherCube->previousboxwidth/2);
    }

inline bool leftSideIsAfterFirstHalfOf (ExtendedQGRI* OtherCube)
    {
        return scenePos().x()< OtherCube->previousxpos+(OtherCube->previousboxwidth/2);
    }
inline bool leftSideIsInSecondHalfOf (ExtendedQGRI* OtherCube)
    {
        return  scenePos().x()>= OtherCube->previousxpos+(OtherCube->previousboxwidth/2) &&
                scenePos().x()< OtherCube->previousxpos+OtherCube->previousboxwidth;
    }

public slots:
    void setAnimatedFalse();



protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *e);
};
#endif // EXTENDEDQGRI_H
