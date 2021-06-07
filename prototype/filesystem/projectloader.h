#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QFileDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QLineEdit>

// this class was intended to be used as dialogsaver and loader, it is still used for small tasks but has to be removed

class ProjectLoader : public QFileDialog
{
Q_OBJECT
public:


    ProjectLoader(bool, QString, QWidget* parent = nullptr);
    static void notifyFailure(QString, QString);
    ~ProjectLoader();
public slots:
    // those functions are called on clicking concerned buttons, they call concerned validation functions. if the validation is fullfilled, they callback the mainwindow to execute
    // the save or loard function
    void attemptSaving();
    void attemptLoading();
    void setEnableStateByTree();
    void setEnableStateByTextInput();
    static bool confirm(QString, QString);

signals:
        void saveRequest(QString);
        void loadRequest(QString);

private :
    bool savingMod;

    // validate the possibility to save or load based on the QlineInput and the

    bool validateSave();
    bool validateLoad();
    inline bool stringIsPrzsqsType(QString);
    inline QString nameToFormat(QString);
    inline QString joinPathAndName(QString, QString);
    bool fileByAbsoluteFilePathExists(QString);

protected :
    void closeEvent(QCloseEvent *e) override;

};

#endif // PROJECTLOADER_H
