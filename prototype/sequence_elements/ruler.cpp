#include "ruler.h"
#include "QDebug"
#include <QGraphicsLineItem>
#include <sequence_elements/timelinescene.h>

Ruler::Ruler(float rulerSize):QGraphicsItem()
{
    setSize(rulerSize);
    background->setHeight(150);
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
    auto timelineView = scene()->views()[0];

    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());
    auto timeline = dynamic_cast<TimelineScene*>(scene());

    // setting the bounding rect in relation with displayed part of the scene

    auto sceneStart = timeline->getVisibleRect().x();
    return QRectF(background->width()-(background->width()-sceneStart),0, background->width()-sceneStart, 150);
}

void Ruler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // draw the non variable elements
    QRect a (0,0,background->width(), 60);
    painter->fillRect(a, QColor(255,255,180));
    double scaleValue =1/painter->transform().m11();
    QRect viewport_rect(0, 0, view->viewport()->width(), view->viewport()->height());
    QRectF visible_scene_rect = view->mapToScene(viewport_rect).boundingRect();

    painter->save(); // save the current state of the painter

    painter->scale(scaleValue, 1); // change the scale to proportion of 1

    for (int i = static_cast<int>(visible_scene_rect.x())-100000; i < static_cast<int>(visible_scene_rect.x()+visible_scene_rect.width())+100000 ; i++)
        // i have encounted a "bug" i extended the area to avoid it
    {
        if(i>= visible_scene_rect.x()-10000 && i <= visible_scene_rect.x()+visible_scene_rect.width()+10000){
            //drawing lines
            if (i % framesize == 0){
                if (scale >0.5 || (i%(framesize*10) == 0 && scale > 0.05)|| (i%(framesize*100)==0 && scale > 0.007) || i%(framesize*1000) == 0){
                    QLineF* l = new QLineF (i*scale, 55, i*scale, 60);
                    painter->drawLine(*l);
                    delete(l);
                }
            }
            // drawing more lines and labels
            if (i%(framesize*10)==0){
                if (scale > 0.3 || (i%(framesize*50)==0 && scale >  0.07) || (i%(framesize*500)==0 && scale > 0.01) || i%(framesize*5000)==0){
                    QString t = QVariant(i/framesize).toString();
                    painter->drawText((i-((5.5*t.length())/2/scale))*scale, 35, t);
                    painter->setPen(QColor(Qt::white));
                    QLineF* l = new QLineF (i*scale, 60, i*scale, 65);
                    painter->drawLine(*l);

                    QLineF* ll = new QLineF (i*scale, 145, i*scale, 150);
                    painter->drawLine(*ll);
                    painter->setPen(QColor(Qt::black));

                    delete(l);
                    delete(ll);
                }
            }
        }
    }
    painter->restore(); // set the state of the painter back to current scaling of timeline
}
