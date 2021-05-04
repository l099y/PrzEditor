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
#include <QFile>
#include <QJsonDocument>


Shot::Shot(): QGraphicsRectItem()
{

    generateParamsFromTemplate();
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

void Shot::generateParamsFromTemplate()
{
    QFile file (":/templates/shot.json");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString stringContent = stream.readAll();
    QJsonDocument configJson = QJsonDocument::fromJson(stringContent.toUtf8());
    auto config = configJson.object();

    foreach (QJsonValue val, config.value("shot").toArray()){
        templateParams.insert(val.toObject().value("name").toString(), val.toObject());
    }
}

void Shot::generateParamsFromJsonShot(QJsonObject jsonShot)
{
    foreach (QJsonValue val, jsonShot.value("templateParams").toArray()){
        templateParams.insert(val.toObject().value("name").toString(), val.toObject());
    }

}

Shot::Shot(QJsonObject jsonShot)
{
    setXToFrame(jsonShot.value("x").toInt());
    setRect(0,0, jsonShot.value("width").toInt(), 100);
    setPreviousToCurrent();
    setBrush(QColor(jsonShot.value("bred").toInt(), jsonShot.value("bgreen").toInt(), jsonShot.value("bblue").toInt()));

    QPen pen (Qt::white);
    setPen(pen);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);

    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));

    animation->setItem(this);
    animation->setTimeLine(timer);

    generateParamsFromJsonShot(jsonShot);
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
        }
    }
    QJsonArray paramsarray;
    QHash<QString, QJsonObject>::const_iterator i = templateParams.constBegin();
    while (i != templateParams.constEnd()) {
        paramsarray.append(i.value());

        ++i;
    }
    QJsonObject ret;

    ret.insert("x", roundedTo10(previousxpos));
    ret.insert("width", roundedTo10(previousboxwidth));
    ret.insert("sequences", array);
    ret.insert("bred", this->brush().color().red());
    ret.insert("bgreen", this->brush().color().green());
    ret.insert("bblue", this->brush().color().blue());
    ret.insert("templateParams", paramsarray);
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

    double scaleValue =1/painter->transform().m11();
    painter->save();
    painter->scale(scaleValue, 1);
        painter->setPen(QColor(0,0,100));
        painter->setBrush(QColor(0,0,0));
        painter->rotate(-90);
        QString qs = templateParams.value("Shot designation").value("value").toString();
        painter->drawText(-95, 13, qs.length()<15? qs : qs.left(14));
        painter->rotate(90);
    painter->restore();
}


