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

    bool max;
    int animTime;



    // SFTP Session
    std::string host;
    std::string password;
    std::string user;
    std::string port;
    sftp_session sftp;
    void connectSftp();

protected:
    // Keyboard Mouse Handling
    //void mouseMoveEvent(QMouseEvent* mouseEvent);
    //void keyPressEvent(QKeyEvent* event);
    //void keyReleaseEvent(QKeyEvent* event);
    void createGuiComponents();
    void createMainLayout();
private slots:

public slots:
    //void connectClicked();

};

#endif // COMET_MAINWINDOW_H
