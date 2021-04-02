#include "graphicsequence.h"
#include <QBrush>
#include <QDebug>
#include <QPen>
#include <QPainter>

GraphicSequence::GraphicSequence(SequenceData* sek): QGraphicsRectItem()
{
    sequence = sek;
    setFlag(QGraphicsItem :: ItemIsMovable);
    setFlag(QGraphicsItem :: ItemIsSelectable);
    QBrush brush(QColor(255,200,100));
    QPen pen (Qt::gray);
    setRect(0,0,80,80);
    setPen(pen);
    setBrush(brush);
    qDebug()<<"created";
}

void GraphicSequence::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawRoundedRect(rect(),5,5);
    painter->setPen(QColor(Qt::black));
    painter->drawText(0, 20, sequence->name.left(13));
}
