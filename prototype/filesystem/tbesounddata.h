#ifndef TBESOUNDDATA_H
#define TBESOUNDDATA_H

#include <QObject>

class TbeSoundData : public QObject
{
    Q_OBJECT
public:
    TbeSoundData(QObject *parent = nullptr);
    TbeSoundData(QJsonObject, QObject *parent = nullptr);
    ~TbeSoundData();

    QString filename = "";
    QString path = "";
    float length = 0;
    float starttime = 0;
    float stoptime = 0;
    bool corrupted = false;

    bool checkIntegrity();
    QJsonObject generateJson();

signals:

};

#endif // TBESOUNDDATA_H
