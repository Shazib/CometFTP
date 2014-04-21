#include "AddressBar.h"

AddressBar::AddressBar(QWidget *parent, bool _disconnectVisible, QString _defaultPath) :
    QWidget(parent)
{
    // Initial Setup
    this->setFixedHeight(24);
    disconnectVisible = _disconnectVisible;
    defaultPath = _defaultPath;
    setupView();

}

void AddressBar::setupView()
{
    // Setup Widget Views
    QIcon backIcon =  QIcon(":/images/left37.svg");
    backBtn = new QPushButton("");
    backBtn->setIcon(backIcon);

    QIcon forwardIcon = QIcon(":/images/right33.svg");
    forwardBtn = new QPushButton("");
    forwardBtn->setIcon(forwardIcon);

    QIcon disconnectIcon = QIcon(":/images/power26.svg");
    disconnectBtn = new QPushButton("");
    disconnectBtn->setIcon(disconnectIcon);
    disconnectBtn->setToolTip("Disconnect From Site");

    addressEdit = new QLineEdit();
    addressEdit->setStyle(QStyleFactory::create("Fusion"));
    addressEdit->setText(defaultPath);

    // Layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(backBtn);
    mainLayout->addWidget(forwardBtn);
    mainLayout->addWidget(addressEdit);
    if (disconnectVisible){
    mainLayout->addWidget(disconnectBtn);
    }
    this->setLayout(mainLayout);


}
