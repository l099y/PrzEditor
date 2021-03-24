#ifndef EXTENDEDQFILESYSTEMMODEL_H
#define EXTENDEDQFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QObject>
#include <QtCore>

class ExtendedQFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    ExtendedQFileSystemModel(QObject* parent);
    ~ExtendedQFileSystemModel();
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const;

public slots:
    void parseExpandedDir(QModelIndex);

private:
    QSet<QString> filteredSet;
    void generatePdflist(QModelIndex idx);
    void populateFilteredSet(int sizeLimit);
    void addAllParentalPaths(const QString& filePath);

};

#endif // EXTENDEDQFILESYSTEMMODEL_H
