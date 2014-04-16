#ifndef COMET_SERVEREXPLORER_H
#define COMET_SERVEREXPLORER_H

#include <QWidget>
#include <QtWidgets>
class ServerExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit ServerExplorer(QWidget *parent = 0);
    std::string _host;
    std::string _user;
    std::string _port;
    std::string _password;
signals:

public slots:

private slots:
    void bookmarkBtnPressed();
    void sftpBtnPressed();
    void connectBtnPressed();

private:
    QWidget* explorer;
    QWidget* siteManager;
    QWidget* bookmarkSlide;
    QWidget* connectSlide;
    void setupSiteManager();
    QPushButton* bookmarkBtn;
    QPushButton* sftpBtn;
    QPushButton* connectBtn;
    QPuchButton* connectBookmarkBtn;
    //Connect Area
    QLineEdit* host;
    QLineEdit* user;
    QLineEdit* password;
    QLineEdit* port;


};

#endif // COMET_SERVEREXPLORER_H
