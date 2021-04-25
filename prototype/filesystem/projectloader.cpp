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
    view->setMinimumSize(700,400);
    view->setColumnWidth(0, 300);
    actionButton = new QPushButton();
    cancelButton = new QPushButton("Cancel");

    nameInput = new QLineEdit(this);


    layout->addWidget(nameInput);
    layout->addWidget(actionButton);
    layout->addWidget(cancelButton);

    QStringList filter;
    filter<< "*.przsqs";

    model->setNameFilters(filter);

    connect (cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect (nameInput, SIGNAL(textEdited(QString)), this, SLOT(enableActionButton()));
    connect (view, SIGNAL(clicked(QModelIndex)),this, SLOT(enableActionButton()));

    if (creationmod){
        actionButton->setText("Save");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptSaving()));
        connect (nameInput, SIGNAL(returnPressed()), this, SLOT(attemptSaving()));
        actionButton->setDisabled(true);
    }
    else {
        actionButton->setText("Load");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptLoading()));
        connect (nameInput, SIGNAL(returnPressed()), this, SLOT(attemptLoading()));
        actionButton->setDisabled(true);
    }
}

ProjectLoader::~ProjectLoader()
{
    delete(model);
}

void ProjectLoader::attemptSaving()
{
    if (validateSave())
    qDebug()<<model->fileInfo(view->currentIndex()).absoluteFilePath()<< " in attempsaving";
    this->close();
}

void ProjectLoader::attemptLoading()
{
    if (validateLoad())
        this->close();
}

void ProjectLoader::enableActionButton()
{
    auto currentSelection = model->fileInfo(view->currentIndex());
    qDebug()<<currentSelection.suffix();
    if (creationmod){
        if (currentSelection.suffix() == "przsqs")
            nameInput->setText(currentSelection.fileName());
        if (nameInput->text().length() != 0 && (currentSelection.isDir() || currentSelection.suffix() == "przsqs")){
            actionButton->setDisabled(false);
        }

        else
        actionButton->setDisabled(true);

    }
    else
    {


    }
}

bool ProjectLoader::validateSave()
{

}

bool ProjectLoader::validateLoad()
{

}

void ProjectLoader::closeEvent(QCloseEvent *e)
{
    qDebug()<<"dialog closed";
    delete(this);
}
