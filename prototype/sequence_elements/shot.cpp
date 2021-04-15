#include "shot.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QGraphicsItemAnimation>
#include <QObject>
#include <QDataStream>


Shot::Shot(): QGraphicsRectItem()
{
    QPen pen (Qt::white);
    QBrush brush (QColor(200,240,200));
    setPen(pen);
    setBrush(brush);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);
    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));
}

Shot::~Shot()
{
 delete(animation);
 delete(timer);
 delete(emitter);
}

void Shot::setXToFrame(float x)
{

    setX(roundedTo10(x));
}

void Shot::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem::mouseDoubleClickEvent(e);
}

void Shot::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    setBrush(QColor(180,200,240));
    update();
    QGraphicsRectItem::hoverEnterEvent(e);
}

void Shot::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem :: mouseReleaseEvent(e);
    mod = BoxState::REGULAR;
    if (scenePos().y()!=0){
        setPos(scenePos().x(), 0);
    }
    setPreviousToCurrent();
}

void Shot::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    if (!isSelected())
        setRegularColor();
    update();
    QGraphicsRectItem::hoverLeaveEvent(e);
}

void Shot::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
//    if(e->pos().x()>= 0 && e->pos().x()<=3)
//    {
//        setCursor(Qt::SplitHCursor);
//        //setMode extend to left
//    }
//    else if(e->pos().x()>= boundingRect().width()-3 && e->pos().x()<= boundingRect().width()){
//        setCursor(Qt::SplitHCursor);
//    }
//    else{
//        setCursor(Qt::ArrowCursor);
//    }


}

void Shot::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    mousePosXonClick = e->pos().x();
    QGraphicsRectItem::mousePressEvent(e);
}

void Shot::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem :: mouseMoveEvent(e);
}

void Shot::setPreviousToCurrent(){
    previousboxwidth = rect().width();
    previousxpos = scenePos().x();
}

void Shot::restore()
{
    setRegularColor();
    setX(previousxpos);
    setRect(0,0,previousboxwidth, 100);
    setVisible(true);
}

void Shot::strechLeft(QGraphicsSceneMouseEvent *e)
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
        setRect(0, 0, 10, 100);
    }
}

void Shot::strechRight(QGraphicsSceneMouseEvent *e)
{
    if (e->scenePos().x()>(previousxpos+previousboxwidth)){
        setRect(0, 0, (roundedTo10(e->scenePos().x()- previousxpos)), 100);
        setX(previousxpos);
    }
    else if (e->scenePos().x() >previousxpos &&  e->scenePos().x()<previousboxwidth+previousxpos){
        setX(previousxpos);
        setRect(0, 0, roundedTo10(e->scenePos().x()-previousxpos), 100);

    }
    else{
        setRect(0, 0, 10, 100);
        setX(previousxpos);
    }
}

void Shot::setModifyingcColorSignal()
{
    QColor cyan30 = (Qt::cyan);
    cyan30.setAlpha(60);
    QBrush brush (cyan30);
    setBrush(brush);
}

void Shot::setRegularColor()
{
    QBrush brush(QColor(200,240,200));
    setBrush(brush);
}

void Shot::animatedMove(float pos)
{



    if(!animated){
         qDebug()<<"create animation to "<<pos;
    float prev = roundedTo10(scenePos().x());
    float dist = pos-scenePos().x();
    animated=true;
    timer->setFrameRange(0, 15);
    animation->setItem(this);
    animation->setTimeLine(timer);
    for (int i = 0; i < 15; ++i)
    animation->setPosAt(i / 15.0, QPointF(prev+=(dist/15), 0));
    timer->start();
    }
}

void Shot::setSize(int realsize)
{
    setRect(0,0,realsize, 100);
}

int Shot::roundedTo10(float xf)
{
    int x = (int)xf;
    x = x % 10 > 5? x + (10-x%10): x - x%10;
    return x % 10 > 5? x + (10-x%10): x - x%10;

}

void Shot::setAnimatedFalse()
{
    if (prevposresetrequested){
        prevposresetrequested =false;
        setPreviousToCurrent();
        update();
    }
    animated=false;
}

void Shot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isSelected()){
        setZValue(1);

        painter->setBrush(QColor(100,255,200));
    }
    else{
        setZValue(-1);
        painter->setBrush(QColor(255,200,100));
    }

    painter->setPen(this->pen());
    painter->drawRoundedRect(rect(),5,5);
}


