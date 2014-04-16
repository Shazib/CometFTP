/*
 * This Class shows the server-side file browser
 * as well as the connect panel and bookmarks areas
 * a sliding widget is used to change between bookmarks and connect area
 * another sliding widget is used to change between site explorer and boomark/connect
 */
#include "ServerExplorer.h"
#include "SlidingStackedWidget.h"
#include <QtWidgets>

ServerExplorer::ServerExplorer(QWidget *parent) :
    QWidget(parent)
{
    // Main Slider Setup
    SlidingStackedWidget* mainSlider = new SlidingStackedWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout();

    // Setup Slides For Main Slider
    QWidget* mainExplorerSlide = new QWidget();
    QWidget* mainManagerSlide = new QWidget();
    setupSiteManager();
    QHBoxLayout* siteManagerLayout = new QHBoxLayout();
    siteManagerLayout->addWidget(siteManager);
    mainManagerSlide->setLayout(siteManagerLayout);

    // Add Slides to Main Slider
    mainSlider->addWidget(mainExplorerSlide);
    mainSlider->addWidget(mainManagerSlide);

    // Setup this Widget with main slider
    mainSlider->addWidget(mainManagerSlide);
    mainSlider->addWidget(mainExplorerSlide);
    mainLayout->addWidget(mainSlider);
    this->setLayout(mainLayout);

    // Connect Button click slots
    QObject::connect(bookmarkBtn,SIGNAL(clicked()),this,SLOT(bookmarkBtnPressed()));
    QObject::connect(sftpBtn,SIGNAL(clicked()),this,SLOT(sftpBtnPressed()));
}


// Setup site manager widget (includes a slider)
void ServerExplorer::setupSiteManager()
{
// Setup Toolbar
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    QFrame* toolbar = new QFrame();
    toolbar->setFixedHeight(22);

    // Bookmark Button
    QIcon bookmarkIcon =  QIcon(":/images/Menu.svg");
    bookmarkBtn = new QPushButton("  Bookmarks ", this);
    bookmarkBtn->setStyle(QStyleFactory::create("Fusion"));
    bookmarkBtn->setIcon(bookmarkIcon);
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px; padding-bottom:4px; } ");

    // SFTP Button
    QIcon sftpIcon =  QIcon(":/images/Screen.svg");
    sftpBtn = new QPushButton("  SFTP ", this);
    sftpBtn->setStyle(QStyleFactory::create("Fusion"));
    sftpBtn->setIcon(sftpIcon);
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px; background-color:#ebebeb; padding-bottom:4px; }" );

    toolbarLayout->addWidget(bookmarkBtn);
    toolbarLayout->addWidget(sftpBtn);
    toolbarLayout->setAlignment(Qt::AlignTop);
    toolbarLayout->setContentsMargins(0,0,0,0);
    toolbar->setLayout(toolbarLayout);

// Setup Slider with bookmarks and connect

    // Init Slider
    SlidingStackedWidget* managerSlider = new SlidingStackedWidget();
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

    // Setup bookmark area


    // Setup slides
    connectSlide->setLayout(connectLayout);

    // Add widgets to managerSlider
    managerSlider->addWidget(connectSlide);
    managerSlider->addWidget(bookmarkSlide);

    // Add slider to siteManager widget
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(managerSlider);
    siteManager->setLayout(mainLayout);

}

/*
 * Handling Button Presses
 */

void SiteExplorer::bookmarkBtnPressed()
{
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px; padding-bottom:4px; background-color:#ebebeb;}");
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px;padding-bottom:4px; background-color:none;}");
}

void SiteExplorer::sftpBtnPressed()
{
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px;padding-bottom:4px; background-color:none;}");
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px;padding-bottom:4px; background-color:#ebebeb;}");

}


void SiteExplorer::connectBtnPressed()
{
    // When connect is pressed, the login details are passed to the main widget.
    _host = host->text().toLocal8Bit().constData();
    _user = user->text().toLocal8Bit().constData();
    _password = password->text().toLocal8Bit().constData();
    _port = port->text().toLocal8Bit().constData();

}
