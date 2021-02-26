#include "przbox.h"
#include <QBrush>
#include <QDebug>

PrzBox::PrzBox(QObject* parent):QGraphicsRectItem(nullptr)
{

}
PrzBox::~PrzBox(){
    delete(this);
}
void PrzBox::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mousePressEvent(event); //Call the ancertor
    QBrush* brush = new QBrush(Qt::red);
    setBrush(*brush);
    qDebug()<<"i clicked on a box";
    emit myPressSignal(); //Emits the signal
}
