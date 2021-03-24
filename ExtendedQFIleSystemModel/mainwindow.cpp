#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "extendedqfilesystemmodel.h"
#include "QGridLayout"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path = "";

       model = new ExtendedQFileSystemModel(this);
       model->setRootPath(path);
       model->setNameFilterDisables(false);
       model->setNameFilters(QStringList()<<"*.pdflist");
       model->setFilter(QDir::AllDirs| QDir::Modified |QDir::NoDotAndDotDot|QDir::Files);

       view = new QTreeView(this);
       view->setModel(model);
       view->setRootIndex(model->index(path));
       view->setAlternatingRowColors(true);
       view->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
       view->header()->setStretchLastSection(true);
       view->setDragEnabled(true);
       view->setSelectionMode(QAbstractItemView::SingleSelection);

       QGridLayout *mainLayout = new QGridLayout;
       mainLayout->addWidget(view,0,0,1,5);

       QWidget* widget = new QWidget(this);
       widget->setLayout(mainLayout);
       setCentralWidget(widget);

       setWindowTitle(tr("File system model with a size filter"));
       resize(480, 330);
       connect (view, SIGNAL(expanded(QModelIndex)), model, SLOT(parseExpandedDir(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

