#include "customshotparameterinterface.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>

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

    if (param.value("type").toString()=="0"){
        InitInt();
    }
    else if (param.value("type").toString()=="1"){
        InitFloat();
    }
    else if (param.value("type").toString()=="2"){
        InitBool();
    }
    else if (param.value("type").toString()=="3"){
        InitFile();
    }
}

void CustomShotParameterInterface::setShot(Shot * shot)
{
    this->shot = shot;

}

void CustomShotParameterInterface::InitInt()
{
    QSpinBox* sb = new QSpinBox(this);
    layout()->addWidget(sb);

}

void CustomShotParameterInterface::InitFloat()
{
    QSlider* sd = new QSlider(this);
    sd->setOrientation(Qt::Horizontal);
    layout()->addWidget(sd);
}

void CustomShotParameterInterface::InitBool()
{
    QCheckBox* cb = new QCheckBox(this);
    layout()->addWidget(cb);
}

void CustomShotParameterInterface::InitFile()
{
    QPushButton* bt = new QPushButton(this);
    bt->setText("select");
    layout()->addWidget(bt);
}
