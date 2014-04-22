/*
 * This class represents a tree view of the local file system
 * It is a single widget which encompasses an address bar and the treeview
 */

#include "LocalExplorer.h"

#include <QtWidgets>

LocalExplorer::LocalExplorer(QWidget* parent) :
    QWidget(parent)
{

    // Setup Directory
    // File System Model
    model = new QFileSystemModel();
    model->setRootPath(QDir::rootPath());

    table = new QTableView();
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setVisible(true);
    table->horizontalHeader()->show();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);
    table->setStyle(QStyleFactory::create("Fusion"));
    table->setObjectName("ServerTableView");
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(18);

    // Address Bar
    addressBar = new AddressBar(0,false,QString::fromStdString( QDir::rootPath().toStdString()));
    QObject::connect(addressBar, SIGNAL(updatedPath(QString)), SLOT(updatedPath(QString)));

    // Layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(addressBar);
    layout->addWidget(table);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);

    // Connect Table Click event
    QObject::connect(table,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(rowSelected(QModelIndex)));


}

void LocalExplorer::updatedPath(QString path)
{
    QDir* dir = new QDir(path);
    if (dir->exists()){
        table->setRootIndex(model->setRootPath(path));
        mainDir = path;
        qDebug() << path;
    }
    else{
        addressBar->updatePath(mainDir);
    }

}
// Row Selected update Path
// Handle Clicks on folders in table
void LocalExplorer::rowSelected(const QModelIndex indx) {

    // Get File Info
    QFileInfo info = model->fileInfo(indx);
    // Check if folder
    if (info.isDir()){
        // Reset Model
        table->setRootIndex(model->setRootPath(info.filePath()));
        // Update AddressBar
        addressBar->updatePath(info.filePath());
        mainDir = info.filePath();

    }

}
