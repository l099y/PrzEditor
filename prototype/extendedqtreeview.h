#ifndef EXTENDEDQTREEVIEW_H
#define EXTENDEDQTREEVIEW_H

#include <QTreeView>
#include <QObject>
#include <QFileSystemModel>

class ExtendedQTreeView : public QTreeView
{
    Q_OBJECT
public:
    ExtendedQTreeView(QWidget *parent);
    ~ExtendedQTreeView();
    void mouseDoubleClickEvent(QMouseEvent *e);
   public slots :
    void function(QModelIndex idx);
};

#endif // EXTENDEDQTREEVIEW_H
