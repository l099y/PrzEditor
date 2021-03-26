#ifndef RULER_H
#define RULER_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QLabel>
#include <QList>
#include <QGraphicsItem>

class ruler : public QGraphicsItem
{
public:
    float scale = 1;
    QRectF *background = new QRectF(0,-100,0,0);
//    QList<QLineF*> grades;
//    QList<QLabel*> gradestext;
    int frames = 0;
    int framesize = 10;
    ruler(float rulersize);
    void xtand(float f);
    void setSize(float f);
    ~ruler();


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
#endif // RULER_H
