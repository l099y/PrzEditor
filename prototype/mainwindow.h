#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// regular app items

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

// file Explorer and Sequences displayer items

#include <QTreeView>
#include <filesystem/extendedqfilesystemmodel.h>
#include <sequence_elements/Storage/sequenceroom.h>
#include <QStandardItem>
#include <QStandardItemModel>

// Sequences elements

#include <sequence_elements/timelinescene.h>
#include <QGraphicsView>

// the undo framework

#include <QUndoStack>
#include <QUndoView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    float currentTimelineScaling = 1;

    // containers and layout


    QWidget *widget = new QWidget;
    QWidget *timelineNutils = new QWidget(widget);
    QWidget *parameters = new QWidget(widget);
    QWidget *timelineButtons = new QWidget(parameters);
    QWidget *supposedtimeslider = new QWidget(parameters);
    QWidget *params1 = new QWidget(parameters);
    QWidget *params2 = new QWidget(parameters);
    QLabel *paramlabel = new QLabel("parameters");
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *sublayoutEditor = new QVBoxLayout(widget);
    QVBoxLayout *sublayoutbutton = new QVBoxLayout(widget);
    QVBoxLayout *sublayouttimeline = new QVBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit0 = new QHBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit = new QHBoxLayout(parameters);
    QVBoxLayout *sublayoutparams1 = new QVBoxLayout(params1);
    QHBoxLayout *sublayoutparams2 = new QHBoxLayout(params2);


    // extended File Explorer

    SequenceRegister* reg = new SequenceRegister (this);
    ExtendedQFileSystemModel *TreeModel = new ExtendedQFileSystemModel(reg, this);
    QModelIndex idx = TreeModel->index("");
    QTreeView *tree = new QTreeView(widget);
    QStandardItemModel* sequencesModel = new QStandardItemModel(this);
    QTreeView* sequencesStorageView = new QTreeView(this);


    // Sequence Representation

    QGraphicsView *timelineView = new QGraphicsView(this);
    TimelineScene *timeline = new TimelineScene(reg, timelineView, this);
    QPushButton* allignbutton = new QPushButton();
    QPushButton* clearbutton = new QPushButton();
    QPushButton* newboxbutton = new QPushButton();
    QPushButton* mod2button = new QPushButton();
    QPushButton* dispbutton = new QPushButton();
    QPushButton* delbutton = new QPushButton();

    // Parameters window elements

    QPushButton* displaceSelectionButton = new QPushButton();
    QPushButton* changeSelectionSizeButton = new QPushButton();
    QLineEdit* framePositionInput = new QLineEdit(this);
    QLineEdit* boxSizeInput = new QLineEdit(this);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeEvent(QEvent *event);
    void generateData();
public slots:
    void changeButtonTxt();
    void scaleUpView();
    void scaleDownView();
    void displaceSelectionInTimeline();
    void changeSelectionSizeInTimeline();
    void displaySequences(QString);
    void clearSequences();
    void clearSequencesAndCollapse(QModelIndex);
    void collapseChildrens(QModelIndex);
    void collapseAllAndExpand(QModelIndex);

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
