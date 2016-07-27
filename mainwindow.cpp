#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting_window.h"
#include "user_profile_window.h"
#include "post_message.h"
#include "send_mona_to_res_window.h"
#include "jsobj.h"
#include "image_window.h"
#include "withdraw_window.h"
#include "withdraw_deposit_history.h"
#include "send_receive_history.h"
#include "new_topic.h"
#include <QCoreApplication>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(state_log_data,SIGNAL(log_changed_signal()),this,SLOT(log_changed()));

    setWindowIcon(QIcon(QString(":/icon.ico")));
    ui->topic->setHtml("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"></head><body>Mona_chaduke_top</body></html>");
    status_bar = ui->statusBar;

    ui->topic_list->setColumnCount(5);
    ui->topic_list->setHeaderLabels(QStringList()<<"ID"<<tr("Title")<<tr("count")<<tr("rank")<<tr("updated"));
    ui->topic_list->setColumnWidth(0,70);
    ui->topic_list->setColumnWidth(1,450);
    ui->topic_list->setColumnWidth(2,40);
    ui->topic_list->setColumnWidth(3,40);
    ui->topic_list->setColumnWidth(4,130);
    QTreeWidgetItem *favorite_topic = new QTreeWidgetItem(ui->topic_list);
    favorite_topic->setText(1,tr("Favorite topics"));
    QTreeWidgetItem *askmona_topic = new QTreeWidgetItem(ui->topic_list);
    askmona_topic->setText(1,tr("AskMona topics"));

    MainWindow::readSettings();

    if(state_log_show == false){
        ui->state_log_area->hide();
    }

    QObject::connect(this,SIGNAL(favorite_topic_reload_signal()),this,SLOT(favorite_topic_reload()));
    QObject::connect(this,SIGNAL(topic_reload_signal()),this,SLOT(topic_reload()));
    QObject::connect(ui->topic->page()->mainFrame(),SIGNAL(contentsSizeChanged(QSize)),this,SLOT(check_contents_size()));

    emit favorite_topic_reload_signal();
    status_bar->showMessage(tr("mona chaduke"));
    state_log_data->add_log(tr("start-up complete"));

}

void MainWindow::log_changed(){
    ui->state_log_area->setPlainText(state_log_data->read_log());
    ui->state_log_area->moveCursor(QTextCursor::End);
}

void MainWindow::check_contents_size(){
    if(initial_contents_size != ui->topic->page()->mainFrame()->contentsSize()){
        ui->topic->page()->mainFrame()->setScrollPosition(QPoint(0,position));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTopicItem(QJsonValue topic_list_object, int indexof_fav_ask){
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0,QString::number(topic_list_object.toObject().value("t_id").toInt()));
    treeItem->setText(1,topic_list_object.toObject().value("title").toString());
    treeItem->setText(2,QString::number(topic_list_object.toObject().value("count").toInt()));
    treeItem->setText(3,QString::number(topic_list_object.toObject().value("rank").toInt()));
    treeItem->setText(4,from_unix_time(topic_list_object.toObject().value("updated").toInt()));
    ui->topic_list->invisibleRootItem()->child(indexof_fav_ask)->addChild(treeItem);
}

void MainWindow::readSettings(){
    QSettings settings("mona_chaduke.ini", QSettings::IniFormat);
    get_topic_limit = settings.value("get_topic_limit").toInt();
    user_id = settings.value("user_id").toInt();
    secret_key = settings.value("secret_key").toString();
    send_mona_amount_1 = settings.value("send_mona_amount_1").toString();
    send_mona_amount_2 = settings.value("send_mona_amount_2").toString();
    send_mona_amount_3 = settings.value("send_mona_amount_3").toString();
    send_mona_amount_4 = settings.value("send_mona_amount_4").toString();
    sage_check = settings.value("sage_check").toBool();
    tx_send_receive_limit = settings.value("tx_send_receive_limit").toString();
    tx_with_depo_limit = settings.value("tx_with_depo_limit").toString();
    if(settings.value("maximized").toString() == "true"){
        showMaximized();
    }else{
        if(settings.value("winWidth").toInt() != 0){
            resize(settings.value("winWidth").toInt(), settings.value("winHeight").toInt());
        }
    }
    if(settings.value("state_log_show").toString() == "true"){
        state_log_show = true;
    }
}

void MainWindow::closeEvent(QCloseEvent* event){
    QSettings settings("mona_chaduke.ini", QSettings::IniFormat);
    settings.setValue("get_topic_limit", get_topic_limit);
    settings.setValue("user_id", user_id);
    settings.setValue("secret_key", secret_key);
    settings.setValue("send_mona_amount_1", send_mona_amount_1);
    settings.setValue("send_mona_amount_2", send_mona_amount_2);
    settings.setValue("send_mona_amount_3", send_mona_amount_3);
    settings.setValue("send_mona_amount_4", send_mona_amount_4);
    settings.setValue("sage_check", sage_check);
    settings.setValue("tx_send_receive_limit",tx_send_receive_limit);
    settings.setValue("tx_with_depo_limit",tx_with_depo_limit);
    if(isMaximized() == true){
        settings.setValue("maximized","true");
    }else{
        settings.setValue("maximized","false");
        settings.setValue("winWidth",width());
        settings.setValue("winHeight",height());
    }
    if(state_log_show == true){
        settings.setValue("state_log_show","true");
    }else{
        settings.setValue("state_log_show","false");
    }
    event->accept();
}

void MainWindow::load_Favorite_topics(){
state_log_data->add_log(QObject::tr("loading favorite topics..."));
    auth_Key auth_key;
    QString api_name = "favorites/list";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("failed to load favorite_topics. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        QJsonArray item = json.object().value("topics").toArray();
        QString topic_list_object;
        QList<QTreeWidgetItem*> check_topic;
        for(int i=0; i<get_topic_limit; i++){
            if ( item.at(i).toObject().value("count").toInt() == 0 ) { break; }
            check_topic = ui->topic_list->findItems(QString::number(item.at(i).toObject().value("t_id").toInt()),Qt::MatchContains,0);
            if(check_topic.count() != 0){
                check_topic.at(0)->setText(2, QString::number(item.at(i).toObject().value("count").toInt()));
                check_topic.at(0)->setText(3, QString::number(item.at(i).toObject().value("rank").toInt()));
                check_topic.at(0)->setText(4, from_unix_time(item.at(i).toObject().value("updated").toInt()));
                check_topic.at(0)->setText(5, from_unix_time(item.at(i).toObject().value("modified").toInt()));
                check_topic.clear();
                continue;
            }else{
                MainWindow::addTopicItem(item.at(i),0);
            }
        }
        ui->topic_list->sortByColumn(4,Qt::DescendingOrder);
    }
    state_log_data->add_log(QObject::tr("loading favorite topics is complete."));
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

void MainWindow::open_profile_window(QString s){
    send_to = s;
    User_Profile_window *window = new User_Profile_window(this);
    window->set_u_id_from_res(s);
    window->show();
    QObject::connect(this,SIGNAL(open_profile_window_from_topic()),window,SLOT(on_pushButton_clicked()));
    emit open_profile_window_from_topic();
}

void MainWindow::on_action_Quit_triggered()
{
    close();
}

void MainWindow::on_actionGet_profile_triggered()
{
    User_Profile_window *window = new User_Profile_window(this);
    window->set_u_id_from_res("");
    window->show();
}

void MainWindow::on_action_Get_topic_list_triggered()
{
state_log_data->add_log(QObject::tr("loading topic list..."));
    QString api_name = "topics/list";
    QUrlQuery api_query;
    api_query.addQueryItem("limit",QString::number(get_topic_limit));
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toString() == "0"){
        state_log_data->add_log(QObject::tr("failed to get topic_list. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        QJsonArray item = json.object().value("topics").toArray();
        QString topic_list_object;
//        QList<QTreeWidgetItem*> check_topic;
        for (;;){
            if(ui->topic_list->invisibleRootItem()->child(1)->child(0)==0){break;}
            delete ui->topic_list->invisibleRootItem()->child(1)->child(0);
//            ui->topic_list->invisibleRootItem()->child(1)
//                ->removeChild(ui->topic_list->invisibleRootItem()->child(1)->child(0));
        }
        for(int i=0; i<get_topic_limit; i++){
            if ( item.at(i).toObject().value("count").toInt() == 0 ) { break; }
/*            check_topic = ui->topic_list->findItems(QString::number(item.at(i).toObject().value("t_id").toInt()),Qt::MatchContains,0);
            if(check_topic.count() != 0){
                check_topic.at(0)->setText(2, QString::number(item.at(i).toObject().value("count").toInt()));
                check_topic.at(0)->setText(3, QString::number(item.at(i).toObject().value("rank").toInt()));
                check_topic.at(0)->setText(4, from_unix_time(item.at(i).toObject().value("updated").toInt()));
                check_topic.at(0)->setText(5, from_unix_time(item.at(i).toObject().value("modified").toInt()));
                check_topic.clear();
                continue;
            }else{
                MainWindow::addTopicItem(item.at(i),1);
            }*/
            MainWindow::addTopicItem(item.at(i),1);
        }
        ui->topic_list->sortByColumn(4,Qt::DescendingOrder);
    }
    state_log_data->add_log(QObject::tr("loading topic list is complete"));
}

void MainWindow::on_topic_list_itemActivated(QTreeWidgetItem *item)
{
if(item->text(0) != ""){
    if (now_topic_id != ""){    //add position data to log
        QFile log_file("./log");
        if(!log_file.open(QFile::ReadOnly)){}
        QTextStream in(&log_file);
        QJsonDocument log = QJsonDocument::fromJson(in.readAll().toUtf8());
        log_file.close();
        QJsonArray log_array = log.object().value("topics").toArray();      //load log to log_array
        int indexof_topic;
        for(int i = 0; i < log_array.count(); i++){
            if (now_topic_id == log_array.at(i).toObject().value("t_id").toString()){
                indexof_topic = i;
            }
        }
        QString save_position = QString::number(ui->topic->page()->mainFrame()->scrollPosition().y());
        QJsonObject adding_log;
        adding_log["t_id"] = now_topic_id;
        adding_log["time"] = log_array.at(indexof_topic).toObject().value("time").toString();
        adding_log["text"] = ui->topic->page()->mainFrame()->toHtml();
        adding_log["position"] = save_position;
        log_array.removeAt(indexof_topic);
        log_array += adding_log;
        if(!log_file.open(QFile::ReadWrite | QFile::Truncate)){}
        QJsonObject root;
        root["topics"] = log_array;
        QJsonDocument formatted_log;
        formatted_log.setObject(root);
        QTextStream out(&log_file);
        out<<formatted_log.toJson();
    }

    ui->topic->setHtml("");
    if (now_topic_id != item->text(0)){
        now_topic_id = item->text(0);
        QFile log_file("./log");
        if(!log_file.open(QFile::ReadOnly)){}
        QTextStream in(&log_file);
        QJsonDocument log = QJsonDocument::fromJson(in.readAll().toUtf8());
        log_file.close();
        QJsonArray log_array = log.object().value("topics").toArray();
        int indexof_topic;
        bool is_there_position = false;
        for(int i = 0; i < log_array.count(); i++){
            if (now_topic_id == log_array.at(i).toObject().value("t_id").toString()){
                indexof_topic = i;
                is_there_position = true;
            }
        }
        if(is_there_position == true){
            position = log_array.at(indexof_topic).toObject().value("position").toString().toInt();
        }else{
            position = 0;
        }

    }
    QFile log_file("./log");
    if(!log_file.open(QFile::ReadOnly)){
    }
    QTextStream log_in(&log_file);
    QJsonDocument log_json = QJsonDocument::fromJson(log_in.readAll().toUtf8());
    log_file.close();
    QJsonArray log_json_array = log_json.object().value("topics").toArray();
    bool exist = false;
    int indexof_now_topic = 0;
    for(int i = 0; i < log_json_array.count(); i++){
        if (now_topic_id == log_json_array.at(i).toObject().value("t_id").toString()){
            exist = true;
            indexof_now_topic = i;
        }
    }
    if(exist == false){     //loading topic isn't exist in log
        state_log_data->add_log(QObject::tr("loading topic isn't exist in log."));
        QString api_name = "responses/list";
        QUrlQuery api_query;
        api_query.addQueryItem("t_id",item->text(0));
        api_query.addQueryItem("from","1");
        api_query.addQueryItem("to",QString::number(GET_RES_LIMIT));
        QString key = knock_api_get(api_name,api_query);
        QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
        if (json.object().value("status").toInt() == 0){
            ui->topic->setHtml("<h2>error</h2><BR>"+json.object().value("error").toString());
        }else{
            int opened_topic_index = -1;
            topic_view *topic;
            for(int i=0; i<ui->tab_view->count(); i++){
                void *w = ui->tab_view->widget(i);
                topic = (topic_view *)w;
                qDebug()<<ui->tab_view->count();
                qDebug()<<topic->get_topic_id();
                if( item->text(0) == topic->get_topic_id() ){
                    opened_topic_index = i;
                }
            }
            if(opened_topic_index == -1){
                topic = new topic_view;
            }else{
                void *w = ui->tab_view->widget(opened_topic_index);
                topic = (topic_view *)w;
            }
            topic->set_topic_id(item->text(0));
            int current_index = ui->tab_view->currentIndex()+1;
            ui->tab_view->setCurrentIndex(ui->tab_view->insertTab(current_index, topic, item->text(1)));
            format_topic(topic->page()->mainFrame(), json, item->text(1));
//            format_topic(ui->topic->page()->mainFrame(), json, item->text(1));
            JsObj *jo = new JsObj();
            ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
            QObject::connect(jo,SIGNAL(send_mona_to_res_signal(QString)),this,SLOT(on_action_send_mona_to_res_clicked(QString)));
            QObject::connect(jo,SIGNAL(open_image_window_signal(QString)),this,SLOT(open_image_window(QString)));
            QObject::connect(jo,SIGNAL(open_profile_window_signal(QString)),this,SLOT(open_profile_window(QString)));
            QObject::connect(jo,SIGNAL(anchor_click_signal(QString)),this,SLOT(anchor_click(QString)));
        }

        if(!log_file.open(QFile::ReadWrite | QFile::Truncate)){
        }
        QJsonObject adding_log;
        adding_log["t_id"] = now_topic_id;
        adding_log["time"] = QString::number(QDateTime::currentDateTime().toTime_t());
        adding_log["text"] = ui->topic->page()->mainFrame()->toHtml();
        adding_log["position"] = "0";
        log_json_array += adding_log;
        QJsonObject root;
        root["topics"] = log_json_array;
        QJsonDocument formatted_log;
        formatted_log.setObject(root);
        QTextStream log_out(&log_file);
        log_out<<formatted_log.toJson();
    }else{              //loading topic is exist in log
        state_log_data->add_log(QObject::tr("loading topic is exist in log"));
        QString api_name = "responses/list";
        QUrlQuery api_query;
        api_query.addQueryItem("t_id",now_topic_id);
        api_query.addQueryItem("from","1");
        api_query.addQueryItem("to",QString::number(GET_RES_LIMIT));
        api_query.addQueryItem("if_modified_since",log_json_array.at(indexof_now_topic).toObject().value("time").toString());
        QString key = knock_api_get(api_name,api_query);
        QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
        if (json.object().value("status").toInt() == 0){
            ui->topic->setHtml("<h2>error</h2><BR>"+json.object().value("error").toString());
        }else{
            if(json.object().value("status").toInt() == 2){     //there is no update
                state_log_data->add_log(QObject::tr("there is no update."));
                ui->topic->setHtml(log_json_array.at(indexof_now_topic).toObject().value("text").toString());

            }else{      //there are update
                state_log_data->add_log(QObject::tr("there are update."));
                format_topic(ui->topic->page()->mainFrame(), json, item->text(1));
                if(!log_file.open(QFile::ReadWrite | QFile::Truncate)){}
                QJsonObject adding_log;
                adding_log["t_id"] = now_topic_id;
                adding_log["time"] = QString::number(QDateTime::currentDateTime().toTime_t());
                adding_log["text"] = ui->topic->page()->mainFrame()->toHtml();
                log_json_array.removeAt(indexof_now_topic);
                log_json_array += adding_log;
                QJsonObject root;
                root["topics"] = log_json_array;
                QJsonDocument formatted_log;
                formatted_log.setObject(root);
                QTextStream log_out(&log_file);
                log_out<<formatted_log.toJson();
            }
            JsObj *jo = new JsObj();
            ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
            QObject::connect(jo,SIGNAL(send_mona_to_res_signal(QString)),this,SLOT(on_action_send_mona_to_res_clicked(QString)));
            QObject::connect(jo,SIGNAL(open_image_window_signal(QString)),this,SLOT(open_image_window(QString)));
            QObject::connect(jo,SIGNAL(open_profile_window_signal(QString)),this,SLOT(open_profile_window(QString)));
            QObject::connect(jo,SIGNAL(anchor_click_signal(QString)),this,SLOT(anchor_click(QString)));
        }
    }
    ui->topic->setFocus();
}
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("about mona chaduke"),
                       tr("<h3>mona chaduke</h3>"
                          "<p><img src=\":/mona.png\"></p>"
                          "<p>Donate<BR><img width=\"150\" src=\":/qr.png\"><BR>M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz</p>"));
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
        state_log_data->add_log(QObject::tr("failed to get balance. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        double balance = json.object().value("balance").toString().toDouble();
        balance = balance / 100000000;
        QMessageBox::information(this,tr("balance"),tr("your balance is\n")+QString::number(balance));
    }
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
        state_log_data->add_log(QObject::tr("failed to get deposit_address. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        QMessageBox::information(this,tr("your deposit address"),tr("your deposit address is\n\n") +
                                 json.object().value("d_address").toString());
    }
}

void MainWindow::topic_reload(){
    position = ui->topic->page()->mainFrame()->scrollPosition().y();
    this->on_topic_list_itemActivated(ui->topic_list->selectedItems().at(0));
}

void MainWindow::favorite_topic_reload(){
    for (;;){
        if(ui->topic_list->invisibleRootItem()->child(0)->child(0)==0){break;}
        ui->topic_list->invisibleRootItem()->child(0)
            ->removeChild(ui->topic_list->invisibleRootItem()->child(0)->child(0));
    }
    load_Favorite_topics();
}

void MainWindow::topic_reload_signal_fire(){
    emit topic_reload_signal();
}

void MainWindow::on_add_favorite_button_clicked()
{
    auth_Key auth_key;
    QString api_name = "favorites/add";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("t_id",now_topic_id);
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("failed to add favorite.")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        state_log_data->add_log(QObject::tr("success add favorite."));
        emit favorite_topic_reload_signal();
    }
}

void MainWindow::on_remove_favorite_button_clicked()
{
    auth_Key auth_key;
    QString api_name = "favorites/delete";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("t_id",ui->topic_list->currentItem()->text(0));
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("failed to remove favorite. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        state_log_data->add_log(QObject::tr("success remove favorite"));
        emit favorite_topic_reload_signal();
    }
}

void MainWindow::on_actionGet_Favorite_topic_list_triggered()
{
    emit favorite_topic_reload_signal();
}

void MainWindow::on_actionForced_to_reload_triggered()
{
    state_log_data->add_log(QObject::tr("Forced to reload topic."));
    QString api_name = "responses/list";
    QUrlQuery api_query;
    api_query.addQueryItem("t_id",now_topic_id);
    api_query.addQueryItem("from","1");
    api_query.addQueryItem("to",QString::number(GET_RES_LIMIT));
    api_query.addQueryItem("topic_detail","1");
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->topic->setHtml("<h2>error</h2><BR>"+json.object().value("error").toString());
    }else{
        QString t_title = json.object().value("topic").toObject().value("title").toString();
        format_topic(ui->topic->page()->mainFrame(), json, t_title);
        JsObj *jo = new JsObj();
        ui->topic->page()->mainFrame()->addToJavaScriptWindowObject("jsobj",jo);
        QObject::connect(jo,SIGNAL(send_mona_to_res_signal(QString)),this,SLOT(on_action_send_mona_to_res_clicked(QString)));
        QObject::connect(jo,SIGNAL(open_image_window_signal(QString)),this,SLOT(open_image_window(QString)));
        QObject::connect(jo,SIGNAL(open_profile_window_signal(QString)),this,SLOT(open_profile_window(QString)));
        QObject::connect(jo,SIGNAL(anchor_click_signal(QString)),this,SLOT(anchor_click(QString)));
    }

    QFile log_file("./log");
    if(!log_file.open(QFile::ReadWrite | QFile::Truncate)){
    }
    QTextStream in(&log_file);
    QJsonDocument log = QJsonDocument::fromJson(in.readAll().toUtf8());
    QJsonArray log_json_array = log.object().value("topics").toArray();
    int indexof_now_topic = 0;
    for(int i = 0; i< log_json_array.count(); i++){
        if(now_topic_id == log_json_array.at(i).toObject().value("t_id").toString()){
            indexof_now_topic = i;
        }
    }
    QJsonObject adding_log;
    adding_log["t_id"] = now_topic_id;
    adding_log["time"] = QString::number(QDateTime::currentDateTime().toTime_t());
    adding_log["text"] = ui->topic->page()->mainFrame()->toHtml();
    adding_log["position"] = "0";
    log_json_array.removeAt(indexof_now_topic);
    log_json_array += adding_log;
    QJsonObject root;
    root["topics"] = log_json_array;
    QJsonDocument formatted_log;
    formatted_log.setObject(root);
    QTextStream log_out(&log_file);
    log_out<<formatted_log.toJson();
}

void MainWindow::on_action_withdraw_balance_triggered()
{
    withdraw_window *window = new withdraw_window(this);
    window->show();
}

void MainWindow::on_actionWithdraw_deposit_history_triggered()
{
    withdraw_deposit_history *window = new withdraw_deposit_history(this);
    window->show();
}

void MainWindow::on_actionSend_receive_h_istory_triggered()
{
    send_receive_history *window = new send_receive_history(this);
    window->show();
}

void MainWindow::on_action_Create_new_topic_triggered()
{
    new_Topic *window = new new_Topic(this);
    window->show();
}

void MainWindow::anchor_click(QString s){
    QString api_name = "responses/list";
    QUrlQuery api_query;
    api_query.addQueryItem("t_id",now_topic_id);
    api_query.addQueryItem("from",s);
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("failed to load response. ")+json.object().value("error").toString());
        qDebug()<<json.object().value("error").toString();
    }else{
        QJsonArray res = json.object().value("responses").toArray();

        QString received_mona;
        double receive = res.at(0).toObject().value("receive").toString().toDouble() / 100000000;
        if(res.at(0).toObject().value("receive").toString()!="0"){
            received_mona = "+" + QString::number(receive,'f',8).replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"") +
                    "MONA / " +
                    QString::number(res.at(0).toObject().value("rec_count").toInt()) + QObject::tr("man");
        }else{
            received_mona = "+" + QString::number(receive) + "MONA / " +
                    QString::number(res.at(0).toObject().value("rec_count").toInt()) + QObject::tr("man") + "";
        }
        QString replace_text = res.at(0).toObject().value("response").toString();
        replace_text.replace(QRegularExpression("\n"),"\n");
        QString created = from_unix_time(res.at(0).toObject().value("created").toInt());

        QString info = QString::number(res.at(0).toObject().value("r_id").toInt()) + " : " +
                res.at(0).toObject().value("u_name").toString() +
                res.at(0).toObject().value("u_dan").toString() + " : " +
                created + " [" +
                res.at(0).toObject().value("u_times").toString() + "] " +
                received_mona;
        QMessageBox anchor_res;
        anchor_res.setText(info);
        anchor_res.setInformativeText(replace_text);
        anchor_res.setStandardButtons(QMessageBox::Ok);
        anchor_res.setDefaultButton(QMessageBox::Ok);
        anchor_res.exec();
    }
}

void MainWindow::on_tab_view_tabCloseRequested(int index)
{
    ui->tab_view->widget(index)->deleteLater();
    ui->tab_view->removeTab(index);
}
