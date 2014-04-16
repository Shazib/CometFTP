#ifndef COMER_LOCALEXPLORER_H
#define COMET_LOCALEXPLORER_H

#include <QWidget>

class LocalExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit LocalExplorer(QWidget* parent = 0);
    QString* mainDir;

signals:

public slots:

protected:


};

#endif // COMET_LOCALEXPLORER_H
