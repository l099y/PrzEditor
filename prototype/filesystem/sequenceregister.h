#ifndef SEQUENCEREGISTER_H
#define SEQUENCEREGISTER_H

#include <QObject>
#include <filesystem/sequencedata.h>
#include <filesystem/tbesounddata.h>
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
    QHash<QString, QList<TbeSoundData*>>* currentExpandedFolderSounds = new QHash<QString, QList<TbeSoundData*>>();
    QSet<QString> corruptedSequences;
    QHash<QString, SequenceData*> usedSequences;
    QHash<QString, TbeSoundData*> usedSoundFiles;
    QList<SequenceData*> GenerateSequencesFromDir(QDir* dir);
    QList<TbeSoundData *> GeneratesTbeFilesFromDir(QDir* dir);
    inline fileInf getReleventInfo(QString* path);

    void printStoredSequences();
    void printStoredSounds();
    void clearSequencesInDir();
};

#endif // SEQUENCEREGISTER_H
