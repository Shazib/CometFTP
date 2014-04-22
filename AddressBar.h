#ifndef COMET_ADDRESSBAR_H
#define COMET_ADDRESSBAR_H

#include <QWidget>
#include <QtWidgets>
class AddressBar : public QWidget
{
    Q_OBJECT
public:
    explicit AddressBar(QWidget *parent = 0,
                        bool _disconnectVisible = true,
                        QString _defaultPath = "");
    void updatePath(QString path);

private:
    QPushButton* backBtn;
    QPushButton* forwardBtn;
    QLineEdit* addressEdit;
    QPushButton* disconnectBtn;
    QString defaultPath;
    QStringList backList;
    QStringList forwardList;
    void setupView();
    bool disconnectVisible;



signals:
    void updatedPath(QString path);
    void disconnect();

public slots:

private slots:
    void backButtonClick();
    void forwardButtonClick();
    void addressEditClick();
    void disconnectClicked();

};

#endif // COMET_ADDRESSBAR_H
