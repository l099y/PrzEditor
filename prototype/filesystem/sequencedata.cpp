#include "sequencedata.h"
#include "QFile"
#include <QtDebug>

SequenceData::SequenceData(QObject* parent): QObject(parent)
{

}

int SequenceData::sequencelength()
{
    return (endIdx+1)-startIdx;
}

bool SequenceData::CheckIntegrity()
{
    foreach (QString currentPath, files){
        QFile file(currentPath);
                if (!file.open(QIODevice::ReadOnly))
                    return false;
    }
    return true;
}
