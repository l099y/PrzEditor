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
    QList<Shot*> shots;
    void setShot(QList<Shot*> shot);
    QList<CustomShotParameterInterface*> parameters;
    QLabel* title = new QLabel(this);
    QLineEdit* path = new QLineEdit(this);

    QSpinBox* widthInput = new QSpinBox(this);
    QSpinBox* positionInput = new QSpinBox(this);
    QSpinBox* frameInInput = new QSpinBox(this);
    QPushButton* validateSeq = new QPushButton(this);

signals:
    void valueChangedRequest(QList<Shot*>, QJsonObject);
    void changeShotSize(int, QString);
    void changeShotPosition(int, QString);
    void changeFrameIn(Shot*, int);

    void displayError(QString, int);
public slots:
    void updateShotPos();
    void changedShotSize();
    void changedShotPosition();
    void changedFrameInValue();
    void RequestValueChanged(QJsonObject);
    void changeSeqPathAndParse();
};

#endif // SHOTPARAMETERSINTERFACE_H
