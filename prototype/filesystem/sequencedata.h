#ifndef SEQUENCEDATA_H
#define SEQUENCEDATA_H

#include <QObject>

class SequenceData : public QObject
{
    Q_OBJECT
public:
    SequenceData(QObject* parent);

    QStringList files;
    QString path;
    QString name;
    int startIdx;
    int endIdx;

    bool corrupted = false;
    inline int lenght();
    bool CheckIntegrity();
};

#endif // SEQUENCEDATA_H
