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
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const;
signals:
    void displaySequences(QString path);
    void setOnlyCurrentFolderExpanded(QModelIndex);
public slots:
    void parseExpandedDir(QModelIndex);
};

#endif // EXTENDEDQFILESYSTEMMODEL_H
