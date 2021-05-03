#ifndef CUSTOMSHOTPARAMETERINTERFACE_H
#define CUSTOMSHOTPARAMETERINTERFACE_H

#include <QWidget>
#include <QJsonObject>
#include <sequence_elements/shot.h>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class CustomShotParameterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CustomShotParameterInterface(QJsonObject, QWidget *parent = nullptr);
    QJsonObject param;
    Shot* shot = nullptr;

    QSpinBox* sb  = nullptr;
    QCheckBox* cb = nullptr;
    QDoubleSpinBox* sd = nullptr;
    QPushButton* bt = nullptr;
    QComboBox* combb = nullptr;

    void setShot(Shot*);
    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();
    void InitPlayMod();
    void setParamValueInShot(QString value);

    QString getParamValueFromShot();

    inline QString paramName();
signals:

public slots:
    void setValue();

protected:
    virtual void focusOutEvent(QFocusEvent *event);
};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
