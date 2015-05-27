#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeWidget>
//#include <QStandardItemModel>

QString knock_api(QString, QUrlQuery);
extern int get_topic_limit;
extern QString secret_key;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Quit_triggered();

    void on_action_Config_triggered();

    void on_actionGet_profile_triggered();

    void on_action_Get_topic_list_triggered();

    void on_topic_list_itemDoubleClicked(QTreeWidgetItem *item);
private:
    Ui::MainWindow *ui;

    void addTopicItem(QJsonValue topic_list_object);
};

#endif // MAINWINDOW_H
