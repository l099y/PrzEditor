#include "sequenceregister.h"
#include "QDebug"
#include <filesystem/sequencedata.h>


SequenceRegister::SequenceRegister(QObject* parent): QObject(parent)
{

}

// parsing a list of string to find sequences and background.. background should be detected in another function for more clarity, but the files are similar so it is done here.

QList<SequenceData*> SequenceRegister::GenerateSequencesFromDir(QDir *dir)
{
    QList<SequenceData*> ret;
    QList<BackgroundPrz*> backgroundlist;

    // getting a filtered list of .prz filenames

    QStringList paths = dir->entryList(QStringList()<<"*.prz", QDir::Files);
    int sequenceCurrentIdx = 0;

    QString sequenceName;
    bool newSeq = true;
    SequenceData *temp;

    for (int i = 0; i <paths.size(); i ++){
        auto current =  getReleventInfo(&paths[i]);
        if (current.idx==-1 && newSeq){
            BackgroundPrz* bg = new BackgroundPrz();
            bg->path = dir->path();
            bg->filename = paths[i];
            backgroundlist.append(bg);
            newSeq = true;

        }
        else{
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
            if (current.idx == -1 || current.idx != sequenceCurrentIdx + 1 || current.name != temp->name){
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
    }
    if (!newSeq){
    temp->endIdx = sequenceCurrentIdx;
    temp->name = temp->name.append(".[%1-%2]").arg(temp->startIdx).arg(temp->endIdx);
    ret.append(temp);
    }
    currentExpandedFolderBackground->insert(dir->absolutePath(), backgroundlist);
    currentExpandedFolderSequences->insert(dir->absolutePath(), ret); // insert the found sequences in hashmap, its key is the dirpath
    return ret;
}

// same as sequences but simpler

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
        while (posInString<path->length() && path->at(posInString) != "."  ){
            idxString.append(path->at(posInString));
            posInString++;
            ret.padding++;
        }
        bool ok;
        if (idxString == "prz")
            ret.idx = -1;
        else
            ret.idx= idxString.toUInt(&ok, 10);
        return ret;
    }
}

//void SequenceRegister::printStoredSequences()
//{
//    QHash<QString, QList<SequenceData*>>::const_iterator i = currentExpandedFolderSequences->constBegin();
//    while (i != currentExpandedFolderSequences->constEnd()) {
//        foreach (SequenceData* data, i.value())
//            {
//            qDebug()<< data->name<<" integrity is "<< data->checkIntegrity();
//        }
//        qDebug() << i.key() << ": " << i.value();
//        ++i;
//    }
//}

//void SequenceRegister::printStoredSounds()
//{
//    QHash<QString, QList<TbeSoundData*>>::const_iterator i = currentExpandedFolderSounds->constBegin();
//    while (i != currentExpandedFolderSounds->constEnd()) {
//        foreach (TbeSoundData* data, i.value())
//            {

//        }
//        ++i;
//    }
//}

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
        j++;
    }
    currentExpandedFolderSequences->clear();
    currentExpandedFolderSounds->clear();
}
