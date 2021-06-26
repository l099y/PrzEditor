#ifndef BACKGROUNDPRZ_H
#define BACKGROUNDPRZ_H

#include <QObject>

class BackgroundPrz : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundPrz(QObject *parent = nullptr);
    QString path = "";

signals:

};

#endif // BACKGROUNDPRZ_H
