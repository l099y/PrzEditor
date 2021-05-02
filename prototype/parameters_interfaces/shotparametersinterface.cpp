#include "shotparametersinterface.h"
#include <QVBoxLayout>
#include "customshotparameterinterface.h"

ShotParametersInterface::ShotParametersInterface(QJsonObject config, QWidget *parent) : QWidget(parent)
{
  auto llayout = new QVBoxLayout();
  llayout->setAlignment(Qt::AlignTop);
  setLayout(llayout);



  foreach (QJsonValue val, config.value("shot").toArray()){
      CustomShotParameterInterface* param = new CustomShotParameterInterface(val.toObject());
      layout()->addWidget(param);
      parameters.append(param);
  }
}

void ShotParametersInterface::setShot(Shot *shot)
{
    this->shot=shot;
    foreach (CustomShotParameterInterface* current, parameters){
        current->setShot(shot);
    }
}
