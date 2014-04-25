/*
 * This is the main class for the application
 * It contains all other widgets and manages them
 */

#include "MainWindow.h"


MainWindow::MainWindow(QFrame *parent)
    : QFrame(parent)
{
    // Initial Setup
    max = false;
    switched = false;
    this->resize(900,500);
    move(QApplication::desktop()->availableGeometry(this).center() - rect().center());
    int _min=500;
    int _max=1500;
    animTime=(_min+_max)>>1;

    // Load GUI
    createGuiComponents();
    createMainLayout();

    // Load Slots
    QObject::connect(btnQuit, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(btnMax, SIGNAL(clicked()), this, SLOT(maxSize()));
    QObject::connect(btnMin, SIGNAL(clicked()), this, SLOT(showMinimized()));



}
// Setup main GUI components
void MainWindow::createGuiComponents()
{

    // Taskbar Buttons
    btnQuit = new QPushButton("",this);
    btnMin = new QPushButton("", this);
    btnMax = new QPushButton("",this);
    btnMenu = new QPushButton("", this);
    btnMenu->setContentsMargins(QMargins(10,0,0,0));

    // Button Naming
    btnQuit->setObjectName("btnQuit");
    btnMin->setObjectName("btnMin");
    btnMax->setObjectName("btnMax");
    btnMenu->setObjectName("btnMenu");

    // Button Sizes
    btnQuit->setMaximumSize(30,26);
    btnMax->setMinimumSize(30,26);
    btnMin->setMaximumSize(30,26);
    btnMenu->setMaximumSize(103,27);

}

// Create the main layout
void MainWindow::createMainLayout()
{

    // Setup Layouts
    mainLayout = new QVBoxLayout();
    topLayout = new QHBoxLayout();
    topLeftLayout = new QHBoxLayout();
    topMiddleLayout = new QHBoxLayout();
    topRightLayout = new QHBoxLayout();


// Create Top Layout
    // Adding Buttons to Layout
    topRightLayout->addWidget(btnMin);
    topRightLayout->addWidget(btnMax);
    topRightLayout->addWidget(btnQuit);
    topLeftLayout->addWidget(btnMenu);

    // Layout spacing
    topRightLayout->setContentsMargins(QMargins(0,0,0,0));
    topRightLayout->setSpacing(0);
    topRightLayout->setAlignment(Qt::AlignRight|Qt::AlignTop);
    topLeftLayout->setContentsMargins(QMargins(10,0,0,0));
    topLeftLayout->setSpacing(0);
    topLeftLayout->setAlignment(Qt::AlignTop);
    topLayout->setSpacing(0);
    topLayout->setAlignment(Qt::AlignTop);
    topLayout->setContentsMargins(QMargins(-1,-1,-1,0));

    // Setup Status Area
    statusArea = new StatusArea(this);
    topMiddleLayout->addWidget(statusArea);
    topMiddleLayout->setContentsMargins(4,4,4,0);

    topLayout->addLayout(topLeftLayout);
    topLayout->addLayout(topMiddleLayout);
    topLayout->addLayout(topRightLayout);
    topLayout->setMargin(0);


// Create Bottom Layout
    // Layouts
    bottomLeftLayout = new QHBoxLayout();
    bottomRightLayout = new QHBoxLayout();
    bottomLayout = new QHBoxLayout();

    // Setup Required Widgets
    localExplorer = new LocalExplorer();
    serverExplorer = new ServerExplorer();
    QObject::connect(serverExplorer,SIGNAL(sendCredentials(std::string,std::string,std::string,std::string)),this,SLOT(recieveCredentials(std::string,std::string,std::string,std::string)));
    QObject::connect(serverExplorer,SIGNAL(sendDropData(QString,QString,QString,QString)),SLOT(recieveDropData(QString,QString,QString,QString)));
    QObject::connect(localExplorer,SIGNAL(sendDropData(QString,QString,QString,QString)),SLOT(recieveDropData(QString,QString,QString,QString)));

    bottomLeftLayout->addWidget(localExplorer);
    bottomRightLayout->addWidget(serverExplorer);
    // Proxy Widgets for Slider
    QWidget* localExplorerWidget = new QWidget();
    QWidget* serverExplorerWidget = new QWidget();
    localExplorerWidget->setLayout(bottomLeftLayout);
    serverExplorerWidget->setLayout(bottomRightLayout);

    // Main Slider Widget 1
    bottomLayout->addWidget(localExplorerWidget);
    bottomLayout->addWidget(serverExplorerWidget);
    bottomLayout->setContentsMargins(4,0,4,4);
    QWidget* explorerSlider = new QWidget();
    explorerSlider->setLayout(bottomLayout);

    // Empty Frame to Test
    //QWidget* temp = new QWidget();
    downloadManager = new DownloadManager(this);
    QObject::connect(this,SIGNAL(sendCredentials(std::string,std::string,std::string,std::string)),downloadManager,SLOT(receiveCredentials(std::string,std::string,std::string,std::string)));
    // Create Bottom Stacked Slider Widget
    mainContent = new SlidingStackedWidget(this);
    mainContent->addWidget(explorerSlider);
    mainContent->addWidget(downloadManager);
    mainContent->setSpeed(animTime);
    mainContent->setVerticalMode(true);
    mainContent->setAnimation(QEasingCurve::OutQuart);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(topLayout,0);
    mainLayout->addWidget(mainContent,0,0);
    this->setLayout(mainLayout);
}

void MainWindow::maxSize()
{

    if(!max) {

        int a = QApplication::desktop()->screenNumber(this);
        resize(QApplication::desktop()->availableGeometry(a).size());
        move(QApplication::desktop()->availableGeometry(a).topLeft());
        max = true;

    } else if (max){
        resize(900,500);
        move(QApplication::desktop()->availableGeometry(this).center() - rect().center());
        max = false;
    }

}

// Slots for status area
void MainWindow::switchSlides()
{

    if( switched == false){

       mainContent->slideInIdx(1,SlidingStackedWidget::BOTTOM2TOP);

       switched = true;
    } else if (switched == true) {

        mainContent->slideInIdx(0,SlidingStackedWidget::BOTTOM2TOP);

        switched = false;
    }


}



void MainWindow::recieveCredentials(std::string host, std::string user, std::string password, std::string port)
{
    qDebug() << "Credentials Recieved";
    emit sendCredentials(host,user, password,port);

}
void MainWindow::recieveDropData(QString type, QString source, QString destination, QString sftpType)
{
    // Send to downloader
    downloadManager->addData(type,source,destination,sftpType);

}

MainWindow::~MainWindow()
{

}
