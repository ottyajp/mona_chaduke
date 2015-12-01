#include "mainwindow.h"
#include "setting_window.h"
#include <QApplication>
#include <QTranslator>


int get_topic_limit;
int user_id;
QString now_topic_id;
QString secret_key;
QString post_text;
QString send_to;
QString send_mona_amount_1;
QString send_mona_amount_2;
QString send_mona_amount_3;
QString send_mona_amount_4;
QString tx_send_receive_limit;
QString tx_with_depo_limit;
bool sage_check;
QSize initial_contents_size;
int position = 0;
QStatusBar *status_bar = 0;
bool state_log_show = false;
state_log *state_log_data;

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

