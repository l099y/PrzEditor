#include "extendedqtreeview.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFileSystemModel>
#include <QDir>
#include <QStringList>

ExtendedQTreeView::ExtendedQTreeView(QWidget* parent): QTreeView(parent)
{
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(function(QModelIndex)));
}

ExtendedQTreeView::~ExtendedQTreeView()
{

}

void ExtendedQTreeView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTreeView :: mouseDoubleClickEvent(e);
    QFileSystemModel *modl = (QFileSystemModel*)this->model();
    if (selectedIndexes().first().isValid()){
        auto fileInf = modl->fileInfo(this->selectedIndexes().first());
        if (fileInf.isDir())
        {

            QDir currentDir (fileInf.absoluteFilePath());
            QStringList paths = currentDir.entryList(/*QStringList()<<"*.pdf" << "*.PDF", QDir::Files*/);
//            qDebug()<< fileInf.absoluteFilePath();
//            for (int i = 0; i<paths.length()-2;i++){
//                qDebug()<<selectedIndexes().first().row();
//                qDebug()<<modl->fileInfo(selectedIndexes().first().child(i, 0)).fileName() << "filename";

//            }
        }
    }
}

void ExtendedQTreeView::function(QModelIndex idx)
{
    QFileSystemModel *modl = (QFileSystemModel*)this->model();
    auto fileInf = modl->fileInfo(idx);
    qDebug()<< fileInf.fileName();
    QDir currentDir (fileInf.absoluteFilePath());
    QStringList paths = currentDir.entryList(/*QStringList()<<"*.pdf" << "*.PDF", QDir::Files*/);
            qDebug()<< fileInf.absoluteFilePath();

                qDebug()<<modl->fileInfo(idx).fileName()<< "filename";

}


