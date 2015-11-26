#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "func_var.h"

#include <QMainWindow>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeWidget>
#include <QSettings>
#include <QUrl>
#include <QJsonArray>
#include <QWebView>
#include <QList>
#include <QWebFrame>
#include <QWebElement>
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDateTime>
#include <QCloseEvent>
#include <QThread>

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

    void favorite_topic_reload_signal();

    void open_profile_window_from_topic();

public slots:
    void topic_reload();

    void topic_reload_signal_fire();

    void favorite_topic_reload();

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

    void open_profile_window(QString);

    void check_contents_size();

    void on_add_favorite_button_clicked();

    void on_remove_favorite_button_clicked();

    void on_actionGet_Favorite_topic_list_triggered();

    void on_actionForced_to_reload_triggered();

    void on_action_withdraw_balance_triggered();

    void on_actionWithdraw_deposit_history_triggered();

    void on_actionSend_receive_h_istory_triggered();

    void on_action_Create_new_topic_triggered();

private:
    Ui::MainWindow *ui;

    void addTopicItem(QJsonValue topic_list_object, int indexof_fav_ask);
    void load_Favorite_topics();
    void readSettings();
    void closeEvent(QCloseEvent* event);
};


#endif // MAINWINDOW_H
