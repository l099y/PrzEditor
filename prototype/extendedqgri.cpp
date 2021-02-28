#include "extendedqgri.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>


ExtendedQGRI::ExtendedQGRI(): QGraphicsRectItem()
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);
}

void ExtendedQGRI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem::mouseDoubleClickEvent(e);
    qDebug()<<e;
    qDebug()<<boundingRect();
}

void ExtendedQGRI::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    qDebug()<<"extended hoverEnter";
    QBrush brush (Qt::red);
    setBrush(brush);
    update();
    //QGraphicsRectItem::hoverEnterEvent(e);
}

void ExtendedQGRI::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouse released";
    QGraphicsItem :: mouseReleaseEvent(e);
    if (scenePos().y()!=0){
        setPos(scenePos().x(), 0);
    }

}

void ExtendedQGRI::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    qDebug()<<"extended hoverExit";
    QBrush brush (Qt::transparent);
    setBrush(brush);
    update();
    //QGraphicsRectItem::hoverEnterEvent(e);
}

