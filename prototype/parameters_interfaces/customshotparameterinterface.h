#ifndef CUSTOMSHOTPARAMETERINTERFACE_H
#define CUSTOMSHOTPARAMETERINTERFACE_H

#include <QWidget>
#include <QJsonObject>


class CustomShotParameterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CustomShotParameterInterface(QJsonObject, QWidget *parent = nullptr);
    QJsonObject param;

signals:

};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
