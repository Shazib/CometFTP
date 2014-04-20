#include "sftpsite.h"

#include <QDebug>
#include <QMessageBox>

#include <libssh/sftp.h>

SFTPSite::SFTPSite(QWidget *parent, std::string host, std::string user, std::string pass, std::string port) :
    QWidget(parent)
{

    // Required variables
    int verbosity = SSH_LOG_PROTOCOL;
    int portNum = atoi(port.c_str());

    // New SSH session

    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL){

        QMessageBox msgBox;
        msgBox.setText("The session could not be created");
        msgBox.setInformativeText("Please check the help documentation");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

    }
    //Setting up session
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &portNum);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, user.c_str());

    // Connecting to server
    int rc;
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK){
        // Get Error
        QString temp = QString::fromUtf8(ssh_get_error(my_ssh_session));
        // Show Alert
        QMessageBox msgBox;
        msgBox.setText("Error Connecting to server");
        msgBox.setInformativeText("Click for details on the error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(temp);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;

    } else {
        status = true;
    }

    // Authenticating server identity
    if (verify_knownhost(my_ssh_session) < 0){
        QMessageBox msgBox;
        msgBox.setText("The server could not be authenticated");
        msgBox.setInformativeText("Please check the help documentation");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;
    } else {status = true;}

    // Authenticating user
    if (verify_user(my_ssh_session,user,pass) < 0){
        status = false;
    } else {
        status = true;
    }

    if (sftp_connection(my_ssh_session) != SSH_OK){

    }
    sftp_listdir(my_ssh_session,sftp);


}

// Verify Host using SSH Key
int SFTPSite::verify_knownhost(ssh_session session)
{

    int state, hlen, rc;
    unsigned char *hash = NULL;
    QMessageBox msgBox;
    state = ssh_is_server_known(session); // Check if key known
    hlen = ssh_get_pubkey_hash(session, &hash); //store binary of key
    if (hlen < 0){return -1;} // Check key is a key
    char *publickeyhash = ssh_get_hexa(hash,hlen);
    QString pubkey(publickeyhash);

    switch (state)
    {
        case SSH_SERVER_KNOWN_OK: // The server is in the known list
            free(hash);return 0;

        case SSH_SERVER_KNOWN_CHANGED:
        // The key has changed, must ask for user input.
            msgBox.setText("Server Host Key Changed");
            msgBox.setInformativeText("The host key for the server has changed. For security you must confirm you want to continue");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setDetailedText("The key for the server: /n" + pubkey);
            rc = msgBox.exec();
            switch (rc) {
                case QMessageBox::Ok: free(hash);return 0;
            case QMessageBox::Cancel: free(hash);return -1;
            }
            free(hash);

            return -1;

        case SSH_SERVER_FOUND_OTHER:
            // A Different Key was found, very bad, ask for input
            msgBox.setText("Different Server Host Key Found");
            msgBox.setInformativeText("A different host key was found. An atacker might change the default server key to confuse your client into thinking the key does not exist. Do you wish to continue?");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setDetailedText("The key for the server: /n" + pubkey);
            rc = msgBox.exec();
            switch (rc) {
                case QMessageBox::Ok: free(hash);return 0;
            case QMessageBox::Cancel: free(hash);return -1;
            }
            free(hash);

            return -1;

        case SSH_SERVER_FILE_NOT_FOUND:
        // A Different Key was found, very bad, ask for input
            msgBox.setText("Could not find a known host file");
            msgBox.setInformativeText("If you accept the host key now,the file will be automatically created. Do you wish to continue?");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setDetailedText("The key for the server: /n" + pubkey);
            rc = msgBox.exec();
            switch (rc) {
                case QMessageBox::Ok:
                    if (ssh_write_knownhost(session) < 0){
                        msgBox.setText("Sorry, there was a problem saving the key");
                        msgBox.setInformativeText("");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.setDetailedText("");
                        int rc = msgBox.exec();
                        free(hash);

                        return -1;
                    }
                    free(hash);

                    return 0;
                case QMessageBox::Cancel: return -1;
            }
            return -1;
        case SSH_SERVER_NOT_KNOWN:
        // Unknown server
            msgBox.setText("The server is unknown");
            msgBox.setInformativeText("If you accept the host key now,the file will be automatically created. Do you trust the key?");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setDetailedText("The key for the server: /n" + pubkey);
            rc = msgBox.exec();
            switch (rc) {
                case QMessageBox::Ok:
                    if (ssh_write_knownhost(session) < 0){
                        msgBox.setText("Sorry, there was a problem saving the key");
                        msgBox.setInformativeText("");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.setDetailedText("");
                        int rc = msgBox.exec();
                        free(hash);

                        return -1;
                    }
                    free(hash);

                    return 0;
                case QMessageBox::Cancel: return -1;
            }
            return -1;
        case SSH_SERVER_ERROR:
        msgBox.setText("There was an error with the server");
        msgBox.setInformativeText("");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText("");
        rc = msgBox.exec();
        free(hash);

        return -1;

    }

    free(hash);

    return -1;
}

// Verify User using Password
int SFTPSite::verify_user(ssh_session session, std::string user, std::string pass){

   int rc = ssh_userauth_password(session, user.c_str(),pass.c_str());
    if (rc != SSH_AUTH_SUCCESS){
        QMessageBox msgBox;
        msgBox.setText("Error Authenticating user");
        msgBox.setInformativeText("Please check your username and password");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return -1;
    }
    return 0;
}

// Open SFTP session using the SSH Session
int SFTPSite::sftp_connection(ssh_session session)
{


    int rc;
    sftp = sftp_new(session);

    if (sftp == NULL){
        // Error allocating SFTP Session
        QString error(ssh_get_error(session));
        return SSH_ERROR;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK){
        // Error initialising SFTP session
        QString error(sftp_get_error(sftp));
        sftp_free(sftp);
        return rc;
    }

    sftp_dir dir;
    sftp_attributes attributes;


    dir = sftp_opendir(sftp,"");
    if (!dir){
        qDebug() << "error opening directory";
    }
    else{
        qDebug() << "SUCCESS" ;
    }


    return SSH_OK;

}


void SFTPSite::sftp_listdir(ssh_session session, sftp_session sftp)
{
    sftp_dir dir;
    sftp_attributes attributes;
    int rc;

    dir = sftp_opendir(sftp,"/home/");
    if (!dir){
        qDebug() << "error opening directory";
    }


}

// Get Permissions in Human Readable syntax
QString SFTPSite::getPermissions(uint32_t permissions)
{

    // Converting the decimal returned by libssh into an octal.
    int rem, i=1, octal= 0;

    while (permissions != 0){
        rem = permissions%8;
        permissions /= 8;
        octal+= rem * i;
        i *= 10;
    }

    // Reading the octal into a string

    // last three digits for permissions.
    QString temp = QString::number(octal);
    QString final = "";
    i = temp.count();

    // Add each set of permissions to string
    for (int a=3; a >= 1; a--){

        switch(temp[i-a].digitValue())
        {
        case 0:
            final.append(QString("---"));
            break;
        case 1:
            final.append(QString("--x"));
            break;
        case 2:
            final.append(QString("-w-"));
            break;
        case 3:
            final.append(QString("-wx"));
            break;
        case 4:
            final.append(QString("r--"));
            break;
        case 5:
            final.append(QString("r-x"));
            break;
        case 6:
            final.append(QString("rw-"));
            break;
        case 7:
            final.append(QString("rwx"));
            break;

        }
    }

    // Return the correct string
    return final;

}

// Get File Type
QString SFTPSite::getType(uint8_t type)
{
    switch(type)
    {
    case SSH_FILEXFER_TYPE_DIRECTORY:
        return QString("Folder");
    case SSH_FILEXFER_TYPE_REGULAR:
        return QString("File");
    case SSH_FILEXFER_TYPE_SYMLINK:
        return QString("Symlink");
    case SSH_FILEXFER_TYPE_SPECIAL:
        return QString("Special");
    case SSH_FILEXFER_TYPE_UNKNOWN:
        return QString("Unknown");
    }
}

















