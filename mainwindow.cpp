#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting_window.h"
#include "user_profile_window.h"
#include "post_message.h"
#include "send_mona_to_res_window.h"
#include "jsobj.h"
#include <QtGlobal>
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
#include <QCryptographicHash>
#include <QDateTime>
#include <QCloseEvent>

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
    send_mona_amount_1 = settings.value("send_mona_amount_1").toString();
    send_mona_amount_2 = settings.value("send_mona_amount_2").toString();
    send_mona_amount_3 = settings.value("send_mona_amount_3").toString();
    send_mona_amount_4 = settings.value("send_mona_amount_4").toString();
}

void MainWindow::closeEvent(QCloseEvent* event){
    QSettings settings("tea_soak_lab", "mona_chaduke");
    settings.setValue("get_topic_limit", get_topic_limit);
    settings.setValue("get_res_limit", get_res_limit);
    settings.setValue("user_id", user_id);
    settings.setValue("secret_key", secret_key);
    settings.setValue("send_mona_amount_1", send_mona_amount_1);
    settings.setValue("send_mona_amount_2", send_mona_amount_2);
    settings.setValue("send_mona_amount_3", send_mona_amount_3);
    settings.setValue("send_mona_amount_4", send_mona_amount_4);
    event->accept();
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
//    url.setQuery(api_query);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    data.append(api_query.toString());
    qDebug()<<"data:"<<data;
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

QString knock_api_get(QString api_name, QUrlQuery api_query){

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
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(req);
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

void MainWindow::on_action_send_mona_to_res_clicked(QString s){
    send_to = s;
    send_mona_to_res_window *window = new send_mona_to_res_window(this);
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
    now_topic_id = item->text(0);
    QString api_name = "responses/list";
    QUrlQuery api_query;
    api_query.addQueryItem("t_id",item->text(0));
    api_query.addQueryItem("from","1");
    api_query.addQueryItem("to",QString::number(get_res_limit));
    QString key = knock_api_get(api_name,api_query);
    qDebug()<<key;
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->topic->setHtml("<h2>error</h2><BR>"+json.object().value("error").toString());
    }else{
            //load template file
        QString source;
        QFile file("./template.html");
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<tr("can't open template file.");
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
        double receive;
        for (int i = 0; i < get_res_limit; i++){
            if(res.at(i).toObject().value("u_name").toString() == ""){
                break;
            }
            QString received_mona;
            receive = res.at(i).toObject().value("receive").toString().toDouble() / 100000000;
            if(res.at(i).toObject().value("receive").toString()!="0"){
                received_mona = "<span class=\"mona_yay\">+" +QString::number(receive) + "MONA</b> / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }else{
                received_mona = "+" + QString::number(receive) + "MONA / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }

            list = ui->topic->page()->mainFrame()->findFirstElement("div.responses");
            QString response = QString::number(res.at(i).toObject().value("r_id").toInt()) +
                    res.at(i).toObject().value("u_name").toString() +
                    res.at(i).toObject().value("u_dan").toString() + " : " +
                    QString::number(res.at(i).toObject().value("created").toInt()) + " [" +
                    res.at(i).toObject().value("u_times").toString() + "] " +
                    received_mona +
                    " <span onclick=\"onClick_send_mona_link(" + QString::number(res.at(i).toObject().value("r_id").toInt()) +
                    ");\" class=\"send_mona\">" + tr("send mona") + "</span><BR>" +
                    "<span class=\"level" + QString::number(res.at(i).toObject().value("res_lv").toInt()) + "\">" +
                    res.at(i).toObject().value("response").toString() +
                    "</span>";
            list.appendInside("<div class=\"response\">"+response+"</div>");
            JsObj *jo = new JsObj();
            ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
            QObject::connect(jo,SIGNAL(send_mona_to_res_signal(QString)),this,SLOT(on_action_send_mona_to_res_clicked(QString)));
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

void MainWindow::on_call_post_window_clicked()
{
    post_message *window = new post_message(this);
    window->setModal(1);
    window->show();
}

void MainWindow::on_actionGet_balance_triggered()
{
    auth_Key auth_key;
    QString api_name = "account/balance";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    qDebug()<<"nonce:"<<auth_key.read_nonce()<<", time:"<<auth_key.read_time()<<", auth_key:"<<auth_key.read_auth_key();
    QString key = knock_api(api_name,api_query);
    qDebug()<<key;
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        qDebug()<<"error";
        qDebug()<<json.object().value("error").toString();
    }else{
        double balance = json.object().value("balance").toString().toDouble();
        balance = balance / 100000000;
        QMessageBox::information(this,tr("balance"),tr("your balance is\n")+QString::number(balance));
    }
}

auth_Key::auth_Key(){
    time = QString::number(QDateTime::currentDateTime().toTime_t());
    int f = 0;
    int pos;
    while(1){
        qsrand(QDateTime::currentDateTime().toTime_t());
        for(int i=0; i<5; i++){
            nonce.append(QString(qrand()%256));
        }

        hash = QCryptographicHash::hash("Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=" +
                                        nonce.toBase64() +
                                        time.toUtf8() +
                                        secret_key.toUtf8(),QCryptographicHash::Sha256);
        pos = nonce.toBase64().toStdString().find("+",0);
        if(pos == -1){
            qDebug()<<nonce.toBase64();
            f = 1;
        }
        pos = hash.toBase64().toStdString().find("+",0);
        if(pos == -1){
            qDebug()<<hash.toBase64();
            f = 1;
        }else{
            f = 0;
        }
        qDebug()<<hash.toBase64();
        if(f==1){
            break;
        }
    }
}

QString auth_Key::read_time(){
    return time;
}

QString auth_Key::read_nonce(){
    return nonce.toBase64();
}

QString auth_Key::read_auth_key(){
    return hash.toBase64();
}

