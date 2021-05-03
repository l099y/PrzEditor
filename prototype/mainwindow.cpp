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
    for (int i = 0; i <50 ; i++)
    {
        scaleDownView();
    }
    this->showMaximized();

    connect(timeline, SIGNAL(selectionChanged()), this, SLOT(changeSelectedShotInParametersInterface()));
}
void MainWindow :: initButtons(){

    allignbutton->setText("allign");
    allignbutton->setMinimumSize(QSize(40,40));
    connect(allignbutton, SIGNAL(clicked(bool)), timeline, SLOT(debugItems()));
    clearbutton->setText("clear");
    clearbutton->setMinimumSize(QSize(40,40));
    connect(clearbutton, SIGNAL(clicked(bool)), timeline, SLOT(clearItems()));
    newboxbutton->setMinimumSize(QSize(40,40));
    newboxbutton->setText("new");
    connect(newboxbutton, SIGNAL(clicked(bool)), timeline, SLOT(newRect()));
    mod2button->setText("mod2");
    mod2button->setMinimumSize(QSize(40,40));
    connect(mod2button, SIGNAL(clicked(bool)), timeline, SLOT(setmod2()));
    dispbutton->setText("disp");
    dispbutton->setMinimumSize(QSize(40,40));
    connect(dispbutton, SIGNAL(clicked(bool)), timeline, SLOT(setdisp()));
    delbutton->setText("disp");
    delbutton->setMinimumSize(QSize(40,40));
    connect(delbutton, SIGNAL(clicked(bool)), timeline, SLOT(deleteSelection()));


}
void MainWindow ::initLayouts(){

    initwidgetsparams();
    initcontenance();
    bindLayoutsToWidgets();

}
void MainWindow::initcontenance(){

    sublayoutsplit0->addWidget(timelineButtons);
    sublayoutsplit0->addWidget(supposedtimeslider);
    sublayoutEditor->addWidget(timelineNutils);
    sublayoutEditor->addWidget(parameters);
    sublayoutsplit->addWidget(tree);
    sublayoutsplit->addWidget(sequencesStorageView);
    sublayoutsplit->addWidget(params1);

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable( true );


    sublayoutsplit->setMargin(20);
    sublayoutbutton->addWidget(newboxbutton);
    sublayoutbutton->addWidget(clearbutton);
    sublayoutbutton->addWidget(allignbutton);
    sublayoutbutton->addWidget(dispbutton);
    sublayoutbutton->addWidget(mod2button);
    sublayoutbutton->addWidget(delbutton);
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
void MainWindow::initwidgetsparams(){

    widget->setMinimumWidth(1600);
    sublayoutsplit->setMargin(0);
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
    timelineNutils->setMaximumHeight(350);
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

    // generateData(); //well you only need to do it once...

    connect (tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
    connect (tree, SIGNAL(clicked(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    connect (TreeModel, SIGNAL(clearSequences()), this, SLOT(clearSequences()));
    connect (TreeModel, SIGNAL(displaySequences(QString)), this, SLOT(displaySequences(QString)));
    connect (TreeModel, SIGNAL(setOnlyCurrentFolderExpanded(QModelIndex)), this, SLOT(collapseAllAndExpand(QModelIndex)));
}

void MainWindow::inittimelinescene(){

    timelineView->setScene(timeline);
    timelineView->setStyleSheet(QString("QScrollBar:horizontal { border: 2px solid grey; background: #505050; height: 15px; margin: 1px; }"));
    timelineView->setDragMode(QGraphicsView::RubberBandDrag);
    timelineView->setAlignment(Qt::AlignLeft);
    timelineView->setMaximumHeight(300);
    timelineView->acceptDrops();
    sublayouttimeline->addWidget(timelineView);
    sublayouttimeline->setAlignment(Qt::AlignTop);
    sublayoutbutton->setAlignment(Qt::AlignTop);
    timelineView->setMinimumHeight(300);
    timelineView->setRenderHint(QPainter::Antialiasing);

    connect(timeline, SIGNAL(scaleUp()), this, SLOT(scaleUpView()));
    connect(timeline, SIGNAL(scaleDown()),this, SLOT(scaleDownView()));
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
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
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
    connect (timeline, SIGNAL(deleteSelectionSignal()), this, SLOT(deleteSelection()));
    connect (timeline, SIGNAL(createShot(QList<SequenceData*>, int , int , TimelineScene*, QVector<Shot*>)), this, SLOT(createdShot(QList<SequenceData*>, int, int, TimelineScene*, QVector<Shot*>)));
    connect (timeline, SIGNAL(moveShotss(TimelineScene*, QVector<Shot*>,int, int)), this, SLOT(movedShots(TimelineScene*, QVector<Shot*>, int, int)));
    connect (timeline, SIGNAL(clearTimeline(TimelineScene*, QVector<Shot*>, int)), this, SLOT(clearedTimeline(TimelineScene*, QVector<Shot*>, int)));
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange && this->isActiveWindow()) {
        qDebug()<<"retrieve the focus";
    }
    QWidget :: changeEvent(event);
}

void MainWindow::generateData()
{
    TreeModel->mkdir(TreeModel->index("c://DataTest"), "NestedTest");
    QString path ("c://DataTest//");

    for (int i = 0; i<3000; i++)
    {
        QString name ("Davinci_workshop.");
        name = name.append(i < 10 ? "00000" : i<100 ? "0000" : i<1000? "000" : i<10000? "00": "0").append("%1").arg(i).append(".prz");
        QFile file(path+name);
        file.open(QIODevice::ReadWrite);
    }
}

void MainWindow::enableParameterInterface(bool mod)
{
        shotparams->setVisible(mod);
        shotparams->setEnabled(mod);
        scrollArea->setVisible(mod);
}

void MainWindow::changeButtonTxt(){

}

void MainWindow::scaleUpView()
{
    if (currentTimelineScaling * 1.1 < 1 ){
        timelineView->scale(1.1 , 1);
        currentTimelineScaling *= 1.1;
        timeline->ruler.scale *=1.1;
    }
    else{
        timelineView->resetTransform();
        currentTimelineScaling = 1;
        timeline->ruler.scale =1;
    }
    qDebug()<<currentTimelineScaling;

}

void MainWindow::scaleDownView()
{
    if (currentTimelineScaling * 0.9 > 0.001 ){
        timelineView->scale(0.9 , 1);
        currentTimelineScaling *= 0.9;
        timeline->ruler.scale *=0.9;
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

    if (reg->currentExpandedFolderSequences->contains(path))
    {
        sequencesModel->clear();
        QList<SequenceData *> list = reg->currentExpandedFolderSequences->value(path);
        QStandardItem root (path);
        int i = 0;

        foreach (SequenceData* current, list){
            sequencesModel->insertRow(i, new QStandardItem(current->name));
            QPixmap logo (10,10);
            logo.fill(QColor(100,150,255));
            sequencesModel->item(i,0)->setData(logo, Qt::DecorationRole);
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
    disconnect(tree, SIGNAL(expanded(QModelIndex)), 0, 0);
    disconnect(tree, SIGNAL(collapsed(QModelIndex)), 0, 0);
    if (!index.isValid()) {
        return;
    }
    collapseChildrens(TreeModel->index(0,0));
    tree->scrollTo(index);
    tree->setExpanded(index, true);
    connect (tree, SIGNAL(expanded(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    connect (tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
}

void MainWindow::deleteSelection()
{
    if (timeline->selectedItems().isEmpty())
        return;

    QUndoCommand *deleteCommand = new DeleteCommand(timeline);
    undoStack->push(deleteCommand);
}

void MainWindow::createdShot(QList<SequenceData *> seq, int xpos, int length, TimelineScene *timeline , QVector<Shot*> movedShots)
{
    QUndoCommand *createCommand = new AddCommand(seq, xpos, length, timeline , movedShots);
    undoStack->push(createCommand);
}

void MainWindow::movedShots(TimelineScene* timeline,QVector<Shot *> movedShots, int prevscenesize, int currentscenesize)
{
    qDebug()<<"reached moveshots in main";
    QUndoCommand *moveCommand = new MoveCommand(timeline, movedShots, prevscenesize, currentscenesize);
    undoStack->push(moveCommand);
}

void MainWindow::clearedTimeline(TimelineScene *timeline, QVector<Shot *> removedshots, int previtimelinesize)
{
    QUndoCommand *clear = new ClearCommand(timeline, removedshots, previtimelinesize);
    undoStack->push(clear);
}

void MainWindow::saveActionTriggered()
{
    saveDialog = new ProjectLoader(true, this);
    saveDialog->setModal(true);
    connect(saveDialog, SIGNAL(saveRequest(QString)), this, SLOT(saveRequestExecuted(QString)));
    saveDialog->exec();

    qDebug()<<"saveAction triggered";
}

void MainWindow::loadActionTriggered()
{
    qDebug()<<"loadAction triggered";
    saveDialog = new ProjectLoader(false, this);
    saveDialog->setModal(true);
    connect(saveDialog, SIGNAL(loadRequest(QString)), this, SLOT(loadRequestExecuted(QString)));
    saveDialog->exec();
}

void MainWindow::saveRequestExecuted(QString filepath)
{
    qDebug()<<filepath;
    QFile file(filepath);
    if (filepath.size()!=0){
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        {
            qDebug()<<"file now exists";
            QJsonDocument content(timeline->generateJson());
            file.write(content.toJson());
            file.close();
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
        scrollArea->setLayout(sublayoutparams1);
        enableParameterInterface(false);
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
            QJsonObject obj;

            // check validity of the document
            if(isValidJsonObject(readJson))
            {
                obj = readJson.object();
                resetUndoStack();
                reg->usedSequences.clear();
                reg->corruptedSequences.clear();
                foreach (QGraphicsItem* current, timeline->items()){
                    Shot* sh = dynamic_cast<Shot*>(current);
                    if (sh)
                       timeline->removeItem(sh);
                }
                timeline->setSceneRect(0,0,obj.value("size").toInt(), 300);
                timeline->newRect();
                QJsonArray shots = obj.value("shots").toArray();
                foreach (QJsonValue current, shots){
                    auto obj =  current.toObject();
                    Shot* shotToBeInsert = new Shot(obj);
                    foreach (QJsonValue currentseq, obj.value("sequences").toArray()){
                        auto curseq = currentseq.toObject();
                        if (reg->usedSequences.contains(curseq.value("name").toString())){
                            shotToBeInsert->seqs.append(reg->usedSequences.value(curseq.value("name").toString()));
                        }
                        else
                        {
                            SequenceData* sq = new SequenceData(curseq, nullptr);
                            shotToBeInsert->seqs.append(sq);
                            reg->usedSequences.insert(sq->name, sq);
                        }

                    }
                    timeline->addItem(shotToBeInsert);
                }
            }
            else
            {
                ProjectLoader :: notifyFailure("document has not the proper format", "error");
            }
        }
        else
        {
            ProjectLoader :: notifyFailure("document has not the proper format", "error");
        }
        file.close();
    }
}

void MainWindow::changeSelectedShotInParametersInterface()
{
    if (timeline->validateParameterTargetChange())
    {
        Shot* sh = dynamic_cast<Shot*>(timeline->selectedItems().at(0));
        if (sh){

            shotparams->setShot(sh);
            enableParameterInterface(true);
        }
    }
    else{
        enableParameterInterface(false);
    }
}


bool MainWindow::isValidJsonObject(QJsonDocument doc)
{
    return !doc.isNull() && doc.isObject();
}
