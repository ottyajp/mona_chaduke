#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network_access.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWebView *view = new QWebView();
    view->setUrl(QUrl("http://google.com/"));
    ui->topic_tab_widget->addTab(view, "test");
//    QUrlQuery query;
//    QJsonDocument json = QJsonDocument::fromJson(access_get("topics/list",query).toUtf8());
//    qDebug()<<json;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_Q_triggered()
{
    QApplication::quit();
    QCoreApplication::quit();
}
