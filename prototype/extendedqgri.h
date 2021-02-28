#ifndef EXTENDEDQGRI_H
#define EXTENDEDQGRI_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class ExtendedQGRI : public QGraphicsRectItem
{
public:
    ExtendedQGRI();

protected:
 virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
 virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
 virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
 virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
};
#endif // EXTENDEDQGRI_H
