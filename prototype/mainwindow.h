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
#include <QLineEdit>
#include <extendedqtreeview.h>
#include <QSortFilterProxyModel>

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
    QLabel *paramlabel = new QLabel("parameters");

    TimelineScene *timeline = new TimelineScene(this);
    QGraphicsView *timelineView;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *sublayoutEditor = new QVBoxLayout(widget);
    QVBoxLayout *sublayoutbutton = new QVBoxLayout(widget);
    QVBoxLayout *sublayouttimeline = new QVBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit0 = new QHBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit = new QHBoxLayout(parameters);
    QVBoxLayout *sublayoutparams1 = new QVBoxLayout(params1);
    QHBoxLayout *sublayoutparams2 = new QHBoxLayout(params2);

    QFileSystemModel *TreeModel = new QFileSystemModel(this);
    QModelIndex idx = TreeModel->index("");
    ExtendedQTreeView *tree = new ExtendedQTreeView(widget);
    QSortFilterProxyModel *fileFilter = new QSortFilterProxyModel(this);

    QPushButton* allignbutton = new QPushButton();
    QPushButton* clearbutton = new QPushButton();
    QPushButton* newboxbutton = new QPushButton();
    QPushButton* mod2button = new QPushButton();
    QPushButton* dispbutton = new QPushButton();
    QPushButton* delbutton = new QPushButton();
    QPushButton* displaceSelectionButton = new QPushButton();
    QPushButton* changeSelectionSizeButton = new QPushButton();
    QLineEdit* framePositionInput = new QLineEdit(this);
    QLineEdit* boxSizeInput = new QLineEdit(this);

    MainWindow(QWidget *parent = nullptr);



    ~MainWindow();
public slots:
    void changeButtonTxt();
    void scaleUpView();
    void scaleDownView();
    void displaceSelectionInTimeline();
    void changeSelectionSizeInTimeline();

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
