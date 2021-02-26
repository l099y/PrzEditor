#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QLabel>
#include <QGraphicsScene>
#include <extendedbutton.h>
#include <QGraphicsScene>
#include <timelinescene.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWidget *widget = new QWidget;
    QWidget *rightFiller = new QWidget(widget);

    QWidget *parameters = new QWidget(widget);
    QWidget *supposedtimeslider = new QWidget(parameters);
    QWidget *params1 = new QWidget(parameters);
    QWidget *params2 = new QWidget(parameters);

    TimelineScene *timeline = new TimelineScene(this);
    QGraphicsView *timelineView;
    QGraphicsView *timelineFitIn;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *sublayoutEditor = new QVBoxLayout(widget);
    QVBoxLayout *sublayouttimeline = new QVBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit = new QHBoxLayout(parameters);
    QHBoxLayout *sublayoutparams1 = new QHBoxLayout(params1);
    QHBoxLayout *sublayoutparams2 = new QHBoxLayout(params2);

    QFileSystemModel *TreeModel = new QFileSystemModel();
    QModelIndex idx = TreeModel->index("");
    QTreeView *tree = new QTreeView(widget);

    QLabel *params1lab = new QLabel(params1);
    QLabel *params2lab = new QLabel(params2);
    ExtendedButton *button = new ExtendedButton(params2);

    MainWindow(QWidget *parent = nullptr);


    ~MainWindow();
public slots:
    void changeButtonTxt();

private:
    void inittimelinescene();
    void initLayouts();
    void bindLayoutsToWidgets();
    void setupTreeItem();
    void initlabs();
    void initwidgetsparams();
    void initcontenance();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
