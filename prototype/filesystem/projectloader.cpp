#include "projectloader.h"
#include "QDebug"

ProjectLoader::ProjectLoader(bool creationmod, QWidget * parent): QDialog(parent)
{
    this->creationmod = creationmod;
    layout = new QVBoxLayout(this);
    view = new QTreeView(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    view->setModel(model);
    this->setLayout(layout);
    layout->addWidget(view);
    view->setMinimumSize(550,400);

    actionButton = new QPushButton();
    cancelButton = new QPushButton("Cancel");

    nameInput = new QLineEdit(this);


    layout->addWidget(nameInput);
    layout->addWidget(actionButton);
    layout->addWidget(cancelButton);


    connect (cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    if (creationmod){
        actionButton->setText("Save");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptSaving()));
    }
    else {
        actionButton->setText("Load");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptLoading()));
    }
}

ProjectLoader::~ProjectLoader()
{
    delete(model);
}

void ProjectLoader::attemptSaving()
{
    // si les conditions sont satisfaites
    qDebug()<<model->fileInfo(view->currentIndex()).absoluteFilePath()<< " in attempsaving";
    //emit (save (nameInput, view->currentIndex().));

    this->close();
}

void ProjectLoader::attemptLoading()
{
    this->close();
}

void ProjectLoader::closeEvent(QCloseEvent *e)
{
    qDebug()<<"dialog closed";
    delete(this);
}
