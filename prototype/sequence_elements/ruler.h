#ifndef RULER_H
#define RULER_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QLabel>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsView>

class Ruler : public QGraphicsItem
{
public:
    float scale = 1;
    QRectF *background = new QRectF(0,-100,0,0);
    QGraphicsView* view;
//    QList<QLineF*> grades;
//    QList<QLabel*> gradestext;
    int frames = 0;
    int framesize = 10;
    Ruler(float rulersize);
    void xtand(float f);
    void setSize(float f);
    ~Ruler();


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
#endif // RULER_H
