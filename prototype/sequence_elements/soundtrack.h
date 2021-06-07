#ifndef SOUNDTRACK_H
#define SOUNDTRACK_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <filesystem/sequencedata.h>
#include <QDataStream>
#include <QHash>
#include <QJsonArray>

#include <QGraphicsRectItem>
#include <QObject>
#include <filesystem/tbesounddata.h>
#include <sequence_elements/shot.h>

class SoundTrack : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    SoundTrack();
    SoundTrack(QJsonObject);
    ~SoundTrack();

    // collection of json parameters, name as the key

    QHash<QString, QJsonObject> templateParams;

    static QList<QColor> usedColor;

    QTimeLine *timer = new QTimeLine(110);
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;

    BoxState mod = BoxState::REGULAR;

    bool inserted = false;
    bool animated = false;

    bool prevposresetrequested = false;

    TbeSoundData* soundfile;
    float previousxpos;
    float previousboxwidth;
    float mousePosXonClick;

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

//    inline bool wasLeftOf(SoundTrack* OtherCube)
//    {
//        return  previousxpos<OtherCube->previousxpos;
//    }


    bool validateSizeChange(int);
    bool validatePosChange(int);
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

#endif // SOUNDTRACK_H
