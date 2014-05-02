/*
 * This Class shows the server-side file browser
 * as well as the connect panel and bookmarks areas
 * a sliding widget is used to change between bookmarks and connect area
 * another sliding widget is used to change between site explorer and boomark/connect
 *
 * Some styles here are required to be inline as they change.
 */
#include "ServerExplorer.h"
#include <openssl/aes.h>


ServerExplorer::ServerExplorer(QWidget *parent) :
    QWidget(parent)
{
    // Initial Setup
    int _min=500;
    int _max=1500;
    animTime=(_min+_max)>>1;
    mainDir = "/";

    QCoreApplication::setOrganizationName("BCU");
    QCoreApplication::setApplicationName("CometFTP");
    QCoreApplication::setApplicationVersion("0.0.1");

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


    // Load Bookmarks
    QSettings settings;
    QVariant a = settings.value("bookmarks");
    bookmarks = settings.value("bookmark").toStringList();


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
    // Get bookmarks

    QHBoxLayout* mainBookmarkLayout = new QHBoxLayout();
    QHBoxLayout* leftBookmarkLayout = new QHBoxLayout();
    QVBoxLayout* rightBookmarkLayout = new QVBoxLayout();

    serverLbl = new QLabel();
    userLbl = new QLabel();
    bookmarkConnect = new QPushButton();
    bookmarkConnect->setText("Connect");
    bookmarkTable = new QListWidget();
    bookmarkTable->setFixedWidth(100);
    rightBookmarkLayout->addWidget(serverLbl);
    rightBookmarkLayout->addWidget(userLbl);
    rightBookmarkLayout->addWidget(bookmarkConnect);
    leftBookmarkLayout->addWidget(bookmarkTable);
    rightBookmarkLayout->setAlignment(Qt::AlignCenter);

    setBookmarks();
    setTableData();

    mainBookmarkLayout->addLayout(leftBookmarkLayout);
    mainBookmarkLayout->addLayout(rightBookmarkLayout);
    bookmarkSlide->setLayout(mainBookmarkLayout);
    QObject::connect(bookmarkTable,SIGNAL(currentRowChanged(int)),this,SLOT(updateBookmarkView(int)));

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
    QObject::connect(connectBookmarkBtn, SIGNAL(clicked()),this,SLOT(addBookmark()));


    managerSlider->setSpeed(animTime);
    managerSlider->setWrap(false);
}

// Handling Button Presses
void ServerExplorer::bookmarkBtnPressed()
{
    // Set Styling
    bookmarkBtn->setStyleSheet("QPushButton{border-style:none; text-align:center;padding-top:2px; padding-bottom:4px; background-color:#ebebeb;}");
    sftpBtn->setStyleSheet("QPushButton{border-style:none; text-align:center; padding-top:2px;padding-bottom:4px; background-color:#cdcdcd;}");



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

    _host = host->text().toLocal8Bit().constData();
    _user = user->text().toLocal8Bit().constData();
    _password = password->text().toLocal8Bit().constData();
    _port = port->text().toLocal8Bit().constData();

    emit sendCredentials(_host, _user, _password, _port);

    // Setup SFTPSite class connection
    site = new SFTPSite(this,_host, _user, _password, _port);
    if( !site->init()) {
        // ERROR
    } else {
        // SUCCESS


    // Setup Explorer slide
    table = new QTableView(this);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setVisible(true);
    table->horizontalHeader()->show();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);
    //table->setStyle(QStyleFactory::create("Fusion"));
    table->setObjectName("ServerTableView");
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(18);


    // Testing Drag Drop

    //table->setDragDropMode(QAbstractItemView::DragDrop);
    table->setDragEnabled(true);
    table->viewport()->setAcceptDrops(true);
    table->setDropIndicatorShown(true);


    // table->setSortingEnabled(true);

    // Setup initial root directory view
    QPair<int,QStringList> pair = site->listDir("/");
    model = new ServerFileModel(this, pair.first, pair.second, "/");

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
    QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString,QString)));
    }

}

void ServerExplorer::updatedPath(QString path)
{
    QPair<int,QStringList> pair = site->listDir(path);
    if (pair.first == -1){ // Dir doesn't exist
        // Reset Address Bar
        addressBar->updatePath(mainDir);
    } else {
    delete model;
    model = new ServerFileModel(this,pair.first, pair.second, path);
    QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString,QString)));
    mainDir = path;
    table->setModel(model);
    }

}

// Handle Clicks on folders in table
void ServerExplorer::rowSelected(const QModelIndex indx) {

    if (indx.model()->data(indx.model()->index(indx.row(),4,indx),Qt::DisplayRole).toString() == QString("Folder")){

        // Set Main Dir String

        mainDir +=indx.model()->data(indx.model()->index(indx.row(),0,indx),Qt::DisplayRole).toString();
        mainDir += "/";
        // Cleanup old model, set with new data.
        delete model;
        QPair<int,QStringList> pair = site->listDir(mainDir);
        model = new ServerFileModel(this,pair.first, pair.second, mainDir);
        QObject::connect(model,SIGNAL(sendDropData(QString,QString,QString,QString)),this,SLOT(receiveDropData(QString,QString,QString,QString)));
        table->setModel(model);

        // Update AddressBar
        addressBar->updatePath(mainDir);


    }

}

//Slot to recieve drop data from model
void ServerExplorer::receiveDropData(QString type, QString source, QString destination, QString sftpType)
{
    // Send Data To Parent
    qDebug() << "Data recieved";
    emit sendDropData(type,source,destination,sftpType);
}


// Bookmark data
void ServerExplorer::addBookmark()
{
    QString temp = host->text();
    if (!temp.isEmpty()) {
    qDebug() << "add bookmark";
    // Save bookmark
    QSettings settings;
    QStringList values = settings.value("bookmark").toStringList();
    values << host->text() << user->text() << encrypt(password->text()) << port->text();
    settings.setValue("bookmark",values);

    qDebug() << "bookmark Added to settings: " << host->text() << " " << user->text() << " " << encrypt(password->text()) << " " << port->text();

    setBookmarks();
}

}

QString ServerExplorer::encrypt(QString pass)
{
    unsigned char ckey[] = "\x09\x8F\x6B\xCD\x46\x21\xD3\x73\xCA\xDE\x4E\x83\x26\x27\xB4\xF6";
    const char* password = pass.toLocal8Bit().constData();
    unsigned char encrypted[strlen(password)];
    unsigned char out[strlen(password)];
    AES_KEY enc_key;
    AES_set_encrypt_key(ckey,128,&enc_key);
    //AES_set_decrypt_key(ckey,128,&dec_key);
    AES_encrypt((unsigned char*) password ,encrypted, &enc_key);
    //AES_decrypt(encrypted, out, &dec_key);
    return QString::fromLocal8Bit((char*)encrypted);
}

QString ServerExplorer::decrypt(QString encryptedPassword)
{
    unsigned char ckey[] = "\x09\x8F\x6B\xCD\x46\x21\xD3\x73\xCA\xDE\x4E\x83\x26\x27\xB4\xF6";
   // qDebug() << "Got Key";
    unsigned char* encryptedPass = (unsigned char*)encryptedPassword.toLocal8Bit().data();
   // qDebug() << "Got pass";
    unsigned char out[strlen(encryptedPassword.toLocal8Bit().data())];
   // qDebug() << "got output variable";
    AES_KEY dec_key;
   // qDebug() << "got decrypt key";
    //AES_set_encrypt_key(ckey,128,&enc_key);
    AES_set_decrypt_key(ckey,128,&dec_key);
  //  qDebug() << "set key";
    //AES_encrypt((unsigned char*) password ,encrypted, &enc_key);

    AES_decrypt(encryptedPass, out, &dec_key);

  //  qDebug() << "Decrypted";

    return "hello";//QString::fromLocal8Bit((char*)out);

}

void ServerExplorer::setBookmarks()
{
    bookmarks.clear();
    int counter = 0;
    QSettings settings;

    foreach(QString a,settings.value("bookmark").toStringList() ){
        counter++;
        if (counter == 3){
            counter = 0;
            bookmarks << decrypt(a);
        }
        else {
            bookmarks << a;
        }
    }
    qDebug() << "bookmarks loaded from settings";
    qDebug() << "count of bookmark list " << bookmarks.count();
    setTableData();
}

void ServerExplorer::updateBookmarkView(int row)
{
    int a = row;
    //Start value = a * 4;

    serverLbl->setText(bookmarks.at(a*4));
    userLbl->setText(bookmarks.at(a*4+1));

}

void ServerExplorer::setTableData()
{
    qDebug() << "Settings data in table";

    int counter = 0;
    bookmarkTable->clear();
    foreach(QString a, bookmarks){
        // Add to table
        if (counter == 0){
            bookmarkTable->addItem(a);
        }
        counter++;
        if (counter == 4){
            counter = 0;
        }

    }

}
