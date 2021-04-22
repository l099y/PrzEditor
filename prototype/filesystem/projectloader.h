#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>

class ProjectLoader : public QDialog
{
public:


    ProjectLoader(QWidget* parent = nullptr);
    ~ProjectLoader();


private :

    QVBoxLayout* layout;
    QTreeView* view;
    QFileSystemModel* model;

protected :
    void closeEvent(QCloseEvent *e) override;

};

#endif // PROJECTLOADER_H
