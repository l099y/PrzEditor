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
    bool operator==(const BackgroundPrz &other) { return filename == other.filename && path == other.path; }
signals:

};

#endif // BACKGROUNDPRZ_H
