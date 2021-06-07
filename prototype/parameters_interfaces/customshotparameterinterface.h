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

    // those vars get an object assigned if the parameter received is of a certain type

    CustomQSlider* cs = nullptr;
    QSpinBox*   sb  = nullptr;
    QCheckBox* cb = nullptr;
    QDoubleSpinBox* sd = nullptr;
    QPushButton* bt = nullptr;
    QComboBox* combb = nullptr;
    QLineEdit* le = nullptr;
    QFileDialog* dialog = nullptr;
    QString selectedFile = "";

    // when the selection is changed, each parameters receive a new list of shots, if you add more parameter type, you have to add definition here

    void setShot(QList<Shot*>);

    // used once in the constructor of the object depending of the type of parameter received, if you add more parameter type, you have to add definition here

    void InitInt();
    void InitFloat();
    void InitBool();
    void InitFile();
    void InitPlayMod();
    void InitLabel();

    // this function receive a value, and ask a parameter change in the qundostack

    void setParamValueInShot(QString value);

    // access de value of parameter in the list of shots

    inline QString getParamValueFromShot();

    inline QString paramName();

    // this should be done in the shots
    inline void evaluateGlowDisplay();
signals:

    // request the actions of changing parameter value in the list of shots

    void valueChangeRequest(QJsonObject newparam);

public slots:

    void evaluateDialogSelection();

    // this function sets the value sent to the undostack depending of the parameter type, if you add more parameter type, you have to add definition here

    void setValue();

    // those should be done by the shot...
    void updateFloatControllersFromCs();
    void updateFloatControllersFromSd();

protected:
    virtual void focusOutEvent(QFocusEvent *event);
    virtual bool event(QEvent*);
};

#endif // CUSTOMSHOTPARAMETERINTERFACE_H
