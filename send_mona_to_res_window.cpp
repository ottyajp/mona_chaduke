#include "send_mona_to_res_window.h"
#include "ui_send_mona_to_res_window.h"
#include "mainwindow.h"
#include <QMessageBox>

send_mona_to_res_window::send_mona_to_res_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_mona_to_res_window)
{
    ui->setupUi(this);
    ui->send_to->setText(tr("How much mona send to >>") + send_to + tr(" ?"));
    ui->send_mona_preset1->setText("(&1)" + send_mona_amount_1);
    ui->send_mona_preset2->setText("(&2)" + send_mona_amount_2);
    ui->send_mona_preset3->setText("(&3)" + send_mona_amount_3);
    ui->send_mona_preset4->setText("(&4)" + send_mona_amount_4);
}

send_mona_to_res_window::~send_mona_to_res_window()
{
    delete ui;
}

void send_mona_to_res_window::on_send_button_clicked()
{
    double mona_amount = ui->send_mona_fixed->text().toDouble();
    if(mona_amount > 0){
        QMessageBox confirm_text;
        confirm_text.setText(tr("do you really want send MONA?"));
        confirm_text.setInformativeText(QString::number(mona_amount));
        confirm_text.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        confirm_text.setDefaultButton(QMessageBox::Ok);
        int ret = confirm_text.exec();
        switch(ret){
            case QMessageBox::Ok:{
                long mona_amount_watanabe = (long)100000000 * mona_amount;
                qDebug()<<mona_amount_watanabe<<","<<mona_amount;

                auth_Key auth_key;
                QString api_name = "account/send";
                QUrlQuery api_query;
                api_query.addQueryItem("app_id","2332");
                api_query.addQueryItem("u_id",QString::number(user_id));
                api_query.addQueryItem("nonce",auth_key.read_nonce());
                api_query.addQueryItem("time",auth_key.read_time());
                api_query.addQueryItem("auth_key",auth_key.read_auth_key());
                api_query.addQueryItem("t_id",now_topic_id);
                api_query.addQueryItem("r_id",send_to);
                api_query.addQueryItem("amount",QString::number(mona_amount_watanabe));
                QString key = knock_api(api_name,api_query);
                QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());

                if (json.object().value("status").toInt() == 0){
                    qDebug()<<"error";
                    qDebug()<<json.object().value("error").toString();
                }else{

                }
                break;
            }
            case QMessageBox::Cancel:{
                qDebug()<<"Cancel";
                break;
            }
        }
    }else{
        QMessageBox::warning(this, "error", tr("invalid amount"),QMessageBox::Ok);
    }
}

void send_mona_to_res_window::on_send_mona_preset1_clicked()
{
    ui->send_mona_fixed->setText(send_mona_amount_1);
}

void send_mona_to_res_window::on_send_mona_preset2_clicked()
{
    ui->send_mona_fixed->setText(send_mona_amount_2);
}

void send_mona_to_res_window::on_send_mona_preset3_clicked()
{
    ui->send_mona_fixed->setText(send_mona_amount_3);
}

void send_mona_to_res_window::on_send_mona_preset4_clicked()
{
    ui->send_mona_fixed->setText(send_mona_amount_4);
}
