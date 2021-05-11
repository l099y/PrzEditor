#ifndef SHOT_H
#define SHOT_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "emitter.h"
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <filesystem/sequencedata.h>
#include <QDataStream>
#include <QHash>
#include <QJsonArray>

enum class BoxState {REGULAR, DISPLACE};

class Shot : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Shot();
    Shot(QJsonObject);
    ~Shot();

    // collection of json parameters, name as the key

    QHash<QString, QJsonObject> templateParams;

    static QList<QColor> usedColor;

    QTimeLine *timer = new QTimeLine(110);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;

    BoxState mod = BoxState::REGULAR;

    bool inserted = false;
    bool modified = false;
    bool animated = false;
    bool prevposresetrequested = false;
    QList <SequenceData*> seqs;
    float previousxpos;
    float previousboxwidth;
    float mousePosXonClick;
    Emitter *emitter = new Emitter(this);

    // if not generated from a saved object, generate templateParams from the static config

    void generateParamsFromTemplate();

    // if the object is created from a jsonTemplate, the function sets the parameters to the templateparams

    void generateParamsFromJsonShot(QJsonObject);

    void setXToFrame(float x);
    void setPreviousToCurrent();
    void restore();
    void animatedMove(float pos);
    void setSize(int realsize);

    QJsonObject generateJson();

    static int roundedTo10(float x);

    inline bool wasLeftOf(Shot* OtherCube)
    {
        return  previousxpos<OtherCube->previousxpos;
    }

    inline int posOfMidd(){
        return previousxpos+(previousboxwidth/2);
    }
    inline bool isMyMiddlePastOrEqual(float e){
        return e >= previousxpos+(rect().width())/2;
    }
    inline bool isInMyFirstHalf(float e){
        return e>= previousxpos && e < previousxpos+(rect().width())/2;
    }
    inline bool isInMySecondHalf(float e){
        return e >= previousxpos+(rect().width())/2 && e < previousxpos+rect().width();
    }
public slots:
    void setAnimatedFalse();



protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *e);

    QRectF boundingRect() const;
};
#endif // SHOT_H
