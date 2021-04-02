#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setAcceptDrops(true);
    ui->setupUi(this);
    setCentralWidget(widget);
    initLayouts();
    timelineView->setMinimumHeight(300);
    timelineView->setRenderHint(QPainter::Antialiasing);
    paramlabel->setMaximumHeight(100);
    // trial for focus restore
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
    displaceSelectionButton->setText("displace selection");
    connect(displaceSelectionButton, SIGNAL(clicked(bool)), this, SLOT(displaceSelectionInTimeline()));
    changeSelectionSizeButton->setText("Change selection Size");
    connect (changeSelectionSizeButton, SIGNAL(clicked(bool)), this, SLOT(changeSelectionSizeInTimeline()));

}
void MainWindow ::initLayouts(){
    setupTreeItem();
    initwidgetsparams();
    initcontenance();
    bindLayoutsToWidgets();
    inittimelinescene();
    initButtons();
}
void MainWindow::initcontenance(){


    sublayoutsplit0->addWidget(timelineButtons);
    sublayoutsplit0->addWidget(supposedtimeslider);
    sublayoutEditor->addWidget(timelineNutils);
    sublayoutEditor->addWidget(parameters);
    sublayoutsplit->addWidget(params1);
    sublayoutsplit->addWidget(storageView);
    sublayoutsplit->addWidget(tree);
    sublayoutsplit->addWidget(sequencesStorageView);
    sublayoutsplit->setMargin(20);
    sublayoutparams1->addWidget(paramlabel);
    sublayoutbutton->addWidget(newboxbutton);
    sublayoutbutton->addWidget(clearbutton);
    sublayoutbutton->addWidget(allignbutton);
    sublayoutbutton->addWidget(dispbutton);
    sublayoutbutton->addWidget(mod2button);
    sublayoutbutton->addWidget(delbutton);
    sublayoutparams1->addWidget(displaceSelectionButton);
    sublayoutparams1->addWidget(framePositionInput);
    sublayoutparams1->addWidget(changeSelectionSizeButton);
    sublayoutparams1->addWidget(boxSizeInput);
    sublayoutparams1->setAlignment(Qt::AlignTop);

}
void MainWindow::initwidgetsparams(){

    widget->setMinimumWidth(1600);
    sublayoutsplit->setMargin(0);
    sublayoutEditor->setMargin(0);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    params1->setAutoFillBackground(true);
    params1->setPalette(pal);
    //params1->setVisible(false);
    params1->setMinimumWidth(400);

    params2->setAutoFillBackground(true);
    params2->setPalette(pal);
    params2->setVisible(false);
}
void MainWindow :: bindLayoutsToWidgets(){
    timelineNutils->setLayout(sublayoutsplit0);
    timelineNutils->setMaximumHeight(350);
    supposedtimeslider->setLayout(sublayouttimeline);
    timelineButtons->setLayout(sublayoutbutton);
    parameters->setLayout(sublayoutsplit);
    params1->setLayout(sublayoutparams1);
    params2->setLayout(sublayoutparams2);
    //widget->setLayout(layout);
}
void MainWindow::setupTreeItem(){
    sequencesStorageView = new QTreeView(this);
    sequencesStorageView->setModel(sequencesModel);
    sequencesStorageView->setMinimumWidth(150);
    sequencesStorageView->setDragEnabled(true);
    storageView = new QGraphicsView(this);
    storageView->setScene(storageScene);
    storageView->setMaximumWidth(140);
    TreeModel->setRootPath("");
    TreeModel->setNameFilterDisables(false);
    TreeModel->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    tree->setRootIndex(idx);
    tree->setMinimumWidth(400);
    tree->setModel(TreeModel);
    tree    ->setColumnHidden(1, true);
    tree->setColumnHidden(2, true);
    tree->setColumnHidden(3, true);
    tree->setHeaderHidden(true);
    connect (TreeModel, SIGNAL(displaySequences(QString)), storageScene, SLOT(displaySequences(QString)));
    connect (tree, SIGNAL(expanded(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
    connect (TreeModel, SIGNAL(displaySequences(QString)), this, SLOT(displaySequences(QString)));
}

void MainWindow::inittimelinescene(){
    timelineView = new QGraphicsView(timeline);
    timelineView->setScene(timeline);
    timelineView->setStyleSheet(QString("QScrollBar:horizontal { border: 2px solid grey; background: #505050; height: 15px; margin: 1px; }"));
    //    timelineView->setAcceptDrops(true);
    //    timelineView->setDragMode(QGraphicsView::RubberBandDrag);
    timelineView->setAlignment(Qt::AlignLeft);
    timelineView->setMaximumHeight(300);

    connect(timeline, SIGNAL(scaleUp()), this, SLOT(scaleUpView()));
    connect(timeline, SIGNAL(scaleDown()),this, SLOT(scaleDownView()));
    sublayouttimeline->addWidget(timelineView);
    sublayouttimeline->setAlignment(Qt::AlignTop);
    sublayoutbutton->setAlignment(Qt::AlignTop);



}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange && this->isActiveWindow()) {
        qDebug()<<"retrieve the focus";
    }
    QWidget :: changeEvent(event);
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
    if (currentTimelineScaling * 0.9 > 0.01 ){
        timelineView->scale(0.9 , 1);
        currentTimelineScaling *= 0.9;
        timeline->ruler.scale *=0.9;
    }
    qDebug()<<currentTimelineScaling;
}

void MainWindow::displaceSelectionInTimeline()
{
    qDebug()<<framePositionInput->text();

    if (framePositionInput->text() != "")
    {
        try
        {
            int i = std::stoi(framePositionInput->text().toStdString());
            qDebug()<<i;
            timeline->displaceSelection(i);

        }
        catch (std::invalid_argument const &e)
        {
            qDebug() << "Bad input: std::invalid_argument thrown";
        }
        catch (std::out_of_range const &e)
        {
            qDebug() << "Integer overflow: std::out_of_range thrown";
        }
    }
}

void MainWindow::changeSelectionSizeInTimeline()
{
    if (boxSizeInput->text() != "")
    {
        try
        {
            int i = std::stoi(boxSizeInput->text().toStdString());
            qDebug()<<i;
            timeline->changeSelectionSize(i);

        }
        catch (std::invalid_argument const &e)
        {
            qDebug() << "Bad input: std::invalid_argument thrown";
        }
        catch (std::out_of_range const &e)
        {
            qDebug() << "Integer overflow: std::out_of_range thrown";
        }
    }

}

void MainWindow::displaySequences(QString path)
{
    qDebug()<< sequencesModel->item(0,0);
    if (reg->storedSequences->contains(path))
    {
        sequencesModel->clear();
        QList<SequenceData *> list = reg->storedSequences->value(path);
        QStandardItem root (path);
        int i = 0;

        foreach (SequenceData* current, list){
            sequencesModel->insertRow(i, new QStandardItem(current->name));
        }
    }
}
