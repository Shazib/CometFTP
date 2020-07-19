#ifndef COMET_MAINWINDOW_H
#define COMET_MAINWINDOW_H

// Qt
#include <QFrame>
#include <QtWidgets>

// Libraries
//#include <libssh/libssh.h>
//#include <libssh/sftp.h>

// Classes
#include "ServerExplorer.h"
#include "LocalExplorer.h"
#include "StatusArea.h"
#include "SlidingStackedWidget.h"
#include "DownloadManager.h"
#include "About.h";

class MainWindow : public QFrame
{
    Q_OBJECT

public:
    explicit MainWindow(QFrame *parent = 0);
    ~MainWindow();

private:

    // Widgets
    QPushButton* btnQuit;
    QPushButton* btnMax;
    QPushButton* btnMin;
    QPushButton* btnMenu;
    StatusArea* statusArea;
    LocalExplorer* localExplorer;
    ServerExplorer* serverExplorer;
    SlidingStackedWidget* mainContent;
    DownloadManager* downloadManager;
    // Layouts
    QVBoxLayout* mainLayout;
    QHBoxLayout* topLayout;
    QHBoxLayout* topLeftLayout;
    QHBoxLayout* topMiddleLayout;
    QHBoxLayout* topRightLayout;
    QHBoxLayout* bottomLayout;
    QHBoxLayout* bottomLeftLayout;
    QHBoxLayout* bottomRightLayout;

    bool max; // For Window Maximising with frameless helper
    int animTime; // For setting animation times on slides
    bool switched;
    QString bookmarks;

    // SFTP Session
    std::string host;
    std::string password;
    std::string user;
    std::string port;
    //sftp_session sftp;
    void connectSftp();

protected:
    void createGuiComponents();
    void createMainLayout();

private slots:
    void maxSize();
    void aboutClick();

public slots:
    void switchSlides();
    void recieveCredentials(std::string host, std::string user, std::string password, std::string port);
    void recieveDropData(QString type, QString source, QString destination, QString stfpType);

signals:
    void sendCredentials(std::string host, std::string user, std::string password, std::string port);

};

#endif // COMET_MAINWINDOW_H
