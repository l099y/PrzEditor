#include "projectloader.h"
#include "QDebug"
#include <QMessageBox>

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
    auto currentSelection = model->fileInfo(view->currentIndex());
    if (validateSave()){
        if (fileByAbsoluteFilePathExists(nameInput->text()))
            emit (saveRequest(nameInput->text()));
        else if (currentSelection.isFile() && currentSelection.suffix() == "przsqs")
        {
            emit(saveRequest(currentSelection.absoluteFilePath()));
        }
        else
            emit (saveRequest(joinPathAndName(currentSelection.absoluteFilePath(), nameToFormat(nameInput->text()))));
        this->close();
    }
}

void ProjectLoader::attemptLoading()
{
    auto currentSelection = model->fileInfo(view->currentIndex());
    if (validateLoad()){
        if (fileByAbsoluteFilePathExists(nameInput->text()))
            emit (loadRequest(nameInput->text()));
        else if (currentSelection.isFile() && currentSelection.suffix() == "przsqs")
        {
            emit(loadRequest(currentSelection.absoluteFilePath()));
        }
        else
            emit (loadRequest(joinPathAndName(currentSelection.absoluteFilePath(), nameToFormat(nameInput->text()))));
        this->close();
    }
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
            if (currentdir.entryList().contains(nameToFormat(nameInput->text()))){
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
    auto currentSelection = model->fileInfo(view->currentIndex());
    if (currentSelection.isDir() && !currentSelection.isRoot() && nameInput->text().length() != 0){
        QDir currentdir (currentSelection.absoluteFilePath());

        if (!currentdir.entryList().contains(nameToFormat(nameInput->text()))){
            return true;
        }
        else{      
            return confirm("Do you want to replace this file", joinPathAndName(currentSelection.absolutePath(), nameToFormat(nameInput->text())));
        }
    }
    else if (currentSelection.isFile() && currentSelection.suffix()=="przsqs"){
         return confirm("Do you want to replace this file", joinPathAndName(currentSelection.absolutePath(), nameToFormat(nameInput->text())));
    }
    else return false;
}

bool ProjectLoader::validateLoad()
{
    auto currentSelection = model->fileInfo(view->currentIndex());

    if (currentSelection.isDir() && !currentSelection.isRoot() && nameInput->text().length() != 0){
        QDir currentdir (currentSelection.absoluteFilePath());

        if (currentdir.entryList().contains(nameToFormat(nameInput->text())) || fileByAbsoluteFilePathExists(nameInput->text())){
            return true;
        }
        else{
            notifyFailure("Specified file doesnt exist", "sorry");
            return false;
        }
    }
    else if (currentSelection.isFile() && currentSelection.suffix()=="przsqs"){
         return true;
    }
    else return false;
}

inline bool ProjectLoader::stringIsPrzsqsType(QString input)
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

QString ProjectLoader::nameToFormat(QString input)
{
    return  input.append(stringIsPrzsqsType(input)? "": ".przsqs");
}

QString ProjectLoader::joinPathAndName(QString path, QString name)
{
    return path.append("/").append(name);
}

bool ProjectLoader::fileByAbsoluteFilePathExists(QString absoluteFilePath)
{
    QFile test(absoluteFilePath);
    return test.exists();
}

bool ProjectLoader::confirm(QString text, QString title)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
      return true;
    }else {
        qDebug()<<"no pressed";
      return false;
    }
}

void ProjectLoader::notifyFailure(QString text , QString title)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.exec();
}

void ProjectLoader::closeEvent(QCloseEvent *e)
{
    qDebug()<<"dialog closed";
    delete(this);
}
