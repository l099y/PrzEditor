#include "sequencedata.h"
#include "QFile"
#include <QtDebug>

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

bool SequenceData::CheckIntegrity()
{
    return true;
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
