#ifndef COMET_CUSTOMFILEMODEL_H
#define COMET_CUSTOMFILEMODEL_H

#include <QFileSystemModel>

#include <QtWidgets>

class CustomFileModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit CustomFileModel(QObject *parent = 0);

    // The only methods that needs reimplementing
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;

    Qt::DropActions supportedDropActions() const;
signals:
    void sendDropData(QString type, QString source, QString destination,QString sftpType);

public slots:

protected:
};

#endif // COMET_CUSTOMFILEMODEL_H
