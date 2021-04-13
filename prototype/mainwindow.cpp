#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsView>
#include <QPixmap>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->move(0,0);
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

    sublayoutsplit->addWidget(tree);
    sublayoutsplit->addWidget(sequencesStorageView);
    sublayoutsplit->addWidget(params1);
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
    sequencesStorageView->setMaximumWidth(200);
    sequencesStorageView->setDragEnabled(true);
    sequencesStorageView->setEditTriggers(nullptr);
    sequencesModel->setHorizontalHeaderLabels(QStringList("directoryContent"));

    //to link the click of the generatedsequencelist to an even that will setup for the drop

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
    generateData();
    connect (tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(clearSequencesAndCollapse(QModelIndex)));
    connect (tree, SIGNAL(expanded(QModelIndex)), TreeModel, SLOT(parseExpandedDir(QModelIndex)));
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

void MainWindow::generateData()
{
    TreeModel->mkdir(TreeModel->index("c://DataTest"), "NestedTest");
    QString path ("c://DataTest//NestedTest/");

    for (int i = 0; i<3000; i++)
        {
        QString name ("Sherlock.");
        name = name.append(i < 10 ? "00000" : i<100 ? "0000" : i<1000? "000" : i<10000? "00": "0").append("%1").arg(i).append(".prz");
        QFile file(path+name);
        file.open(QIODevice::ReadWrite);
    }
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
