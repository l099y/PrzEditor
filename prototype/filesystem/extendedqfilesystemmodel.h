#ifndef EXTENDEDQFILESYSTEMMODEL_H
#define EXTENDEDQFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QObject>
#include <QtCore>
#include <filesystem/sequenceregister.h>

class ExtendedQFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    SequenceRegister* przreg;
    ExtendedQFileSystemModel(SequenceRegister* reg, QObject* parent);
    ~ExtendedQFileSystemModel();

    // this is an overrided function, it handles the way the treeview shows the model
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const;
signals:

    // those signals will triggers named action in the different elements of the mainwindow
    void displaySequences(QString path);
    void setOnlyCurrentFolderExpanded(QModelIndex);
    void clearSequences();
public slots:

    //this function parse the qlist of string
    void parseExpandedDir(QModelIndex);
};

#endif // EXTENDEDQFILESYSTEMMODEL_H
