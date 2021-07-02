#ifndef SHOT_H
#define SHOT_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <filesystem/sequencedata.h>
#include <QDataStream>
#include <QHash>
#include <QJsonArray>
#include <QTimer>
#include <QResource>
#include <QPixmap>
#include <filesystem/backgroundprz.h>

enum class BoxState {REGULAR, DISPLACE};

class Shot : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:

    // was a test to check if the program supports well moving pixmap

    QPixmap* pixmap = new QPixmap(100 ,100);

    Shot();
    Shot(QJsonObject);
    ~Shot();

    BackgroundPrz* background = nullptr;
    BackgroundPrz* tempBackground =  nullptr;

    QTimer* paintTimer = new QTimer(this);

    // collection of json parameters, name as the key

    QHash<QString, QJsonObject> templateParams;

    // collection of used sequences, it is currently 1

    QList <SequenceData*> seqs;

    // those are used to handle the animated move of the shot

    QTimeLine *timer = new QTimeLine(80);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;

    // this defines the kind of displacement the timeline has to do

    BoxState mod = BoxState::REGULAR;

    // the position of seq[0] in the shot, when multiboxing, this variable has to be in the sequence
    int frameIn = 0;

    // flags used by the timeline on insertion and movement
    bool inserted = false;
    bool animated = false;
    bool prevposresetrequested = false;

    // valuable informations retaining the wanted value this could be considered the real model of the shots

    float previousxpos;
    float previousboxwidth;
    float mousePosXonClick;

    // if not generated from a saved object, generate templateParams from the static config

    void generateParamsFromTemplate();

    // if the object is created from a jsonTemplate, the function sets the parameters to the templateparams

    void generateParamsFromJsonShot(QJsonObject);

    // shots have to be positionned on frame, not on pixels, on each positionning, the closed start of a frame is evaluated with x and shot is set on this pos

    void setXToFrame(float x);

    // validate the changed done, the temp variables are not now set to current situation

    void setPreviousToCurrent();

    // restore shot to previous settings

    void restore();

    // handle animation while selection is moving
    void animatedMove(float pos);


    void setSize(int realsize);

    // for the save format
    QJsonObject generateJson();
    // for the export format
    QJsonObject generateExportJson(int);

    // usefull functions to evaluate relative pos

    static int roundedTo10(float x);

    inline bool wasLeftOf(Shot* OtherCube)
    {
        return  previousxpos<OtherCube->previousxpos;
    }

    inline int posOfMidd(){
        return previousxpos+(previousboxwidth/2);
    }
    inline bool isMyMiddBefore(float e){
        return e >= previousxpos+(rect().width())/2;
    }
    inline bool isInMyFirstHalf(float e){
        return e>= previousxpos && e < previousxpos+(rect().width())/2;
    }
    inline bool isInMySecondHalf(float e){
        return e >= previousxpos+(rect().width())/2 && e < previousxpos+rect().width();
    }

    // validation to modify
    bool validateSizeChange(int);
    bool validatePosChange(int);
    int smallestSequence();

    // evaluate if a point is the xrange of shot

    bool inXRange(int, Shot* other);

    void displayError();
    void checkIntegrity();


public slots:

    void setAnimatedFalse();



protected:

    // those are overrided functions called on event, there was a time when i had to detect position of the mouse in the object to trigger actions

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *e);

    // defines de calculed area of this object, whenever the scene has to calculate informations related to this object, this is called
    QRectF boundingRect() const;
};
#endif // SHOT_H
