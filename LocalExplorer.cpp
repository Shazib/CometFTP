/*
 * This class represents a tree view of the local file system
 * It is a single widget which encompasses an address bar and the treeview
 */

#include "LocalExplorer.h"

#include <QtWidgets>

LocalExplorer::LocalExplorer(QWidget *parent) :
    QWidget(parent)
{

    // Setup Directory
    //QDir* directory = new QDir(QDir::root());
    //mainDir = new QString(QDir::currentPath());

    // File System Model
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());

    // Create File Tree
    QTreeView *tree = new QTreeView(this);
    tree->setModel(model);
    tree->setStyle(QStyleFactory::create("Fusion"));
    tree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tree->setContentsMargins(-1,0,-1,-1);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tree);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);

}
