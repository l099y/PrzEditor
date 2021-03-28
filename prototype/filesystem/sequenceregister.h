#ifndef SEQUENCEREGISTER_H
#define SEQUENCEREGISTER_H

#include <QObject>
#include <filesystem/sequencedata.h>
#include <QHash>
#include <QSet>
#include <QDir>

struct fileInf{
    QString name;
    int idx;
};

class SequenceRegister : public QObject
{
    Q_OBJECT
public:
    SequenceRegister(QObject* parent);
    QHash<QString, SequenceData>* storedSequences = new QHash<QString, SequenceData>();
    QSet<QString>* corruptedSequences = new QSet<QString>();
    QList<SequenceData*> GenerateSequencesFromDir(QDir* dir);
    inline fileInf getReleventInfo(QString* path);
};

#endif // SEQUENCEREGISTER_H
