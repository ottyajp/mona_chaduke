#include "new_topic.h"
#include "ui_new_topic.h"
#include <QDebug>

new_Topic::new_Topic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_Topic)
{
    ui->setupUi(this);

    QStringList category_list = (QStringList()<<
                                 QObject::tr("Others")<<
                                 QObject::tr("Baramaki")<<
                                 QObject::tr("Beginner")<<
                                 QObject::tr("News")<<
                                 QObject::tr("Debate")<<
                                 QObject::tr("Hobby/Life")<<
                                 QObject::tr("Creation/Culture")<<
                                 QObject::tr("Story/Chat")<<
                                 QObject::tr("Economy/Social")<<
                                 QObject::tr("Science/IT")<<
                                 QObject::tr("mining")<<
                                 QObject::tr("R18")
                                 );
    ui->category->addItems(category_list);

    ui->able->setChecked(true);
    ui->not_show->setChecked(true);
}

new_Topic::~new_Topic()
{
    delete ui;
}

void new_Topic::on_cancel_clicked()
{
    delete this;
}

void new_Topic::on_create_topic_clicked()
{
    QString editable;
    QString sh_host;
    if(ui->able->isChecked() == true){
        editable = "1";
    }else{
        editable = "0";
    }
    if(ui->not_show->isChecked() == true){
        sh_host = "0";
    }else{
        if(ui->id->isChecked() == true){
            sh_host = "1";
        }else{
            if(ui->id_and_isp->isChecked() == true){
                sh_host = "2";
            }
        }
    }
    auth_Key auth_key;
    QString api_name = "topics/new";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("title",ui->title->text());
    api_query.addQueryItem("text",ui->text->toPlainText());
    api_query.addQueryItem("cat_id",QString::number(ui->category->currentIndex()));
    if(ui->tag->text() == ""){
    }else{
        api_query.addQueryItem("tags",ui->tag->text());
    }
    api_query.addQueryItem("editable",editable);
    api_query.addQueryItem("sh_host",sh_host);

    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        qDebug()<<"error";
        qDebug()<<json.object().value("error").toString();
        state_log_data->add_log(QObject::tr("Topic creation Failed. ")+json.object().value("error").toString());
    }else{
        state_log_data->add_log(QObject::tr("Topic creation Success!"));
        delete this;
    }
}
