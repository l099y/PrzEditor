#include <QHashIterator>
#include "extendedqfilesystemmodel.h"
#include <QDebug>

ExtendedQFileSystemModel::ExtendedQFileSystemModel(QObject* parent): QFileSystemModel(parent)
{
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
        if (fInfo.suffix()=="pdf"){
            filteredSet.insert(fInfo.absolutePath());
            containsPdf = true;
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

//void ExtendedQTreeView::mouseDoubleClickEvent(QMouseEvent *e)
//{
//    QSortFilterProxyModel *filter = (QSortFilterProxyModel*)this->model();
//    auto fileinfo = filter->itemData(this->selectedIndexes().first());
//    qDebug()<<filter->data(this->selectedIndexes().first());

////    QTreeView :: mouseDoubleClickEvent(e);
////    QFileSystemModel *modl = (QFileSystemModel*)this->model();
////    if (selectedIndexes().first().isValid()){
////        auto fileInf = modl->fileInfo(this->selectedIndexes().first());
////        if (fileInf.isDir())
////        {

////            QDir currentDir (fileInf.absoluteFilePath());
////            QStringList paths = currentDir.entryList(QStringList()<<"*.pdf" << "*.PDF", QDir::Files);
////            //            qDebug()<< fileInf.absoluteFilePath();
////            //            for (int i = 0; i<paths.length()-2;i++){
////            //                qDebug()<<selectedIndexes().first().row();
////            //                qDebug()<<modl->fileInfo(selectedIndexes().first().child(i, 0)).fileName() << "filename";

////            //            }
////        }
////    }
//}

void ExtendedQFileSystemModel::generatePdflist(QModelIndex idx)
{
    auto fileInf = fileInfo(idx);
    QDir currentDir (fileInf.filePath());
    QStringList paths = currentDir.entryList(QStringList()<<"*.pdf" << "*.PDF", QDir::Files);
    QString path =currentDir.absolutePath();
    QFile file(path+"/listofPdf.pdflist");
    if (paths.size()!=0){
        if (file.open(QIODevice::ReadWrite))
        {
            qDebug()<<"file now exists";
            QTextStream stream(&file);
            foreach (QString str, paths){
                stream << str <<'\n';
            }
            file.close();
            qDebug() << "Writing finished";
            filteredSet.insert(path+"/listofPdf.pdflist");
        }
    }
}






