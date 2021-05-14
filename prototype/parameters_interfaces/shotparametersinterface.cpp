#include "shotparametersinterface.h"
#include <QVBoxLayout>
#include "customshotparameterinterface.h"
#include <QSizePolicy>
#include <QGraphicsScene>

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

void ShotParametersInterface::setShot(Shot *shot)
{
    this->shot=shot;
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
    title->setText(shot->seqs[0]->sequencefilename);
    path->setText(shot->seqs[0]->path);

    foreach (CustomShotParameterInterface* current, parameters){
        current->setShot(shot);
    }

    positionInput->setMaximum(shot->scene()->sceneRect().width()/10);
    positionInput->setValue(shot->previousxpos/10);
    positionInput->setMinimum(0);

    widthInput->setMaximum(shot->smallestSequence());
    widthInput->setValue(shot->previousboxwidth/10);
    widthInput->setMinimum(0);


}

void ShotParametersInterface::updateShotPos()
{
    positionInput->setValue(shot->scenePos().x()/10);
}

void ShotParametersInterface::changedShotSize()
{
    if (shot->rect().width()!=widthInput->value()*10)
    emit (changeShotSize(widthInput->value(), "shot"));
}

void ShotParametersInterface::changedShotPosition()
{
    qDebug()<<shot->scenePos().x() <<"- shotpos" << positionInput->value()*10 << "input pos in interface updateshotpos";
    if (shot->scenePos().x()!=positionInput->value()*10)
    emit (changeShotPosition(positionInput->value()*10, "shot"));
}

void ShotParametersInterface::RequestValueChanged(QJsonObject newparam)
{
    emit (valueChangedRequest(shot, newparam));
    qDebug()<<"request paramchange";
}
