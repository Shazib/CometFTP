#ifndef COMET_STATUSAREA_H
#define COMET_STATUSAREA_H

#include <QWidget>

class StatusArea : public QWidget
{
    Q_OBJECT
public:
    explicit StatusArea(QWidget *parent = 0);

signals:

public slots:
    void paintEvent(QPaintEvent* event);

};

#endif // COMET_STATUSAREA_H
