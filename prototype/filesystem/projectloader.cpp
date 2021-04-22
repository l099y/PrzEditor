#include "projectloader.h"
#include "QDebug"

ProjectLoader::ProjectLoader(QWidget * parent): QDialog(parent)
{
    layout = new QVBoxLayout(this);
    view = new QTreeView(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    view->setModel(model);
    this->setLayout(layout);
    layout->addWidget(view);
    view->setMinimumSize(550,400);
}

ProjectLoader::~ProjectLoader()
{
    delete(model);
}

void ProjectLoader::closeEvent(QCloseEvent *e)
{
    qDebug()<<"dialog closed";
    delete(this);
}
