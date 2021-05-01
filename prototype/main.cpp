#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ressources);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setStyleSheet("QMainWindow {background: rgb(80,80,80) ;}");

    return a.exec();
}
