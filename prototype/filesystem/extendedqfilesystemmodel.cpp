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
    przreg->currentExpandedFolderSequences->clear();
    emit (clearSequences());
    auto fileInf = fileInfo(idx);
    QDir currentDir (fileInf.filePath());
    QDirIterator it(fileInfo(idx).filePath());
    bool containsPdf = false;
    bool containsTbe = false;
    while (it.hasNext()) {
        it.next();
        const QFileInfo& fInfo = it.fileInfo();
        if (fInfo.suffix()=="prz")
            containsPdf = true;
        else if(fInfo.suffix()=="tbe")
        {
            containsTbe = true;
            qDebug()<<"contains tbe";
        }
    }
        if (containsPdf){
        przreg->GenerateSequencesFromDir(&currentDir);
        }
        if (containsTbe)
        {
        przreg->GeneratesTbeFilesFromDir(&currentDir);
        }
    emit displaySequences(currentDir.path());
    emit setOnlyCurrentFolderExpanded(idx);
}

QVariant ExtendedQFileSystemModel::data(const QModelIndex &index,
                                        int role) const {
    const QFileInfo& fInfo = fileInfo(index);
    if(role == Qt::TextColorRole){
        if(przreg->currentExpandedFolderSequences->contains(fInfo.filePath()))
            return QColor(100,200,255);
    }
    else if( role == Qt::DecorationRole )
    {
        if(przreg->currentExpandedFolderSequences->contains(fInfo.filePath())){
            QPixmap logo(15,15);
            logo.fill(QColor(100,200,255));
            return logo;
        }
    }
    return QFileSystemModel::data(index, role);
}






