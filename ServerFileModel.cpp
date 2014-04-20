#include "ServerFileModel.h"

ServerFileModel::ServerFileModel(QObject* parent, int numRows, QStringList* dataList) :
    QAbstractTableModel(parent)
{
    dataList_ = dataList;
    row = numRows;
}

int ServerFileModel::columnCount(const QModelIndex &parent) const{
    return 5;
}
int ServerFileModel::rowCount(const QModelIndex &parent) const{
    return row;
}
QVariant ServerFileModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole){
        int i =(index.row()*5)+(index.column());
        return dataList_->at(i);
    }
    else
        return QVariant();
}

QVariant ServerFileModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            switch (section)
            {
            case 0:
                return QString("Name");
            case 1:
                return QString("Size");
            case 2:
                return QString("Permissions");
            case 3:
                return QString("Owner");
            case 4:
                return QString("Type");
            }
        }
    }
    return QVariant();
}
