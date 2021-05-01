#include "shotparametersinterface.h"
#include <QHBoxLayout>

ShotParametersInterface::ShotParametersInterface(QJsonObject config, QWidget *parent) : QWidget(parent)
{
   auto llayout = new QHBoxLayout();
  llayout->setAlignment(Qt::AlignTop);
  setLayout(llayout);
  displaceSelectionButton->setText("displace selection");
  changeSelectionSizeButton->setText("Change selection Size");
  framePositionInput->setStyleSheet("background: rgb(220,220,220);");
  boxSizeInput->setStyleSheet("background: rgb(220,220,220);");
  llayout->addWidget(displaceSelectionButton);
  llayout->addWidget(framePositionInput);
  llayout->addWidget(changeSelectionSizeButton);
  llayout->addWidget(boxSizeInput);

  foreach (QJsonValue val, config.value("shot").toArray()){
      qDebug()<<val.toObject().value("name");
  }
}
