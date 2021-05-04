    #include "ruler.h"
#include "QDebug"
#include <QGraphicsLineItem>

Ruler::Ruler(float rulerSize):QGraphicsItem()
{
    setSize(rulerSize);
    background->setHeight(40);
}

void Ruler::xtand(float f)
{
    background->setWidth(background->width()+f);
}

void Ruler::setSize(float f)
{

    background->setWidth(f);

}

Ruler::~Ruler()
{
    //        delete(&grades);
    //        delete(&gradestext);
    //        delete(&background);
}

QRectF Ruler::boundingRect() const
{
    return *background;
}

void Ruler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(*background, QColor(80,150,150));
    double scaleValue =1/painter->transform().m11();
    QRect viewport_rect(0, 0, view->viewport()->width(), view->viewport()->height());
    QRectF visible_scene_rect = view->mapToScene(viewport_rect).boundingRect();
    painter->save();
    painter->scale(scaleValue, 1);
    qDebug()<<"scenepos x: "<<visible_scene_rect.x()<< " - width : "<<visible_scene_rect.width();
    for (int i = static_cast<int>(visible_scene_rect.x()); i < static_cast<int>(visible_scene_rect.x()+visible_scene_rect.width()) ; i++)
    {
        if(i>= visible_scene_rect.x() && i <= visible_scene_rect.x()+visible_scene_rect.width()){
        if (i % framesize == 0){
            if (scale >0.5 || (i%(framesize*10) == 0 && scale > 0.05)|| (i%(framesize*100)==0 && scale > 0.007) || i%(framesize*1000) == 0){
                QLineF* l = new QLineF (i*scale, -60, i*scale, -65);
                painter->drawLine(*l);
                delete(l);
            }
        }

        if (i%(framesize*10)==0){
            if (scale > 0.3 || (i%(framesize*50)==0 && scale >  0.05) || (i%(framesize*500)==0 && scale > 0.007) || i%(framesize*5000)==0){
            QString t = QVariant(i/framesize).toString();
            painter->drawText((i-((5.5*t.length())/2/scale))*scale, -78, t);
            }
        }
        }
    }
    painter->restore();
}
