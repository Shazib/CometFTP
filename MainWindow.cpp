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
    this->resize(900,500);
    move(QApplication::desktop()->availableGeometry(this).center() - rect().center());


}

MainWindow::createGuiComponents(){

    // Buttons
    btnQuit = new QPushButton("",this);
    btnMin = new QPushButton("", this);
    btnMax = new QPushButton("",this);
    btnMenu = new QPushButton("", this);
    btnMenu->setContentsMargins(QMargins(10,0,0,0));

    // Button naming
    btn->setObjectName("btn");
    btnMin->setObjectName("btnMin");
    btnMax->setObjectName("btnMax");
    btnMenu->setObjectName("btnMenu");

    // Button Sizes
    btnQuit->setMaximumSize(30,26);
    btnMax->setMinimumSize(30,26);
    btnMin->setMaximumSize(30,26);
    btnMenu->setMaximumSize(103,27);

}
MainWindow::createMainLayout(){

    // Setup Layouts
    mainLayout = new QVBoxLayout();
    topLayout = new QHBoxLayout();
    topLeftLayout = new QHBoxLayout();
    topMiddleLayout = new QHBoxLayout();
    topRightLayout = new QHBoxLayout();
    bottomLayout = new QHBoxLayout();

    // Create the Stacked Widget to handle the bottom
    bottomLeftLayout = new QHBoxLayout();
    bottomRightLayout = new QHBoxLayout();

}









MainWindow::~MainWindow()
{

}
