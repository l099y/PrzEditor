#include <QHashIterator>
#include "extendedqfilesystemmodel.h"
#include <QDebug>
#include <QStandardItem>

ExtendedQFileSystemModel::ExtendedQFileSystemModel(QObject* parent): QFileSystemModel(parent)
{
    setReadOnly(false);
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
        if (fInfo.suffix()=="pdf"){
            filteredSet.insert(fInfo.absolutePath());
            containsPdf = true;
            break;
        }
    }
    if (containsPdf)
        generatePdflist(idx);
}

QVariant ExtendedQFileSystemModel::data(const QModelIndex &index,
                                        int role) const {
    if(role == Qt::TextColorRole){
        const QFileInfo& fInfo = fileInfo(index);
        if(filteredSet.contains(fInfo.filePath()))
            return QColor(Qt::blue);
    }
    else if( role == Qt::DecorationRole )
    {
        const QFileInfo& info = fileInfo(index);
        if(info.isFile() && info.suffix() == "pdflist" && index.column()==0){
            QPixmap logo(20,20);
            logo.fill(QColor(100,170,255));
            return logo;//I pick the icon depending on the extension
        }
    }
    return QFileSystemModel::data(index, role);
}


void ExtendedQFileSystemModel::generatePdflist(QModelIndex idx)
{
    auto fileInf = fileInfo(idx);
    QDir currentDir (fileInf.filePath());
    insertRow(0,idx);
    QStringList paths = currentDir.entryList(QStringList()<<"*.pdf" << "*.PDF", QDir::Files);
    QString path =currentDir.absolutePath();

    if (paths.size()!=0){

    }
}







