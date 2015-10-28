#include "send_receive_history.h"
#include "ui_send_receive_history.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>

send_receive_history::send_receive_history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_receive_history)
{
    ui->setupUi(this);

    ui->view->setColumnCount(8);
    ui->view->setHeaderLabels(QStringList()<<tr("Date")<<tr("type")<<tr("amount")<<tr("anonymous")
                              <<tr("user")<<tr("topic")<<tr("res")<<tr("text"));
    ui->view->setColumnWidth(0,150);//date
    ui->view->setColumnWidth(1,60);//type
    ui->view->setColumnWidth(2,100);//amount
    //anonymous
    ui->view->setColumnHidden(4,true);//user
    ui->view->setColumnHidden(5,true);//topic
    ui->view->setColumnHidden(6,true);//res
    ui->view->setColumnHidden(7,true);//text

    emit on_reload_clicked();
}

send_receive_history::~send_receive_history()
{
    delete ui;
}

void send_receive_history::on_close_clicked()
{
    delete this;
}

void send_receive_history::on_reload_clicked()
{
    //delete all item
    for (;;){
        if(ui->view->invisibleRootItem()->child(0) == 0){break;}
        delete ui->view->invisibleRootItem()->child(0);
    }
    status_bar->showMessage(tr("get send history."));
    //get history
    QJsonDocument send_json = get_tx_history("send",tx_send_receive_limit);
    status_bar->showMessage(tr("get receive history."));
    QThread::sleep(1);
    QJsonDocument receive_json = get_tx_history("receive",tx_send_receive_limit);

    for(int i=0;i<tx_send_receive_limit.toInt();i++){
        QJsonObject json = send_json.object().value("transactions").toArray().at(i).toObject();
        if(json.value("created").toInt() == 0){break;}
        QString created = from_unix_time(json.value("created").toInt());
        QString type = json.value("item").toString();
        if(type == "send") type = tr("send");
        QString amount = QString::number(
                    json.value("amount").toString().toDouble() / 100000000,'f',8)
                .replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"");
        //anonymous user topic res text
        QString anonymous = QString::number(json.value("anonymous").toInt());
        if(anonymous == "1"){
            anonymous = tr("anonymous");
        }else{
            anonymous = tr("known");
        }
        QString user = QString::number(json.value("user").toObject().value("u_id").toInt());
        QString topic,res;
        if(json.value("t_id").toInt() != 0){
            topic = QString::number(json.value("t_id").toInt());
            res = QString::number(json.value("r_id").toInt());
        }
        QString text = json.value("msg_text").toString();
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,created);
        item->setText(1,type);
        item->setText(2,amount);
        item->setText(3,anonymous);
        item->setText(4,user);
        item->setText(5,topic);
        item->setText(6,res);
        item->setText(7,text);
        ui->view->addTopLevelItem(item);
    }

    for(int i=0;i<tx_send_receive_limit.toInt();i++){
        QJsonObject json = receive_json.object().value("transactions").toArray().at(i).toObject();
        if(json.value("created").toInt() == 0){break;}
        QString created = from_unix_time(json.value("created").toInt());
        QString type = json.value("item").toString();
        if(type == "send") type = tr("send");
        QString amount = QString::number(
                    json.value("amount").toString().toDouble() / 100000000,'f',8)
                .replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"");
        //anonymous user topic res text
        QString anonymous = QString::number(json.value("anonymous").toInt());
        if(anonymous == "1"){
            anonymous = tr("anonymous");
        }else{
            anonymous = tr("known");
        }
        QString user = QString::number(json.value("user").toObject().value("u_id").toInt());
        QString topic,res;
        if(json.value("t_id").toInt() != 0){
            topic = QString::number(json.value("t_id").toInt());
            res = QString::number(json.value("r_id").toInt());
        }
        QString text = json.value("msg_text").toString();
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,created);
        item->setText(1,type);
        item->setText(2,amount);
        item->setText(3,anonymous);
        item->setText(4,user);
        item->setText(5,topic);
        item->setText(6,res);
        item->setText(7,text);
        ui->view->addTopLevelItem(item);
    }
    ui->view->sortByColumn(0,Qt::DescendingOrder);
}

void send_receive_history::on_view_itemDoubleClicked(QTreeWidgetItem *item)
{
    //0date 1type 2amount 3anonymous 4user 5topic 6res 7text
if(item->text(3) == tr("known")){
    QString api_name = "responses/list";
    QUrlQuery query;
    query.addQueryItem("t_id",item->text(5));
    query.addQueryItem("from",item->text(6));
    query.addQueryItem("topic_detail","1");
    QString key = knock_api_get(api_name, query);
    QJsonDocument json0 = QJsonDocument::fromJson(key.toUtf8());
    if (json0.object().value("status").toInt() == 0){
        status_bar->showMessage(tr("failed to get detail.")+json0.object().value("error").toString());
        qDebug()<<json0.object().value("error").toString();
    }else{
        QJsonObject json = json0.object().value("responses").toArray().at(0).toObject();
        QString amount = QString::number(json.value("receive").toString().toDouble() / 100000000,'f',8)
                .replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"");
        QString info = json.value("topic").toObject().value("title").toString() + "\n\n" +//title
                item->text(0) + " " + item->text(1)+ "\n" + //date type
                QString::number(json.value("r_id").toInt()) + "  :" + json.value("u_name").toString() +//res info 1
                json.value("u_dan").toString() + ":" + from_unix_time(json.value("created").toInt()) + " " +//res info 2
                amount + "MONA/" + QString::number(json.value("rec_count").toInt()) + QObject::tr("man") + "\n\n" +//res info 3
                json.value("response").toString() + "\n\n";
        QString informativetext = QObject::tr("sent MONA : ") + item->text(2) + "MONA\n" +
                QObject::tr("sent message :\n") + item->text(7);//amount
        QMessageBox detail;
        detail.setText(info);
        detail.setInformativeText(informativetext);
        detail.setStandardButtons(QMessageBox::Ok);
        detail.setDefaultButton(QMessageBox::Ok);
        detail.exec();
    }
}
}
