#include "customshotparameterinterface.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

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
    QLineEdit* le = new QLineEdit(this);
    le->setMinimumWidth(200);
    le->setMinimumHeight(25);
    layout->addWidget(namelab);
    layout->addWidget(le);
}
