#ifndef COMET_SFTPSITE_H
#define COMET_SFTPSITE_H

#include <QWidget>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <libssh/callbacks.h>
#include <pthread.h>


class SFTPSite : public QWidget
{
    Q_OBJECT
public:
    explicit SFTPSite(QWidget *parent = 0, std::string host = "", std::string user = "", std::string pass ="", std::string port = "");

signals:

public slots:

private:
    ssh_session my_ssh_session;
    sftp_session sftp;
    int verify_knownhost();
    int verify_user(std::string user, std::string pass);
    int sftp_connection();
    //int sftp_upload
    QString getPermissions(uint32_t permissions);
    QString getType(uint8_t type);
    QStringList values;
    int sftp_listdir(QString path);
    std::string host, user, pass, port;

public:
   bool status;
   bool init();
   void cleanup();
   QPair<int, QStringList> listDir(QString path);
   int numRows;
};

#endif // COMET_SFTPSITE_H
