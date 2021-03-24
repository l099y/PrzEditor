#include "extendedqtreeview.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFileSystemModel>
#include <QDir>
#include <QStringList>
#include <QSortFilterProxyModel>

ExtendedQTreeView::ExtendedQTreeView(QWidget* parent): QTreeView(parent)
{
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(function(QModelIndex)));
    this->dragEnabled();

}

ExtendedQTreeView::~ExtendedQTreeView()
{

}

void ExtendedQTreeView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QSortFilterProxyModel *filter = (QSortFilterProxyModel*)this->model();
    auto fileinfo = filter->itemData(this->selectedIndexes().first());
    qDebug()<<filter->data(this->selectedIndexes().first());

//    QTreeView :: mouseDoubleClickEvent(e);
//    QFileSystemModel *modl = (QFileSystemModel*)this->model();
//    if (selectedIndexes().first().isValid()){
//        auto fileInf = modl->fileInfo(this->selectedIndexes().first());
//        if (fileInf.isDir())
//        {

//            QDir currentDir (fileInf.absoluteFilePath());
//            QStringList paths = currentDir.entryList(QStringList()<<"*.pdf" << "*.PDF", QDir::Files);
//            //            qDebug()<< fileInf.absoluteFilePath();
//            //            for (int i = 0; i<paths.length()-2;i++){
//            //                qDebug()<<selectedIndexes().first().row();
//            //                qDebug()<<modl->fileInfo(selectedIndexes().first().child(i, 0)).fileName() << "filename";

//            //            }
//        }
//    }
}

void ExtendedQTreeView::function(QModelIndex idx)
{
    QSortFilterProxyModel *filter = (QSortFilterProxyModel*)this->model();
    qDebug()<<filter->itemData(idx);
   // auto fileInf = modl->fileInfo(idx);
//    QDir currentDir (fileInf.absoluteFilePath());
//    QStringList paths = currentDir.entryList(QStringList()<<"*.pdf" << "*.PDF", QDir::Files);
//    qDebug()<< fileInf.absoluteFilePath();
//    QString path =currentDir.absolutePath();
//    QFile file(path+"/listofPdf.pdflist");
//    if (paths.size()!=0){
//        if(!currentDir.exists(path))
//        {
//            currentDir.mkpath(path);
//        }
//        if (file.open(QIODevice::ReadWrite))
//        {
//            qDebug()<<"file now exists";
//            QTextStream stream(&file);
//            foreach (QString str, paths){

//                stream << str <<'\n';


//            }
//            file.close();
//            qDebug() << "Writing finished";
//        }
//    }
}




