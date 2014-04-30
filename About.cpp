#include "About.h"
#include <QtWidgets>
About::About(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(300);
    this->setFixedWidth(500);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    QLabel* title = new QLabel("CometFTP 0.0.1");
    QLabel* build = new QLabel("Built on Windows using MinGW 4.8");
    QLabel* license = new QLabel("Licensed under GPL v3.0");
    QLabel* thanks = new  QLabel("Powered by Qt 5.2.0, Libssh and OpenSSL");
    QLabel* icons = new QLabel("Some Icons licensed under CC by flaticon.com. Author: Appzgear");
    QLabel* warranty = new QLabel("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.");
    warranty->setWordWrap(true);
    icons->setWordWrap(true);

    QImage qt = QImage(":/images/qt-logo.png");
    QImage libssh = QImage(":/images/libssh-logo.png");
    QImage openssl = QImage(":/images/openssl-logo.png");



    QPixmap qtMap =  QPixmap::fromImage(qt);
    QPixmap libsshMap =  QPixmap::fromImage(libssh);
    QPixmap opensslMap =   QPixmap::fromImage(openssl);

    qtMap = qtMap.scaledToWidth(50);
    libsshMap = libsshMap.scaledToWidth(100);
    opensslMap = opensslMap.scaledToWidth(100);

    QLabel* qtLbl = new QLabel();
    QLabel* libsshLbl = new QLabel();
    QLabel* opensslLbl = new QLabel();

    qtLbl->setPixmap(qtMap);
    libsshLbl->setPixmap(libsshMap);
    opensslLbl->setPixmap(opensslMap);
    qtLbl->setFixedWidth(50);
    libsshLbl->setFixedWidth(100);
    opensslLbl->setFixedWidth(100);

    leftLayout->addWidget(qtLbl);
    leftLayout->addWidget(libsshLbl);
    leftLayout->addWidget(opensslLbl);

    rightLayout->addWidget(title);
    rightLayout->addWidget(build);
    rightLayout->addWidget(license);
    rightLayout->addWidget(icons);
    rightLayout->addWidget(thanks);
    rightLayout->addWidget(warranty);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    this->setLayout(mainLayout);


}
