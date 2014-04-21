#ifndef COMET_SERVERFILEMODEL_H
#define COMET_SERVERFILEMODEL_H

#include <QAbstractTableModel>

class ServerFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ServerFileModel(QObject* parent = 0, int numRows = 0, QStringList dataList = QStringList());
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QStringList dataList_;
    int row;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
signals:

public slots:

};

#endif // COMET_SERVERFILEMODEL_H

