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

    ui->anonymous_check->setChecked(true);
    ui->msg_edit->setEnabled(false);

    QObject::connect(this,SIGNAL(send_success()),parent,SLOT(topic_reload_signal_fire()));
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
                state_log_data->add_log(QObject::tr("sending mona...")+mona_amount+"[MONA]");

                QString anonymous_send;
                if(ui->anonymous_check->isChecked() == Qt::Unchecked){//known send
                    anonymous_send = "0";
                }else{//ui->anonymous_check->isChecked() == Qt::Checked //anonymous send
                    anonymous_send = "1";
                }
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
                api_query.addQueryItem("anonymous",anonymous_send);
                if(ui->anonymous_check->isChecked() == Qt::Unchecked){
                    api_query.addQueryItem("msg_text",ui->msg_edit->toPlainText());
                }
                QString key = knock_api(api_name,api_query);
                QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());

                if (json.object().value("status").toInt() == 0){
                    state_log_data->add_log(QObject::tr("failed to send mona. ")+json.object().value("error").toString());
                    qDebug()<<json.object().value("error").toString();
                }else{
                    this->send_success_fire();
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

void send_mona_to_res_window::send_success_fire(){
    emit send_success();
    state_log_data->add_log(QObject::tr("mona send success!"));
}

void send_mona_to_res_window::on_anonymous_check_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        ui->msg_edit->setEnabled(false);
    }
    if(arg1 == Qt::Unchecked){
        ui->msg_edit->setEnabled(true);
    }

}
