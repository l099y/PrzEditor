#ifndef EXTENDEDBUTTON_H
#define EXTENDEDBUTTON_H

#include <QPushButton>
#include <QObject>

class ExtendedButton : public QPushButton
{
    Q_OBJECT
public:

    ExtendedButton(QWidget *parent = nullptr);
    ~ExtendedButton();
signals:

};

#endif // EXTENDEDBUTTON_H
