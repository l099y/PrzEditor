#ifndef BACKGROUNDPRZ_H
#define BACKGROUNDPRZ_H

#include <QObject>

class BackgroundPrz : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundPrz(QObject *parent = nullptr);
    BackgroundPrz(QJsonObject, QObject *parent = nullptr);
    QString path = "";
    QString filename = "";


    bool corrupted = false;
    bool checkIntegrity();
    QJsonObject generateJson();
signals:

};

#endif // BACKGROUNDPRZ_H
