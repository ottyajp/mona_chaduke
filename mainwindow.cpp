#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting_window.h"
#include "user_profile_window.h"
#include "post_message.h"
#include "send_mona_to_res_window.h"
#include "jsobj.h"
#include "image_window.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->topic->setHtml("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"></head><body>Mona_chaduke_top</body></html>");

    ui->topic_list->setColumnCount(6);
    ui->topic_list->setHeaderLabels(QStringList()<<"ID"<<tr("Title")<<tr("count")<<tr("rank")<<tr("updated")<<tr("modified"));
    ui->topic_list->setColumnWidth(0,60);
    ui->topic_list->setColumnWidth(1,340);
    ui->topic_list->setColumnWidth(2,40);
    ui->topic_list->setColumnWidth(3,40);
    ui->topic_list->setColumnWidth(4,130);
    ui->topic_list->setColumnWidth(5,130);

    MainWindow::readSettings();

    QObject::connect(this,SIGNAL(topic_reload_signal()),this,SLOT(topic_reload()));

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
    treeItem->setText(4,from_unix_time(topic_list_object.toObject().value("updated").toInt()));
    treeItem->setText(5,from_unix_time(topic_list_object.toObject().value("modified").toInt()));
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

void MainWindow::open_image_window(QString s){
    send_to = s;
    image_Window *window = new image_Window(this);
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
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toString() == "0"){
        qDebug()<<"error";
    }else{
        QJsonArray item = json.object().value("topics").toArray();
        QString topic_list_object;
        QList<QTreeWidgetItem*> check_topic;
        for(int i=0; i<get_topic_limit; i++){
            check_topic = ui->topic_list->findItems(QString::number(item.at(i).toObject().value("t_id").toInt()),Qt::MatchContains,0);
            if(check_topic.count() != 0){
                check_topic.at(0)->setText(2, QString::number(item.at(i).toObject().value("count").toInt()));
                check_topic.at(0)->setText(3, QString::number(item.at(i).toObject().value("rank").toInt()));
                check_topic.at(0)->setText(4, from_unix_time(item.at(i).toObject().value("updated").toInt()));
                check_topic.at(0)->setText(5, from_unix_time(item.at(i).toObject().value("modified").toInt()));
                check_topic.clear();
                continue;
            }else{
                MainWindow::addTopicItem(item.at(i));
            }
        }
        ui->topic_list->sortByColumn(4,Qt::DescendingOrder);
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
                received_mona = "<span class=\"mona_yay\">+" +QString::number(receive,'f',8) + "MONA</b> / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }else{
                received_mona = "+" + QString::number(receive) + "MONA / " +
                        QString::number(res.at(i).toObject().value("rec_count").toInt()) + tr("man") + "</span>";
            }
            QString replace_imgur = res.at(i).toObject().value("response").toString();
            replace_imgur.replace(QRegularExpression("(http://i.imgur.com/.+)(..{3})"),
                                  "<span onclick=\"onClick_image('\\1\\2');\"><img src=\"\\1m\\2\" class=\"imgur\"></span>");
            QString created = from_unix_time(res.at(i).toObject().value("created").toInt());

            list = ui->topic->page()->mainFrame()->findFirstElement("div.responses");
            QString response = QString::number(res.at(i).toObject().value("r_id").toInt()) + " : " +
                    res.at(i).toObject().value("u_name").toString() +
                    res.at(i).toObject().value("u_dan").toString() + " : " +
                    created + " [" +
                    res.at(i).toObject().value("u_times").toString() + "] " +
                    received_mona +
                    " <span onclick=\"onClick_send_mona_link(" + QString::number(res.at(i).toObject().value("r_id").toInt()) +
                    ");\" class=\"send_mona\">" + tr("send mona") + "</span><BR>" +
                    "<span class=\"level" + QString::number(res.at(i).toObject().value("res_lv").toInt()) + "\">" +
                    replace_imgur +
                    "</span>";
            list.appendInside("<div class=\"response\">"+response+"</div>");
//            qDebug()<<response;
        }
        JsObj *jo = new JsObj();
        ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
        QObject::connect(jo,SIGNAL(send_mona_to_res_signal(QString)),this,SLOT(on_action_send_mona_to_res_clicked(QString)));
        QObject::connect(jo,SIGNAL(open_image_window_signal(QString)),this,SLOT(open_image_window(QString)));
    }
    ui->topic->setFocus();
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("about mona chaduke"),
                       tr("<h3>mona chaduke</h3>"
                          "<p><img src=\":/mona.png\"></p>"
                          "<p>Donate:M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz</p>"));
}

void MainWindow::on_call_post_window_clicked()
{
    post_message *window = new post_message(this);
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
    QString key = knock_api(api_name,api_query);
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
            f = 1;
        }
        pos = hash.toBase64().toStdString().find("+",0);
        if(pos == -1){
            f = 1;
        }else{
            f = 0;
        }
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


void MainWindow::on_actionGet_deposit_address_triggered()
{
    auth_Key auth_key;
    QString api_name = "account/deposit";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        qDebug()<<"error";
        qDebug()<<json.object().value("error").toString();
    }else{
        QMessageBox::information(this,tr("your deposit address"),tr("your deposit address is\n\n") +
                                 json.object().value("d_address").toString());
    }
}

void MainWindow::on_topic_list_itemActivated(QTreeWidgetItem *item)
{
    this->on_topic_list_itemDoubleClicked(item);
}

void MainWindow::topic_reload(){
    QPoint pos = ui->topic->page()->mainFrame()->scrollPosition();
    this->on_topic_list_itemDoubleClicked(ui->topic_list->selectedItems().at(0));
//    ui->topic->page()->mainFrame()->scroll(0,pos.y());
    QThread::sleep(1);
    ui->topic->page()->mainFrame()->setScrollPosition(QPoint(0,pos.y()));
    qDebug()<<pos.y();
    pos = ui->topic->page()->mainFrame()->scrollPosition();
    qDebug()<<pos.y();
}

void MainWindow::topic_reload_signal_fire(){
    emit topic_reload_signal();
}
