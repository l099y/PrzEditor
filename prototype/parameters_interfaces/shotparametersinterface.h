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

    // selected shots

    QList<Shot*> shots;

    // relay the list to each parameterwidget

    void setShot(QList<Shot*> shot);

    //Collection of the widgts used to interact with each parameter

    QList<CustomShotParameterInterface*> parameters;

    //Some of the parameters are not handled by the JSON config, so the controllers are here

    QLabel* title = new QLabel(this);
    QLineEdit* path = new QLineEdit(this);
    QLineEdit* bgPath = new QLineEdit(this);
    QSpinBox* widthInput = new QSpinBox(this);
    QSpinBox* positionInput = new QSpinBox(this);
    QSpinBox* frameInInput = new QSpinBox(this);
    QPushButton* validateSeq = new QPushButton(this);
    QPushButton* clearBackgroundPrz = new QPushButton(this);

signals:
    // call to create the proper action in qundostack

    void valueChangedRequest(QList<Shot*>, QJsonObject);
    void changeShotSize(int, QString);
    void changeShotPosition(int, QString);
    void changeFrameIn(Shot*, int);
    void displayError(QString, int);
    void clearBackgrounds(QList <Shot*>);

public slots:

    //on controllers input change, those are triggered

    void updateShotPos();
    void clearBackgrounds();
    void changedShotSize();
    void changedShotPosition();
    void changedFrameInValue();
    void RequestValueChanged(QJsonObject);
    void changeSeqPathAndParse();
};

#endif // SHOTPARAMETERSINTERFACE_H
