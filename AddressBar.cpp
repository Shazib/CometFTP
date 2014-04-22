#include "AddressBar.h"

AddressBar::AddressBar(QWidget *parent, bool _disconnectVisible, QString _defaultPath) :
    QWidget(parent)
{
    // Initial Setup
    this->setFixedHeight(24);
    disconnectVisible = _disconnectVisible;
    defaultPath = _defaultPath;
    setupView();


    QObject::connect(backBtn, SIGNAL(clicked()),this, SLOT(backButtonClick()));
    QObject::connect(forwardBtn, SIGNAL(clicked()),this,SLOT(forwardButtonClick()));
    QObject::connect(addressEdit, SIGNAL(returnPressed()), this, SLOT(addressEditClick()));
    QObject::connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(disconnectClicked()));
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

void AddressBar::disconnectClicked()
{
    emit disconnect();
}

void AddressBar::updatePath(QString path)
{
    // Add old path to back list
    backList << defaultPath;
    // Set new default path
    defaultPath = path;
    // Set Line Edit
    addressEdit->setText(path);
}

void AddressBar::backButtonClick()
{
    qDebug() << "Back Button Clicked";

    if (backList.count() >= 1){
    // Add to forward
    forwardList << defaultPath;
    // Set New Default
    defaultPath = backList.last();
    // Remove path from back
    backList.removeLast();
    //set Line edit
    addressEdit->setText(defaultPath);

    emit updatedPath(defaultPath);

    }
}

void AddressBar::forwardButtonClick()
{
    if (forwardList.count() >= 1){
    backList << defaultPath;
    defaultPath = forwardList.last();
    forwardList.removeLast();
    addressEdit->setText(defaultPath);
    emit updatedPath(defaultPath);
    }
}

void AddressBar::addressEditClick()
{
    if (disconnectVisible) {
        // SSH, send path
        /********************************/
        emit updatedPath(addressEdit->text());

    } else {
        QDir dir(addressEdit->text());
        if (!dir.exists()){
            // Does Not Exist
            qDebug() << "does not exist";
            addressEdit->setText(defaultPath);
            // Do nothing
        } else {
            qDebug() << "adding default path " << defaultPath << " to list";
           backList << defaultPath;
           qDebug() << "setting new path " <<addressEdit->text();
           defaultPath = addressEdit->text();
            // Local, Send path
           emit updatedPath(addressEdit->text());

           qDebug() << "backlist last " << backList.last();

        }
   }

}







