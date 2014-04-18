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
#include <QtWidgets>
#include <QDebug>

ServerExplorer::ServerExplorer(QWidget *parent) :
    QWidget(parent)
{
    qDebug() << "Starting server explorer";
    // Initial Setup
    int _min=500;
    int _max=1500;
    animTime=(_min+_max)>>1;

    // Main Slider Setup
    SlidingStackedWidget* mainSlider = new SlidingStackedWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout();

    // Setup Slides For Main Slider
    QWidget* mainExplorerSlide = new QWidget(); // Server File Browser
    QWidget* mainManagerSlide = new QWidget(); // Connect + Bookmarks

    qDebug() << "Starting site manager";
    setupSiteManager();
    qDebug() << "Finishing site manager ";

    // Add Slides to Main Slider
    mainSlider->addWidget(siteManager);
    mainSlider->addWidget(mainManagerSlide);

    // Setup this Widget with main slider
    mainSlider->addWidget(mainManagerSlide);
    mainSlider->addWidget(mainExplorerSlide);
    //mainLayout->addWidget(toolbar);
    mainLayout->addWidget(mainSlider);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    qDebug() << "connecting slots";

    // Connect Button click slots
    QObject::connect(bookmarkBtn,SIGNAL(clicked()),this,SLOT(bookmarkBtnPressed()));
    QObject::connect(sftpBtn,SIGNAL(clicked()),this,SLOT(sftpBtnPressed()));
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

    qDebug() << "toolbar setup done";

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

    qDebug() << "Connect Area Done";
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
    QObject::connect(bookmarkBtn, SIGNAL(clicked()),managerSlider,SLOT(slideInNext()));
    QObject::connect(sftpBtn, SIGNAL(clicked()),managerSlider, SLOT(slideInPrev()));
    managerSlider->setSpeed(animTime);
    managerSlider->setWrap(false);
}

/*
 * Handling Button Presses
 */
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
    // When connect is pressed, the login details are passed to the main widget.
    _host = host->text().toLocal8Bit().constData();
    _user = user->text().toLocal8Bit().constData();
    _password = password->text().toLocal8Bit().constData();
    _port = port->text().toLocal8Bit().constData();

    // Init explorer

    // Switch to explorer

}
