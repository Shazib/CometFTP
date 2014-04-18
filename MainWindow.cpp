/*
 * This is the main class for the application
 * It contains all other widgets and manages them
 */

#include "MainWindow.h"
#include "QDebug"

MainWindow::MainWindow(QFrame *parent)
    : QFrame(parent)
{

    qDebug() << "Creating Main Window";
    // Initial Setup
    max = false;
    this->resize(900,500);
    move(QApplication::desktop()->availableGeometry(this).center() - rect().center());
    int _min=500;
    int _max=1500;
    animTime=(_min+_max)>>1;

    // Load GUI
    qDebug() << "About to load gui";

    createGuiComponents();
    qDebug() << "GUI Components Created";
    qDebug() << "About to create main Layout";
    createMainLayout();
    qDebug() << "Finished with layout";


}
// Setup main GUI components
void MainWindow::createGuiComponents(){

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
void MainWindow::createMainLayout(){

    qDebug() << "Main Layout Started";
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
    statusArea = new StatusArea();
    topMiddleLayout->addWidget(statusArea);
    topMiddleLayout->setContentsMargins(4,4,4,0);

    topLayout->addLayout(topLeftLayout);
    topLayout->addLayout(topMiddleLayout);
    topLayout->addLayout(topRightLayout);
    topLayout->setMargin(0);

    qDebug() << "All top layout done";

// Create Bottom Layout
    // Layouts
    bottomLeftLayout = new QHBoxLayout();
    bottomRightLayout = new QHBoxLayout();
    bottomLayout = new QHBoxLayout();

    qDebug() << "Layouts done";
    // Setup Required Widgets
    localExplorer = new LocalExplorer();
    qDebug() << "Local explorer done";
    serverExplorer = new ServerExplorer();

    qDebug() << "widgets Created";
    bottomLeftLayout->addWidget(localExplorer);
    bottomRightLayout->addWidget(serverExplorer);
    qDebug() << "Widgets Done";
    // Proxy Widgets for Slider
    qDebug() << "Proxy widgets done";
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
    QWidget* temp = new QWidget();


    qDebug() << "All Bottom Widgets Done";

    // Create Bottom Stacked Slider Widget
    mainContent = new SlidingStackedWidget(this);
    mainContent->addWidget(explorerSlider);
    mainContent->addWidget(temp);
    mainContent->setSpeed(animTime);
    qDebug() <<"Slider setup complete";
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(topLayout,0);
    mainLayout->addWidget(mainContent,0,0);
    this->setLayout(mainLayout);
}



MainWindow::~MainWindow()
{

}
