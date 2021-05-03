#include "customshotparameterinterface.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>


CustomShotParameterInterface::CustomShotParameterInterface(QJsonObject data, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    param=data;
    QLabel* namelab = new QLabel(this);
    layout->setMargin(3);
    this->setMinimumHeight(30);
    namelab->setText(param.value("name").toString());
    namelab->setMinimumWidth(200);
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

}

void CustomShotParameterInterface::InitInt()
{
    sb = new QSpinBox(this);
    layout()->addWidget(sb);

}

void CustomShotParameterInterface::InitFloat()
{
    sd = new QSlider(this);
    sd->setOrientation(Qt::Horizontal);
    sd->setMinimum(param.value("minval").toInt());
    sd->setMinimum(param.value("maxval").toInt());
    layout()->addWidget(sd);
}

void CustomShotParameterInterface::InitBool()
{
    cb = new QCheckBox(this);
    layout()->addWidget(cb);
}

void CustomShotParameterInterface::InitFile()
{
    bt = new QPushButton(this);
    bt->setText("select");
    layout()->addWidget(bt);
}

QString CustomShotParameterInterface::getParamValueFromShot()
{
    auto json = shot->templateParams.value(paramName());
    return json.value("value").toString();
}

void CustomShotParameterInterface::setParamValueInShot(QString value)
{
    auto json = shot->templateParams.value(param.value("name").toString());
    json["value"]=value;
}

QString CustomShotParameterInterface::paramName()
{
    return param.value("name").toString();
}
