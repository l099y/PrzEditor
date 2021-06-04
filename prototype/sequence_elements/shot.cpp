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
#include <QGraphicsView>
#include <QScrollBar>
#include <sequence_elements/timelinescene.h>
#include <QImage>




Shot::Shot(): QGraphicsRectItem()
{
    QImage aaa(":/images/dv.jpg");

    pixmap->convertFromImage(aaa.scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QTime time = time.currentTime();
    generateParamsFromTemplate();
    QRandomGenerator rdm((uint)(time.msec()%10));
    QPen pen (Qt::white);
    QColor a;
    a.setRgb(rdm.bounded(0,74)+100,rdm.bounded(0,74)+180,240);
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
    //QImage aaa(":/images/dv.jpg");
    //pixmap->convertFromImage(aaa.scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setXToFrame(jsonShot.value("x").toInt());
    setRect(0,0, jsonShot.value("width").toInt(), 100);
    frameIn =  jsonShot.value("frameIn").toInt();
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


    auto timelineView = scene()->views()[0];

    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());
    QRectF visible_scene_rect = timelineView->mapToScene(viewport_rect).boundingRect();
    auto timeline = dynamic_cast<TimelineScene*>(scene());



    auto sceneStart = timeline->getVisibleRect().x();
    auto sceneWidth = visible_scene_rect.width();
    auto sceneEnd = sceneStart+ sceneWidth;


    // if current shot is in the visible scene

    if (((scenePos().x() + this->rect().width() >= sceneStart && scenePos().x()+this->rect().width() < sceneEnd)
         ||(scenePos().x() <= sceneEnd && scenePos().x() >= sceneStart)) ||(scenePos().x()<= sceneStart && scenePos().x()+this->rect().width()>=sceneEnd)){

        // if current shot is smaller than the visible scene.. yes this could be better but to avoid small calculs and save few ressources is a balance

        if (this->rect().width()<visible_scene_rect.width())
            return this->rect();
        else{
            if (scenePos().x()<sceneEnd && scenePos().x()>=sceneStart){
                return QRectF(0,-200,sceneEnd-scenePos().x(),300);
            }
            else if (scenePos().x()+rect().width()>=sceneStart && scenePos().x()+rect().width() <= sceneEnd){
                return QRectF(rect().width()-(scenePos().x()+rect().width()-sceneStart),-200, scenePos().x()+rect().width()-sceneStart, 300);

            }
            else {
                return QRectF(sceneStart-scenePos().x(), -200, sceneWidth, 300);
            }
        }
    }
    else
    {
        return QRectF(0,0,0,0);
    }

    //    qDebug()<<( std::max(scenePos().x(), scenePos().x()+rect().width()) >= std::min(sceneStart, sceneStart+sceneWidth) && std::min(scenePos().x(),  scenePos().x()+rect().width()) <= std::max(sceneStart, sceneWidth));

    //    if ( std::max(sceneStart, sceneEnd) >= std::min(scenePos().x(), scenePos().x()+rect().width()) && std::min(sceneStart, sceneEnd) <= std::max(scenePos().x(),  scenePos().x()+rect().width())){
    //        qDebug()<<"sound in visible part of the scene with std";

    //    }
    //      qDebug()<<scenePos().x()<<"rect scenepos" << this->rect().width() ;
    //      qDebug()<< sceneStart << "starting point of the view"<< sceneEnd << "in bounding rect";


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
    ret.insert("frameIn", frameIn);
    ret.insert("sequences", array);
    ret.insert("bred", this->brush().color().red());
    ret.insert("bgreen", this->brush().color().green());
    ret.insert("bblue", this->brush().color().blue());
    ret.insert("templateParams", paramsarray);
    return ret;
}

QJsonObject Shot::generateExportJson(int fileIndex)
{
    QJsonObject seq;
    foreach (QJsonObject param, templateParams)
    {
        if (param.value("name").toString()=="Framerate"){
            if (param.value("value").toString().toInt() != param.value("default_value").toString().toInt()){
                seq.insert("framerate", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Playmode"){
            if (param.value("value").toString().toInt() != param.value("default_value").toString().toInt()){
                seq.insert("playmode", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Autoplay"){
            if (param.value("value").toString().toInt() != param.value("default_value").toString().toInt()){
                seq.insert("autoplay", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Reset View"){
            if (param.value("value").toString().toInt() != param.value("default_value").toString().toInt()){
                seq.insert("resetView", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Scene Rotation"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("sceneRotation", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="Scale"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("scale", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="CameraHeight"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("cameraHeight", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="Fade From Black Frame Out"){
            if (param.value("value").toString().toInt() != 0){
                seq.insert("fadeFromBLackFrameIn", 0);
                seq.insert("fadeFromBlackFrameOut", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Fade To Black Frame Out"){
            if (param.value("value").toString().toInt() != 0){
                seq.insert("fadeToBlackFrameIn", rect().width()/10-param.value("value").toString().toInt());
                seq.insert("fadeToBlackFrameOut", rect().width()/10);
            }
        }
        else if (param.value("name").toString()=="Glow Intensity"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("glowIntensity", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="Glow Power"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("glowPower", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="Continue Audio"){
            if (param.value("value").toString().toInt() != param.value("default_value").toString().toInt()){
                seq.insert("continueAudio", param.value("value").toString().toInt());
            }
        }
        else if (param.value("name").toString()=="Audio Rotation"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("audioRotation", param.value("value").toString().toDouble());
            }
        }
        else if (param.value("name").toString()=="Background"){
            if (param.value("value").toString() != param.value("default_value").toString()){
                seq.insert("backgroundFile", param.value("value"));
            }
        }
        else if (param.value("name").toString()=="Background Rotation"){
            if (param.value("value").toString().toDouble() != param.value("default_value").toString().toDouble()){
                seq.insert("backgroundRotation", param.value("value").toString().toDouble());
            }
        }
    }
    QJsonObject positions;
    positions.insert("x",0);
    positions.insert("y",0);
    positions.insert("z", 0);

    QJsonArray frames;
    for (int i = 0; i <rect().width()/10; i++)
    {
        QJsonArray framee;
        QJsonObject frame;
        frame.insert("file", fileIndex);
        frame.insert("frame", seqs[0]->startIdx+i+frameIn);
        framee.append(frame);
        frames.append(framee);
    }
    positions.insert("frames", frames);
    QJsonArray container;
    container.append(positions);
    seq.insert("frames", rect().width()/10);

    seq.insert("positions", container);
    return seq;
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

void Shot::checkIntegrity()
{
    seqs[0]->checkIntegrity();
    update();
}

void Shot::setAnimatedFalse()
{
    animated=false;
}

void Shot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    if (animated){
//        painter->setBrush(QColor(Qt::red));
//    }
    if (isSelected()){
        setZValue(1);
        painter->setPen(QColor(Qt::yellow));
        painter->setBrush(QColor(240,220,114, 180));
    }
    else{
        setZValue(-1);
        painter->setBrush(this->brush());
        painter->setPen(this->pen());
    }

    auto timelineView =this->scene()->views()[0];
    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());

    auto visiblerectsize = static_cast<int>((rect().width()*painter->transform().m11()));

    painter->drawRoundedRect(rect(),5,5);

    // Drawing corrupted sequences

    if (seqs[0]->corrupted)
    {
        painter->setBrush(QColor(Qt::red));
        if (seqs[0]->corruptedSubSequences.size()!=0){
            QHash<int, int>::const_iterator i = seqs[0]->corruptedSubSequences.constBegin();
            while (i != seqs[0]->corruptedSubSequences.constEnd()) {
                painter->drawRect(i.key()*10,0,i.value()*10, 100);
                i++;
            }
        }
        else{
            painter->drawRect(rect());
        }
    }

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
        qreal endPointY2 = 0;//fadin size;

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

    double scaleValue =1/painter->transform().m11();
    painter->save();

    //disable scaling on painter, to have fix representation of graphics

    painter->scale(scaleValue, 1);
    painter->setPen(QColor(0,0,0,50));
    QLineF* ll = new QLineF (0, 0, 0, 100);
    painter->drawLine(*ll);
    QLineF* lll = new QLineF (rect().width(), 0, rect().width(), 100);
    painter->drawLine(*lll);

//            if (visiblerectsize>pixmap->width())
//            painter->drawPixmap(0,0, *pixmap,0,0,100,100);
//            else{
//            painter->drawPixmap(0,0, *pixmap,0,0,visiblerectsize, 100);
//            }

    painter->setPen(QColor(0,0,100));
    painter->setBrush(QColor(0,0,0));
    QString qs = templateParams.value("Shot designation").value("value").toString();
    if (qs.length()*6.5<visiblerectsize){
        painter->drawText(5, 92, qs);
        this->setToolTip("");
    }
    else{
        this->setToolTip(qs);
    }
    delete(ll);
    delete(lll);
    painter->restore();
}


