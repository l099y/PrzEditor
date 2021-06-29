#include "backgroundprz.h"
#include <QJsonObject>
#include <QFile>

BackgroundPrz::BackgroundPrz(QObject *parent) : QObject(parent)
{

}

BackgroundPrz::BackgroundPrz(QJsonObject obj, QObject *parent)
{
  this->path = obj.value("path").toString();
  this->filename = obj.value("filename").toString();
}

bool BackgroundPrz::checkIntegrity()
{
    corrupted = !(QFile::exists(path+"/"+filename));
    return !corrupted;
}

QJsonObject BackgroundPrz::generateJson()
{
    QJsonObject ret;
    ret.insert("filename", filename);
    ret.insert("path", path);
    return ret;

}
