#include "customqslider.h"
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

CustomQSlider::CustomQSlider(QWidget* parent): QSlider(parent)
{

    //setStyleSheet("QSlider::handle:horizontal{background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b0b0b0, stop:1 white); border: 0px solid #5c5c5c; width: 40px; margin: -6px 0; border-radius: 5px;}");
}

bool CustomQSlider::event(QEvent * e)
{
    if (e->type()==QEvent::Wheel || e->type()==QEvent::KeyPress){
        e->ignore();
        return false;

    }
    QSlider::event(e);
    return true ;
}

void CustomQSlider::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
       {
           if (orientation() == Qt::Vertical)
               setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
           else
               setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;
       }


    emit(clicked());
    QSlider::mousePressEvent(event);
}
