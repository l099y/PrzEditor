#ifndef CUSTOMSHOTPARAMETERINTERFACE_H
#define CUSTOMSHOTPARAMETERINTERFACE_H

#include <QWidget>
#include <QJsonObject>
#include <sequence_elements/shot.h>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

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
    QLabel* sdinfo = nullptr;

    void setShot(Shot*);
    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();
    void setParamValueInShot(QString value);

    QString getParamValueFromShot();

    inline QString paramName();
signals:

public slots:
    void setValue();
    void refreshSdLabel();

protected:
    virtual void focusOutEvent(QFocusEvent *event);
};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
