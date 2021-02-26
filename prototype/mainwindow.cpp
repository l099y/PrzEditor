#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(widget);
    initLayouts();
    QPushButton* a = new QPushButton();
    a->setText("info");
    connect(a, SIGNAL(clicked(bool)), timeline, SLOT(debugItems()));
    sublayoutEditor->addWidget(a);
    QPushButton* b = new QPushButton();
    b->setText("clear");
    connect(b, SIGNAL(clicked(bool)), timeline, SLOT(clearItems()));
    QPushButton* c = new QPushButton();
    c->setText("new");
    connect(c, SIGNAL(clicked(bool)), timeline, SLOT(newRect()));
    sublayoutEditor->addWidget(c);
}
void MainWindow ::initLayouts(){
    setupTreeItem();
    initwidgetsparams();
    initlabs();
    initcontenance();
    bindLayoutsToWidgets();
    inittimelinescene();
    connect(button, SIGNAL(clicked(bool)), this, SLOT(changeButtonTxt()));

}
void MainWindow::initcontenance(){
    layout->addWidget(tree);
    layout->addWidget(rightFiller);
    sublayoutEditor->addWidget(supposedtimeslider);
    sublayoutEditor->addWidget(parameters);
    sublayoutsplit->addWidget(params1);
    sublayoutsplit->addWidget(params2);
    sublayoutparams1->addWidget(params1lab);
    sublayoutparams1->addWidget(button);
    sublayoutparams2->addWidget(params2lab);
}
void MainWindow::initwidgetsparams(){
    rightFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->setMinimumWidth(1200);
    sublayoutsplit->setMargin(0);
    sublayoutEditor->setMargin(0);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    params1->setAutoFillBackground(true);
    params1->setPalette(pal);
    params2->setAutoFillBackground(true);
    params2->setPalette(pal);
    button->setText("will i change?");
    supposedtimeslider->setMaximumHeight(500);
}
void MainWindow :: bindLayoutsToWidgets(){
    rightFiller->setLayout(sublayoutEditor);
    supposedtimeslider->setLayout(sublayouttimeline);
    parameters->setLayout(sublayoutsplit);
    params1->setLayout(sublayoutparams1);
    params2->setLayout(sublayoutparams2);
    widget->setLayout(layout);
}
void MainWindow::setupTreeItem(){
    TreeModel->setRootPath("");
    tree->setModel(TreeModel);
    tree->setRootIndex(idx);
    tree->setMinimumWidth(400);
    tree->setColumnHidden(1, true);
    tree->setColumnHidden(2, true);
    tree->setColumnHidden(3, true);
    tree->setHeaderHidden(true);
}
void MainWindow::initlabs(){
    params1lab->setText("parameters area");
    params2lab->setText("another parameters area");
}
void MainWindow::inittimelinescene(){
    timelineView = new QGraphicsView(timeline);
    timelineFitIn = new QGraphicsView(timeline);
    timelineView->setScene(timeline);
    timelineFitIn->setScene(timeline);
    timelineFitIn->fitInView(timeline->itemsBoundingRect(), Qt::KeepAspectRatio);
    sublayouttimeline->addWidget(timelineView);
    //sublayouttimeline->addWidget(timelineFitIn);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::changeButtonTxt(){
    button->setText("button");
}
