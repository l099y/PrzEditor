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
    bool containsPrz = false;
    bool containsTbe = false;
    while (it.hasNext()) {
        it.next();
        const QFileInfo& fInfo = it.fileInfo();
        if (fInfo.suffix()=="prz")
            containsPrz = true;
        else if(fInfo.suffix()=="tbe")
        {
            containsTbe = true;
        }
    }
        if (containsPrz){
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
    return QFileSystemModel::data(index, role);
}






