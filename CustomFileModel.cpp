/*
 * This Class is a reimplementation of QFileSystemModel.
 * The default class provides all required functionality by default, and as such,
 * handles drop events internally.
 *
 * Files draged from the server technically do not exist on the local system
 * The default model does not know how to handle the custom mime type
 * It does not need to try and move/copy the files
 * It just needs to send the data to the downloader class to add to the queue.
 */
#include "CustomFileModel.h"

CustomFileModel::CustomFileModel(QObject *parent) :
    QFileSystemModel(parent)
{


}
// Returns the mime types that this class supports for drops
QStringList CustomFileModel::mimeTypes() const
{
    QStringList Temp = QStringList(QLatin1String("text/uri-list"));
    Temp << "text/comet-upload-download";

    return Temp;
}

// Called when data is dropped
bool CustomFileModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(data->data("text/comet-upload-download").count() > 0) {
        qDebug() << "Comet file";
        // Emit the signal
        emit sendDropData("Download",QString(data->data("text/comet-upload-download")),filePath(parent),data->text());
        return true;
    } else {



    // The Default Implementation from QFileSystemModel.cpp for copying normal mime types.
    Q_UNUSED(row);
    Q_UNUSED(column);
    if (!parent.isValid() || isReadOnly())
        return false;

    bool success = true;
    QString to = filePath(parent) + QDir::separator();

    QList<QUrl> urls = data->urls();
    QList<QUrl>::const_iterator it = urls.constBegin();

    switch (action) {
    case Qt::CopyAction:
        for (; it != urls.constEnd(); ++it) {
            QString path = (*it).toLocalFile();
            success = QFile::copy(path, to + QFileInfo(path).fileName()) && success;
          }
        break;
    case Qt::LinkAction:
        for (; it != urls.constEnd(); ++it) {
            QString path = (*it).toLocalFile();
            success = QFile::link(path, to + QFileInfo(path).fileName()) && success;
        }
        break;
    case Qt::MoveAction:
        for (; it != urls.constEnd(); ++it) {
            QString path = (*it).toLocalFile();
            success = QFile::copy(path, to + QFileInfo(path).fileName())
            && QFile::remove(path) && success;
        }
        break;
    default:
        return false;
    }
    return success;
    }

}

Qt::DropActions CustomFileModel::supportedDropActions() const
{
return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

