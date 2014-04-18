#ifndef COMET_MAINWINDOW_H
#define COMET_MAINWINDOW_H

// Qt
#include <QFrame>
#include <QtWidgets>

// Libraries
#include <libssh/libssh.h>
#include <libssh/sftp.h>

// Classes
#include "ServerExplorer.h"
#include "LocalExplorer.h"
#include "StatusArea.h"
#include "SlidingStackedWidget.h"

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

    // SFTP Session
    std::string host;
    std::string password;
    std::string user;
    std::string port;
    sftp_session sftp;
    void connectSftp();

protected:
    void createGuiComponents();
    void createMainLayout();

private slots:
    void maxSize();

public slots:
    //void connectClicked();
    void switchSlides();

};

#endif // COMET_MAINWINDOW_H
