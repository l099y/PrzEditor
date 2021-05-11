#include "tbesounddata.h"
#include <QJsonObject>
#include <QFile>

TbeSoundData::TbeSoundData(QObject *parent) : QObject(parent)
{

}

TbeSoundData::TbeSoundData(QJsonObject data, QObject *parent):QObject(parent)
{
    filename = data.value("filename").toString();
    path = data.value("path").toString();
    length = data.value("length").toDouble();
    starttime = data.value("starttime").toDouble();
    stoptime = data.value("stoptime").toDouble();
}

TbeSoundData::~TbeSoundData()
{

}

bool TbeSoundData::checkIntegrity()
{
    return QFile::exists(path.append("/").append(filename));
}

QJsonObject TbeSoundData::generateJson()
{
    QJsonObject ret;

    ret.insert("filename", filename);
    ret.insert("path", path);
    ret.insert("length",length);
    ret.insert("starttime", starttime);
    ret.insert("stoptime", stoptime);

    return ret;
}
