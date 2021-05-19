#include "customshotparameterinterface.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>


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

    //init dialog
    dialog  = new QFileDialog(this);
    QStringList filter;
    filter<< "*.mp4"<<"*.avi"<<"*.mkv";
    dialog->setNameFilters(filter);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setWindowTitle("select background");
}

void CustomShotParameterInterface::setShot(QList<Shot*>  shot)
{
    this->shots = shot;
    if (shots.length()==1){
        if (param.value("type").toString()=="0"){ //INT type
            sb->setValue(getParamValueFromShot().toInt());
            if (param.value("name").toString()=="Fade From Black Frame Out" || param.value("name").toString()=="Fade To Black Frame Out"){
                qDebug()<<shots[0]->templateParams.value("Fade From Black Frame Out")<<"in setshot";
                sb->setMaximum(shots[0]->rect().width()/10);
            }
            if (param.value("name").toString()=="Fade From Black Frame Out" || param.value("name").toString()=="Fade To Black Frame Out"){
                sb->setDisabled(false);
            }
        }
        else if (param.value("type").toString()=="1"){ //FLOAT type
            sd->setValue(getParamValueFromShot().toFloat()+0.05);
            cs->setValue(getParamValueFromShot().toFloat()/param.value("stepvalue").toString().toDouble());
            evaluateGlowDisplay();
        }
        else if (param.value("type").toString()=="2"){ //BOOL type
            cb->setChecked  (getParamValueFromShot().toInt());
        }
        else if (param.value("type").toString()=="3"){ //FILE type
            le->setText(getParamValueFromShot());
            cb->setChecked(getParamValueFromShot()==""? false: true);
            dialog->selectFile(getParamValueFromShot());
            le->setDisabled(false);
            cb->setDisabled(false);
            dialog->setDisabled(false);
        }
        else if (param.value("type").toString()=="4"){ //PLAYMOD type
            combb->setCurrentIndex(getParamValueFromShot().toInt());

        }
        else if (param.value("type").toString()=="5"){ //QString type
            le->setText(getParamValueFromShot());
            le->setDisabled(false);
        }
    }
    else{
        if (param.value("type").toString()=="0"){ //INT type
            int avg = 0;
            int count = 0;
            foreach (Shot* shot, shots){
                count++;
                avg+=shot->templateParams.value(paramName()).value("value").toString().toInt();
            }
            sb->setValue(avg/count);
            if (param.value("name").toString()=="Fade From Black Frame Out" || param.value("name").toString()=="Fade To Black Frame Out"){
                sb->setValue(0);
                sb->setDisabled(true);
            }
        }
         if (param.value("type").toString()=="1"){ //FLOAT type

            float avg = 0;
            int count = 0;
            foreach (Shot* shot, shots){
                count++;
                avg+=shot->templateParams.value(paramName()).value("value").toString().toDouble();
                evaluateGlowDisplay();
            }
            qDebug()<<avg<<"-"<<count<<"in setShotparams float";
            sd->setValue(avg/count);
            cs->setValue(avg/count/param.value("stepvalue").toString().toDouble());

        }
        else if (param.value("type").toString()=="2"){ //BOOL type

            int yes = 0;
            int no = 0;
            foreach (Shot* shot, shots){

                shot->templateParams.value(paramName()).value("value").toString().toInt()==0?no++:yes++;
            }
            cb->setChecked(yes>no);
        }
        else if (param.value("type").toString()=="3"){ //FILE type
            le->setDisabled(true);
            cb->setDisabled(true);
            dialog->setDisabled(true);
        }
        else if (param.value("type").toString()=="4"){ //PLAYMOD type
            int total = 0;
            int count = 0;

            foreach (Shot* shot, shots){

                total += shot->templateParams.value(paramName()).value("value").toString().toInt();
                count++;
            }

            combb->setCurrentIndex(total/count);

        }
        else if (param.value("type").toString()=="5"){ //QString type
            le->setDisabled(true);
        }

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

    connect (sd, SIGNAL(valueChanged(double)), this, SLOT(updateFloatControllersFromSd()));
    connect (cs, SIGNAL(valueChanged(int)), this, SLOT(updateFloatControllersFromCs()));
    connect (sd, SIGNAL(editingFinished()), this, SLOT(setValue()));
    connect (cs, SIGNAL(sliderReleased()), this, SLOT(setValue()));
    connect (cs, SIGNAL(clicked()), this, SLOT(setValue()));
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
    connect(bt, SIGNAL(clicked(bool)), this, SLOT(evaluateDialogSelection()));
    connect(cb, SIGNAL(clicked()), this, SLOT(evaluateFileCheckBox()));
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
    auto json = shots[0]->templateParams.value(paramName());
    return json.value("value").toString();
}

void CustomShotParameterInterface::setParamValueInShot(QString value)
{
    // sending a new configuration to the paraminterface, relaying it to the main to handle the change by Qundo

    QJsonObject newparam = shots[0]->templateParams.value(paramName());
    if (newparam.value("value").toString() != value){
        newparam.insert("value", value);
        emit (valueChangeRequest(newparam));
    }

}

QString CustomShotParameterInterface::paramName()
{
    return param.value("name").toString();
}

void CustomShotParameterInterface::evaluateGlowDisplay()
{
    foreach(Shot* current, shots){
        if (param.value("name").toString()=="Glow Intensity")
        {

            QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
            effect->setYOffset((getParamValueFromShot().toFloat()/param.value("stepvalue").toString().toDouble()/10)*2);
        }
        else if(param.value("name").toString()=="Glow Power")
        {

            QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
            effect->setBlurRadius(10 - getParamValueFromShot().toFloat()/param.value("stepvalue").toString().toDouble()/10);
        }
    }
}
void CustomShotParameterInterface::evaluateDialogSelection()
{
    dialog->exec();

    if (dialog->selectedFiles().length()!=0){
        setParamValueInShot(dialog->selectedFiles()[0]);
        le->setText(dialog->selectedFiles()[0]);
        cb->setChecked(true);
    }
}

void CustomShotParameterInterface::evaluateFileCheckBox()
{

    if (getParamValueFromShot() == ""){
        if (!cb->isChecked()){
            cb->setChecked(false);
        }
        else
        {
            cb->setChecked(false);
            evaluateDialogSelection();
        }
    }
    else
    {
        if (!cb->isChecked()){
            setParamValueInShot("");
            le->setText("");
            cb->setChecked(false);
        }
    }
}

void CustomShotParameterInterface::setValue()
{

    if (param.value("type").toString()=="0"){ //INT type
        setParamValueInShot(QString::fromStdString(std::to_string(sb->value())));
    }
    else if (param.value("type").toString()=="1"){ //FLOAT type
        updateFloatControllersFromCs();
        qDebug()<<QString::fromStdString(std::to_string(sd->value()))<<"in setvalue float";
        setParamValueInShot(QString::fromStdString(std::to_string(sd->value())));
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
    foreach(Shot* current, shots){
    if (param.value("name").toString()=="Glow Intensity")
    {

        QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
        effect->setYOffset(sd->value()*2);
    }
    else if(param.value("name").toString()=="Glow Power")
    {

        QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
        effect->setBlurRadius(10 - sd->value());
    }
    }
}

void CustomShotParameterInterface::updateFloatControllersFromSd()
{
    cs->setValue(sd->value()/param.value("stepvalue").toString().toDouble());
    foreach(Shot* current, shots){
    if (param.value("name").toString()=="Glow Intensity")
    {

        QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
        effect->setYOffset(sd->value()*2);
    }
    else if(param.value("name").toString()=="Glow Power")
    {

        QGraphicsDropShadowEffect* effect = dynamic_cast<QGraphicsDropShadowEffect*>(current->graphicsEffect());
        effect->setBlurRadius(10 - sd->value());
    }
    }
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


