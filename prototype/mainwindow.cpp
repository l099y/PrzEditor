#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsView>
#include <QPixmap>
#include <QHeaderView>
#include "undo_framework/commands.h"
#include <sequence_elements/timelinescene.h>
#include <QJsonObject>
#include <QResource>
#include <filesystem/projectloader.h>
#include <filesystem/tbesounddata.h>
#include <sequence_elements/soundtrack.h>
#include <QScrollBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    this->move(0,0);
    ui->setupUi(this);
    setCentralWidget(widget);
    undoStack = new QUndoStack(this);
    initLayouts();
    setupTreeItem();
    inittimelinescene();
    initShotsParameters();
    initButtons();
    createActions();
    createMenus();
    createUndoView();
    bindUndoElements();
    this->setWindowTitle("Presenz movie editor");



    this->statusBar()->setFixedHeight(30);
    this->statusBar()->setStyleSheet("QStatusBar{padding-left:8px;background:rgba(55,55,55,255);color:red;font-weight:bold; font-size:20px}");
    connect(timeline, SIGNAL(selectionChanged()), this, SLOT(changeSelectedShotInParametersInterface()));
    showMaximized();

    scaleViewToScene();

    // sadly
//    QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::CTRL, Qt::NoModifier);
//    QCoreApplication::postEvent (this, event);
}
void MainWindow :: initButtons(){

    QPixmap b(":/images/align.png");
    QIcon bb(b);
    alignbutton->setIcon(bb);
    alignbutton->setIconSize(QSize(30,30));
    alignbutton->setMaximumSize(QSize(40,40));
    alignbutton->setToolTip("align shots");
    connect(alignbutton, SIGNAL(clicked(bool)), timeline, SLOT(debugItems()));

    QPixmap a(":/images/sweeping.png");
    QIcon aa(a);
    clearbutton->setIcon(aa);
    clearbutton->setIconSize(QSize(30,30));
    clearbutton->setMaximumSize(QSize(40,40));
    clearbutton->setToolTip("clear the timeline");
    connect(clearbutton, SIGNAL(clicked(bool)), timeline, SLOT(clearItems()));

    QPixmap c(":/images/displace.png");
    QIcon cc(c);
    dispbutton->setIcon(cc);
    dispbutton->setIconSize(QSize(30,30));
    dispbutton->setMaximumSize(QSize(40,40));
    dispbutton->setToolTip("next selection movement will displace other shots");
    connect(dispbutton, SIGNAL(clicked(bool)), timeline, SLOT(setdisp()));

    QPixmap pixmap(":/images/delete.png");
    QIcon ButtonIcon(pixmap);
    delbutton->setIcon(ButtonIcon);
    delbutton->setIconSize(QSize(30,30));
    delbutton->setMaximumSize(QSize(40,40));
    delbutton->setToolTip("delete selection");
    connect(delbutton, SIGNAL(clicked(bool)), timeline, SLOT(deleteSelection()));


}
void MainWindow ::initLayouts(){

    initwidgetsparams();
    initcontenance();
    bindLayoutsToWidgets();

}
void MainWindow::initcontenance(){


    sublayoutsplit0->addWidget(supposedtimeslider);
    sublayoutsplit0->addWidget(timelineButtons);
    sublayoutEditor->addWidget(timelineNutils);
    sublayoutEditor->addWidget(parameters);
    sublayoutsplit->addWidget(tree);
    sublayoutsplit->addWidget(sequencesStorageView);
    sublayoutsplit->addWidget(params1);

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable( true );


    sublayoutsplit->setMargin(10);
    sublayoutbutton->setAlignment(Qt::AlignRight);
    sublayoutbutton->addWidget(clearbutton);
    sublayoutbutton->addWidget(alignbutton);
    sublayoutbutton->addWidget(dispbutton);
    sublayoutbutton->addWidget(delbutton);
    timelineButtons->setMaximumWidth(300);
    sublayoutbutton->setAlignment(Qt::AlignRight);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    scrollArea->setMinimumHeight(params1->height());
    scrollArea->setMaximumHeight(params1->height());
    scrollArea->setMinimumWidth(params1->width());
    scrollArea->setMaximumWidth(params1->width());
    qDebug()<<"resized";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"closed";
    if (!isSaved)
    {
        if (ProjectLoader::confirm("do you want to save modified document","unsaved modifications"))
            saveActionTriggered();
    }
}
void MainWindow::initwidgetsparams(){

    widget->setMinimumWidth(1600);
    sublayoutsplit->setMargin(0);
    sublayoutsplit0->setMargin(0);
    sublayoutEditor->setMargin(0);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    params1->setAutoFillBackground(true);
    params1->setPalette(pal);
    params1->setStyleSheet("background: rgb(120,120,120);");
    paramlabel->setMaximumHeight(100);

}
void MainWindow :: bindLayoutsToWidgets(){
    timelineNutils->setLayout(sublayoutsplit0);
    timelineNutils->setMaximumHeight(600);
    supposedtimeslider->setLayout(sublayouttimeline);
    timelineButtons->setLayout(sublayoutbutton);
    parameters->setLayout(sublayoutsplit);
    params1->setLayout(sublayoutparams1);
    //widget->setLayout(layout);
}
void MainWindow::setupTreeItem(){
    sequencesStorageView->setModel(sequencesModel);
    sequencesStorageView->setMaximumWidth(450);
    sequencesStorageView->setDragEnabled(true);
    sequencesStorageView->setEditTriggers(nullptr); // this disable the ability to rename the current selected row;
    sequencesStorageView->setStyleSheet("background: rgb(120,120,120);");

    TreeModel->setRootPath("");
    TreeModel->setNameFilterDisables(false);
    TreeModel->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    tree->setRootIndex(idx);
    tree->setMaximumWidth(400);
    tree->setModel(TreeModel);
    tree->setColumnHidden(1, true);
    tree->setColumnHidden(2, true);
    tree->setColumnHidden(3, true);
    tree->setHeaderHidden(true);
    tree->setStyleSheet("background: rgb(120,120,120);");

    generateData(); //well you only need to do it once...
    connect (sequencesStorageView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(insertComponentAtEndOfTimeline(QModelIndex)));
    //connect (tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
    connect (tree, SIGNAL(clicked(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    connect (TreeModel, SIGNAL(clearSequences()), this, SLOT(clearSequences()));
    connect (TreeModel, SIGNAL(displaySequences(QString)), this, SLOT(displaySequences(QString)));
    connect (TreeModel, SIGNAL(setOnlyCurrentFolderExpanded(QModelIndex)), this, SLOT(collapseAllAndExpand(QModelIndex)));
    connect (reg, SIGNAL(displayError(QString, int)), this, SLOT(displayStatusBarMessage(QString, int)));
}

void MainWindow::inittimelinescene(){

    timelineView->setScene(timeline);

    timelineView->setDragMode(QGraphicsView::RubberBandDrag);
    timelineView->acceptDrops();
    timelineView->setOptimizationFlags(QGraphicsView::DontSavePainterState|QGraphicsView::DontAdjustForAntialiasing);
    sublayouttimeline->addWidget(timelineView);
    sublayouttimeline->setAlignment(Qt::AlignTop);
    sublayoutbutton->setAlignment(Qt::AlignTop);
    timelineView->setMinimumHeight(450);
    timelineView->setRenderHint(QPainter::Antialiasing);
    timelineView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    timelineView->horizontalScrollBar()->setBackgroundRole(QPalette::Highlight);
    timelineView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    timelineView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //connect(this, (SIGNAL(selectionChanged())), this, SLOT(realignSelectionOn260()));
    connect(timeline, SIGNAL(scaleUp()), this, SLOT(scaleUpView()));
    connect(timeline, SIGNAL(scaleDown()),this, SLOT(scaleDownView()));
    connect(timeline, SIGNAL(displayError(QString, int)), this, SLOT(displayStatusBarMessage(QString, int)));

    auto hsb = timelineView->horizontalScrollBar();

    connect(timeline, SIGNAL(scaleToViewRequest()), this, SLOT(scaleViewToScene()));
    connect(hsb, SIGNAL(valueChanged(int)), timeline, SLOT(refreshRuler(int)));
    connect(hsb, SIGNAL(sliderReleased()), timeline, SLOT(update()));

}
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::resetUndoStack()
{
    while (undoStack->canUndo())
        undoStack->undo();
    undoStack->clear();
}

void MainWindow::createActions()
{
    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    saveAction = new QAction (tr("&Save"), this);
    connect (saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTriggered()));

    loadAction = new QAction (tr("&Load"), this);
    connect (loadAction, SIGNAL(triggered(bool)), this, SLOT(loadActionTriggered()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    saveAsAction = new QAction(tr("Save&As"), this);
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAsTriggered()));

    exportAction = new QAction(tr("&Export"), this);
    connect (exportAction, SIGNAL(triggered(bool)), this, SLOT(exportTriggered()));

    newAction = new QAction(tr("&New"), this);
    connect (newAction, SIGNAL(triggered(bool)), this, SLOT(newTriggered()));

}

void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
}

void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::bindUndoElements()
{


    //TbeSoundData*, int xpos, int length, TimelineScene* timeline, QVector<SoundTrack*>

    connect (timeline, SIGNAL(deleteSelectionSignal()), this, SLOT(deleteSelection()));
    connect (timeline, SIGNAL(createShot(QList<SequenceData*>, int , int , TimelineScene*, QVector<Shot*>)), this, SLOT(createdShot(QList<SequenceData*>, int, int, TimelineScene*, QVector<Shot*>)));
    connect (timeline, SIGNAL(moveShots(TimelineScene*, QVector<Shot*>,int, int)), this, SLOT(movedShots(TimelineScene*, QVector<Shot*>, int, int)));
    connect (timeline, SIGNAL(clearTimeline(TimelineScene*, QVector<Shot*>, int)), this, SLOT(clearedTimeline(TimelineScene*, QVector<Shot*>, int)));
    connect (timeline, SIGNAL(babar(SoundTrack*, TbeSoundData*, int, int, TimelineScene*, QVector<SoundTrack*>)), this, SLOT(createdSound(SoundTrack*, TbeSoundData*,int, int, TimelineScene*,QVector<SoundTrack*>)));
    connect (timeline, SIGNAL(moveSoundtracks(TimelineScene*, QVector<SoundTrack*>, int, int)), this, SLOT(movedSoundtracks(TimelineScene*, QVector<SoundTrack*>, int, int)));//TbeSoundData*, int xpos, int length, TimelineScene* timeline,
    connect (timeline, SIGNAL(resizeShot(TimelineScene*, QVector<Shot*>, Shot* ,int, int)), this, SLOT(resizedShot(TimelineScene*, QVector<Shot*>, Shot* ,int, int)));
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange && this->isActiveWindow()) {
        qDebug()<<"retrieve the focus";
        if (ValidationOnFocusEnabled){
            validateSequenceIntegrity();
            timeline->update();

            TreeModel->parseExpandedDir(tree->currentIndex());
            if (shotparams!=nullptr && shotparams->shots.length()!=0 && timeline->selectedItems().length()!=0){
                shotparams->setShot(shotparams->shots);
            }
        }
    }
    QWidget :: changeEvent(event);
}

void MainWindow::generateData()
{
    QDir dir ("c://DataTests");
    if (!dir.exists()){
        TreeModel->mkdir(TreeModel->index("c://"), "DataTests");
        TreeModel->mkdir(TreeModel->index("c://DataTests"), "NestedDataTests");
        QString path ("c://DataTests//");
        QStringList namelist ({"Davinci.", "Eiffel.", "Podracing.", "porgy_and_bess.", "robot-24."});
        QStringList soundlist ({"sound_1.", "Eiffelsound.", "atmosphere.", "wildfd.", "robod."});

        for (int j = 0; j<5; j++){
            for (int i = 0; i<((j+1)*100); i++)
            {
                QString name (namelist[j]);
                name = name.append(i < 10 ? "00000" : i<100 ? "0000" : i<1000? "000" : i<10000? "00": "0").append("%1").arg(i).append(".prz");
                QFile file(path+name);
                file.open(QIODevice::ReadWrite);
                file.write("content.toJson()");
            }
            QFile file(path+soundlist[j].append("tbe"));
            file.open(QIODevice::ReadWrite);
        }
    }
}
void MainWindow::enableParameterInterface(bool mod)
{
    shotparams->setVisible(mod);
    shotparams->setEnabled(mod);
    scrollArea->setVisible(mod);
}


QJsonObject MainWindow::toJSON()
{
    QJsonObject ret;
    ret.insert("timeline", timeline->generateJson());
    ret.insert("savepath", savepath);
    ret.insert("framerate", framerate);
    return ret;
}

void MainWindow::scaleUpView()
{

    if (currentTimelineScaling * 1.111 < 1 ){
        timelineView->scale(1.111 , 1);
        currentTimelineScaling *= 1.111;
        timeline->ruler.scale *=1.111;
    }
    else{
        timelineView->resetTransform();
        currentTimelineScaling = 1;
        timeline->ruler.scale =1;

        //on no scaling timeline, i encounter bugs, so its a dirty trick, i don't think i will have the time to figure out the reason.

        timelineView->scale(0.999 , 1);
        currentTimelineScaling *= 0.999;
        timeline->ruler.scale *=0.999;
    }
}

void MainWindow::scaleDownView()
{
    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());
    QRectF visible_scene_rect = timelineView->mapToScene(viewport_rect).boundingRect();

    if (visible_scene_rect.width()*1.111< timeline->sceneRect().width() ){
        timelineView->scale(0.9 , 1);
        currentTimelineScaling *= 0.9;
        timeline->ruler.scale *=0.9;
    }
    else
    {

        timeline->scaleViewToScene();
    }
    qDebug()<<currentTimelineScaling;
}

//void MainWindow::displaceSelectionInTimeline()
//{
//    qDebug()<<framePositionInput->text();

//    if (framePositionInput->text() != "")
//    {
//        try
//        {
//            int i = std::stoi(framePositionInput->text().toStdString());
//            qDebug()<<i;
//            timeline->displaceSelection(i);

//        }
//        catch (std::invalid_argument const &e)
//        {
//            qDebug() << "Bad input: std::invalid_argument thrown";
//        }
//        catch (std::out_of_range const &e)
//        {
//            qDebug() << "Integer overflow: std::out_of_range thrown";
//        }
//    }
//}

//void MainWindow::changeSelectionSizeInTimeline()
//{
//    if (boxSizeInput->text() != "")
//    {
//        try
//        {
//            int i = std::stoi(boxSizeInput->text().toStdString());
//            qDebug()<<i;
//            timeline->changeSelectionSize(i);

//        }
//        catch (std::invalid_argument const &e)
//        {
//            qDebug() << "Bad input: std::invalid_argument thrown";
//        }
//        catch (std::out_of_range const &e)
//        {
//            qDebug() << "Integer overflow: std::out_of_range thrown";
//        }
//    }

//}

void MainWindow::displaySequences(QString path)
{

    if (reg->currentExpandedFolderSequences->contains(path)||reg->currentExpandedFolderSounds->contains(path))
    {
        sequencesModel->clear();
        QList<SequenceData *> list = reg->currentExpandedFolderSequences->value(path);
        QList<TbeSoundData *> list2 =  reg->currentExpandedFolderSounds->value(path);
        QStandardItem root (path);
        int i = 0;

        foreach (SequenceData* current, list){
            sequencesModel->insertRow(i, new QStandardItem(current->name));
            QPixmap logo (10,10);
            logo.fill(QColor(100,150,255));
            sequencesModel->item(i,0)->setData(logo, Qt::DecorationRole);
        }
        foreach (TbeSoundData* current, list2){
            QPixmap a(":/images/360.png");
            QIcon logo(a);
            sequencesModel->insertRow(i, new QStandardItem(current->filename));
            auto soundrep = sequencesModel->item(i,0);
            soundrep->setData(logo, Qt::DecorationRole);
        }
        QStringList a;
        a.append(path);
        sequencesModel->setHorizontalHeaderLabels(a);
    }
}


void MainWindow::clearSequencesAndCollapse(QModelIndex index)
{
    collapseChildrens(index);
    reg->clearSequencesInDir();
    sequencesModel->clear();
}

void MainWindow::clearSequences()
{
    sequencesModel->clear();
}
void MainWindow::collapseChildrens(QModelIndex index)
{
    if (!index.isValid()) {
        return;
    }

    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; i++) {
        const QModelIndex &child = index.child(i, 0);
        // Recursively call the function for each child node.
        collapseChildrens(child);
    }

    if (tree->isExpanded(index)) {
        tree->collapse(index);
    }
}

void MainWindow::collapseAllAndExpand(QModelIndex index)
{
    disconnect(tree, SIGNAL(expanded(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    disconnect(tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
    if (!index.isValid()) {
        return;
    }
    collapseChildrens(TreeModel->index(0,0));
    tree->scrollTo(index);
    tree->setExpanded(index, true);
    connect (tree, SIGNAL(expanded(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    //connect (tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
}

void MainWindow::insertComponentAtEndOfTimeline(QModelIndex)
{
    auto parent = sequencesStorageView;
    auto file = parent->selectionModel()->selectedIndexes()[0].data().toString();
    auto path =  parent->model()->headerData(0, Qt::Horizontal, 0).toString();
    if (file.right(3)=="tbe") // this evaluation should be done differently
    {

        auto list = reg->currentExpandedFolderSounds->value(path);
        foreach (TbeSoundData* current, list){
            if(current->filename == file){
                timeline->insertSoundAtEnd(current);
            }
        }
    }
    else{
        auto list = reg->currentExpandedFolderSequences->value(path);
        foreach (SequenceData* current, list){
            if(current->name == file){
                timeline->insertShotAtEnd({current});
            }
        }
    }
}

void MainWindow::scaleViewToScene()
{
    QRect viewport_rect(0, 0, timelineView->viewport()->width(), timelineView->viewport()->height());
    QRectF visible_scene_rect = timelineView->mapToScene(viewport_rect).boundingRect();
    float resizefactor = visible_scene_rect.width()/timeline->sceneRect().width();
    timelineView->scale(resizefactor , 1);
    currentTimelineScaling *= resizefactor;
    timeline->ruler.scale *=resizefactor;
}

void MainWindow::validateSequenceIntegrity()
{
    timeline->validateDataIntegrity();
}


void MainWindow::deleteSelection()
{
    if (timeline->selectedItems().isEmpty())
        return;
    QList<Shot*> deletedShots;
    QList<SoundTrack*> deletedSounds;
    foreach (QGraphicsItem* current, timeline->selectedItems()){
        auto sh = dynamic_cast<Shot*>(current);
        auto sound = dynamic_cast<SoundTrack*>(current);
        if (sh)
            deletedShots.append(sh);
        else if (sound)
            deletedSounds.append(sound);
    }
    QUndoCommand *deleteCommand = new DeleteShotsCommand(deletedShots, deletedSounds, timeline);
    isSaved = false;
    undoStack->push(deleteCommand);
}

void MainWindow::createdShot(QList<SequenceData *> seq, int xpos, int length, TimelineScene *timeline , QVector<Shot*> movedShots)
{
    sequencesStorageView->clearSelection();
    QUndoCommand *createCommand = new AddCommand(seq, xpos, length, timeline , movedShots);
    isSaved = false;
    undoStack->push(createCommand);
}

void MainWindow::createdSound(SoundTrack* suppressedSound, TbeSoundData* sd, int xpos, int length, TimelineScene* timeline, QVector<SoundTrack*> movedSounds)
{
    sequencesStorageView->clearSelection();
    QUndoCommand *createCommand = new AddSoundCommand(suppressedSound, sd, xpos, length, timeline , movedSounds);
    isSaved = false;
    undoStack->push(createCommand);
}




//void MainWindow::createdSound(SoundTrack, int xpos, int length, TimelineScene *timeline, QVector<SoundTrack *> movedSounds)
//{
//    qDebug()<<"createdSoundTriggered";
//}

void MainWindow::movedShots(TimelineScene* timeline,QVector<Shot *> movedShots, int prevscenesize, int currentscenesize)
{
    qDebug()<<"reached moveshots in main";
    QUndoCommand *moveCommand = new MoveCommand(timeline, movedShots, prevscenesize, currentscenesize);
    isSaved = false;
    undoStack->push(moveCommand);
}
void MainWindow::movedSoundtracks(TimelineScene * timeline, QVector<SoundTrack *> movedsounds, int prevscenesize, int currentscenesize)
{
    QUndoCommand *moveSoundsCommand = new MoveSoundsCommand(timeline, movedsounds, prevscenesize, currentscenesize);
    isSaved = false;
    undoStack->push(moveSoundsCommand);
}

void MainWindow::clearedTimeline(TimelineScene *timeline, QVector<Shot *> removedshots, int previtimelinesize)
{
    timeline->clearSelection();
    changeSelectedShotInParametersInterface();
    QUndoCommand *clear = new ClearCommand(timeline, removedshots, previtimelinesize);
    undoStack->push(clear);
}

void MainWindow::resizedShot(TimelineScene *timeline ,  QVector<Shot *>movedshot, Shot* resizedshot, int  newsize, int newscenesize)
{
    QUndoCommand* resizeShot=  new ResizeShotCommand(timeline, movedshot, resizedshot, newsize);
    undoStack->push(resizeShot);
}

void MainWindow::resizedSound(TimelineScene *, QVector<SoundTrack *>, SoundTrack *, int, int, int)
{

}


void MainWindow::changeParameterInAShot(QList<Shot *> sh, QJsonObject obj)
{
    qDebug()<<"arrived in main"<< obj.value("value").toString();
    QUndoCommand *changeparam = new ChangeParameterInShotCommand(this, sh, obj);
    isSaved = false;
    undoStack->push(changeparam);
}

void MainWindow::changeFrameIn(Shot * sh , int val)
{
    QUndoCommand *changeparam = new changeFrameInCommand(sh, val);
    isSaved = false;
    undoStack->push(changeparam);
}

void MainWindow::saveActionTriggered()
{
    if (savepath == ""){
        saveAsTriggered();
    }
    else{
        saveRequestExecuted(savepath);
    }

}

void MainWindow::saveAsTriggered()
{
    ValidationOnFocusEnabled =false;

    saveDialog = new ProjectLoader(true, "", this);
    saveDialog->setModal(true);
    saveDialog->exec();
    if (saveDialog->result()){
        savepath = saveDialog->selectedFiles().at(0);
        saveRequestExecuted(saveDialog->selectedFiles().at(0));

    }
    ValidationOnFocusEnabled =true;
}

void MainWindow::loadActionTriggered()
{
    ValidationOnFocusEnabled =false;
    if (!isSaved)
    {
        if (ProjectLoader::confirm("do you want to save modified document","unsaved modifications"))
            saveActionTriggered();
    }
    saveDialog = new ProjectLoader(false, "", this);
    saveDialog->setModal(true);
    saveDialog->exec();
    if (saveDialog->result()){
        loadRequestExecuted(saveDialog->selectedFiles().at(0));
    }
    ValidationOnFocusEnabled =true;
}

void MainWindow::exportTriggered()
{
    ValidationOnFocusEnabled =false;
    if ( timeline->validateDataIntegrity()){


        saveDialog = new ProjectLoader(true, "", this);
        saveDialog->setDefaultSuffix("prztoc");
        saveDialog->setNameFilters({"*.prztoc"});
        saveDialog->setWindowTitle("Export");
        saveDialog->exec();
        if (saveDialog->result()){
            exportRequestExecuted(saveDialog->selectedFiles().at(0));
        }
    }
    else{
        displayStatusBarMessage("movie contains corrupted sequences, it cannot be formated to play", 3000);
    }
    ValidationOnFocusEnabled =true;
}

void MainWindow::newTriggered()
{
    qDebug()<<"reached new Triggered";
    if (!isSaved)
    {
        if (ProjectLoader::confirm("do you want to save modified document","unsaved modifications"))
            saveActionTriggered();
    }
    newRequestExecuted();
}

void MainWindow::saveRequestExecuted(QString filepath)
{
    qDebug()<<filepath;
    QFile file(filepath);
    if (filepath.size()!=0){
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        {
            qDebug()<<"file now exists";

            QJsonDocument content(toJSON());
            file.write(content.toJson());
            file.close();
            isSaved = true;
            qDebug() << "Writing finished";
        }
    }
}
void MainWindow::initShotsParameters()
{
    QFile file (":/templates/shot.json");
    file.open(QIODevice::ReadOnly);

    QTextStream stream(&file);
    QString stringContent = stream.readAll();
    QJsonDocument configJson = QJsonDocument::fromJson(stringContent.toUtf8());

    if(isValidJsonObject(configJson))
    {
        shotparams = new ShotParametersInterface(configJson.object(), this);
        sublayoutparams1->addWidget(shotparams);
        scrollArea->setWidget(shotparams);
        connect (shotparams, SIGNAL(errorDisplay(QString, int)), this, SLOT(displayStatusBarMessage(QString, int)));
        connect (shotparams, SIGNAL (changeShotSize(int, QString)), timeline, SLOT( changeSelectionSize(int, QString)));
        connect (shotparams, SIGNAL (changeShotPosition(int, QString)), timeline, SLOT(displaceSelection(int, QString)));
        connect (timeline, SIGNAL(selectionXChanged()), shotparams, SLOT(updateShotPos()));
        scrollArea->setLayout(sublayoutparams1);
        enableParameterInterface(false);
        connect(shotparams, SIGNAL(valueChangedRequest(QList<Shot*>, QJsonObject)), this, SLOT(changeParameterInAShot(QList<Shot*>, QJsonObject)));
        connect(shotparams, SIGNAL(changeFrameIn(Shot*, int)), this, SLOT(changeFrameIn(Shot*, int)));
    }

}

void MainWindow::loadRequestExecuted(QString filepath)
{
    QFile file(filepath);
    if (filepath.size()!=0){
        if (file.open(QIODevice::ReadWrite))
        {

            QTextStream stream(&file);
            QString stringContent = stream.readAll();
            QJsonDocument readJson =  QJsonDocument::fromJson(stringContent.toUtf8());
            QJsonObject save;

            if(isValidJsonObject(readJson))
            {
                newRequestExecuted();
                save = readJson.object();
                savepath = save.value("savepath").toString();
                framerate = save.value("framerate").toInt();
                timeline->loadJSONfromSave(save.value("timeline").toObject());

            }
            else
            {
                ProjectLoader :: notifyFailure("document has not the proper format", "error");
            }
        }
        else
        {
            ProjectLoader :: notifyFailure("file doesnt exist", "error");
        }
        file.close();
    }
}

void MainWindow::exportRequestExecuted(QString filepath)
{
    qDebug()<<filepath <<"should created prztoc";
    QFile file(filepath);


    if (filepath.size()!=0){
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        {
            int index = 1;

            //to not use same sequence mutliple time
            QHash<SequenceData*, int> writtenSequences;

            QJsonObject exportJson;
            QJsonArray files;
            QJsonArray sound;
            QJsonArray sequences;
            SoundTrack* soundd = nullptr;
            QVector<Shot*> sortedlist;

            foreach (QGraphicsItem *current, timeline->items()){
                Shot *rec = dynamic_cast<Shot*>(current);
                auto sd = dynamic_cast<SoundTrack*>(current);
                if (rec)
                    sortedlist.append(rec);
                if (sd)
                    soundd = sd;
            }
            std::sort(sortedlist.begin(), sortedlist.end(), [](Shot *a, Shot *b){
                return a->previousxpos<b->previousxpos;
            });

            if (soundd != nullptr){
                QJsonObject soun;
                QString soundpath ("");
                soundpath.append(soundd->soundfile->path).append("/").append(soundd->soundfile->filename);
                soun.insert("file", soundpath);
                sound.append(soun);
            }

            int currentframewritten = 0;
            int blanqSeqCount = 0;
            foreach (Shot* current, sortedlist){

                //if the shots are not alligned

                if (current->scenePos().x()!=currentframewritten){
                    auto blankSeq = generateEmptyScene((current->scenePos().x()-currentframewritten)/10);
                    if (soundd != nullptr && soundd->scenePos().x() == currentframewritten)
                        blankSeq.insert("audio", 0);
                    sequences.append(blankSeq);
                    blanqSeqCount++;

                }

                int fileIndex;
                if (current->seqs.size()>0){
                    if (!writtenSequences.contains(current->seqs[0])){
                        auto fileToBeInserted = current->seqs[0]->generateJsonForExport();
                        fileIndex =  index;
                        writtenSequences.insert(current->seqs[0], index);
                        fileToBeInserted.insert("index", index++);
                        fileToBeInserted.insert("format", 3);
                        files.append(fileToBeInserted);

                    }
                    else{
                        fileIndex = writtenSequences.value(current->seqs[0]);
                    }
                }
                currentframewritten = current->scenePos().x()+current->rect().width();
                auto finalseq = current->generateExportJson(fileIndex);
                if (soundd != nullptr && soundd->scenePos().x() == current->scenePos().x())
                    finalseq.insert("audio", 0);
                sequences.append(finalseq);
            }
            //now that we have parsed all the timeline to know all the required frames, we can reduce the amount of frame used in the file definition.

            files = formatUtilRange(files, sequences);

            //we need a blank sequence if there is unassigned frames in the timeline

            if (blanqSeqCount != 0)
                files.append(generateBlankPrz());
            exportJson.insert("files",  files);
            exportJson.insert("scenes", sequences);
            exportJson.insert("audio", sound);
            QJsonDocument print(exportJson);

            file.write(print.toJson());
            file.close();
        }
    }
}

void MainWindow::newRequestExecuted()
{
    initShotsParameters();
    resetUndoStack();
    reg->corruptedSequences.clear();
    foreach (QGraphicsItem* current, timeline->items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        SoundTrack* sound = dynamic_cast<SoundTrack*>(current);
        if (sh){
            timeline->removeItem(sh);
            //delete(&sh);
        }
        else if(sound)
        {
            timeline->removeItem(sound);
        }
    }
    timeline->setSceneRect(0,0, 10000, 400);
    isSaved = true;
    savepath = "";
}

QJsonArray MainWindow::formatUtilRange(QJsonArray files, QJsonArray sequences)
{
    QJsonArray rettt;
    qDebug()<<sequences;
    foreach (QJsonValue current, files){
        QJsonArray ret;
        auto currentFile = current.toObject();
        int fileIndex = currentFile.value("index").toInt();
        auto frames = currentFile.value("frames").toArray();
        QSet<int> set;
        foreach (QJsonValue currents, sequences){
            auto currentSeq = currents.toObject();

            // well this would be long aswell to define all those json objects and all those arrays, if the current file is associated with this sequence, i get the starting frame
            // and the ending frame, in the loop i write all the util range in the set

            if (fileIndex == currentSeq.value("positions").toArray().first().toObject().value("frames").toArray().first().toArray().first().toObject().value("file").toInt()){
                int startframe = currentSeq.value("positions").toArray().first().toObject().value("frames").toArray().first().toArray().first().toObject().value("frame").toInt();
                int endframe = currentSeq.value("positions").toArray().first().toObject().value("frames").toArray().last().toArray().first().toObject().value("frame").toInt();
                for (int i = startframe; i<=endframe; i++){
                    set.insert(i);
                }
            }
        }

        // yes, i'm rewritting a new file object.. i encountered problem using json not retaining the values i changed, maybe it was latenight, now i'm afraid
        foreach (QJsonValue currentf, frames){
            auto currentframe = currentf.toObject();
            if (set.contains(currentframe.value("frame").toInt()))
                ret.append(currentframe);
        }
        currentFile.insert("frames", ret);
        rettt.append(currentFile);
    }
    return rettt;
}


QJsonObject MainWindow::generateBlankPrz()
{
    QFile blankframe("./blank.0000.prz");
    if (blankframe.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        blankframe.write("");
        blankframe.close();
    }
    QJsonObject blankSequence;
    blankSequence.insert("index", 0);
    blankSequence.insert("file", "./blank");
    blankSequence.insert("padding", 4);
    blankSequence.insert("format", 3);
    QJsonArray blankframear;
    QJsonObject blankfram;

    blankfram.insert("frame", 0);
    blankfram.insert("filesize", 0);

    blankframear.append(blankfram);
    blankSequence.insert("frames", blankframear);
    return blankSequence;
}

QJsonObject MainWindow::generateEmptyScene(int size)
{
    QJsonObject seq;
    seq.insert("framerate", 0);
    seq.insert("playmode", 0);
    seq.insert("autoplay", 0);
    seq.insert("resetView", 0);
    seq.insert("sceneRotation", 0);
    seq.insert("scale", 0);
    seq.insert("cameraHeight", 0);
    seq.insert("fadeFromBLackFrameIn", 0);
    seq.insert("fadeFromBlackFrameOut", 0);
    seq.insert("fadeToBlackFrameIn", 0);
    seq.insert("fadeToBlackFrameOut", 0);
    seq.insert("glowIntensity", 0);
    seq.insert("glowPower", 0);
    seq.insert("continueAudio",0);
    seq.insert("audioRotation", 0);
    seq.insert("backgroundRotation", 0);
    QJsonObject positions;
    positions.insert("x",0);
    positions.insert("y",0);
    positions.insert("z", 0);

    QJsonArray frames;
    for (int i = 0; i < size; i++)
    {
        QJsonObject frame;
        frame.insert("file", 0);
        frame.insert("frame", 0);
        frames.append(frame);
    }
    positions.insert("frames", frames);
    seq.insert("positions", positions);
    return seq;
}

void MainWindow::changeSelectedShotInParametersInterface()
{

    if (timeline->validateParameterTargetChange())
    {
        if (timeline->selectedItems().length()==1){
            auto* sound = dynamic_cast<SoundTrack*>(timeline->selectedItems()[0]);
            if (sound){
                enableParameterInterface(false);
            }
            else{
                auto* sh = dynamic_cast<Shot*>(timeline->selectedItems()[0]);
                shotparams->setShot({sh});
                enableParameterInterface(true);
            }
        }
        else{
            QList<Shot*> selectedShots;
            foreach(QGraphicsItem* current, timeline->selectedItems()){
                auto* shot = dynamic_cast<Shot*>(current);
                auto* sound = dynamic_cast<SoundTrack*>(timeline->selectedItems()[0]);
                if (sound){
                    sound->setSelected(false);
                }
                else if (shot){
                    selectedShots.append(shot);
                }

            }
            shotparams->setShot(selectedShots);
            enableParameterInterface(true);
        }
    }
    else{
        enableParameterInterface(false);
    }
}

void MainWindow::displayStatusBarMessage(QString message , int durationInMs)
{
    this->statusBar()->showMessage(message, durationInMs);
}


bool MainWindow::isValidJsonObject(QJsonDocument doc)
{
    return !doc.isNull() && doc.isObject();
}
