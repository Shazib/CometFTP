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

private:
    QPushButton* backBtn;
    QPushButton* forwardBtn;
    QLineEdit* addressEdit;
    QPushButton* disconnectBtn;
    QString defaultPath;
    void setupView();
    bool disconnectVisible;

signals:

public slots:

};

#endif // COMET_ADDRESSBAR_H
