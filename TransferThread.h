#ifndef COMET_TRANSFERTHREAD_H
#define COMET_TRANSFERTHREAD_H

#include <QThread>

class TransferThread : public QThread
{
    Q_OBJECT
public:
    explicit TransferThread(QObject *parent = 0);

signals:

public slots:

protected:
    void run();

};

#endif // COMET_TRANSFERTHREAD_H
