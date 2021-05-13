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
    QLabel* title = new QLabel(this);
    QLineEdit* path = new QLineEdit(this);

    QSpinBox* widthInput = new QSpinBox(this);
    QSpinBox* positionInput = new QSpinBox(this);

signals:
    void valueChangedRequest(Shot*, QJsonObject);
    void changeShotSize(int, QString);
    void changeShotPosition(int, QString);
public slots:
    void updateShotPos();
    void changedShotSize();
    void changedShotPosition();
    void RequestValueChanged(QJsonObject);
};

#endif // SHOTPARAMETERSINTERFACE_H
