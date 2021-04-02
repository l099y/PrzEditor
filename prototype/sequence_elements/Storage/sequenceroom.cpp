#include "sequenceroom.h"
#include "sequence_elements/Storage/graphicsequence.h"
#include <QDebug>

SequenceRoom::SequenceRoom(QObject *parent, SequenceRegister* reg): QGraphicsScene(parent)
{
    przreg = reg;
    QBrush brush(QColor(50,150,200));
    setBackgroundBrush(brush);
    setSceneRect(0,0,120,300);
}

void SequenceRoom::displaySequences(QString path)
{
 if (przreg->storedSequences->contains(path))
    {
    QList<SequenceData *> list = przreg->storedSequences->value(path);
    qDebug()<<"reached displaySequences";
    foreach (QGraphicsItem *current, items()){
     this->removeItem(current);
     delete(current);
 }
 int i = 0;
 foreach (SequenceData* current, list){
     GraphicSequence*  temp =  new GraphicSequence(current);
     temp->setPos(20, (i*80)+(30*i)+10);
     addItem(temp);
     i++;
 }
 }
}
