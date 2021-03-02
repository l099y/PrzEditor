#ifndef EMITTER_H
#define EMITTER_H

#include <QObject>
#include <QGraphicsRectItem>

class Emitter : public QObject
{
    Q_OBJECT
public:
    explicit Emitter(QObject *parent = nullptr);

    Emitter();
    ~Emitter();
    void emitleftxtndactivated();
    void emitrightxtndactivated();
    void emitxtendDeactived();
signals:
     void leftxtndactivated();
     void rightxtndactivated();
     void xtendDeactived();

};

#endif // EMITTER_H
