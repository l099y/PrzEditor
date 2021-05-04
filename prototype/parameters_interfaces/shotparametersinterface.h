#ifndef SHOTPARAMETERSINTERFACE_H
#define SHOTPARAMETERSINTERFACE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <sequence_elements/shot.h>
#include <parameters_interfaces/customshotparameterinterface.h>

class ShotParametersInterface : public QWidget
{
    Q_OBJECT
public:
    explicit ShotParametersInterface(QJsonObject config, QWidget *parent = nullptr);

    // Parameters window elements
    Shot* shot = nullptr;
    void setShot(Shot* shot);
    QList<CustomShotParameterInterface*> parameters;

signals:
    void valueChangedRequest(Shot*, QJsonObject);
public slots:
    void RequestValueChanged(QJsonObject);
};

#endif // SHOTPARAMETERSINTERFACE_H
