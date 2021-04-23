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
    void attemptSaving();
    void attemptLoading();

private :
    bool creationmod;
    QPushButton* actionButton;
    QPushButton* cancelButton;
    QLineEdit* nameInput;
    QVBoxLayout* layout;
    QTreeView* view;
    QFileSystemModel* model;

protected :
    void closeEvent(QCloseEvent *e) override;

};

#endif // PROJECTLOADER_H
