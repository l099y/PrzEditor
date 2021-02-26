#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
    public:

    TimelineScene(QObject* parent = nullptr);
    ~TimelineScene();

public slots:
   void newRect();
   void debugItems();
   void clearItems();
};

#endif // TIMELINESCENE_H
