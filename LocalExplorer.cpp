/*
 * This class represents a tree view of the local file system
 * It is a single widget which encompasses an address bar and the treeview
 *
 * A table is setup with a model of the local file system. Directory changes are handled by
 * implementing the doubleclick event on a row. There is also an addressBar object,
 * of the AddressBar class
 * This object communicates with the table+model to point out back/forward presses, and
 * address edit area updates.
 *
 * The class always stores a string of the current directory to be used when communicating
 * to the address bar
 */

#include "LocalExplorer.h"

#include <QtWidgets>

LocalExplorer::LocalExplorer(QWidget* parent) :
    QWidget(parent)
{

    // Setup Directory
    // File System Model
    model = new CustomFileModel();
    model->setReadOnly(false);
    model->setRootPath(QDir::rootPath());
    QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString,QString)));

    // Setup The View
    table = new QTableView();
    //table->setStyle(QStyleFactory::create("Fusion"));
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setVisible(true);
    table->horizontalHeader()->show();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);    
    table->setObjectName("ServerTableView");
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(18);
    table->resizeColumnsToContents();
    QHeaderView* header = table->horizontalHeader();
    header->setResizeContentsPrecision(99);
    table->setHorizontalHeader(header);

    // Enabling Drag Drop on Table
    table->setDragDropMode(QAbstractItemView::DragDrop);
    table->setSortingEnabled(true);

    // Setup Address Bar
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

// This method checks if what was entered into the addresbar is valid
// If it is valid then this is submitted to the table
// If it is not valid, the existing (valid) path is reset on the address bar text edit
void LocalExplorer::updatedPath(QString path)
{
   // QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString)));

    QDir* dir = new QDir(path);
    if (dir->exists()){
        table->setRootIndex(model->setRootPath(path));
        table->resizeColumnsToContents();
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

   // QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString)));
    // Get File Info
    QFileInfo info = model->fileInfo(indx);
    // Check if folder
    if (info.isDir()){
        // Reset Model
        table->setRootIndex(model->setRootPath(info.filePath()));
        table->resizeColumnsToContents();
        // Update AddressBar
        addressBar->updatePath(info.filePath());
        mainDir = info.filePath();

    }

}
void LocalExplorer::receiveDropData(QString type, QString source, QString destination,QString sftpType)
{
    // Send Data To Parent
    qDebug() << "Data recieved";
    emit sendDropData(type,source,destination,sftpType);
}

