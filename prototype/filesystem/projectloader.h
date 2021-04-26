#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QLineEdit>

class ProjectLoader : public QDialog
{
Q_OBJECT
public:


    ProjectLoader(bool, QWidget* parent = nullptr);
    ~ProjectLoader();

public slots:

    // those functions are called on clicking concerned buttons, they call concerned validation functions. if the validation is fullfilled, they callback the mainwindow to execute
    // the save or loard function

    void attemptSaving();
    void attemptLoading();

    void setEnableStateByTree();
    void setEnableStateByTextInput();

private :
    bool savingMod;
    QPushButton* actionButton;
    QPushButton* cancelButton;
    QLineEdit* nameInput;
    QVBoxLayout* layout;
    QTreeView* view;
    QFileSystemModel* model;

    // validate the possibility to save or load based on the QlineInput and the

    bool validateSave();
    bool validateLoad();
    bool stringIsPrzsqsType(QString);



protected :
    void closeEvent(QCloseEvent *e) override;

};

#endif // PROJECTLOADER_H
