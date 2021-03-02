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
    delete(this);
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
    if (xtendleft ||xtendright)
        emitter->emitxtendDeactived();
    xtendleft = false;
    xtendright = false;
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
    qDebug()<<e;
    if(e->pos().x()>= 0 && e->pos().x()<=3)
    {
        emitter->leftxtndactivated();
        xtendleft = true;
    }
    else if(e->pos().x()>= boundingRect().width()-3 && e->pos().x()<= boundingRect().width()){
        xtendright = true;
        emitter->rightxtndactivated();
    }
     QGraphicsRectItem::mousePressEvent(e);


}

void ExtendedQGRI::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (xtendleft){

    }
    else if (xtendright){

    }
    else{
        QGraphicsRectItem :: mouseMoveEvent(e);
        //qDebug()<<e;
    }
}


