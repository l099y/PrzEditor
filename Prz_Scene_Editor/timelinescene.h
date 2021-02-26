#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <przbox.h>

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:

    TimelineScene(QObject* parent = nullptr);
    ~TimelineScene();
protected:
   void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public slots:
   void displayItems();
};

#endif // TIMELINESCENE_H
