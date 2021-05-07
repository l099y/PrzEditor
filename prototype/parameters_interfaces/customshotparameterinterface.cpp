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
    else if (param.value("type").toString()=="5"){ //QString type
        InitLabel();
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
        cs->setValue(getParamValueFromShot().toFloat()/param.value("stepvalue").toString().toDouble());
    }
    else if (param.value("type").toString()=="2"){ //BOOL type
        qDebug()<<getParamValueFromShot().toInt() << "in setshot";
        cb->setChecked  (getParamValueFromShot().toInt());
    }
    else if (param.value("type").toString()=="3"){ //FILE type
        // need to configure the dialog
    }
    else if (param.value("type").toString()=="4"){ //PLAYMOD type
        combb->setCurrentIndex(getParamValueFromShot().toInt());
        // need to configure the dialog
    }
    else if (param.value("type").toString()=="5"){ //QString type
        le->setText(getParamValueFromShot());
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
    cs = new CustomQSlider(this);
    cs->setOrientation(Qt::Horizontal);
    sd->setMinimum(param.value("minval").toString().toInt());
    sd->setMaximum(param.value("maxval").toString().toInt());
    cs->setRange(param.value("softmin").toString().toInt()/param.value("stepvalue").toString().toDouble(),
                 param.value("softmax").toString().toInt()/param.value("stepvalue").toString().toDouble());
    qDebug()<<param.value("stepvalue").toString().toDouble()<< "stepvalue";

    sd->setSingleStep(param.value("stepvalue").toString().toDouble());
    sd->setMinimumWidth(50);
    cs->setMinimumWidth(150);
    layout()->addWidget(sd);
    layout()->addWidget(cs);
    connect (sd, SIGNAL(editingFinished()), this, SLOT(setValue()));
    connect (cs, SIGNAL(sliderReleased()), this, SLOT(setValue()));
    connect (sd, SIGNAL(valueChanged(double)), this, SLOT(updateFloatControllersFromSd()));
    connect (cs, SIGNAL(sliderMoved(int)), this, SLOT(updateFloatControllersFromCs()));
    connect (cs, SIGNAL(clicked()), this, SLOT(updateFloatControllersFromCs()));
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
    le = new QLineEdit(this);
    le->setReadOnly(true);
    cb = new QCheckBox(this);
    layout()->addWidget(bt);
    layout()->addWidget(le);
    layout()->addWidget(cb);
}

void CustomShotParameterInterface::InitPlayMod()
{
    combb = new QComboBox(this);
    combb->setEditable(false);
    combb->setMinimumWidth(120);
    combb->setFixedHeight(30);
    combb->addItem("Pause at end");
    combb->addItem("To next scene");
    combb->addItem("Stop at Begining");
    combb->addItem("Repeat");
    connect (combb, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue()));
    layout()->addWidget(combb);

}

void CustomShotParameterInterface::InitLabel()
{
    le = new QLineEdit(this);
    le->setMinimumWidth(220);
    le->setMinimumHeight(20);
    layout()->addWidget(le);
    connect(le, SIGNAL(editingFinished()), this, SLOT(setValue()));
}

QString CustomShotParameterInterface::getParamValueFromShot()
{
    auto json = shot->templateParams.value(paramName());
    return json.value("value").toString();
}

void CustomShotParameterInterface::setParamValueInShot(QString value)
{
    // sending a new configuration to the paraminterface, relaying it to the main to handle the change by Qundo

      QJsonObject newparam = shot->templateParams.value(paramName());


      if (newparam.value("value").toString() != value){
      newparam.insert("value", value);
      emit (valueChangeRequest(newparam));
      }
//    QJsonObject a = shot->templateParams.value(paramName());
//    a.insert("value", value);
//    shot->templateParams.insert(paramName(), a);
//    qDebug()<<a;
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
        setParamValueInShot(QString::fromStdString(std::to_string(cs->value()*param.value("stepvalue").toString().toDouble())));
    }
    else if (param.value("type").toString()=="2"){ //BOOL type
        setParamValueInShot(QString::fromStdString(std::to_string(!cb->isChecked())));
    }
    else if (param.value("type").toString()=="4"){ //PLAYMOD type
        setParamValueInShot(QString::fromStdString(std::to_string(combb->currentIndex())));
    }
    else if (param.value("type").toString()=="5"){ //QString type
        setParamValueInShot(le->text());
    }
}

void CustomShotParameterInterface::updateFloatControllersFromCs()
{
    sd->setValue(cs->value()*param.value("stepvalue").toString().toDouble());
}

void CustomShotParameterInterface::updateFloatControllersFromSd()
{
    cs->setValue(sd->value()/param.value("stepvalue").toString().toDouble());
}

void CustomShotParameterInterface::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<"i focused out a parameter widget";
    QWidget::focusOutEvent(event);
}

bool CustomShotParameterInterface::event(QEvent * e)
{
    if (e->type()==QEvent::Wheel)
        qDebug()<<e->type();
    return true ;
}


