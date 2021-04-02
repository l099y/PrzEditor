#ifndef GRAPHICSEQUENCE_H
#define GRAPHICSEQUENCE_H

#include <QGraphicsRectItem>
#include <QObject>
#include <filesystem/sequencedata.h>

class GraphicSequence : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    SequenceData* sequence;
    GraphicSequence(SequenceData* sek);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSEQUENCE_H
