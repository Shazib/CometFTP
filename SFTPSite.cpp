// Chunk Size For Transfers
#define MAX_XFER_BUF_SIZE 16384

#include "SFTPSite.h"

#include <libssh/callbacks.h>
#include <sys/stat.h>
#include <fcntl.h>

SFTPSite::SFTPSite(QObject *parent, std::string _host, std::string _user, std::string _pass, std::string _port) :
    QObject(parent)
{

    qDebug() << "Stfp created";
    //ssh_threads_set_callbacks(ssh_threads_get_pthread());
    ssh_init();

    // Required variables
    int verbosity = SSH_LOG_NOLOG;
    int portNum = atoi(port.c_str());
    host = _host;
    user = _user;
    port = _port;
    pass = _pass;

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

}

// Init the connection
bool SFTPSite::init()
{
    // Connect to Site
    int rc;
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK){
        // Get Error
        QString temp = QString::fromUtf8(ssh_get_error(my_ssh_session));
        // Show Alert
        QMessageBox msgBox;
        msgBox.setText("Error calling ssh_connect");
        msgBox.setInformativeText("Click for details on the error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(temp);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;
        return false;
    } else {status = true;}

    // Authenticating server identity
    if (verify_knownhost() < 0){
        QMessageBox msgBox;
        msgBox.setText("The server could not be authenticated");
        msgBox.setInformativeText("Please check the help documentation");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;
        return false;
    } else {status = true;}

    // Authenticating user
    if (verify_user(user,pass) < 0){
        status = false;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return false;
    } else { status = true; }

    // Setup SFTP connection over SSH session

    if (sftp_connection() != SSH_OK){
        status = false;
        return false;
    } else { return true;}

}

// Verify Host using SSH Key
int SFTPSite::verify_knownhost()
{

    int state, hlen, rc;
    unsigned char *hash = NULL;
    QMessageBox msgBox;
    state = ssh_is_server_known(my_ssh_session); // Check if key known
    hlen = ssh_get_pubkey_hash(my_ssh_session, &hash); //store binary of key
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
                    if (ssh_write_knownhost(my_ssh_session) < 0){
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
                    if (ssh_write_knownhost(my_ssh_session) < 0){
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

// Silent verify for already verififed sites
int SFTPSite::silent_verify_knownhost()
{
    int state, hlen, rc;
    unsigned char *hash = NULL;
    //QMessageBox msgBox;
    state = ssh_is_server_known(my_ssh_session); // Check if key known
    hlen = ssh_get_pubkey_hash(my_ssh_session, &hash); //store binary of key
    if (hlen < 0){return -1;} // Check key is a key
    char *publickeyhash = ssh_get_hexa(hash,hlen);
    QString pubkey(publickeyhash);

    free(hash);

    return 0;
}

// Silent init for background
bool SFTPSite::silent_init()
{
    // Connect to Site
    int rc;
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK){
        // Get Error
        QString temp = QString::fromUtf8(ssh_get_error(my_ssh_session));
        // Show Alert
        QMessageBox msgBox;
        msgBox.setText("Error calling ssh_connect");
        msgBox.setInformativeText("Click for details on the error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(temp);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;
        return false;
    } else {status = true;}

    // Authenticating server identity
    if (silent_verify_knownhost() < 0){
        QMessageBox msgBox;
        msgBox.setText("The server could not be authenticated");
        msgBox.setInformativeText("Please check the help documentation");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        status = false;
        return false;
    } else {status = true;}

    // Authenticating user
    if (verify_user(user,pass) < 0){
        status = false;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return false;
    } else { status = true; }

    // Setup SFTP connection over SSH session

    if (sftp_connection() != SSH_OK){
        status = false;
        return false;
    } else { return true;}

}

// Verify User using Password
int SFTPSite::verify_user(std::string user, std::string pass)
{

   int rc = ssh_userauth_password(my_ssh_session, user.c_str(),pass.c_str());
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
int SFTPSite::sftp_connection()
{


    int rc;
    sftp = sftp_new(my_ssh_session);

    if (sftp == NULL){
        // Error allocating SFTP Session
        QString error(ssh_get_error(my_ssh_session));
        return SSH_ERROR;
        QMessageBox msgBox;
        msgBox.setText("Error Creating SFTP");
        msgBox.setInformativeText("Click for details on the error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(error);
        msgBox.exec();
        return rc;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK){
        // Error initialising SFTP session
        QString error(sftp_get_error(sftp));
        sftp_free(sftp);
        QMessageBox msgBox;
        msgBox.setText("Error Initing STFP");
        msgBox.setInformativeText("Click for details on the error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(error);
        msgBox.exec();
        return rc;
    }

    return SSH_OK;

}

int SFTPSite::sftp_listdir(QString path)
{
    // Initial Setup
    values.clear();
    sftp_dir dir;
    sftp_attributes attributes;
    int rc = 0;
    QByteArray ba = path.toLocal8Bit();
    const char* directoryPath = ba.data();

    // Open the Directory
    dir = sftp_opendir(sftp, directoryPath);

    if (!dir){
        qDebug() << "error opening directory";
        // Directory Probably Doesn't Exist!
        return -1;


    } else {

    // Create the StringList to store the data
    while ((attributes = sftp_readdir(sftp, dir)) != NULL){

        // check its not hidden.
        if (strncmp(attributes->name,".",1) != 0) {

        values << QString::fromStdString(attributes->name) <<
                                 QString::number(attributes->size) <<
                                 //getPermissions(attributes->permissions) <<
                                 QString::number(attributes->permissions) <<
                                 QString::fromStdString(attributes->owner) <<
                                 getType(attributes->type);
        rc++;
        }
    }

    sftp_closedir(dir);
    sftp_attributes_free(attributes);
    qDebug() << "Number of Rows" << rc;

    return rc;
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

// Cleanup before deletion
void SFTPSite::cleanup()
{
    sftp_free(sftp);
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}

// Public Access Method
QPair<int, QStringList> SFTPSite::listDir(QString path)
{
    int a = sftp_listdir(path);

    return { a, values};
}

// Download a file
int SFTPSite::downloadfile(QString source, QString Destination)
{
    int res = DLOAD_OVERWRITE;
    // Check if the file exists locally first

    // Temp strings


    // Get Filename.
    QStringList split = source.split("/");
    //split.removeLast();
    QString fileName = split.last();
    qDebug() << "Source " << source;
    qDebug() << "File name " << fileName;
    // Check if file exists locally
    QString localPath = Destination + "/" + fileName;
    qDebug() << "Local Path "  << localPath;
    QFile file;
    file.setFileName(localPath);

    // Check if folder exists.
    QDir dir(Destination);
    if (!dir.exists()){
        dir.mkdir(Destination);
    }

    // Now we can continue to download the file
    if (res == DLOAD_OVERWRITE){

        QThread* thread = new QThread(this);
        QTimer* timer= new QTimer(0);
        timer->setInterval(5000);
        timer->moveToThread(thread);
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(receiveTimer()),Qt::DirectConnection);
        QObject::connect(thread,SIGNAL(started()),timer,SLOT(start()));
        thread->start();




        int result;
        int access_type;
        sftp_file file; // file handle
        char buffer[MAX_XFER_BUF_SIZE]; // Buffer where files are written
        int nbytes; // num bytes transferred

        // Counting bytes for progress bar
        sftp_attributes attributes;
        attributes = sftp_stat(sftp,source.toLocal8Bit().data());
        uint64_t fileSize = attributes->size;
        int percent = fileSize / 100;
        int percentCounter = 0;

        // Open local file for writing
        FILE* fp;
        fp = fopen(localPath.toLocal8Bit().data(),"ab");
        // Error Checking
        if (fp == NULL){ qDebug() << "File is null"; }

        // Open SFTP file
        access_type = O_RDONLY; // Read Only
        file = sftp_open(sftp, source.toLocal8Bit().data(),access_type,0);
        if (file == NULL) {
            // Can't open file
            ssh_get_error(my_ssh_session);
            qDebug() << SSH_ERROR;
        }


        for(;;) {
            if (pause){
                usleep(100000);
            } else {

                nbytes = sftp_read(file,buffer, sizeof(buffer));
                if (nbytes == 0){
                    qDebug() << "EOF";
                    break; // EOF
                } else if (nbytes < 0){
                    // Error
                    sftp_close(file);
                    qDebug() << "error closing file";
                }
                result = fwrite(buffer,1,nbytes,fp);
                totalBytes+=nbytes;
                percentCounter = percentCounter + nbytes;
                if (percentCounter >= percent){
                    percentCounter = 0;
                    emit updateProgress();
                    //qDebug() << this->thread();
                }

                // Cancel

                if (cancel) {
                    // Cancel Download
                    fclose(fp);
                    remove(localPath.toLocal8Bit().data());
                    sftp_close(file);
                   // emit updateProgress();
                    thread->terminate();
                    return DLOAD_CANCEL;

                }

            }
        }

        // Close files
        fclose(fp);
        sftp_close(file);

        thread->terminate();
    }

    return DLOAD_COMPLETE;



}

// Upload a file
int SFTPSite::uploadFile(QString source, QString destination)
{
    qDebug() << "Source: " << source;
    qDebug() << "dest: " << destination;

    qDebug() << "Starting Upload method";
    int res = DLOAD_OVERWRITE;

    QThread* thread = new QThread(this);
    QTimer* timer= new QTimer(0);
    timer->setInterval(5000);
    timer->moveToThread(thread);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(receiveTimer()),Qt::DirectConnection);
    QObject::connect(thread,SIGNAL(started()),timer,SLOT(start()));
    thread->start();
    qDebug() << "Timer started";

    // Check if file exists on server
    int access_type = O_WRONLY | O_CREAT | O_TRUNC;
    QStringList split = source.split("/");
    QString fileName = split.last();
    sftp_file file;
    sftp_attributes attrib;
    FILE* fp;
    // Destination full path
    QString serverPath = destination + fileName;

    qDebug() << "got full path";

    // Check if folder exists on server
    //file = sftp_open(sftp, serverPath.toLocal8Bit().data(),access_type,0);

    sftp_dir dir;
    dir = sftp_opendir(sftp, destination.toLocal8Bit().data());

    if (dir != NULL){
        // Is a directory
    } else {
        // MkDir
        int a = makeDir(destination);
        qDebug() << "Made folder";
        //if (a != 0) {return DLOAD_ERROR;}
    }

    qDebug() << "done with checking";

    // Carry on with Upload
    //sftp_attributes_free(attrib);
   // sftp_close(file);
    qDebug() << "Serverpath " << serverPath;
    file = sftp_open(sftp, serverPath.toLocal8Bit().data(),access_type,  S_IWUSR);

    if (file == NULL){
        qDebug() << "Can't open file for writing";
        qDebug() << ssh_get_error(my_ssh_session);
        thread->terminate();
        return SSH_ERROR;
    }
    qDebug() << "SFTP File opened";


    fp = fopen(source.toLocal8Bit().data(),"rb");
    fseek (fp,0,SEEK_END);
    long size = ftell(fp);
    int fileSize = (int)size;
    int percent = fileSize / 100;
    int percentCounter = 0;
    rewind(fp);
    int nbytes;
    char* buffer[MAX_XFER_BUF_SIZE];

    nbytes = fread(buffer,1,MAX_XFER_BUF_SIZE,fp);
    for (;;){

        totalBytes+=nbytes;
        percentCounter = percentCounter + nbytes;
        if (percentCounter >= percent){
            percentCounter = 0;
            emit updateProgress();
            //qDebug() << this->thread();
        }
        if (pause){
            usleep(100000);
        } else {


            sftp_write(file,buffer,nbytes);
            nbytes = fread(buffer,1,MAX_XFER_BUF_SIZE,fp);

            if (cancel){
                sftp_unlink(sftp,serverPath.toLocal8Bit().data());
                sftp_close(file);
                fclose(fp);
                thread->terminate();
               return DLOAD_CANCEL;

            }
            if (feof(fp)){
                break;
            }

        }

    }

    fclose(fp);
    sftp_close(file);
    thread->terminate();
    return DLOAD_COMPLETE;

}

// Return list of files only
int SFTPSite::sftp_getAllFiles(QString path, QString destination)
{
    // Initial Setup
    globalServer = path;
    globalLocal = destination;

    sftp_dir dir;
    sftp_attributes attributes;
    int rc = 0;
    QByteArray ba = path.toLocal8Bit();
    const char* directoryPath = ba.data();

    // Open the Directory
    dir = sftp_opendir(sftp, directoryPath);

    // Check successful opening
    if (!dir){
        qDebug() << "error opening directory";
        // Directory Probably Doesn't Exist!
        return -1;
    } else {

        // Read the directory, 1 file at a time
        while ((attributes = sftp_readdir(sftp, dir)) != NULL){

            // Check its not hidden.
            if (strncmp(attributes->name,".",1) != 0) {
                // If is file
                    if (attributes->type == SSH_FILEXFER_TYPE_REGULAR){
                        // Add Paths
                        qDebug() << globalLocal + QString::fromStdString(attributes->name);
                        qDebug() << globalServer + QString::fromStdString(attributes->name);

                        values << (globalServer + QString::fromStdString(attributes->name))
                               << globalLocal;

                    }
                    // If is dir
                    if (attributes->type == SSH_FILEXFER_TYPE_DIRECTORY){
                        newDir = QString::fromStdString(attributes->name) + "/";
                        qDebug() << "newdir is" << newDir;
                        globalLocal = globalLocal + newDir;
                        globalServer = globalServer + newDir;
                        // Recursive function call
                        sftp_getAllFiles(globalServer,globalLocal);


                    }

            rc++;
            }
        }
        // Once loop completes, reset path;
        globalLocal.remove((globalLocal.count()-newDir.count()),newDir.count());
        globalServer.remove((globalServer.count()-newDir.count()),newDir.count());
        newDir = "";

        // Cleanup
        sftp_closedir(dir);
        sftp_attributes_free(attributes);

    }

    return rc;
}

// Make a directory
int SFTPSite::makeDir(QString path)
{
    sftp_mkdir(sftp,path.toLocal8Bit().data(),S_IRWXU);
}

// Public Accessors
QStringList SFTPSite::getAllFiles(QString path, QString destination)
{
    values.clear();
    sftp_getAllFiles(path, destination);
    return values;
}
/*
int SFTPSite::dloadFile(QString source, QString dest)
{
    int a =  downloadfile(source,dest);
    if ( a == DLOAD_COMPLETE || a == DLOAD_CANCEL){
        emit downloadComplete(a);
    }
}

int SFTPSite::uloadFile(QString source, QString dest)
{
    int a = uploadFile(source,dest);
    if (a == DLOAD_COMPLETE || a == DLOAD_CANCEL){
        emit downloadComplete(a);
    }

}
*/
// Slots for threaded download/upload handling
void SFTPSite::startDownload(QString source, QString destination)
{
    qDebug() << "Download in sftp started";
    int a = downloadfile(source, destination);
    emit downloadComplete(a);
    qDebug() << "signal emit";
}

void SFTPSite::startUpload(QString source, QString destination)
{
     int a = uploadFile(source,destination);
     if (a == DLOAD_COMPLETE || a == DLOAD_CANCEL){
         emit downloadComplete(a);
     }

}

void SFTPSite::threadInit(std::string _host, std::string _user, std::string _pass, std::string _port)
{
    host = _host;
    user = _user;
    pass = _pass;
    port= _port;
    int portNum = atoi(port.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
    //ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &portNum);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, user.c_str());
    this->silent_init();
    qDebug() << "Thread SFTP Init";

}

void SFTPSite::cancelDownload()
{
    cancel = !cancel;
}

void SFTPSite::pauseDownload()
{
    pause = !pause;
}

void SFTPSite::receiveTimer()
{
    emit sendSpeed(totalBytes);
    qDebug() << "sending Speed";
    mutex.lock();
    totalBytes = 0;
    mutex.unlock();
}









