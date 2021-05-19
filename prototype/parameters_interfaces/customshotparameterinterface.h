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
#include <QLineEdit>
#include <parameters_interfaces/customqslider.h>
#include <QFileDialog>

class CustomShotParameterInterface : public QWidget
{
    Q_OBJECT
public:
    explicit CustomShotParameterInterface(QJsonObject, QWidget *parent = nullptr);
    QJsonObject param;
    QList<Shot*> shots;

    CustomQSlider* cs = nullptr;
    QSpinBox*   sb  = nullptr;
    QCheckBox* cb = nullptr;
    QDoubleSpinBox* sd = nullptr;
    QPushButton* bt = nullptr;
    QComboBox* combb = nullptr;
    QLineEdit* le = nullptr;


    QFileDialog* dialog = nullptr;
    QString selectedFile = "";

    void setShot(QList<Shot*>);
    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();
    void InitPlayMod();
    void InitLabel();
    void setParamValueInShot(QString value);

    inline QString getParamValueFromShot();

    inline QString paramName();
    inline void evaluateGlowDisplay();
signals:
    void valueChangeRequest(QJsonObject newparam);
public slots:
    void evaluateDialogSelection();
    void evaluateFileCheckBox();
    void setValue();
    void updateFloatControllersFromCs();
    void updateFloatControllersFromSd();

protected:
    virtual void focusOutEvent(QFocusEvent *event);
    virtual bool event(QEvent*);
};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
