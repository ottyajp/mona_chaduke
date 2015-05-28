#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting_window.h"
#include "user_profile_window.h"
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebView>
#include <QList>
#include <QWebFrame>
#include <QWebElement>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->topic->setHtml("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"></head><body><h1>Hello World!</h1><ul class=\"list\"><li>test</li></ul></body></html>");

    ui->topic_list->setColumnCount(6);
    ui->topic_list->setHeaderLabels(QStringList()<<"ID"<<tr("Title")<<tr("count")<<tr("rank")<<tr("updated")<<tr("modified"));
    ui->topic_list->setColumnWidth(0,80);
    ui->topic_list->setColumnWidth(1,400);
    ui->topic_list->setColumnWidth(2,50);
    ui->topic_list->setColumnWidth(3,50);
    ui->topic_list->setColumnWidth(4,80);
    ui->topic_list->setColumnWidth(5,80);

    get_topic_limit = 10;
    get_res_limit = 10;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTopicItem(QJsonValue topic_list_object){
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->topic_list);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0,QString::number(topic_list_object.toObject().value("t_id").toInt()));
    treeItem->setText(1,topic_list_object.toObject().value("title").toString());
    treeItem->setText(2,QString::number(topic_list_object.toObject().value("count").toInt()));
    treeItem->setText(3,QString::number(topic_list_object.toObject().value("rank").toInt()));
    treeItem->setText(4,QString::number(topic_list_object.toObject().value("updated").toInt()));
    treeItem->setText(5,QString::number(topic_list_object.toObject().value("modified").toInt()));
}

QString knock_api(QString api_name, QUrlQuery api_query){

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QString address = "http://askmona.org/v1/" + api_name;
    QUrl url(address);
    //QNetworkRequest req( QUrl( QString("http://askmona.org/v1/auth/secretkey") ) );
    url.setQuery(api_query);
    QNetworkRequest req(url);
    QByteArray data;
    data.append(api_query.toString());
    QNetworkReply *reply = mgr.post(req,data);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
//        qDebug() << "Success" <<reply->readAll();
        QString r = reply->readAll();
        delete reply;
        return r;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        QString r = "1";
        delete reply;
        return r;
    }
}

void MainWindow::on_action_Config_triggered()
{
    Setting_window *window = new Setting_window(this);
    window->setModal(1);
    window->show();
}

void MainWindow::on_action_Quit_triggered()
{
    close();
}

void MainWindow::on_actionGet_profile_triggered()
{
    User_Profile_window *window = new User_Profile_window(this);
    window->setModal(1);
    window->show();
}

void MainWindow::on_action_Get_topic_list_triggered()
{
    QString api_name = "topics/list";
    QUrlQuery api_query;
    api_query.addQueryItem("limit",QString::number(get_topic_limit));
    QString key = knock_api(api_name,api_query);
//    qDebug()<<key;
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toString() == "0"){
        qDebug()<<"error";
//        ui->profile->setText(json.object().value("error").toString());
    }else{
        QJsonArray item = json.object().value("topics").toArray();
        qDebug()<<item;
        QString topic_list_object;
        QList<QTreeWidgetItem*> check_topic;
        for(int i=0; i<get_topic_limit; i++){
            qDebug()<<item.at(i);
            check_topic = ui->topic_list->findItems(QString::number(item.at(i).toObject().value("t_id").toInt()),Qt::MatchContains,0);
            qDebug()<<"check_topic is"<<check_topic;
            if(check_topic.count() != 0){
                check_topic.clear();
                continue;
            }else{
                MainWindow::addTopicItem(item.at(i));
            }
        }
    }
}

void MainWindow::on_topic_list_itemDoubleClicked(QTreeWidgetItem *item)
{
//        ui->topic->load(QUrl("http://askmona.org/"+item->text(0)));
    QString api_name = "responses/list";
    QUrlQuery api_query;
    api_query.addQueryItem("t_id",item->text(0));
    api_query.addQueryItem("from","1");
    api_query.addQueryItem("to",QString::number(get_res_limit));
    QString key = knock_api(api_name,api_query);
    qDebug()<<key;
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->topic->setHtml("<h2>error</h2><BR>"+json.object().value("error").toString());
    }else{
            //load template file
        QString source;
        QFile file("./template.html");
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<"auti";
        }
        QTextStream in(&file);
        source = in.readAll();
        ui->topic->setHtml(source);
            //set title
        QWebElement title;
        title = ui->topic->page()->mainFrame()->findFirstElement("div.title");
        title.appendInside(item->text(1));
            //set responses
        QJsonArray res = json.object().value("responses").toArray();
        QWebElement list;
        for (int i = 0; i < get_res_limit; i++){
            list = ui->topic->page()->mainFrame()->findFirstElement("div.responses");
            QString response = QString::number(res.at(i).toObject().value("r_id").toInt()) + " " +
                    res.at(i).toObject().value("u_name").toString() +
                    res.at(i).toObject().value("u_dan").toString() + " : " +
                    QString::number(res.at(i).toObject().value("created").toInt()) + " [" +
                    res.at(i).toObject().value("u_times").toString() + "] +" +
                    res.at(i).toObject().value("receive").toString() + "watanabe / " +
                    QString::number(res.at(i).toObject().value("res_count").toInt()) + tr("man") +
                    res.at(i).toObject().value("response").toString();
            list.appendInside("<div class=\"response\">"+response+"</div>");
//            qDebug()<<response;
        }
    }
}
