#ifndef SEQUENCEROOM_H
#define SEQUENCEROOM_H

#include <QGraphicsScene>
#include <filesystem/sequencedata.h>
#include <filesystem/sequenceregister.h>
#include <QList>

class SequenceRoom : public QGraphicsScene
{
    Q_OBJECT
public:

    SequenceRoom(QObject *parent, SequenceRegister* reg);
    SequenceRegister* przreg;
public slots:
    void displaySequences(QString path);
};

#endif // SEQUENCEROOM_H
