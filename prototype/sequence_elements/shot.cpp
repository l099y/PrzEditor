#include "shot.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QGraphicsItemAnimation>
#include <QObject>
#include <QDataStream>
#include <QRandomGenerator>
#include <chrono>
#include <ctime>


Shot::Shot(): QGraphicsRectItem()
{
    QRandomGenerator rdm(std::time(0));
    QPen pen (Qt::white);
    QColor a;
    a.setRgb(rdm.bounded(0,74)+180,rdm.bounded(0,74)+180,150);
    setPen(pen);
    setBrush(a);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);

    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));

    animation->setItem(this);
    animation->setTimeLine(timer);
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

void Shot::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem :: mouseReleaseEvent(e);
    mod = BoxState::REGULAR;
    if (scenePos().y()!=0){
        setPos(scenePos().x(), 0);
    }
    setPreviousToCurrent();
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

void Shot::animatedMove(float pos)
{
    if(!animated){
        qDebug()<<"create animation to "<<pos;
        float prev = roundedTo10(scenePos().x());
        float dist = pos-scenePos().x();
        animated=true;
        timer->setFrameRange(0, 15);
        for (int i = 0; i < 15; ++i)
            animation->setPosAt(i / 15.0, QPointF(prev+=(dist/15), 0));
        timer->start();
    }
}

void Shot::setSize(int realsize)
{
    setRect(0,0,realsize, 100);
}

QJsonObject Shot::generateJson()
{
    QJsonArray array;
    foreach (SequenceData* current, seqs){
        SequenceData* seq = dynamic_cast<SequenceData*>(current);
        if (seq)
        {

          array.push_back(seq->generateJson());
          qDebug()<<seq->name<<" in generatedjson in shot";
        }
    }

    QJsonObject ret;

    ret.insert("x", roundedTo10(previousxpos));
    ret.insert("width", roundedTo10(previousboxwidth));
    ret.insert("sequences", array);
    ret.insert("bred", this->brush().color().red());
    ret.insert("bgreen", this->brush().color().green());
    ret.insert("bblue", this->brush().color().blue());
    return ret;
}

int Shot::roundedTo10(float xf)
{
    int x = (int)xf;
    x = x % 10 > 5? x + (10-x%10): x - x%10;
    return x % 10 > 5? x + (10-x%10): x - x%10;

}

void Shot::setAnimatedFalse()
{
    animated=false;
}

void Shot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isSelected()){
        setZValue(1);
        painter->setPen(QColor(Qt::yellow));
        painter->setBrush(QColor(100,220,255));

    }
    else{
        setZValue(-1);
        painter->setBrush(this->brush());
        painter->setPen(this->pen());
    }
    painter->drawRoundedRect(rect(),5,5);
}


