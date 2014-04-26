#include "MainWindow.h"
#include <QApplication>
#include "NcFramelessHelper.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    qDebug() << "Started";
    QApplication a(argc, argv);
    MainWindow w;
    NcFramelessHelper fh;
    fh.activateOn(&w);

    QCoreApplication::setOrganizationName("BCU");
    QCoreApplication::setApplicationName("CometFTP");
    QCoreApplication::setApplicationVersion("0.0.1");

    QFile cssFile(":/Main.css");
    cssFile.open(QFile::ReadOnly);
    QString cssString = QLatin1String(cssFile.readAll());

    a.setStyleSheet(cssString);

    qDebug() << "Activated Helper";
    w.show();

    return a.exec();
}
