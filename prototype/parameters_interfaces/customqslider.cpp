#include "customqslider.h"
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

CustomQSlider::CustomQSlider(QWidget* parent): QSlider(parent)
{

}

bool CustomQSlider::event(QEvent * e)
{
    if (e->type()==QEvent::Wheel || e->type()==QEvent::KeyPress){
        qDebug()<<"specific event in slider triggered";
        return false;
    }
    QSlider::event(e);
    return true ;
}

void CustomQSlider::mousePressEvent(QMouseEvent* event)
{
    qDebug()<<"slider clicked";
    if (event->button() == Qt::LeftButton)
       {
           if (orientation() == Qt::Vertical)
               setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
           else
               setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

           event->accept();
       }
    QSlider::mousePressEvent(event);

    emit(clicked());
}
