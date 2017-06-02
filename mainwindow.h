#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebView>

#include "network_access.h"
#include "func_var.h"

#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString secretkey;
    QString u_id;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_Q_triggered();

    void on_action_Load_topic_list_triggered();

    void on_actionConfig_C_triggered();

    void set_secretkey_uid(QString key, QString id);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
