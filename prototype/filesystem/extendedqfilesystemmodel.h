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

public slots:
    void parseExpandedDir(QModelIndex);

private:
    QSet<QString> filteredSet;
    void generatePrzlist(QModelIndex idx);
};

#endif // EXTENDEDQFILESYSTEMMODEL_H
