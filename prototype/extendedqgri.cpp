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
    QColor green50= Qt::green;
    green50.setAlphaF( 0.5 );
    setBrush(green50);
    update();
    QGraphicsRectItem::hoverEnterEvent(e);
}

void ExtendedQGRI::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    qDebug()<<"mouse released in Rect class";
    QGraphicsItem :: mouseReleaseEvent(e);
    mod = BoxState::REGULAR;
    if (scenePos().y()!=0){
        setPos(scenePos().x(), 0);
    }
    setPreviousToCurrent();
}

void ExtendedQGRI::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    qDebug()<<"extended hoverExit";
    setRegularColor();
    update();
    QGraphicsRectItem::hoverEnterEvent(e);
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
        mod = BoxState::STRETCH_LEFT;
    }
    else if(e->pos().x()>= boundingRect().width()-3 && e->pos().x()<= boundingRect().width()){
        mod = BoxState::STRETCH_RIGHT;
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

void ExtendedQGRI::restore()
{
    setRegularColor();
    setX(previousxpos);
    setRect(0,0,previousboxwidth, 100);
    setVisible(true);
}

void ExtendedQGRI::strechLeft(QGraphicsSceneMouseEvent *e)
{
    if (previousxpos - e->scenePos().x()>=0){
        setRect(0, 0, (previousxpos - e->scenePos().x())+previousboxwidth, 100);
        setX(e->scenePos().x());
    }
    else if (previousxpos - e->scenePos().x()<=0 && previousxpos +previousboxwidth - e->scenePos().x()>=0){
        setRect(0, 0, (previousxpos - e->scenePos().x())+previousboxwidth, 100);
        setX(e->scenePos().x());

    }
    else{
        setX(previousxpos + previousboxwidth - 4);
        setRect(0, 0, 4, 100);
    }
}

void ExtendedQGRI::strechRight(QGraphicsSceneMouseEvent *e)
{
    if (e->scenePos().x()>(previousxpos+previousboxwidth)){
        setRect(0, 0, (e->scenePos().x()- previousxpos), 100);
        setX(previousxpos);
    }
    else if (e->scenePos().x() >previousxpos &&  e->scenePos().x()<previousboxwidth+previousxpos){
        setX(previousxpos);
        setRect(0, 0, (e->scenePos().x()-previousxpos), 100);

    }
    else{
        setRect(0, 0, 4, 100);
        setX(previousxpos);
    }
}

void ExtendedQGRI::setModifyingcColorSignal()
{
     QColor cyan30 = (Qt::cyan);
     cyan30.setAlpha(60);
    QBrush brush (cyan30);
    setBrush(brush);
}

void ExtendedQGRI::setRegularColor()
{
    QColor blue70 = Qt::blue;
    blue70.setAlphaF( 0.4 );
    QBrush brush(blue70);
    setBrush(brush);
}


