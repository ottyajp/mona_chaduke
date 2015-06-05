#include "mainwindow.h"
#include "setting_window.h"
#include <QApplication>
#include <QTranslator>

int get_topic_limit;
int get_res_limit;
int user_id;
QString now_topic_id;
QString secret_key;
QString post_text;
bool sage_check;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load(":/ja");
    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.setWindowTitle(QObject::tr("Mona Cha-du-ke!"));
    w.show();

    return a.exec();
}

