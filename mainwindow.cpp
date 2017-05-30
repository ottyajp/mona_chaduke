#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network_access.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/icon.ico"));

    ui->topic_list->setColumnWidth(0, 70); //id
    ui->topic_list->setColumnWidth(1, 40); //rank
    ui->topic_list->setColumnWidth(2, 800);//title
    ui->topic_list->setColumnWidth(3, 40); //count
    ui->topic_list->setColumnWidth(4, 100);//updated
    ui->topic_list->setColumnWidth(5, 100);//created
    ui->topic_list->setColumnWidth(6, 100);//category
    ui->topic_list->setColumnWidth(7, 100);//tags
    QWebView *view = new QWebView();
    view->setUrl(QUrl("http://google.com/"));
    ui->topic_tab_widget->addTab(view, "test");
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

void MainWindow::on_action_Load_topic_list_triggered()
{
    QUrlQuery query;
    query.addQueryItem("limit","10");
    QJsonDocument json = QJsonDocument::fromJson(access_get("topics/list", query).toUtf8());
    if(json.object().value("status").toString() == "0"){
        qDebug()<<json.object().value("error").toString();
    }else{
        ui->topic_list->clear();
        QJsonArray topics = json.object().value("topics").toArray();
        for(int i=0; i<10; i++){
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->topic_list);
            item->setText(0, QString::number(topics.at(i).toObject().value("t_id").toInt()));
            item->setText(1, QString::number(topics.at(i).toObject().value("rank").toInt()));
            item->setText(2, topics.at(i).toObject().value("title").toString());
            item->setText(3, QString::number(topics.at(i).toObject().value("count").toInt()));
            item->setText(4, QString::number(topics.at(i).toObject().value("updated").toInt()));
            item->setText(5, QString::number(topics.at(i).toObject().value("created").toInt()));
            item->setText(6, topics.at(i).toObject().value("category").toString());
            item->setText(7, topics.at(i).toObject().value("tags").toString());
        }
    }
}
