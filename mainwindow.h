#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeWidget>
#include <QSettings>

QString knock_api(QString, QUrlQuery);
QString knock_api_get(QString, QUrlQuery);
QString from_unix_time(int);
extern int get_topic_limit;
extern int get_res_limit;
extern int user_id;
extern QString now_topic_id;
extern QString secret_key;
extern QString post_text;
extern QString send_to;
extern QString send_mona_amount_1;
extern QString send_mona_amount_2;
extern QString send_mona_amount_3;
extern QString send_mona_amount_4;
extern bool sage_check;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void call_post_window();

signals:
    void topic_reload_signal();

public slots:
    void topic_reload();

    void topic_reload_signal_fire();

private slots:
    void on_action_Quit_triggered();

    void on_action_Config_triggered();

    void on_actionGet_profile_triggered();

    void on_action_Get_topic_list_triggered();

    void on_topic_list_itemDoubleClicked(QTreeWidgetItem *item);
    void on_action_About_triggered();

    void on_call_post_window_clicked();

    void on_actionGet_balance_triggered();

    void on_action_send_mona_to_res_clicked(QString);

    void on_actionGet_deposit_address_triggered();

    void on_topic_list_itemActivated(QTreeWidgetItem *item);

    void open_image_window(QString);

private:
    Ui::MainWindow *ui;

    void addTopicItem(QJsonValue topic_list_object);
    void readSettings();
    void closeEvent(QCloseEvent* event);
};

class auth_Key{
private:
    QString time;
    QByteArray nonce;
    QByteArray hash;
public:
    auth_Key();
    QString read_time();
    QString read_nonce();
    QString read_auth_key();
};

class Nonce{
private:
    QByteArray nonce;
public:
    Nonce();
    QString read_Nonce();
};

#endif // MAINWINDOW_H
