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
#include <filesystem/projectloader.h>

// file Explorer and Sequences displayer libs

#include <QTreeView>
#include <filesystem/extendedqfilesystemmodel.h>
#include <sequence_elements/Storage/sequenceroom.h>
#include <QStandardItem>
#include <QStandardItemModel>

// Sequences elements libs

#include <sequence_elements/timelinescene.h>
#include <QGraphicsView>

// the undo framework

#include <QUndoStack>
#include <QUndoView>


// the toolbar libs

#include <QAction>
#include <QToolBar>
#include <QMenu>

//
#include <parameters_interfaces/shotparametersinterface.h>
#include <sequence_elements/soundtrack.h>
#include <filesystem/tbesounddata.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    float currentTimelineScaling = 1;
    int framerate = 24;
    QString savepath = "";
    bool isSaved = true;

    //flag to disable the parsing of corrupted Sequences on losing focus used on triggering an internal dialog

    bool ValidationOnFocusEnabled = true;

    // containers and layout

    QWidget *widget = new QWidget;
    QWidget *timelineNutils = new QWidget(widget);
    QWidget *parameters = new QWidget(widget);
    QWidget *timelineButtons = new QWidget(parameters);
    QWidget *supposedtimeslider = new QWidget(parameters);
    QWidget *params1 = new QWidget(parameters);
    QLabel *paramlabel = new QLabel("parameters");
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *sublayoutEditor = new QVBoxLayout(widget);
    QHBoxLayout *sublayoutbutton = new QHBoxLayout(widget);
    QVBoxLayout *sublayouttimeline = new QVBoxLayout(parameters);
    QVBoxLayout *sublayoutsplit0 = new QVBoxLayout(parameters);
    QHBoxLayout *sublayoutsplit = new QHBoxLayout(parameters);
    QVBoxLayout *sublayoutparams1 = new QVBoxLayout(params1);


    //load the shot template and setup the parameters ui comps

    void initShotsParameters();

        //utilies for this usecase
        inline bool isValidJsonObject(QJsonDocument);


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
    QPushButton* alignbutton = new QPushButton();
    QPushButton* clearbutton = new QPushButton();
    QPushButton* newboxbutton = new QPushButton();
    QPushButton* mod2button = new QPushButton();
    QPushButton* dispbutton = new QPushButton();
    QPushButton* delbutton = new QPushButton();

    // toolbars elements

    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QAction *undoAction = nullptr;
    QAction *redoAction = nullptr;
    QAction *exitAction = nullptr;
    QAction *saveAction = nullptr;
    QAction *saveAsAction = nullptr;
    QAction *loadAction = nullptr;
    QAction *exportAction = nullptr;
    QAction *newAction = nullptr;


    // Undo framework elements

    QUndoStack *undoStack = nullptr;
    QUndoView *undoView = nullptr;
    void resetUndoStack();

    // load/save Dialog

    ProjectLoader* saveDialog;

    // Shot Parameter interface

    ShotParametersInterface* shotparams = nullptr;
    QScrollArea *scrollArea = new QScrollArea(params1);

    //init functions

    void createUndoView();
    void bindUndoElements();

    void changeEvent(QEvent *event) override;
    void loop();

    // dataseed should be in the directory of the project

    void generateData();

    // this function should be a property

    inline void enableParameterInterface(bool);

    // this function is used filter the pointed files in the export format, allowing the player to load only the required prz frames

    QJsonArray formatUtilRange(QJsonArray files, QJsonArray sequences);

    // used in the save JSON format

    QJsonObject toJSON();

public slots:

    // resizing the visible area of the timeline

    void scaleUpView();
    void scaleDownView();
    void scaleViewToScene();

    void validateSequenceIntegrity();

    // used to implement the use case of exploring the treefile to display found .prz sequences

    void displaySequences(QString);
    void clearSequences();
    void clearSequencesAndCollapse(QModelIndex);
    void collapseChildrens(QModelIndex);
    void collapseAllAndExpand(QModelIndex);
    void insertComponentAtEndOfTimeline(QModelIndex);


    // QUndo functions allowing the stack of action to operate

    void deleteSelection();
    void createdShot(QList<SequenceData*> seq, int xpos, int length, TimelineScene* timeline , QVector<Shot*> movedShots);
    void createdSound(SoundTrack*, TbeSoundData*, int, int, TimelineScene*, QVector<SoundTrack*>);
    void movedSoundtracks(TimelineScene*, QVector<SoundTrack*>, int, int);
    void movedShots(TimelineScene*, QVector<Shot*> movedShots, int prevscenewidth, int currentscenewidth);
    void clearedTimeline(TimelineScene*, QVector<Shot*>, int);
    void resizedShot(TimelineScene*, QVector<Shot*>,Shot*, int, int);
    void resizedSound(TimelineScene*, QVector<SoundTrack*>, SoundTrack* ,int, int, int);
    void changeParameterInAShot(QList<Shot*>, QJsonObject);
    void changeFrameIn(Shot*, int);

    // Saving and Loading projet related slots

    void saveActionTriggered();
    void saveAsTriggered();
    void loadActionTriggered();
    void exportTriggered();
    void newTriggered();

    // those function are triggered by the modalDialog if an action is required

    void saveRequestExecuted(QString);
    void loadRequestExecuted(QString);
    void exportRequestExecuted(QString);
    void newRequestExecuted();

    // those functions are used in the exportRequest.

    QJsonObject generateBlankPrz();
    QJsonObject generateEmptyScene(int size);


    // attempt to change the target of the parameter interface

    void changeSelectedShotInParametersInterface();

    // error handling .. basic

    void displayStatusBarMessage(QString, int);

private:
    void createActions();
    void createMenus();
    void initButtons();
    void inittimelinescene();
    void initLayouts();
    void bindLayoutsToWidgets();
    void setupTreeItem();
    void initlabs();
    void initwidgetsparams();
    void initcontenance();
    Ui::MainWindow *ui;

protected:
    void resizeEvent(QResizeEvent *event) override ;
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
