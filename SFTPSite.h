#ifndef COMET_SFTPSITE_H
#define COMET_SFTPSITE_H

#include <QWidget>
#include <libssh/libssh.h>
#include <libssh/sftp.h>

class SFTPSite : public QWidget
{
    Q_OBJECT
public:
    explicit SFTPSite(QWidget *parent = 0, std::string host = "", std::string user = "", std::string pass ="", std::string port = "");

signals:

public slots:

private:
    int verify_knownhost(ssh_session session);
    int verify_user(ssh_session session, std::string user, std::string pass);
    int sftp_connection(ssh_session session);

public:
   bool status;
   sftp_session my_sftp_session;
   void sftp_listdir(ssh_session session, sftp_session sftp);
};

#endif // COMER_SFTPSITE_H
