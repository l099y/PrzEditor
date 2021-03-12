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
#include <QPushButton>
#include <QGraphicsScene>
#include <timelinescene.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    float currentTimelineScaling = 1;

    QWidget *widget = new QWidget;
    QWidget *timelineNutils = new QWidget(widget);
    QWidget *parameters = new QWidget(widget);
    QWidget *timelineButtons = new QWidget(parameters);
    QWidget *supposedtimeslider = new QWidget(parameters);
    QWidget *params1 = new QWidget(parameters);
    QWidget *params2 = new QWidget(parameters);

    TimelineScene *timeline = new TimelineScene(this);
    QGraphicsView *timelineView;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *sublayoutEditor = new QVBoxLayout(widget);
    QVBoxLayout *sublayoutbutton = new QVBoxLayout(widget);
    QVBoxLayout *sublayouttimeline = new QVBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit0 = new QHBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit = new QHBoxLayout(parameters);
    QHBoxLayout *sublayoutparams1 = new QHBoxLayout(params1);
    QHBoxLayout *sublayoutparams2 = new QHBoxLayout(params2);

    QFileSystemModel *TreeModel = new QFileSystemModel();
    QModelIndex idx = TreeModel->index("");
    QTreeView *tree = new QTreeView(widget);

    QPushButton* allignbutton = new QPushButton();
    QPushButton* clearbutton = new QPushButton();
    QPushButton* newboxbutton = new QPushButton();
    QPushButton* mod2button = new QPushButton();
    QPushButton* dispbutton = new QPushButton();
    QPushButton* delbutton = new QPushButton();

    MainWindow(QWidget *parent = nullptr);



    ~MainWindow();
public slots:
    void changeButtonTxt();
    void scaleUpView();
    void scaleDownView();

private:
    void initButtons();
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
