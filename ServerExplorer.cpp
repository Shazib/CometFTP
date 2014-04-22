/*
 * This Class shows the server-side file browser
 * as well as the connect panel and bookmarks areas
 * a sliding widget is used to change between bookmarks and connect area
 * another sliding widget is used to change between site explorer and boomark/connect
 *
 * Some styles here are required to be inline as they change.
 */
#include "ServerExplorer.h"
#include "SlidingStackedWidget.h"
#include "AddressBar.h"

#include <QtWidgets>

ServerExplorer::ServerExplorer(QWidget *parent) :
    QWidget(parent)
{
    // Initial Setup
    int _min=500;
    int _max=1500;
    animTime=(_min+_max)>>1;

    // Main Slider Setup
    mainSlider = new SlidingStackedWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout();

    // Setup Slides For Main Slider
    setupSiteManager();
    explorerSlide = new QWidget();

    // Add Slides to Main Slider
    mainSlider->addWidget(siteManager);
    mainSlider->addWidget(explorerSlide);

    // Setup this Widget with main slider
    mainLayout->addWidget(mainSlider);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);



}


// Setup site manager widget (includes a slider)
// Includes connect area and bookmarks area.
void ServerExplorer::setupSiteManager()
{
// Setup Toolbar
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    toolbar = new QFrame();
    toolbar->setFixedHeight(22);
    toolbar->setObjectName("Explorer_Toolbar");

    // Bookmark Button
    QIcon bookmarkIcon =  QIcon(":/images/Menu.svg");
    bookmarkBtn = new QPushButton("  Bookmarks ", this);
    bookmarkBtn->setStyle(QStyleFactory::create("Fusion"));
    bookmarkBtn->setIcon(bookmarkIcon);
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px; padding-bottom:4px; background-color:#cdcdcd; } ");

    // SFTP Button
    QIcon sftpIcon =  QIcon(":/images/Screen.svg");
    sftpBtn = new QPushButton("  SFTP ", this);
    sftpBtn->setStyle(QStyleFactory::create("Fusion"));
    sftpBtn->setIcon(sftpIcon);
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px; padding-bottom:4px; }" );

    toolbarLayout->addWidget(bookmarkBtn);
    toolbarLayout->addWidget(sftpBtn);
    toolbarLayout->setAlignment(Qt::AlignTop);
    toolbarLayout->setContentsMargins(0,0,0,0);
    toolbar->setLayout(toolbarLayout);



// Setup Slider with bookmarks and connect

    // Init Slider
    SlidingStackedWidget* managerSlider = new SlidingStackedWidget(this);
    managerSlider->setObjectName("Server_Slider");
    QWidget* bookmarkSlide = new QWidget();
    QWidget* connectSlide = new QWidget();

    // Setup Connect Area
    host = new QLineEdit();
    host->setPlaceholderText("Host");
    host->setFixedWidth(200);
    host->setStyle(QStyleFactory::create("Fusion"));
    user = new QLineEdit();
    user->setPlaceholderText("User");
    user->setFixedWidth(200);
    user->setStyle(QStyleFactory::create("Fusion"));
    password = new QLineEdit();
    password->setPlaceholderText("Password");
    password->setFixedWidth(200);
    password->setStyle(QStyleFactory::create("Fusion"));
    password->setEchoMode(QLineEdit::Password);
    password->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    port = new QLineEdit();
    port->setPlaceholderText("Port");
    port->setFixedWidth(200);
    port->setStyle(QStyleFactory::create("Fusion"));
    QIntValidator *val = new QIntValidator(0,99999,this);
    port->setValidator(val);
    // Setup Connect Buttons
    connectBtn = new QPushButton("Connect");
    connectBtn->setStyle(QStyleFactory::create("Fusion"));
    connectBookmarkBtn = new QPushButton("Bookmark");
    connectBookmarkBtn->setStyle(QStyleFactory::create("Fusion"));

    // Setup connect layout
    QVBoxLayout* connectLayout = new QVBoxLayout();
    QHBoxLayout* connectBtnsLayout = new QHBoxLayout();
    connectBtnsLayout->addWidget(connectBtn);
    connectBtnsLayout->addWidget(connectBookmarkBtn);
    connectLayout->addWidget(host);
    connectLayout->addWidget(user);
    connectLayout->addWidget(password);
    connectLayout->addWidget(port);
    connectLayout->addLayout(connectBtnsLayout);
    connectLayout->setAlignment(Qt::AlignCenter);
    connectLayout->setContentsMargins(0,0,0,0);

    // Setup bookmark area


    // Setup slides
    connectSlide->setLayout(connectLayout);

    // Add widgets to managerSlider
    managerSlider->addWidget(connectSlide);
    managerSlider->addWidget(bookmarkSlide);

    // Add slider to siteManager widget
    siteManager = new QWidget();
    siteManager->setObjectName("Server_Widget");
    QVBoxLayout* mainLayout = new QVBoxLayout();

    siteManager->setLayout(mainLayout);
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(managerSlider);
    mainLayout->setContentsMargins(0,0,0,0);


    // Create Connections
    QObject::connect(bookmarkBtn,SIGNAL(clicked()),this,SLOT(bookmarkBtnPressed()));
    QObject::connect(sftpBtn,SIGNAL(clicked()),this,SLOT(sftpBtnPressed()));
    QObject::connect(bookmarkBtn, SIGNAL(clicked()),managerSlider,SLOT(slideInNext()));
    QObject::connect(sftpBtn, SIGNAL(clicked()),managerSlider, SLOT(slideInPrev()));
    QObject::connect(connectBtn, SIGNAL(clicked()),this,SLOT(connectBtnPressed()));
    managerSlider->setSpeed(animTime);
    managerSlider->setWrap(false);
}

// Handling Button Presses
void ServerExplorer::bookmarkBtnPressed()
{
    // Set Styling
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px; padding-bottom:4px; background-color:#ebebeb;}");
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px;padding-bottom:4px; background-color:#cdcdcd;}");

    // Switch to bookmark slide
}

void ServerExplorer::sftpBtnPressed()
{
    // Setup Style
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px;padding-bottom:4px; background-color:#cdcdcd;}");
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px;padding-bottom:4px; background-color:#ebebeb;}");

    // Switch to sftp slide

}

void ServerExplorer::connectBtnPressed()
{
    // When connect is pressed, the login details are passed

    _host = "nova.so"; //host->text().toLocal8Bit().constData();
    _user = "user"; //user->text().toLocal8Bit().constData();
    _password = "O0h4n7hony="; //password->text().toLocal8Bit().constData();
    _port = "22"; //port->text().toLocal8Bit().constData();

    // Setup SFTPSite class connection
    site = new SFTPSite(this,_host, _user, _password, _port);
    if( !site->init()) {
        // ERROR
    } else {
        // SUCCESS
    }

    // Setup Explorer slide
    table = new QTableView(this);
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



    // Setup initial root directory view
    QPair<int,QStringList> pair = site->listDir("/");
    model = new ServerFileModel(this, pair.first, pair.second);
    table->setModel(model);

    // Switch Main Slider;
    QVBoxLayout* mainExplorerLayout = new QVBoxLayout();
    mainExplorerLayout->setContentsMargins(0,0,0,0);
    mainExplorerLayout->setSpacing(0);

    // Address Bar
    addressBar = new AddressBar(0,true,"/");
    QObject::connect(addressBar,SIGNAL(updatedPath(QString)),this,SLOT(updatedPath(QString)));

    mainExplorerLayout->addWidget(addressBar);
    mainExplorerLayout->addWidget(table);
    explorerSlide->setLayout(mainExplorerLayout);
    mainSlider->slideInNext();

    // Connect Table Click event
    QObject::connect(table,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(rowSelected(QModelIndex)));

}

void ServerExplorer::updatedPath(QString path)
{
    QPair<int,QStringList> pair = site->listDir(path);
    if (pair.first == -1){ // Dir doesn't exist
        // Reset Address Bar
        addressBar->updatePath(mainDir);
    } else {
    delete model;
    model = new ServerFileModel(this,pair.first, pair.second);
    mainDir = path;
    table->setModel(model);
    }

}

// Handle Clicks on folders in table
void ServerExplorer::rowSelected(const QModelIndex indx) {

    if (indx.model()->data(indx.model()->index(indx.row(),4,indx),Qt::DisplayRole).toString() == QString("Folder")){

        // Set Main Dir String
        mainDir += "/";
        mainDir +=indx.model()->data(indx.model()->index(indx.row(),0,indx),Qt::DisplayRole).toString();

        // Cleanup old model, set with new data.
        delete model;
        QPair<int,QStringList> pair = site->listDir(mainDir);
        model = new ServerFileModel(this,pair.first, pair.second);
        table->setModel(model);

        // Update AddressBar
        addressBar->updatePath(mainDir);


    }

}
