#ifndef COMER_LOCALEXPLORER_H
#define COMET_LOCALEXPLORER_H

#include <QWidget>
#include <QtWidgets>


// Custom Classes
#include "AddressBar.h"
#include "CustomFileModel.h"

class LocalExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit LocalExplorer(QWidget* parent = 0);
    QTableView* table;

private:    
    CustomFileModel* model; // Custom Class
    AddressBar* addressBar;
    QString mainDir;

signals:
    void sendDropData(QString type, QString source, QString desination, QString sftpType);

public slots:

protected:

private slots:
    void updatedPath(QString Path);
    void rowSelected(const QModelIndex indx);
    void receiveDropData(QString type, QString source, QString destination,QString sftpType);



};

#endif // COMET_LOCALEXPLORER_H
