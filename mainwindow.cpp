#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting_window.h"
#include "user_profile_window.h"
#include "jsobj.h"
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
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->topic->setHtml("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"></head><body>Mona_chaduke_top</body></html>");

    ui->topic_list->setColumnCount(6);
    ui->topic_list->setHeaderLabels(QStringList()<<"ID"<<tr("Title")<<tr("count")<<tr("rank")<<tr("updated")<<tr("modified"));
    ui->topic_list->setColumnWidth(0,80);
    ui->topic_list->setColumnWidth(1,400);
    ui->topic_list->setColumnWidth(2,50);
    ui->topic_list->setColumnWidth(3,50);
    ui->topic_list->setColumnWidth(4,80);
    ui->topic_list->setColumnWidth(5,80);

    MainWindow::readSettings();

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

void MainWindow::readSettings(){
    QSettings settings("tea_soak_lab", "mona_chaduke");
    get_topic_limit = settings.value("get_topic_limit").toInt();
    get_res_limit = settings.value("get_res_limit").toInt();
    user_id = settings.value("user_id").toInt();
    secret_key = settings.value("secret_key").toString();
}

void MainWindow::closeEvent(QCloseEvent* event){

    QSettings settings("tea_soak_lab", "mona_chaduke");
    settings.setValue("get_topic_limit", get_topic_limit);
    settings.setValue("get_res_limit", get_res_limit);
    settings.setValue("user_id", user_id);
    settings.setValue("secret_key", secret_key);
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
            QString received_mona;
            if(res.at(i).toObject().value("receive").toString()!="0"){
                received_mona = "<span class=\"mona_yay\">+" +res.at(i).toObject().value("receive").toString() + "watanabe</b> / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }else{
                received_mona = "+" + res.at(i).toObject().value("receive").toString() + "watanabe / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }
            list = ui->topic->page()->mainFrame()->findFirstElement("div.responses");
            QString response = QString::number(res.at(i).toObject().value("r_id").toInt()) + " " +
                    res.at(i).toObject().value("u_name").toString() +
                    res.at(i).toObject().value("u_dan").toString() + " : " +
                    QString::number(res.at(i).toObject().value("created").toInt()) + " [" +
                    res.at(i).toObject().value("u_times").toString() + "] " +
                    received_mona +
                    " <span onclick=\"jsobj.send_mona_to_res();\" class=\"send_mona\">" + tr("send mona") + "</span><BR>" +
                    res.at(i).toObject().value("response").toString();
            list.appendInside("<div class=\"response\">"+response+"</div>");
            JsObj *jo = new JsObj();
            ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
//            qDebug()<<response;
        }
    }
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("about mona chaduke"),
                       tr("<h3>mona chaduke</h3>"
                          "<p>Ah, monamona</p>"
                          "<p>Donate:M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz</p>"));
}
