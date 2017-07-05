#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QWebEngineView>

#include "network_access.h"
#include "func_var.h"
#include "topic_view.h"

#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString secretkey;
    QString u_id;
    QString topics_limit;
    QString responses_limit;
    topic_view *back_topic;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_Q_triggered();

    void on_action_Load_topic_list_triggered();

    void on_actionConfig_C_triggered();

    void saveSettings(QMap<QString, QString> data);

    void loadSettings();

    void set_secretkey_uid(QString key, QString id);

    void on_topic_list_itemActivated(QTreeWidgetItem *item);

    void on_topic_tab_widget_tabCloseRequested(int index);

    void on_actionLoad_Favorite_topic_list_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
