#ifndef SHOTPARAMETERSINTERFACE_H
#define SHOTPARAMETERSINTERFACE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <sequence_elements/shot.h>
class ShotParametersInterface : public QWidget
{
    Q_OBJECT
public:
    explicit ShotParametersInterface(QJsonObject config, QWidget *parent = nullptr);

    // Parameters window elements
    Shot* shot = nullptr;

signals:

};

#endif // SHOTPARAMETERSINTERFACE_H
