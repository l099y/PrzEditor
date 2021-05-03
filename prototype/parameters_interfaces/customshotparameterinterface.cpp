#include "customshotparameterinterface.h"
#include <QDebug>
#include <QHBoxLayout>

CustomShotParameterInterface::CustomShotParameterInterface(QJsonObject data, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    param=data;
    QLabel* namelab = new QLabel(this);
    this->setMinimumHeight(30);
    namelab->setText(param.value("name").toString());
    namelab->setMinimumWidth(300);
    layout->addWidget(namelab);

    if (param.value("type").toString()=="0"){ //INT type
        InitInt();
    }
    else if (param.value("type").toString()=="1"){ //FLOAT type
        InitFloat();
    }
    else if (param.value("type").toString()=="2"){ //BOOL type
        InitBool();
    }
    else if (param.value("type").toString()=="3"){ //FILE type
        InitFile();
    }
    else if (param.value("type").toString()=="4"){ //PLAYMOD type
        InitPlayMod();
    }
}

void CustomShotParameterInterface::setShot(Shot * shot)
{
    this->shot = shot;
    if (param.value("type").toString()=="0"){ //INT type
        sb->setValue(getParamValueFromShot().toInt());
    }
    else if (param.value("type").toString()=="1"){ //FLOAT type
        sd->setValue(getParamValueFromShot().toFloat());
    }
    else if (param.value("type").toString()=="2"){ //BOOL type
        cb->setChecked(getParamValueFromShot().toInt());
    }
    else if (param.value("type").toString()=="3"){ //FILE type
        // need to configure the dialog
    }
    else if (param.value("type").toString()=="4"){ //PLAYMOD type
        combb->setCurrentIndex(getParamValueFromShot().toInt());
        // need to configure the dialog
    }

}

void CustomShotParameterInterface::InitInt()
{
    sb = new QSpinBox(this);
    sb->setMinimum(param.value("minval").toString().toInt());
    sb->setMaximum(param.value("maxval").toString().toInt());
    connect (sb, SIGNAL(editingFinished()), this, SLOT(setValue()));
    sb->setMinimumWidth(120);
    layout()->addWidget(sb);

}

void CustomShotParameterInterface::InitFloat()
{
    sd = new QDoubleSpinBox(this);
    sd->setMinimum(param.value("minval").toString().toInt());
    sd->setMaximum(param.value("maxval").toString().toInt());
    sd->setMinimumWidth(120);
    layout()->addWidget(sd);
    connect (sd, SIGNAL(editingFinished()), this, SLOT(setValue()));
}

void CustomShotParameterInterface::InitBool()
{
    cb = new QCheckBox(this);
    layout()->addWidget(cb);
    connect (cb, SIGNAL(pressed()), this, SLOT(setValue()));
}

void CustomShotParameterInterface::InitFile()
{
    bt = new QPushButton(this);
    bt->setText("select");
    layout()->addWidget(bt);
}

void CustomShotParameterInterface::InitPlayMod()
{
    combb = new QComboBox(this);
    combb->setEditable(false);
    combb->addItem("Pause at end");
    combb->addItem("To next scene");
    combb->addItem("Stop at Begining");
    combb->addItem("Repeat");
    connect (combb, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue()));
    layout()->addWidget(combb);

}

QString CustomShotParameterInterface::getParamValueFromShot()
{
    auto json = shot->templateParams.value(paramName());
    return json.value("value").toString();
}

void CustomShotParameterInterface::setParamValueInShot(QString value)
{

    QJsonObject a = shot->templateParams.value(paramName());
    a.insert("value", value);
    shot->templateParams.insert(paramName(), a);
    qDebug()<<a;
}

QString CustomShotParameterInterface::paramName()
{
    return param.value("name").toString();
}

void CustomShotParameterInterface::setValue()
{

    if (param.value("type").toString()=="0"){ //INT type
        setParamValueInShot(QString::fromStdString(std::to_string(sb->value())));
    }
    else if (param.value("type").toString()=="1"){ //FLOAT type
        setParamValueInShot(QString::fromStdString(std::to_string(sd->value())));
    }
    else if (param.value("type").toString()=="2"){ //BOOL type
        setParamValueInShot(QString::fromStdString(std::to_string(!cb->isChecked())));
    }
    else if (param.value("type").toString()=="4"){ //PLAYMOD type
        setParamValueInShot(QString::fromStdString(std::to_string(combb->currentIndex())));
    }
}

void CustomShotParameterInterface::focusOutEvent(QFocusEvent *event)
{

    QWidget::focusOutEvent(event);
}
