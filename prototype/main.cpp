#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ressources);
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    w.setStyleSheet("QMainWindow {background: rgb(40,40,40) ;}");

    return a.exec();
}
