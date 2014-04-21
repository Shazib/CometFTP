/*
 * This class represents a tree view of the local file system
 * It is a single widget which encompasses an address bar and the treeview
 */

#include "LocalExplorer.h"
#include "AddressBar.h"

#include <QtWidgets>

LocalExplorer::LocalExplorer(QWidget* parent) :
    QWidget(parent)
{

    // Setup Directory
    //QDir* directory = new QDir(QDir::root());
    //mainDir = new QString(QDir::currentPath());

    // File System Model
    QFileSystemModel* model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());

    // Test File System
    QFileSystemModel* model2 = new QFileSystemModel;
    model2->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    model2->setRootPath("C:/");

    QTableView* table = new QTableView();
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
    AddressBar* addressBar = new AddressBar(0,false,QString::fromStdString( QDir::rootPath().toStdString()));

    // Layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(addressBar);
    layout->addWidget(table);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);

}
