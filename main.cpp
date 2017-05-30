#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    QTranslator tr;
    qDebug()<<tr.load(":/lang/ja");
    a.installTranslator(&tr);
    return a.exec();
}
