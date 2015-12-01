#include "setting_window.h"
#include "ui_setting_window.h"
#include "mainwindow.h"
#include "QUrlQuery"

Setting_window::Setting_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting_window)
{
    ui->setupUi(this);
    ui->get_topic_limit->setText(QString::number(get_topic_limit));
    QString api_name = "users/profile";
    QUrlQuery query;
    query.addQueryItem("u_id",QString::number(user_id));
    QString key = knock_api_get(api_name,query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->u_id->setText("error");
    }else{
        ui->u_id->setText(json.object().value("u_name").toString() +
                          json.object().value("u_dan").toString());
        ui->pushButton->setEnabled(false);
    }
    ui->send_mona_amount_1->setText(send_mona_amount_1);
    ui->send_mona_amount_2->setText(send_mona_amount_2);
    ui->send_mona_amount_3->setText(send_mona_amount_3);
    ui->send_mona_amount_4->setText(send_mona_amount_4);
    ui->tx_send_receive_limit->setText(tx_send_receive_limit);
    ui->tx_with_depo_limit->setText(tx_with_depo_limit);
    ui->show_state_log_check->setChecked(state_log_show);
    QString secret_key;
}

Setting_window::~Setting_window()
{
    state_log_data->add_log(QObject::tr("setting complete."));
    delete ui;
}

void Setting_window::on_pushButton_clicked()//authentication
{
    state_log_data->add_log(QObject::tr("authenticate"));
    QString api_name = "auth/secretkey";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("app_secretkey","Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=");
    api_query.addQueryItem("u_address",ui->u_address->text());
    api_query.addQueryItem("pass",ui->pass->text());
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if(json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("authentication failed.")+json.object().value("error").toString());
    }else{
        key = json.object().value("secretkey").toString();
        secret_key = key;
        ui->u_id->setText(QString::number(json.object().value("u_id").toInt()));
        user_id = json.object().value("u_id").toInt();
        state_log_data->add_log(QObject::tr("authentication success."));
    }
}

void Setting_window::on_buttonBox_accepted()
{
    get_topic_limit = ui->get_topic_limit->text().toInt();
    send_mona_amount_1 = ui->send_mona_amount_1->text();
    send_mona_amount_2 = ui->send_mona_amount_2->text();
    send_mona_amount_3 = ui->send_mona_amount_3->text();
    send_mona_amount_4 = ui->send_mona_amount_4->text();
    tx_send_receive_limit = ui->tx_send_receive_limit->text();
    tx_with_depo_limit = ui->tx_with_depo_limit->text();
    state_log_show = ui->show_state_log_check->isChecked();

}

void Setting_window::on_u_address_textChanged()
{
    ui->pushButton->setEnabled(true);
}

void Setting_window::on_pass_textChanged()
{
    ui->pushButton->setEnabled(true);
}
