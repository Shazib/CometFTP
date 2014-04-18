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

    //QString cssString("");
    QFile cssFile(":/Main.css");
    cssFile.open(QFile::ReadOnly);
    QString cssString = QLatin1String(cssFile.readAll());

   // QTextStream cssStream(&cssFile);

   // while (!cssStream.atEnd()) {
   //     cssString = cssString.append(cssStream.readLine());
    //    cssString = cssString.append("\n");
  //  }

    qDebug() << cssString;
    a.setStyleSheet(cssString);

    qDebug() << "Activated Helper";
    w.show();

    return a.exec();
}
