#include "shotparametersinterface.h"
#include <QVBoxLayout>
#include "customshotparameterinterface.h"
#include <QSizePolicy>
#include <QGraphicsScene>
#include <sequence_elements/timelinescene.h>
#include <QJsonObject>

ShotParametersInterface::ShotParametersInterface(QJsonObject config, QWidget *parent) : QWidget(parent)
{
    auto llayout = new QVBoxLayout();
    setLayout(llayout);
    QSizePolicy a;
    a.setHorizontalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(a);

    layout()->addWidget(title);
    layout()->addWidget(path);
    title->setVisible(false);
    path->setVisible(false);

    QWidget* widthWidget = new QWidget(this);
    QWidget* posWidget = new QWidget(this);


    widthWidget->setLayout(new QHBoxLayout(this));
    posWidget->setLayout(new QHBoxLayout(this));

    widthWidget->layout()->setAlignment(Qt::AlignLeft);
    posWidget->layout()->setAlignment(Qt::AlignLeft);

    QLabel* wlab = new QLabel("frame length");
    wlab->setMinimumWidth(300);
    widthInput->setMinimumWidth(150);
    widthWidget->layout()->addWidget(wlab);
    widthWidget->layout()->addWidget(widthInput);

    QLabel* flab = new QLabel("Frame Position");
    flab->setMinimumWidth(300);
    positionInput->setMinimumWidth(150);
    posWidget->layout()->addWidget(flab);
    posWidget->layout()->addWidget(positionInput);

    layout()->addWidget(widthWidget);
    layout()->addWidget(posWidget);

    foreach (QJsonValue val, config.value("shot").toArray()){
        CustomShotParameterInterface* param = new CustomShotParameterInterface(val.toObject());
        connect (param, SIGNAL(valueChangeRequest(QJsonObject)), this, SLOT(RequestValueChanged(QJsonObject)));
        layout()->addWidget(param);
        layout()->setAlignment(param, Qt::AlignLeft);
        parameters.append(param);
    }
    connect (positionInput, SIGNAL(editingFinished()), this, SLOT(changedShotPosition()));
    connect (widthInput, SIGNAL(editingFinished()), this, SLOT(changedShotSize()));
}

void ShotParametersInterface::setShot(QList<Shot *>shot)
{
    qDebug()<<"request setShot in main panel";

    this->shots=shot;
    title->setVisible(true);
    QFont font = title->font();
    font.setPointSize(17);
    font.setBold(true);
    title->setFont(font);
    title->setMargin(10);
    path->setVisible(true);
    path->setReadOnly(true);
    path->setMaximumWidth(300);
    path->setMinimumHeight(25);

    if (shots.length()==1){
        title->setText(shot[0]->seqs[0]->name);
        path->setText(shot[0]->seqs[0]->path);
        if (shots[0]->seqs[0]->corrupted){
            path->setText("corrupted");
        }
    }
    else{
        title->setText("Multiple selection");
        path->setText("-");
    }



    foreach (CustomShotParameterInterface* current, parameters){
        current->setShot(shots);
    }

    if (shots.length()==1){
    positionInput->setMaximum((shot[0]->scene()->sceneRect().width()/10)-(shot[0]->rect().width()/10));
    positionInput->setValue(shot[0]->previousxpos/10);
    positionInput->setMinimum(0);

    widthInput->setMaximum(shot[0]->smallestSequence());
    widthInput->setValue(shot[0]->previousboxwidth/10);
    widthInput->setMinimum(0);
    }
    else{
        positionInput->setEnabled(false);
        widthInput->setEnabled(false);
    }


}

void ShotParametersInterface::updateShotPos()
{
    if (shots.length()==1)
    positionInput->setValue(shots[0]->scenePos().x()/10);
}

void ShotParametersInterface::changedShotSize()
{
    if (shots.length()==1){
    if (shots[0]->rect().width()!=widthInput->value()*10 && shots[0]->validateSizeChange(widthInput->value())){
        emit (changeShotSize(widthInput->value(), "shot"));
    }
    else{
        emit (displayError("Invalid size, this shot contains shorter sequences", 3000));
    }
    }
}

void ShotParametersInterface::changedShotPosition()
{
    if (shots.length()==1){
    qDebug()<<shots[0]->scenePos().x() <<"- shotpos" << positionInput->value()*10 << "input pos in interface updateshotpos";
    if (shots[0]->scenePos().x()!=positionInput->value()*10)
        emit (changeShotPosition(positionInput->value()*10, "shot"));
    }
}

void ShotParametersInterface::RequestValueChanged(QJsonObject newparam)
{
    emit (valueChangedRequest(shots, newparam));
}
