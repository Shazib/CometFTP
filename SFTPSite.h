#ifndef COMET_SFTPSITE_H
#define COMET_SFTPSITE_H

#include <QObject>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <QtWidgets>

class SFTPSite : public QObject
{
    Q_OBJECT
public:
    explicit SFTPSite(QObject *parent = 0, std::string host = "", std::string user = "", std::string pass ="", std::string port = "");

public slots:
    void startDownload(QString source, QString destination);
    void startUpload(QString source, QString destination);
    void threadInit(std::string _host, std::string _user, std::string _pass, std::string _port);
    void cancelDownload();
    void pauseDownload();
    void receiveTimer();


signals:
    void downloadComplete(int status);
    void updateProgress();
    void sendSpeed(int bytes);

private:
    int sftp_getAllFiles(QString path, QString destination);
    ssh_session my_ssh_session;
    sftp_session sftp;
    int verify_knownhost();
    int verify_user(std::string user, std::string pass);
    int silent_verify_knownhost();
    int sftp_connection();
    //int sftp_upload
    QString getPermissions(uint32_t permissions);
    QString getType(uint8_t type);
    QStringList values;
    int sftp_listdir(QString path);
    std::string host, user, pass, port;
    int downloadfile(QString source, QString Destination);
    enum returnCodes {
        DLOAD_FILE_EXISTS,
        DLOAD_COMPLETE,
        DLOAD_FAILED,
        DLOAD_OVERWRITE,
        DLOAD_CANCEL,
        DLOAD_ERROR
    };
    bool cancel = false;
    bool pause = false;
    QMutex mutex;
    int totalBytes;


public:
    QString globalServer;
    QString globalLocal;
    QString newDir;
    bool status;
    bool init();
    bool silent_init();
    void cleanup();
    QPair<int, QStringList> listDir(QString path);
    int numRows;
    int isDir(QString fileName);
    QStringList getAllFiles(QString path, QString destination);
    int dloadFile(QString source, QString dest);
};

#endif // COMET_SFTPSITE_H
