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

ExtendedQGRI::~ExtendedQGRI()
{

}

void ExtendedQGRI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem::mouseDoubleClickEvent(e);
}

void ExtendedQGRI::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    QBrush brush (Qt::red);
    setBrush(brush);
    update();
    //QGraphicsRectItem::hoverEnterEvent(e);
}

void ExtendedQGRI::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouse released in Rect class";
    QGraphicsItem :: mouseReleaseEvent(e);
    leftxtend = false;
    rightxtend = false;
    if (scenePos().y()!=0){
        setPos(scenePos().x(), 0);
    }
    setPreviousToCurrent();
}

void ExtendedQGRI::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    qDebug()<<"extended hoverExit";

    QBrush brush (Qt::transparent);
    setBrush(brush);
    update();
    //QGraphicsRectItem::hoverEnterEvent(e);
}

void ExtendedQGRI::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
   if(e->pos().x()>= 0 && e->pos().x()<=3)
   {
       setCursor(Qt::SizeHorCursor);
       //setMode extend to left
   }
   else if(e->pos().x()>= boundingRect().width()-3 && e->pos().x()<= boundingRect().width()){
       setCursor(Qt::SizeHorCursor);
   }
   else{
       setCursor(Qt::ArrowCursor);
   }

}

void ExtendedQGRI::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(e->pos().x()>= 0 && e->pos().x()<=3)
    {
        emitter->leftxtndactivated();
        leftxtend = true;
        rightxtend = false;
    }
    else if(e->pos().x()>= boundingRect().width()-3 && e->pos().x()<= boundingRect().width()){
        rightxtend = true;
        leftxtend = false;
        emitter->rightxtndactivated();
    }
     QGraphicsRectItem::mousePressEvent(e);
}

void ExtendedQGRI::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
        QGraphicsRectItem :: mouseMoveEvent(e);
}

void ExtendedQGRI::setPreviousToCurrent(){
    previousboxwidth = rect().width();
    previousxpos = scenePos().x();
}


