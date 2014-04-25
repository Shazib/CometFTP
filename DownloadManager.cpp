#include "DownloadManager.h"

DownloadManager::DownloadManager(QWidget *parent) :
    QWidget(parent)
{
    // Setup View + initial states
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    table = new QTableWidget();
    mainLayout->addWidget(table);
    this->setLayout(mainLayout);

    // Setup table widget
    numRows = 0;
    table->setColumnCount(4);
    table->setRowCount(numRows);

    percentage = 0;
    fileCounter = 0;
    // Header

}

void DownloadManager::addData(QString _type, QString _source, QString _destination, QString sftpType){


    // The manager must check if folders are added.
    // Add data to model

    //destination = _destination;

    // If Upload
    if (_type == "Upload")
    {
        // Lets check if its a folder
        QFileInfo info(_source);
        if (info.isDir()){
            // Get every individual file
            addLocalFolder(info.absoluteFilePath());

        } else {
            numRows++;
            table->setRowCount(numRows);
            QTableWidgetItem* typeItem = new QTableWidgetItem(_type,1);
            QTableWidgetItem* sourceItem = new QTableWidgetItem(_source,2);
            QTableWidgetItem* destinationItem = new QTableWidgetItem(_destination,3);
            QTableWidgetItem* statusItem = new QTableWidgetItem("Pending",3);
            table->setItem((numRows-1),0,typeItem);
            table->setItem((numRows-1),1,sourceItem);
            table->setItem((numRows-1),2,destinationItem);
            table->setItem((numRows-1),2,statusItem);
        }

    }

    // If Download
    if (_type == "Download"){
        if (sftpType == "Folder"){
        // Now get all values;
            addServerFolder(_source,_destination);
        }
        else {
            numRows++;
            table->setRowCount(numRows);
            QTableWidgetItem* typeItem = new QTableWidgetItem(_type,1);
            QTableWidgetItem* sourceItem = new QTableWidgetItem(_source,2);
            QTableWidgetItem* destinationItem = new QTableWidgetItem(_destination,3);
            QTableWidgetItem* statusItem = new QTableWidgetItem("Pending",3);
            table->setItem((numRows-1),0,typeItem);
            table->setItem((numRows-1),1,sourceItem);
            table->setItem((numRows-1),2,destinationItem);
            table->setItem((numRows-1),2,statusItem);
        }
    }
    // When Data is added, the queue is automatically processed.
    // The site will respond with a success or failure case and the queue will continue to process.

    if (!isAlive) {
        // Create thread
        sftp = new SFTPSite();
        thread = new QThread();
        sftp->moveToThread(thread);
        QObject::connect(this,SIGNAL(initThread(std::string,std::string,std::string,std::string)),sftp,SLOT(threadInit(std::string,std::string,std::string,std::string)));
        QObject::connect(this,SIGNAL(startDownload(QString,QString)),sftp,SLOT(startDownload(QString,QString,int)));
        QObject::connect(this,SIGNAL(startUpload(QString,QString)),sftp,SLOT(startupload(QString,QString)));
        QObject::connect(sftp,SIGNAL(updateProgress()),this,SLOT(receivePercentage()));
        QObject::connect(sftp,SIGNAL(downloadComplete(int)), this,SLOT(recieveDownloadComplete()));
        emit initThread(host,user,password,port);
    }
    // Start Queue
    if (!downloading){
        // Start queue
        recieveDownloadComplete();


    }
}
void DownloadManager::addLocalFolder(QString path)
{
    //qDebug() << "add folder";
   // qDebug() << "root folder" << path;
    QDir dir(path);  
    // Get everything in directory
    QFileInfoList list = dir.entryInfoList();
    
    // For each file/folder
    for (int i = 2; i < list.size(); i++){

        // If is file
        if (list.at(i).isFile()){
            QFileInfo info = list.at(i);
            //qDebug() <<  list.at(i).absoluteFilePath();
            //qDebug() << destination;
            numRows++;
            table->setRowCount(numRows);
            QTableWidgetItem* typeItem = new QTableWidgetItem("Upload",1);
            QTableWidgetItem* sourceItem = new QTableWidgetItem(list.at(i).absoluteFilePath(),2);
            QTableWidgetItem* destinationItem = new QTableWidgetItem(destination,3);
            QTableWidgetItem* statusItem = new QTableWidgetItem("Pending",3);
            table->setItem((numRows-1),0,typeItem);
            table->setItem((numRows-1),1,sourceItem);
            table->setItem((numRows-1),2,destinationItem);
            table->setItem((numRows-1),3,statusItem);
        // If is dir
        } else if (list.at(i).isDir()){
            // Recursive call
            list.at(i).fileName();
            destinationTemp = list.at(i).fileName(); + "/";
            destination = destination + destinationTemp + "/";
            addLocalFolder(list.at(i).absoluteFilePath());


        }

    }
    // Reset Destination folder for parent
    destination.remove((destination.count()-destinationTemp.count()-1),destinationTemp.count());
    destination.remove(destination.count()-1,1);
    destinationTemp = "";

}

void DownloadManager::addServerFolder(QString path, QString destination)
{
    site = new SFTPSite(this, host, user, password, port);

    if( !site->silent_init()) {
        // ERROR
    } else {
        // Get all files
        QString dest = path + "/";
        QStringList values =  site->getAllFiles(dest,destination);
        int a = values.count();
        while (a != 0){

            qDebug() << values.count();
            numRows++;
            table->setRowCount(numRows);
            QTableWidgetItem* typeItem = new QTableWidgetItem("Download",1);
            QTableWidgetItem* sourceItem = new QTableWidgetItem(values.first(),2);
            values.removeFirst();
            a--;
            QTableWidgetItem* destinationItem = new QTableWidgetItem(values.first(),3);
            values.removeFirst();
            a--;
            QTableWidgetItem* statusItem = new QTableWidgetItem("Pending",4);
            table->setItem((numRows-1),0,typeItem);
            table->setItem((numRows-1),1,sourceItem);
            table->setItem((numRows-1),2,destinationItem);
            table->setItem((numRows-1),3,statusItem);

        }


    }
}

void DownloadManager::receiveCredentials(std::string _host, std::string _user, std::string _password, std::string _port){

    host = _host;
    user = _user;
    password = _password;
    port = _port;
}


void DownloadManager::recieveDownloadComplete(){

    // Start or continue queue processing
    // Select data
    QString _source = table->item(fileCounter,1)->text();
    QString _destination = table->item(fileCounter,3)->text();

}


















