#include "sequenceregister.h"
#include "QDebug"
#include <filesystem/sequencedata.h>


SequenceRegister::SequenceRegister(QObject* parent): QObject(parent)
{

}



QList<SequenceData*> SequenceRegister::GenerateSequencesFromDir(QDir *dir) // this function will be long... i need to evaluate the hole content of the dir and make the proper cuts in the sequences
{
    QList<SequenceData*> ret;
    QStringList paths = dir->entryList(QStringList()<<"*.prz", QDir::Files);
    int sequenceCurrentIdx = 0;

    QString sequenceName;
    bool newSeq = true;
    SequenceData *temp;

    for (int i = 0; i <paths.size(); i ++){
        auto current =  getReleventInfo(&paths[i]);

        if (newSeq){
            temp = new SequenceData(this);
            temp->sequencefilename = current.name;
            temp->name = current.name;
            temp->startIdx = current.idx;
            temp->path = dir->path();
            temp->padding = current.padding;
            sequenceCurrentIdx = current.idx;
            newSeq = false;
        }
        else{
            if (current.idx != sequenceCurrentIdx + 1 || current.name != temp->name){
                temp->endIdx = sequenceCurrentIdx;
                temp->name = temp->name.append(".[%1-%2]").arg(temp->startIdx).arg(temp->endIdx);
                ret.append(temp);
                i--;
                newSeq = true;
            }
            else{
                sequenceCurrentIdx = current.idx;
            }
        }
    }
    temp->endIdx = sequenceCurrentIdx;
    temp->name = temp->name.append(".[%1-%2]").arg(temp->startIdx).arg(temp->endIdx);
    ret.append(temp);

    currentExpandedFolderSequences->insert(dir->absolutePath(), ret);
    //printStoredSequences();
    return ret;
}

QList<TbeSoundData *> SequenceRegister::GeneratesTbeFilesFromDir(QDir *dir)
{

    QList<TbeSoundData*> ret;
    QStringList paths = dir->entryList(QStringList()<<"*.tbe", QDir::Files);



    TbeSoundData *temp;

    for (int i = 0; i <paths.size(); i ++){
        temp = new TbeSoundData();
           temp->filename = paths[i];
           temp->path = dir->path();
           ret.append(temp);
    }
    currentExpandedFolderSounds->insert(dir->absolutePath(), ret);
    printStoredSounds();
    qDebug()<<ret<<"soundfiles found";
    return ret;
}

inline fileInf SequenceRegister::getReleventInfo(QString* path)
{
    if (path->length() != 0){

        fileInf ret;
        ret.padding=0;
        int posInString = 0;
        while (path->at(posInString)!="."){
            ret.name.append(path->at(posInString));
            posInString++;
        }
        posInString++;
        QString idxString;
        while (path->at(posInString) != "."){
            idxString.append(path->at(posInString));
            posInString++;
            ret.padding++;
        }
        bool ok;
        ret.idx= idxString.toUInt(&ok, 10);
        return ret;
    }
}

void SequenceRegister::printStoredSequences()
{
    QHash<QString, QList<SequenceData*>>::const_iterator i = currentExpandedFolderSequences->constBegin();
    while (i != currentExpandedFolderSequences->constEnd()) {
        foreach (SequenceData* data, i.value())
            {
            qDebug()<< data->name<<" integrity is "<< data->checkIntegrity();
        }
        qDebug() << i.key() << ": " << i.value();
        ++i;
    }
}

void SequenceRegister::printStoredSounds()
{
    QHash<QString, QList<TbeSoundData*>>::const_iterator i = currentExpandedFolderSounds->constBegin();
    while (i != currentExpandedFolderSounds->constEnd()) {
        foreach (TbeSoundData* data, i.value())
            {
            qDebug()<< data->filename<< "in the currentfolder";
        }
        qDebug() << i.key() << ": " << i.value();
        ++i;
    }
}

void SequenceRegister::validateUsedContent()
{
    //validateSequences();
    validateSounds();
}

void SequenceRegister::validateSequences()
{
    QHash<QString, SequenceData*>::const_iterator i = usedSequences.constBegin();
    while (i !=usedSequences.constEnd()) {
        if (!i.value()->checkIntegrity()){
            emit(displayError(i.key(), 3000));
        }
        i++;
    }
}

void SequenceRegister::validateSounds()
{
    QHash<QString, TbeSoundData*>::const_iterator i = usedSoundFiles.constBegin();
    while (i != usedSoundFiles.constEnd()) {
        if (!i.value()->checkIntegrity()){
             emit(displayError(i.key(), 3000));
        }
        i++;
    }
}

void SequenceRegister::clearSequencesInDir()
{
    QHash<QString, QList<SequenceData*>>::const_iterator i = currentExpandedFolderSequences->constBegin();
    while (i != currentExpandedFolderSequences->constEnd()) {
        foreach (SequenceData* data, i.value())
            {
            delete(data);
        }
        ++i;
    }
    QHash<QString, QList<TbeSoundData*>>::const_iterator j = currentExpandedFolderSounds->constBegin();
    while (j != currentExpandedFolderSounds->constEnd()) {
        foreach (TbeSoundData* data, j.value())
            {
            delete(data);
        }
        ++i;
    }
    currentExpandedFolderSequences->clear();
    currentExpandedFolderSounds->clear();
}
