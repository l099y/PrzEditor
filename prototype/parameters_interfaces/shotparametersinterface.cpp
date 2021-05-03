#include "shotparametersinterface.h"
#include <QVBoxLayout>
#include "customshotparameterinterface.h"
#include <QSizePolicy>

ShotParametersInterface::ShotParametersInterface(QJsonObject config, QWidget *parent) : QWidget(parent)
{
  auto llayout = new QVBoxLayout();
  setLayout(llayout);
  QSizePolicy a;
  a.setHorizontalPolicy(QSizePolicy::Expanding);
  this->setSizePolicy(a);



  foreach (QJsonValue val, config.value("shot").toArray()){
      CustomShotParameterInterface* param = new CustomShotParameterInterface(val.toObject());
      layout()->addWidget(param);
      layout()->setAlignment(param, Qt::AlignLeft);
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
