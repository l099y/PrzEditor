#include "projectloader.h"
#include "QDebug"

ProjectLoader::ProjectLoader(bool creationmod, QWidget * parent): QDialog(parent)
{
    this->savingMod = creationmod;
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
    connect (nameInput, SIGNAL(textEdited(QString)), this, SLOT(setEnableStateByTextInput()));
    connect (view, SIGNAL(clicked(QModelIndex)),this, SLOT(setEnableStateByTree()));

    if (savingMod){
        actionButton->setText("Save");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptSaving()));
        connect (nameInput, SIGNAL(returnPressed()), this, SLOT(attemptSaving()));
        this->setWindowTitle("Save");
    }
    else {
        actionButton->setText("Load");
        connect (actionButton, SIGNAL(clicked(bool)), this, SLOT(attemptLoading()));
        connect (nameInput, SIGNAL(returnPressed()), this, SLOT(attemptLoading()));
        this->setWindowTitle("Load");
    }
    actionButton->setDisabled(true);
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

void ProjectLoader::setEnableStateByTree()
{
    auto currentSelection = model->fileInfo(view->currentIndex());
    //qDebug()<<currentSelection.fileName()<< "name file" <<currentSelection.isDir() << " is Dir" << currentSelection.isFile() << "is File" << currentSelection.isRoot();


    if (savingMod){
        if (currentSelection.suffix() == "przsqs"){
            nameInput->setText(currentSelection.fileName());
            actionButton->setDisabled(false);
        }
        else{
            if (currentSelection.isDir() && !currentSelection.isRoot() && nameInput->text().length() != 0){
                actionButton->setDisabled(false);
            }
            else{
                actionButton->setDisabled(true);
            }
        }
    }

    else{
        if (currentSelection.suffix() == "przsqs"){
            nameInput->setText(currentSelection.fileName());
            actionButton->setDisabled(false);
        }
        else{
            if (currentSelection.isDir() &&  !currentSelection.isRoot()){
                QDir currentdir (currentSelection.absoluteFilePath());
                qDebug()<<currentSelection.absoluteFilePath()<< "- filepath" << currentdir.entryList();
                if (currentdir.entryList().contains(nameInput->text().append(stringIsPrzsqsType(nameInput->text())? "": ".przsqs"))){
                    actionButton->setDisabled(false);
                }
            }
            else
            actionButton->setDisabled(true);
        }
    }
}

void ProjectLoader::setEnableStateByTextInput()
{
    auto currentSelection = model->fileInfo(view->currentIndex());
    //qDebug()<<currentSelection.fileName()<< "name file" <<currentSelection.isDir() << " is Dir" << currentSelection.isFile() << "is File" << currentSelection.isRoot();


    if (savingMod){
        if (nameInput->text().length() != 0 && (currentSelection.isDir() && !currentSelection.isRoot())){
            actionButton->setDisabled(false);
        }
        else
        actionButton->setDisabled(true);

    }
    else
    {
        if (currentSelection.isDir() &&  !currentSelection.isRoot())
        {
            QDir currentdir (currentSelection.absoluteFilePath());
            qDebug()<<currentSelection.absoluteFilePath()<< "- filepath" << currentdir.entryList();
            if (currentdir.entryList().contains(nameInput->text().append(stringIsPrzsqsType(nameInput->text())? "": ".przsqs"))){
                actionButton->setDisabled(false);
            }
            else
                actionButton->setDisabled(true);
        }
        else if (currentSelection.isFile() && currentSelection.suffix() == "przsqs"){
            actionButton->setDisabled(false);
        }
        else {
            actionButton->setDisabled(true);
        }


    }
}

bool ProjectLoader::validateSave()
{

}

bool ProjectLoader::validateLoad()
{

}

bool ProjectLoader::stringIsPrzsqsType(QString input)
{
    if(input.length()!= 0){
    int i = input.length()-1;
    QString ret;
    while (input.at(i)!='.' && i>0)
    {
        ret.append(input.at(i--));
    }
    return ret == "sqszrp";
    }
    else return false;
}

void ProjectLoader::closeEvent(QCloseEvent *e)
{
    qDebug()<<"dialog closed";
    delete(this);
}
