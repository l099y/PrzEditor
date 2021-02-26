#ifndef PRZBOX_H
#define PRZBOX_H

#include <QGraphicsRectItem>

class PrzBox : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    PrzBox(QObject* parent = nullptr);
    ~PrzBox();
signals :
    void myPressSignal();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // PRZBOX_H
