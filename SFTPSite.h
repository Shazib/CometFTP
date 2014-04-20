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
    ssh_session my_ssh_session;
    sftp_session sftp;
    int verify_knownhost(ssh_session session);
    int verify_user(ssh_session session, std::string user, std::string pass);
    int sftp_connection(ssh_session session);
    QString SFTPSite::getPermissions(uint32_t permissions);
    QString SFTPSite::getType(uint8_t type);

public:
   bool status;

   void sftp_listdir();
};

#endif // COMET_SFTPSITE_H
