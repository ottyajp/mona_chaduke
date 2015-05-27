#include "setting_window.h"
#include "ui_setting_window.h"
#include "mainwindow.h"

Setting_window::Setting_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting_window)
{
    ui->setupUi(this);
    ui->get_topic_limit->setText("10");
    extern QString secret_key;
}

Setting_window::~Setting_window()
{
    delete ui;
}

void Setting_window::on_pushButton_clicked()
{
    QString api_name = "auth/secretkey";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("app_secretkey","Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=");
    api_query.addQueryItem("u_address",ui->u_address->text());
    api_query.addQueryItem("pass",ui->pass->text());
    QString key = knock_api(api_name,api_query);
    qDebug()<<key;
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    key = json.object().value("secretkey").toString();
    qDebug()<<key;
    secret_key = key;
    qDebug()<<secret_key;
    ui->u_id->setText(QString::number(json.object().value("u_id").toInt()));
}

void Setting_window::on_buttonBox_accepted()
{
    qDebug()<<ui->get_topic_limit->text().toInt();
    get_topic_limit = ui->get_topic_limit->text().toInt();

}
