#ifndef CUSTOMQSLIDER_H
#define CUSTOMQSLIDER_H

#include <QSlider>
#include <QObject>

class CustomQSlider : public QSlider
{
    Q_OBJECT
public:
    CustomQSlider(QWidget* parent);

protected:
    virtual bool event(QEvent*) override;
    virtual void mousePressEvent(QMouseEvent *ev) override;
signals:
    void clicked();
};


#endif // CUSTOMQSLIDER_H
