#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/icon.ico"));

    this->loadSettings();

    ui->topic_list->setColumnWidth(0, 70); //id
    ui->topic_list->setColumnWidth(1, 40); //rank
    ui->topic_list->setColumnWidth(2, 700);//title
    ui->topic_list->setColumnWidth(3, 40); //count
    ui->topic_list->setColumnWidth(4, 100);//updated
    ui->topic_list->setColumnWidth(5, 100);//created
    ui->topic_list->setColumnWidth(6, 100);//category
    ui->topic_list->setColumnWidth(7, 100);//tags

    QFont font;
    font.setBold(true);
    this->favorite_topic = new QTreeWidgetItem(ui->topic_list);
    favorite_topic->setText(2, tr("favorite topics"));
    favorite_topic->setFont(2, font);
    this->all_topic = new QTreeWidgetItem(ui->topic_list);
    all_topic->setText(2, tr("all topics"));
    all_topic->setFont(2, font);
    for(int i=0; i<8; i++){
        favorite_topic->setBackgroundColor(i, QColor(240, 255, 240, 200));
        all_topic->setBackgroundColor(i, QColor(240, 255, 240, 200));
    }
    if(this->aa_topic_list) {
        this->on_action_Load_topic_list_triggered();
    }
    if(this->aa_fav_topic_list) {
        this->on_actionLoad_Favorite_topic_list_triggered();
    }
    topic_view *topic = new topic_view();
    topic->setInitScreen();
    back_topic = new topic_view();
    ui->topic_tab_widget->addTab(topic, "test");
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
    query.addQueryItem("limit",this->topics_limit);
    QJsonDocument json = QJsonDocument::fromJson(access_get("topics/list", query).toUtf8());
    if(json.object().value("status").toString() == "0"){
        qDebug()<<json.object().value("error").toString();
    }else{
        while(true){
            if(this->all_topic->childCount() == 0) {break;}
            QTreeWidgetItem *item = this->all_topic->child(0);
            this->all_topic->removeChild(item);
            delete item;
        }
        QJsonArray topics = json.object().value("topics").toArray();
        for(int i=0; i<this->topics_limit.toInt(); i++){
            QTreeWidgetItem *item = new QTreeWidgetItem(this->all_topic);
            item->setText(0, QString::number(topics.at(i).toObject().value("t_id").toInt()));
            item->setText(1, QString::number(topics.at(i).toObject().value("rank").toInt()));
            item->setText(2, topics.at(i).toObject().value("title").toString());
            item->setText(3, QString::number(topics.at(i).toObject().value("count").toInt()));
            item->setText(4, from_unix_time(topics.at(i).toObject().value("updated").toInt()));
            item->setText(5, from_unix_time(topics.at(i).toObject().value("created").toInt()));
            item->setText(6, topics.at(i).toObject().value("category").toString());
            item->setText(7, topics.at(i).toObject().value("tags").toString());
        }
    }
    this->all_topic->setExpanded(true);
    ui->topic_list->resizeColumnToContents(0);
    ui->topic_list->resizeColumnToContents(3);
    ui->topic_list->resizeColumnToContents(4);
    ui->topic_list->resizeColumnToContents(5);
}

void MainWindow::on_actionConfig_C_triggered()
{
    config *conf = new config(this);
    connect(conf, SIGNAL(success_auth(QString,QString)),
            this, SLOT(set_secretkey_uid(QString,QString)));
    connect(conf, SIGNAL(save(QMap<QString, QString>)),
            this, SLOT(saveSettings(QMap<QString, QString>)));
    conf->setWindowModality(Qt::ApplicationModal);
    conf->set_uid(this->u_id);
    conf->show();
}

void MainWindow::saveSettings(QMap<QString, QString> data){
    QSettings set("settings.ini", QSettings::IniFormat);
    set.setValue("secretkey", this->secretkey);
    set.setValue("u_id", this->u_id);
    set.setValue("topics_limit", data["topics_limit"]);
    set.setValue("responses_limit", data["responses_limit"]);
    set.setValue("aa_topic_list", data["aa_topic_list"]);
    set.setValue("aa_fav_topic_list", data["aa_fav_topic_list"]);
    set.setValue("always_sage", data["always_sage"]);
    this->loadSettings();
}

void MainWindow::loadSettings(){
    QSettings set("settings.ini", QSettings::IniFormat);
    this->secretkey = set.value("secretkey").toString();
    this->u_id = set.value("u_id").toString();
    this->topics_limit = set.value("topics_limit").toString();
    this->responses_limit = set.value("responses_limit").toString();
    if(qvariant2bool(set.value("aa_topic_list"))) {
        this->aa_topic_list = true;
    }else{
        this->aa_topic_list = false;
    }
    if(qvariant2bool(set.value("aa_fav_topic_list"))) {
        this->aa_fav_topic_list = true;
    }else{
        this->aa_fav_topic_list = false;
    }
}

void MainWindow::set_secretkey_uid(QString key, QString id){
    this->secretkey = key;
    this->u_id = id;
}

void MainWindow::on_topic_list_itemActivated(QTreeWidgetItem *item)
{
    if(item->text(0) == ""){
        return;
    }
    for(int i=0; i<ui->topic_tab_widget->count(); i++){
        auto *topic = static_cast<topic_view*>(ui->topic_tab_widget->widget(i));
        if(topic->getTopicID() == item->text(0)){
            ui->topic_tab_widget->setCurrentIndex(i);
            return;
        }
    }
    int pos = ui->topic_tab_widget->currentIndex();
    this->back_topic->loadTopic(item->text(0));
    ui->topic_tab_widget->insertTab(pos + 1, this->back_topic, item->text(2));
    ui->topic_tab_widget->setCurrentIndex(pos + 1);
    this->back_topic->setFocus();
    this->back_topic = new topic_view();
}

void MainWindow::on_topic_tab_widget_tabCloseRequested(int index)
{
    auto *topic = static_cast<topic_view*>(ui->topic_tab_widget->widget(index));
    ui->topic_tab_widget->removeTab(index);
    delete topic;
}

void MainWindow::on_actionLoad_Favorite_topic_list_triggered()
{
    auth_key key;
    QUrlQuery query;
    query.addQueryItem("app_id", "2332");
    query.addQueryItem("u_id", this->u_id);
    query.addQueryItem("nonce", key.read_nonce());
    query.addQueryItem("time", key.read_time());
    query.addQueryItem("auth_key", key.read_auth_key());
    query.addQueryItem("order", "updated");
    QJsonDocument json = QJsonDocument::fromJson(
                access_post("favorites/list", query).toUtf8());
    if(json.object().value("status").toInt() == 0){
        qDebug()<<json.object().value("error").toString();
    }else{
        while(true){
            if(this->favorite_topic->childCount() == 0) {break;}
            QTreeWidgetItem *item = this->favorite_topic->child(0);
            this->favorite_topic->removeChild(item);
            delete item;
        }
        for(int i=0; i<200; i++){
            QJsonObject topic = json.object().value("topics").toArray().at(i).toObject();
            if(topic.value("t_id").toInt() == 0){
                break;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(this->favorite_topic);
            item->setText(0, QString::number(topic.value("t_id").toInt()));
            item->setText(1, QString::number(topic.value("rank").toInt()));
            item->setText(2, topic.value("title").toString());
            item->setText(3, QString::number(topic.value("count").toInt()));
            item->setText(4, from_unix_time(topic.value("updated").toInt()));
            item->setText(5, from_unix_time(topic.value("created").toInt()));
            item->setText(6, topic.value("category").toString());
            item->setText(7, topic.value("tags").toString());
        }
    }
    this->favorite_topic->setExpanded(true);
    ui->topic_list->resizeColumnToContents(0);
    ui->topic_list->resizeColumnToContents(3);
    ui->topic_list->resizeColumnToContents(4);
    ui->topic_list->resizeColumnToContents(5);
}

void MainWindow::on_actionGet_balance_triggered()
{
    get_balance *window = new get_balance(this);
    window->set_uid(this->u_id);
    window->get();
    window->show();
}

void MainWindow::on_actionGet_deposit_address_triggered()
{
    auth_key key;
    QUrlQuery query;
    query.addQueryItem("app_id", "2332");
    query.addQueryItem("u_id", this->u_id);
    query.addQueryItem("nonce", key.read_nonce());
    query.addQueryItem("time", key.read_time());
    query.addQueryItem("auth_key", key.read_auth_key());
    QJsonDocument json = QJsonDocument::fromJson(
                access_post("account/deposit", query).toUtf8());
    if(json.object().value("status").toInt() == 0){
        qDebug()<<json.object().value("error").toString();
    }else{
        QMessageBox box;
        box.setText(tr("deposit address") + "<p>" +
              json.object().value("d_address").toString() +
              "</p>");
        box.exec();
    }
}

void MainWindow::on_actionabout_triggered()
{
    QString text = "mona-chaduke<BR>Developed by @ottyajp<BR><BR> \
            <img src=\":/image/mona.png\"> \
            <p>Donation is welcome<BR>M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz \
            <img src=\":/image/qr.png\"></p>";
    QMessageBox::about(this, tr("mona chaduke"), text);
}

void MainWindow::on_postButton_clicked()
{
    topic_view* view = static_cast<topic_view*>(ui->topic_tab_widget->currentWidget());
    if(view->getTopicID() == ""){
        return;
    }
    post_window* window = new post_window(this);
    window->setTopicID(view->getTopicID());
    window->setUserID(this->u_id);
    window->show();
}

void MainWindow::on_action_Close_current_tab_triggered()
{
    this->on_topic_tab_widget_tabCloseRequested(ui->topic_tab_widget->currentIndex());
}

void MainWindow::on_actionMove_to_the_right_tab_triggered()
{
    if(ui->topic_tab_widget->currentIndex() == ui->topic_tab_widget->count()-1){
        ui->topic_tab_widget->setCurrentIndex(0);
    }else{
        ui->topic_tab_widget->setCurrentIndex(ui->topic_tab_widget->currentIndex()+1);
    }
}

void MainWindow::on_actionMove_to_the_left_tab_triggered()
{
    if(ui->topic_tab_widget->currentIndex() == 0){
        ui->topic_tab_widget->setCurrentIndex(ui->topic_tab_widget->count()-1);
    }else{
        ui->topic_tab_widget->setCurrentIndex(ui->topic_tab_widget->currentIndex()-1);
    }
}
