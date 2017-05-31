#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator tr;
    tr.load(":/lang/ja");
    a.installTranslator(&tr);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
