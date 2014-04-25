#include "ServerFileModel.h"

#include <QStringListModel>

ServerFileModel::ServerFileModel(QObject* parent, int numRows, QStringList dataList, QString _path) :
    QAbstractTableModel(parent)
{
    dataList_ = dataList;
    row = numRows;
    path = _path;
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
        return dataList_.at(i);
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

Qt::DropActions ServerFileModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions ServerFileModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction | Qt::IgnoreAction | Qt::TargetMoveAction | Qt::LinkAction;
}

Qt::ItemFlags ServerFileModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList ServerFileModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    //return types;
    return QStringList(QLatin1String("text/uri-list"));
}

// The Data for our mime type is set.
// This passes just the url
// The local file widget needs to handle this specific mime type
QMimeData* ServerFileModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData* mimeData = new QMimeData;

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {

            QString filename = index.model()->data(index.model()->index(index.row(),0,index),Qt::DisplayRole).toString();

            QString filePath = path + filename;

            QByteArray ba = filePath.toLocal8Bit().data();

            mimeData->setData("text/comet-upload-download", ba);
            mimeData->setText(index.model()->data(index.model()->index(index.row(),4,index),Qt::DisplayRole).toString());
            return mimeData;
        }
        else{
            return mimeData;
        }
    }

}

bool ServerFileModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    QString recipientPath;
    // Get The Path of the destination
    // Setup the recipients path
    if (parent.isValid()){
        // Check if item was dropped onto a folder or a file
        if (parent.model()->data(parent.model()->index(parent.row(),4,parent),Qt::DisplayRole).toString() == QString("Folder")){
            qDebug() << "Dropped onto Folder";

            // Get path of folder
            recipientPath = path + parent.model()->data(parent.model()->index(parent.row(),0,parent),Qt::DisplayRole).toString() + "/";
            qDebug() << "PATH  " << recipientPath;

        }
        // Dropped onto file
        else {
            recipientPath = path;
            qDebug() << "PATH  " << recipientPath;
        }
    }else {
        // Dropped on blank space
        recipientPath = path;
        qDebug() << "PATH  " << recipientPath;
    }

    // Get the path of the source file/folder

    QList<QUrl> urls = data->urls();
    QList<QUrl>::const_iterator it = urls.constBegin();

    for (; it != urls.constEnd(); ++it) {
        QString _path = (*it).toLocalFile();
        //success = QFile::copy(path, to + QFileInfo(path).fileName()) && success;
        qDebug() << "PATH OF SOURCE" << _path;
        source = _path;
    }

    // Now The source + destination path's must be sent to the download manager, along with a flag that this is a upload action.
    emit sendDropData("Upload",source,recipientPath,"");



/*
    QByteArray encodedData = data->data("text/plain");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString temp;
    data->urls();
    stream >> temp;
    //QFile file(data->urls().b);
    if (file.exists()){
        qDebug() << "file exist";
    }
    QFileInfo info(file);
    if (info.isDir()){
       qDebug() << "Is a dir";
    }
    qDebug() << "TEMP" << data->urls() << "  " << data->text();
    */
    return true;
}

bool ServerFileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "TESTING";
    return true;
}
bool ServerFileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "INSERTED";
    return true;
}









