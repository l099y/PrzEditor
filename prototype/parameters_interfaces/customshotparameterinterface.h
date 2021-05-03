#ifndef CUSTOMSHOTPARAMETERINTERFACE_H
#define CUSTOMSHOTPARAMETERINTERFACE_H

#include <QWidget>
#include <QJsonObject>
#include <sequence_elements/shot.h>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>

class CustomShotParameterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CustomShotParameterInterface(QJsonObject, QWidget *parent = nullptr);
    QJsonObject param;
    Shot* shot = nullptr;

    QSpinBox* sb  = nullptr;
    QCheckBox* cb = nullptr;
    QSlider* sd = nullptr;
    QPushButton* bt = nullptr;

    void setShot(Shot*);
    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();

    QString getParamValueFromShot();
    void setParamValueInShot(QString value);
    inline QString paramName();
signals:

};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
