#ifndef CUSTOMSHOTPARAMETERINTERFACE_H
#define CUSTOMSHOTPARAMETERINTERFACE_H

#include <QWidget>
#include <QJsonObject>
#include <sequence_elements/shot.h>

class CustomShotParameterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CustomShotParameterInterface(QJsonObject, QWidget *parent = nullptr);
    QJsonObject param;
    Shot* shot = nullptr;

    void setShot(Shot*);
    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();
signals:

};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
