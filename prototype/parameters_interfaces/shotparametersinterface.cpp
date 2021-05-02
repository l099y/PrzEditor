#include "shotparametersinterface.h"
#include <QVBoxLayout>
#include "customshotparameterinterface.h"

ShotParametersInterface::ShotParametersInterface(QJsonObject config, QWidget *parent) : QWidget(parent)
{
  auto llayout = new QVBoxLayout();
  llayout->setAlignment(Qt::AlignTop);
  setLayout(llayout);



  foreach (QJsonValue val, config.value("shot").toArray()){
      layout()->addWidget(new CustomShotParameterInterface(val.toObject()));
  }
}
