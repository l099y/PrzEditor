#include "sequencedata.h"
#include <QFile>
#include <QtDebug>
#include <QDir>
#include <filesystem/sequenceregister.h>
#include <QFuture>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QFile>

SequenceData::SequenceData(QObject* parent): QObject(parent)
{

}

SequenceData::SequenceData(QJsonObject data, QObject* parent): QObject(parent)
{
    this->sequencefilename = data.value("sequencefilename").toString();
    this->path =  data.value("path").toString();
    this->name = data.value("name").toString();
    this->startIdx = data.value("startIdx").toInt();
    this->endIdx = data.value("endIdx").toInt();
}

SequenceData::~SequenceData()
{

}

int SequenceData::sequencelength()
{
    return (endIdx+1)-startIdx;
}

bool SequenceData::checkIntegrity()
{
    return checkIntegrityAsync();
}

bool SequenceData::checkIntegrityAsync()
{
    corruptedSubSequences.clear();
    int startPosInList= 0;
    int seqIndex= 0;

    QList<QList<int>> missingIndexesList;

    QDir dir (path);
    if (dir.exists()){


        QStringList przlists = dir.entryList(QStringList()<<"*.prz", QDir::Files);
        QStringList przlist;

        // i encountered crashes because the filter isnt applied, so i have to filter it after sadly

        foreach (QString st, przlists){
            QFile file (path+"/"+st);
            QFileInfo fi (file);
            if (fi.suffix()=="prz"){
                przlist.append(st);
            }
        }
        for (int i = 0; i <przlist.size(); i ++){
            auto current =  getReleventInfo(&przlist[i]);
            if (current.name == this->sequencefilename){ // if the filename without padding matches the current filename
                if (current.idx == this->startIdx){ // if we detect the startingindex file
                    if (current.idx == startIdx && current.idx == endIdx)
                        return true; // if the sequence is a unique frame
                    seqIndex++; //looking for next index
                    startPosInList=i; // init the variable handling decalage in the sequence
                } //
                else if (seqIndex == 0 && (current.idx>startIdx && current.idx<=endIdx)){
                    seqIndex++;
                    startPosInList = i - (current.idx); // we have to set a decalage because the first file found isn't first frame in the sequence
                    QList<int> missingIndexes;
                    for (int c = 0; c<(current.idx - startIdx); c++){ // creating the missing frame information, i could just take the first and the last
                        missingIndexes.append(startIdx+c);
                    }
                    missingIndexesList.append(missingIndexes);
                }
                else if(seqIndex != 0){ // same process on the rest of the list
                    if (current.idx == (i - startPosInList) + startIdx)
                        seqIndex++;
                    else{
                        QList<int> missingIndexes;

                        int missingindexcount = 0;
                        foreach (QList<int> current, missingIndexesList)
                            missingindexcount+=current.length();
                        for (int c = 0; c< current.idx-seqIndex-missingindexcount; c++){
                            missingIndexes.append(( seqIndex+missingindexcount)+c);
                        }

                        missingIndexesList.append(missingIndexes);

                        startPosInList = (seqIndex - current.idx);
                        seqIndex++;
                    }
                }
                if (seqIndex==sequencelength()){
                    corrupted = false;
                    return true;
                }
            }
        }
        foreach (QList<int> current, missingIndexesList){
            if (current.length()>0)
            corruptedSubSequences.insert(current[0], current.length());
        }
        corrupted = !(seqIndex>=sequencelength());
        return seqIndex>=sequencelength();
    }
    else{
        corrupted = true;
        return false;
    }
}

// parsing a string to get the proper info

fileInf SequenceData::getReleventInfo(QString* pathh)
{
    if (pathh->length() != 0){

        fileInf ret;
        ret.padding=0;
        int posInString = 0;
        while (pathh->at(posInString)!="."){
            ret.name.append(pathh->at(posInString));
            posInString++;
        }
        posInString++;
        QString idxString;
        while (posInString < pathh->length() && pathh->at(posInString) != "."){
            idxString.append(pathh->at(posInString));
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
    else{
        fileInf ret;
        ret.padding = 0;
        ret.name = "";
        ret.idx = -1;
    }
}

QJsonObject SequenceData::generateJson()
{
    QJsonObject ret;
    ret.insert("sequencefilename", sequencefilename);
    ret.insert("path", path);
    ret.insert("name", name);
    ret.insert("startIdx", startIdx);
    ret.insert("endIdx", endIdx);
    return ret;
}

QJsonObject SequenceData::generateJsonForExport()
{
    QJsonObject ret;
    QString a ("");
    a.append(path).append("/").append(sequencefilename);
    ret.insert("file", a);
    ret.insert("padding", padding);
    ret.insert("frames", generateFrames());
    return ret;
}

QJsonArray SequenceData::generateFrames()
{
    QJsonArray ret;
    QDir dir (path);
    if (dir.exists()){
        QStringList przlist = dir.entryList(QStringList()<<"*.prz", QDir::Files);
        QStringList realfilter;
        foreach (QString st, przlist){
            QFile file (path+"/"+st);
            QFileInfo fi (file);
            if (fi.suffix()=="prz"){
                realfilter.append(st);
            }
        }

        foreach (QString st, realfilter){
            auto current =  getReleventInfo(&st);
            if (current.name == this->sequencefilename && current.idx >= startIdx && current.idx <= endIdx){
                QFile file (path+"/"+st);


                QJsonObject frame;
                frame.insert("frame", current.idx);

                if(file.open(QFile::ReadOnly| QIODevice::Text)){
                int size = file.size();
                file.close();


                    qDebug()<<file.size()<< file.exists() << "in generateframe from Sequencedata" << current.name << file.errorString() << path+"/"+st  ;

                frame.insert("filesize", file.size());
                ret.append(frame);
                }
            }
        }
    }
    return ret;
}
