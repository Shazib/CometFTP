#ifndef COMET_SERVERFILEMODEL_H
#define COMET_SERVERFILEMODEL_H

#include <QAbstractTableModel>
#include <QtWidgets>
#include <QMimeData>
class ServerFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ServerFileModel(QObject* parent = 0, int numRows = 0, QStringList dataList = QStringList(), QString _path = "");

    // Drag And Drop
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDragActions() const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);




    // Default Model Actions

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QStringList dataList_;
    int row;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QString path;
    QString source;




signals:
    void sendDropData(QString type, QString source, QString destination, QString sftpType);

public slots:

};

#endif // COMET_SERVERFILEMODEL_H

