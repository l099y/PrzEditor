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
     qDebug()<< painter->transform().m11()<< " - "<<scale;

    for (int i = 0; i < background->width() ; i++)
    {
        if (i % framesize == 0){
            QLineF* l = new QLineF (i, -60, i, i%(framesize*10) == 0 ? -75 : -65);

            if (i%(framesize*10)==0){
                double scaleValue =1/painter->transform().m11();
                    painter->save();
                    painter->scale(scaleValue, 1);
                    QString t = QVariant(i/framesize).toString();
                    painter->drawText((i-((3.8*t.length())))*scale, -78, t);
                    painter->restore();

            }
            painter->drawLine(*l);
            delete(l);
        }
    }
}
