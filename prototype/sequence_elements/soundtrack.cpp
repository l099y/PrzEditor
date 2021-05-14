#include "soundtrack.h"
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
#include <QGraphicsDropShadowEffect>

SoundTrack::SoundTrack(): QGraphicsRectItem()
{

    generateParamsFromTemplate();
    QRandomGenerator rdm(std::time(0));
    QPen pen (Qt::white);
    QColor a;
    a.setRgb(rdm.bounded(0,74)+50,rdm.bounded(0,74)+100, 255);
    setPen(pen);
    setBrush(a);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);

    QGraphicsDropShadowEffect * dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(10);
    dse->setColor(QColor(255,255,255,50));
    dse->setXOffset(0);
    dse->setYOffset(0);
    setGraphicsEffect(dse);
    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));
    setY(360);
    animation->setItem(this);
    animation->setTimeLine(timer);
}

void SoundTrack::generateParamsFromTemplate()
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

void SoundTrack::generateParamsFromJsonShot(QJsonObject jsonShot)
{
    foreach (QJsonValue val, jsonShot.value("templateParams").toArray()){
        templateParams.insert(val.toObject().value("name").toString(), val.toObject());
    }

}

SoundTrack::SoundTrack(QJsonObject jsonShot)
{
    setXToFrame(jsonShot.value("x").toInt());
    setRect(0,0, jsonShot.value("width").toInt(), 20);
    setPreviousToCurrent();
    setBrush(QColor(jsonShot.value("bred").toInt(), jsonShot.value("bgreen").toInt(), jsonShot.value("bblue").toInt()));

    QPen pen (Qt::white);
    setPen(pen);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);

    QGraphicsDropShadowEffect * dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(10);
    dse->setColor(QColor(255,255,255,50));
    dse->setXOffset(0);
    dse->setYOffset(0);
    setGraphicsEffect(dse);

    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));

    animation->setItem(this);
    animation->setTimeLine(timer);
    setY(360);
    generateParamsFromJsonShot(jsonShot);
}



SoundTrack::~SoundTrack()
{
    delete(animation);
    delete(timer);
}


void SoundTrack::setXToFrame(float x)
{
    setX(roundedTo10(x));
}

void SoundTrack::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem::mouseDoubleClickEvent(e);
}

void SoundTrack::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem :: mouseReleaseEvent(e);
    mod = BoxState::REGULAR;
    if (scenePos().y()!=260){
        setPos(scenePos().x(), 260);
    }
    setPreviousToCurrent();
}

void SoundTrack::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    mousePosXonClick = e->pos().x();
    QGraphicsRectItem::mousePressEvent(e);
}

void SoundTrack::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem :: mouseMoveEvent(e);
}

QRectF SoundTrack::boundingRect() const
{
    return this->rect();
}

void SoundTrack::setPreviousToCurrent(){
    previousboxwidth = rect().width();
    previousxpos = scenePos().x();
}

void SoundTrack::restore()
{
    setX(previousxpos);

    setRect(0,0,previousboxwidth, 20);
    setY(260);
    setVisible(true);
}

void SoundTrack::animatedMove(float pos)
{
    if(!animated){
        qDebug()<<"create animation to "<<pos;
        float prev = roundedTo10(scenePos().x());
        float dist = pos-scenePos().x();
        animated=true;
        timer->setFrameRange(0, 15);
        for (int i = 0; i < 15; ++i)
            animation->setPosAt(i / 15.0, QPointF(prev+=(dist/15), 260));
        timer->start();
    }
}

void SoundTrack::setSize(int realsize)
{
    setRect(0,0,realsize, 20);
}

QJsonObject SoundTrack::generateJson()
{
    QJsonArray array;
    QJsonArray paramsarray;
    QHash<QString, QJsonObject>::const_iterator i = templateParams.constBegin();
    while (i != templateParams.constEnd()) {
        paramsarray.append(i.value());

        ++i;
    }
    QJsonObject ret;

    ret.insert("x", roundedTo10(previousxpos));
    ret.insert("width", roundedTo10(previousboxwidth));
    ret.insert("sequences", soundfile->generateJson());
    ret.insert("bred", this->brush().color().red());
    ret.insert("bgreen", this->brush().color().green());
    ret.insert("bblue", this->brush().color().blue());
    ret.insert("templateParams", paramsarray);
    return ret;
}

int SoundTrack::roundedTo10(float xf)
{
    int x = (int)xf;
    x = x % 10 > 5? x + (10-x%10): x - x%10;
    return x % 10 > 5? x + (10-x%10): x - x%10;

}

bool SoundTrack::validateSizeChange(int)
{
    return true;
}

bool SoundTrack::validatePosChange(int)
{
    return true;
}

void SoundTrack::setAnimatedFalse()
{
    animated=false;
}

void SoundTrack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
//    if (animated)
//        {
//        painter->setPen(QColor(Qt::yellow));
//        painter->setBrush(QColor(255,0,0));
//    }
    painter->drawRoundedRect(rect(),5,5);
}


