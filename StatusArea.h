#ifndef COMET_STATUSAREA_H
#define COMET_STATUSAREA_H

#include <QWidget>
#include <QtWidgets>

class StatusArea : public QWidget
{
    Q_OBJECT
public:
    explicit StatusArea(QWidget *parent = 0);
    QPushButton* cancel;
    QPushButton* pause;
private:
    QLabel* file;
    QPushButton* numFiles;

    QLabel *speed;
    QProgressBar* progress;

signals:


public slots:
    void paintEvent(QPaintEvent* event);
    void setFileName(QString fileName);
    void setNumFiles(int _numFiles);
    void setSpeed(QString _speed);
    void setProgress(int progressValue);


};

#endif // COMET_STATUSAREA_H
