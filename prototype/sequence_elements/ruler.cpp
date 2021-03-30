#include "ruler.h"
#include "QDebug"
#include <QGraphicsLineItem>

ruler::ruler(float rulerSize):QGraphicsItem()
{
    setSize(rulerSize);
    background->setHeight(40);
}

void ruler::xtand(float f)
{
    background->setWidth(background->width()+f);
}

void ruler::setSize(float f)
{

    background->setWidth(f);

}

ruler::~ruler()
{
    //        delete(&grades);
    //        delete(&gradestext);
    //        delete(&background);
}

QRectF ruler::boundingRect() const
{
    return *background;
}

void ruler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(*  background, QColor(230,240,200));
    double scaleValue =1/painter->transform().m11();
    painter->save();
    painter->scale(scaleValue, 1);
    int lineCount = 0;
    for (int i = 0; i < background->width() ; i++)
    {
        if (i % framesize == 0){
            if (scale >0.5 || (i%(framesize*10) == 0 && scale > 0.05)|| i%(framesize*100)==0){
                QLineF* l = new QLineF (i*scale, -60, i*scale, lineCount%(5) == 0 ? -70 : -65);
                painter->drawLine(*l);
                delete(l);
                lineCount++;
            }
        }

        if (i%(framesize*10)==0){
            if (scale > 0.3 || (i%(framesize*50)==0 && scale >  0.05) || i%(framesize*500)==0 ){
            QString t = QVariant(i/framesize).toString();
            painter->drawText((i-((5.5*t.length())/2/scale))*scale, -78, t);
            }
        }
    }
    painter->restore();
}
