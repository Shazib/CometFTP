#ifndef COMER_LOCALEXPLORER_H
#define COMET_LOCALEXPLORER_H

#include <QWidget>
#include <QtWidgets>
#include "AddressBar.h"

class LocalExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit LocalExplorer(QWidget* parent = 0);
    QTableView* table;


private:
    QFileSystemModel* model;
    AddressBar* addressBar;
    QString mainDir;
signals:

public slots:

protected:

private slots:
    void updatedPath(QString Path);
    void rowSelected(const QModelIndex indx);



};

#endif // COMET_LOCALEXPLORER_H
