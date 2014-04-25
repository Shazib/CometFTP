#ifndef COMET_STATUSAREA_H
#define COMET_STATUSAREA_H

#include <QWidget>
#include <QtWidgets>

class StatusArea : public QWidget
{
    Q_OBJECT
public:
    explicit StatusArea(QWidget *parent = 0);

private:
    QLabel* file;
    QPushButton* numFiles;
    QPushButton* cancel;
    QPushButton* pause;
    QLabel *speed;
    QProgressBar* progress;
signals:
    void cancelPushed(bool pushed);
    void pausePushed(bool pushed);

public slots:
    void paintEvent(QPaintEvent* event);
    void setFileName(QString fileName);
    void setNumFiles(QString _numFiles);
    void setSpeed(QString _speed);
    void setProgress(int progressValue);


};

#endif // COMET_STATUSAREA_H
