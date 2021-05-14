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
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>


Shot::Shot(): QGraphicsRectItem()
{

    generateParamsFromTemplate();
    QRandomGenerator rdm(std::time(0));
    QPen pen (Qt::white);
    QColor a;
    a.setRgb(rdm.bounded(0,74)+180,rdm.bounded(0,74)+180,100);
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
    scenePos().setY(160);
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
    scenePos().setY(160);
    QPen pen (Qt::white);
    setPen(pen);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);



    connect(timer, SIGNAL(finished()), this, SLOT(setAnimatedFalse()));

    animation->setItem(this);
    animation->setTimeLine(timer);

    generateParamsFromJsonShot(jsonShot);

    QGraphicsDropShadowEffect * dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(10);
    dse->setColor(QColor(255,255,255,50));
    dse->setXOffset(0);
    dse->setYOffset(0);
    setGraphicsEffect(dse);
}



Shot::~Shot()
{
    delete(animation);
    delete(timer);
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
    if (scenePos().y()!=160){
       scenePos().setY(160);
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

QRectF Shot::boundingRect() const
{
    return this->rect();
}

void Shot::setPreviousToCurrent(){
    previousboxwidth = rect().width();
    previousxpos = scenePos().x();
    setY(160);
}

void Shot::restore()
{
    setX(previousxpos);

    setRect(0,0,previousboxwidth, 100);
    setY(160);
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
            animation->setPosAt(i / 15.0, QPointF(prev+=(dist/15), 160));
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

bool Shot::validateSizeChange(int newSize)
{
    return newSize<=smallestSequence();
}

bool Shot::validatePosChange(int pos)
{
        bool validated = true;
        foreach (QGraphicsItem* current, this->scene()->items()){
            auto* shot = dynamic_cast<Shot*>(current);
            if (shot && shot != this && inXRange(pos, shot)){
                shot->displayError();
                validated = false;
            }
        }
        return validated;
}

int Shot::smallestSequence()
{

    if (seqs.isEmpty())
            return 0;
    else{
        int min = seqs[0]->sequencelength();
        qDebug()<< seqs[0]->sequencelength() << "in min size eval";
        foreach (SequenceData* current, seqs){
            SequenceData* seq = dynamic_cast<SequenceData*>(current);
            if (seq)
            {
                if (seq->sequencelength()< min)
                {
                    min = seq->sequencelength();
                }
            }
        }
        qDebug()<< min << "in min size eval";
        return min;
    }
}

bool Shot::inXRange(int pos, Shot* other)
{
    return ((pos + this->rect().width() > other->scenePos().x() && pos+this->rect().width() < other->scenePos().x()+other->rect().width())
                ||(pos <= other->scenePos().x()+other->rect().width() && pos > other->scenePos().x()))
            ||(pos<= other->scenePos().x() && pos+this->rect().width()>=other->scenePos().x()+other->rect().width());
}

void Shot::displayError()
{
    qDebug()<<"erroranim should trigger";
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
//    if (animated)
//        {
//        painter->setPen(QColor(Qt::yellow));
//        painter->setBrush(QColor(255,0,0));
//    }
    painter->drawRoundedRect(rect(),5,5);

    // Drawing the fadin / fadeout triangles;

    int val =  templateParams.value("Fade From Black Frame Out").value("value").toString().toInt();

    if (val != 0){
    qreal startPointX1 = 0.0;
    qreal startPointY1 = 0.0;

    qreal endPointX1   = 0.0;
    qreal endPointY1   = 100.0;

    qreal endPointX2 = val*10;
    qreal endPointY2 = 0;//fadin size;

    QPainterPath path;

    path.moveTo (startPointX1, startPointY1);

    path.moveTo (startPointX1, startPointY1);
    // Draw line from pen point to this point.
    path.lineTo (endPointX1, endPointY1);
    path.lineTo (endPointX2,   endPointY2);
    path.lineTo (startPointX1, startPointY1);

    painter->setPen (Qt :: NoPen);
    QColor color(0,0,0);
    color.setAlpha(40);
    painter->fillPath (path, QBrush (color));
    }

    val =  templateParams.value("Fade To Black Frame Out").value("value").toString().toInt();

    if (val!=0){

    qreal startPointX1 = rect().width();
    qreal startPointY1 = 0.0;

    qreal endPointX1   = rect().width();
    qreal endPointY1   = 100.0;

    qreal endPointX2 = rect().width()-(val)*10;
    qreal endPointY2 = 100;//fadin size;

    QPainterPath path;

    path.moveTo (startPointX1, startPointY1);

    path.moveTo (startPointX1, startPointY1);
    path.lineTo (endPointX1, endPointY1);
    path.lineTo (endPointX2,   endPointY2);
    path.lineTo (startPointX1, startPointY1);

    painter->setPen (Qt :: NoPen);
    QColor color(0,0,0);
    color.setAlpha(40);
    painter->fillPath (path, QBrush (color));
    }

    // retrieving the size of the visible shot

    auto visiblerectsize = static_cast<int>((rect().width()*painter->transform().m11()));
    double scaleValue =1/painter->transform().m11();
    painter->save();

    //disable scaling on painter, to have fix representation of graphics

    painter->scale(scaleValue, 1);
        painter->setPen(QColor(0,0,100));
        painter->setBrush(QColor(0,0,0));
        QString qs = templateParams.value("Shot designation").value("value").toString();
        if (qs.length()*6<visiblerectsize){
        painter->drawText(5, 90, qs);
        this->setToolTip("");
        }
        else{
        this->setToolTip(qs);

        }
    painter->restore();
}


