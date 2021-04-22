#include "sequencedata.h"
#include "QFile"
#include <QtDebug>

SequenceData::SequenceData(QObject* parent): QObject(parent)
{

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
