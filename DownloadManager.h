#ifndef COMET_DOWNLOADMANAGER_H
#define COMET_DOWNLOADMANAGER_H

#include <QWidget>
#include <QtWidgets>

#include "SFTPSite.h"
class DownloadManager : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadManager(QWidget *parent = 0);
    void getDataFromParent(QStringList data);
    void setCredentials(QStringList credentials);

signals:
    //void downloadFinished(int a);
    void initThread(std::string host, std::string user, std::string pass, std::string port);
    void startDownload(QString source, QString destination);
    void startUpload(QString source, QString destination);


private:
    std::string host, user, password, port;
    QTableWidget* table;
    QStringList queue;
    SFTPSite* sftp;
    SFTPSite* site;
    QThread* thread;
    int numRows;
    QString destination;
    QString destinationTemp;
    bool downloading;
    bool isAlive;
    int percentage;
    int fileCounter;

public slots:
    // Thread slots
    void receivePercentage();
    void recieveDownloadComplete();

    void addData(QString _type, QString _source, QString _destination, QString sftpType);
    void addLocalFolder(QString path);
    void addServerFolder(QString path, QString destination);
    void receiveCredentials(std::string _host, std::string _user, std::string _password, std::string _port);

};

#endif // COMET_DOWNLOADMANAGER_H
