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
    title->setVisible(false);
    path->setVisible(false);

    QWidget* widthWidget = new QWidget(this);
    QWidget* posWidget = new QWidget(this);
    QWidget* frameInWidget = new QWidget(this);
    QWidget* pathWidget = new QWidget(this);

    widthWidget->setLayout(new QHBoxLayout(this));
    posWidget->setLayout(new QHBoxLayout(this));
    frameInWidget->setLayout(new QHBoxLayout(this));
    pathWidget->setLayout(new QHBoxLayout(this));

    widthWidget->layout()->setAlignment(Qt::AlignLeft);
    posWidget->layout()->setAlignment(Qt::AlignLeft);
    frameInWidget->layout()->setAlignment(Qt::AlignLeft);
    pathWidget->layout()->setAlignment(Qt::AlignLeft);

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

    QLabel* fflab = new QLabel("Starting frame of the sequence in the shot");
    fflab->setMinimumWidth(300);
    frameInInput->setMinimumWidth(150);
    frameInWidget->layout()->addWidget(fflab);
    frameInWidget->layout()->addWidget(frameInInput);

    pathWidget->layout()->addWidget(path);
    pathWidget->layout()->addWidget(validateSeq);

    validateSeq->setText("Change source");
    validateSeq->setMaximumWidth(200);

    layout()->addWidget(pathWidget);
    layout()->addWidget(widthWidget);
    layout()->addWidget(posWidget);
    layout()->addWidget(frameInWidget);

    foreach (QJsonValue val, config.value("shot").toArray()){
        CustomShotParameterInterface* param = new CustomShotParameterInterface(val.toObject());
        connect (param, SIGNAL(valueChangeRequest(QJsonObject)), this, SLOT(RequestValueChanged(QJsonObject)));
        layout()->addWidget(param);
        layout()->setAlignment(param, Qt::AlignLeft);
        parameters.append(param);
    }
    connect (positionInput, SIGNAL(editingFinished()), this, SLOT(changedShotPosition()));
    connect (widthInput, SIGNAL(editingFinished()), this, SLOT(changedShotSize()));
    connect (frameInInput, SIGNAL(editingFinished()), this, SLOT(changedFrameInValue()));
    connect (validateSeq, SIGNAL(clicked(bool)), this, SLOT(changeSeqPathAndParse()));
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
            QPalette *palette = new QPalette();
            palette->setColor(QPalette::Text,Qt::red);
            path->setPalette(*palette);
            path->setText("corrupted");
            validateSeq->setVisible(true);
        }
        else{
            QPalette *palette = new QPalette();
            palette->setColor(QPalette::Text,Qt::black);
            path->setPalette(*palette);
            validateSeq->setVisible(false);
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

        int smallestSequenceSize=  shot[0]->smallestSequence();
        positionInput->setMaximum((shot[0]->scene()->sceneRect().width()/10)-(shot[0]->rect().width()/10));
        positionInput->setValue(shot[0]->previousxpos/10);
        positionInput->setMinimum(0);

        widthInput->setMaximum(smallestSequenceSize);
        widthInput->setValue(shot[0]->previousboxwidth/10);
        widthInput->setMinimum(0);

        frameInInput->setMaximum(smallestSequenceSize-(shots[0]->rect().width()/10));
        frameInInput->setValue(shots[0]->frameIn > (smallestSequenceSize -shots[0]->rect().width()/10)? 0 : shots[0]->frameIn);

        positionInput->setEnabled(true);
        widthInput->setEnabled(true);
        frameInInput->setEnabled(true);
    }
    else{
        positionInput->setEnabled(false);
        widthInput->setEnabled(false);
        frameInInput->setEnabled(false);
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
        if (shots[0]->scenePos().x()!=positionInput->value()*10)
            emit (changeShotPosition(positionInput->value()*10, "shot"));
    }
}

void ShotParametersInterface::changedFrameInValue()
{
     if (shots.length()==1)
     emit (changeFrameIn(shots[0], frameInInput->value()));
}

void ShotParametersInterface::RequestValueChanged(QJsonObject newparam)
{
    emit (valueChangedRequest(shots, newparam));
}

void ShotParametersInterface::changeSeqPathAndParse()
{
    auto dialog  = new QFileDialog(this);
    QStringList filter;
    filter<< "*.mp4"<<"*.avi"<<"*.mkv";
    dialog->setNameFilters(filter);
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setWindowTitle("select directory containing proper sequence");
    dialog->exec();
    if (dialog->result()){
            auto savepath = dialog->selectedFiles().at(0);
            auto temp = shots[0]->seqs[0]->path;
            shots[0]->seqs[0]->path = savepath;
            if (!shots[0]->seqs[0]->checkIntegrity()){
                shots[0]->seqs[0]->path = temp;
                shots[0]->seqs[0]->checkIntegrity();
                emit (displayError("the new path specified doesn't validate the sequence integrity", 3000));
            }
            else{
                shots[0]->setSelected(false);
                shots[0]->setSelected(true);
                shots[0]->update();
            }

    }
}
