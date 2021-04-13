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
    QHash<QString, QList<SequenceData*>>* currentExpandedFolderSequences = new QHash<QString, QList<SequenceData*>>();
    QSet<QString> corruptedSequences;
    QSet<QString> usedSequences;
    QList<SequenceData*> GenerateSequencesFromDir(QDir* dir);
    inline fileInf getReleventInfo(QString* path);
    void printStoredSequences();
    void clearSequencesInDir();
};

#endif // SEQUENCEREGISTER_H
