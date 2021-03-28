#include "sequenceregister.h"
#include "QDebug"

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
            temp->name = current.name;
            temp->startIdx = current.idx;
            temp->path = dir->path();
            temp->files.append(dir->path().append("/").append(paths[i]));
            sequenceCurrentIdx = current.idx;
            newSeq = false;
        }
        else{
            if (current.idx != sequenceCurrentIdx + 1 || current.name != temp->name){
                temp->endIdx = sequenceCurrentIdx;
                temp->name = temp->name.append(".%1-%2").arg(temp->startIdx).arg(temp->endIdx);
                ret.append(temp);
                i--;
                newSeq = true;
            }
            else{
                temp->files.append(dir->path().append("/").append(paths[i]));
                sequenceCurrentIdx = current.idx;
            }
        }
    }
    temp->endIdx = sequenceCurrentIdx;
    temp->name = temp->name.append(".%1-%2").arg(temp->startIdx).arg(temp->endIdx);
    ret.append(temp);
    return ret;
}




inline fileInf SequenceRegister::getReleventInfo(QString* path)
{
    if (path->length() != 0){

        fileInf ret;
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
        }
        bool ok;
        ret.idx= idxString.toUInt(&ok, 10);
        return ret;
    }
}
