#include "user_profile_window.h"
#include "ui_user_profile_window.h"
#include "mainwindow.h"

User_Profile_window::User_Profile_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_Profile_window)
{
    ui->setupUi(this);

    ui->tabs->setTabText(0,tr("Profile"));
    ui->tabs->setTabText(1,tr("send"));
    ui->tabs->setTabEnabled(1,false);

    ui->anonymous_check->setChecked(true);
    ui->msg_edit->setEnabled(false);

    ui->send_button->setEnabled(false);
}

User_Profile_window::~User_Profile_window()
{
    delete ui;
}

void User_Profile_window::on_pushButton_clicked()
{//reload
    state_log_data->add_log(QObject::tr("loading user profile"));
    QString api_name = "users/profile";
    QUrlQuery api_query;
    api_query.addQueryItem("u_id",ui->u_id->text());
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->u_name->setText("error");
        ui->profile->setText(json.object().value("error").toString());
        state_log_data->add_log(QObject::tr("failed to load user profile"));
    }else{
        ui->u_name->setText(json.object().value("u_name").toString());
        ui->u_dan->setText(json.object().value("u_dan").toString());
        ui->profile->setText(json.object().value("profile").toString());
        ui->tabs->setTabEnabled(1,true);
        state_log_data->add_log(QObject::tr("loading user profile success."));
    }
}

void User_Profile_window::on_pushButton_2_clicked()
{//close
    close();
}

void User_Profile_window::set_u_id_from_res(QString s){
    u_id_from_res = s;
    ui->u_id->setText(u_id_from_res);
}

void User_Profile_window::on_anonymous_check_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked){
        ui->msg_edit->setEnabled(true);
    }
    if(arg1 == Qt::Checked){
        ui->msg_edit->setEnabled(false);
    }
}

void User_Profile_window::on_amount_textChanged()
{
    ui->send_button->setEnabled(true);
}

void User_Profile_window::on_send_button_clicked()
{
    state_log_data->add_log(QObject::tr("sending MONA..."));
    QString anonymous_send;
    if(ui->anonymous_check->isChecked() == Qt::Unchecked){//known send
        anonymous_send = "0";
    }else{//ui->anonymous_check->isChecked() == Qt::Checked //anonymous send
        anonymous_send = "1";
    }
    QString amount = QString::number(ui->amount->text().toDouble()*100000000,'f',0);
    auth_Key auth_key;
    QString api_name = "account/send";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("to_u_id",ui->u_id->text());
    api_query.addQueryItem("amount",amount);
    api_query.addQueryItem("anonymous",anonymous_send);
    if(ui->anonymous_check->isChecked() == Qt::Unchecked){
        api_query.addQueryItem("msg_text",ui->msg_edit->toPlainText());
    }
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        state_log_data->add_log(QObject::tr("failed to send MONA. ")+json.object().value("error").toString());
    }else{
        QMessageBox alert;
        alert.setText(tr("Success!"));
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.exec();
        state_log_data->add_log(QObject::tr("success. balance:")+json.object().value("balance").toString()+"watanabe");
    }
}
