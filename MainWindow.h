#ifndef COMET_MAINWINDOW_H
#define COMET_MAINWINDOW_H

// Qt
#include <QFrame>
#include <QtWidgets>

// Libraries
#include <libssh/libssh.h>
#include <libssh/sftp.h>

// Classes

class MainWindow : public QFrame
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    // Widgets
    QPushButton* btnQuit;
    QPushButton* btnMax;
    QPushButton* btnMin;
    QPushButton* btnMenu;

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
    AddressBar* bar;
    void createGuiComponents();
    void createMainLayout();

    // SFTP Session
    std::string host;
    std::string password;
    std::string user;
    std::string port;
    sftp_session sftp;
    void connectSftp();

protected:
    // Keyboard Mouse Handling
    void mouseMoveEvent(QMouseEvent* mouseEvent);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private slots:

public slots:
    void connectClicked();

};

#endif // COMET_MAINWINDOW_H
