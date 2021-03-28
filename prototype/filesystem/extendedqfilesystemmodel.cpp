#include <QHashIterator>
#include "extendedqfilesystemmodel.h"
#include <QDebug>
#include <QStandardItem>

ExtendedQFileSystemModel::ExtendedQFileSystemModel(SequenceRegister* reg, QObject* parent): QFileSystemModel(parent)
{
    przreg = reg;
    setReadOnly(true);
}

ExtendedQFileSystemModel::~ExtendedQFileSystemModel()
{

}

void ExtendedQFileSystemModel::parseExpandedDir(QModelIndex idx){
    QDirIterator it(fileInfo(idx).filePath());
    bool containsPdf = false;
    while (it.hasNext()) {
        it.next();
        const QFileInfo& fInfo = it.fileInfo();
        if (fInfo.suffix()=="prz"){
            filteredSet.insert(fInfo.absolutePath());
            containsPdf = true;
            break;
        }
    }
    if (containsPdf)
        generatePrzlist(idx);
}

QVariant ExtendedQFileSystemModel::data(const QModelIndex &index,
                                        int role) const {
    const QFileInfo& fInfo = fileInfo(index);
    if(role == Qt::TextColorRole){
        if(filteredSet.contains(fInfo.filePath()))
            return QColor(100,200,255);
    }
    else if( role == Qt::DecorationRole )
    {
        if(filteredSet.contains(fInfo.filePath())){
            QPixmap logo(15,15);
            logo.fill(QColor(100,200,255));
            return logo;
        }
    }
    return QFileSystemModel::data(index, role);
}


void ExtendedQFileSystemModel::generatePrzlist(QModelIndex idx)
{
    auto fileInf = fileInfo(idx);
    QDir currentDir (fileInf.filePath());
    auto test = przreg->GenerateSequencesFromDir(&currentDir);
    foreach (SequenceData* current, test){
        qDebug()<< current->name <<" - "<<current->files;

    }

}







