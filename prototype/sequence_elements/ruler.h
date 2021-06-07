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
    Ruler(float rulersize);
    ~Ruler();

    // retaining the state of scaling, this info can be retrieved from the view

    float scale = 1;

    // rect defining the area of the ruler
    QRectF *background = new QRectF(0,0,0,0);

    // it is helpfull to have acces to the view, but the object can access its scene, and its scene its view, so it is kinda pointless, i realised it not long ago
    QGraphicsView* view;

    // determines pixel size of a frame
    int framesize = 10;

    //manipulate the size of the ruler

    void xtand(float f);
    void setSize(float f);

public:

    // returns the area the scene will evaluate for this object

    QRectF boundingRect() const;

    // paint is called when the object has to be rerendered

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};
#endif // RULER_H
