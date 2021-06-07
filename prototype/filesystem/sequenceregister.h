#ifndef SEQUENCEREGISTER_H
#define SEQUENCEREGISTER_H

#include <QObject>
#include <filesystem/sequencedata.h>
#include <filesystem/tbesounddata.h>
#include <QHash>
#include <QSet>
#include <QDir>


class SequenceRegister : public QObject
{
    Q_OBJECT
public:
    SequenceRegister(QObject* parent);

    // the only usefull collection, when the filesystemmodel reads a dir and finds usefull data, the register parse again this list, create seqdatas or/and sounddatas and store it there

    QHash<QString, QList<SequenceData*>>* currentExpandedFolderSequences = new QHash<QString, QList<SequenceData*>>();
    QHash<QString, QList<TbeSoundData*>>* currentExpandedFolderSounds = new QHash<QString, QList<TbeSoundData*>>();
    QSet<QString> corruptedSequences;

    // those might be use later

    QHash<QString, SequenceData*> usedSequences;
    QHash<QString, TbeSoundData*> usedSoundFiles;

    // main functions

    QList<SequenceData*> GenerateSequencesFromDir(QDir* dir);
    QList<TbeSoundData *> GeneratesTbeFilesFromDir(QDir* dir);

    // it is used by sequencedata aswell, it could be called from here

    static fileInf getReleventInfo(QString* path);

    // dev tools
//    void printStoredSequences();
//    void printStoredSounds();


    // those are now done by the timeline as it contains the objects to be validated
    void validateUsedContent();
    void validateSequences();
    void validateSounds();

    void clearSequencesInDir();

    signals:
    void displayError(QString, int);
};

#endif // SEQUENCEREGISTER_H
