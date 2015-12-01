#include "post_message.h"
#include "ui_post_message.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDateTime>

post_message::post_message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::post_message)
{
    ui->setupUi(this);
    if(sage_check == true){
        ui->sage_check->setCheckState(Qt::Checked);
    }
    QObject::connect(this,SIGNAL(post_success()),parent,SLOT(topic_reload_signal_fire()));
}

post_message::~post_message()
{
    delete ui;
}

void post_message::on_buttonBox_accepted()
{
    if(ui->sage_check->checkState() == Qt::Checked){
        sage_check = true;
    }
    if(ui->sage_check->checkState() == Qt::Unchecked){
        sage_check = false;
    }
    post_text = ui->text->toPlainText();
    QMessageBox confirm_text;
    confirm_text.setText(tr("will you posting following sentences?"));
    confirm_text.setInformativeText(post_text);
    confirm_text.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    confirm_text.setDefaultButton(QMessageBox::Ok);
    int ret = confirm_text.exec();
    switch(ret){
        case QMessageBox::Ok:
            this->call_post_message_api();
            break;
        case QMessageBox::Cancel:
            qDebug()<<"Cancel";
            break;
    }
}

void post_message::call_post_message_api(){
    state_log_data->add_log(QObject::tr("posting message..."));
    QString sage;
    if(ui->sage_check->checkState() == Qt::Unchecked){
        sage = "0";
    }
    if(ui->sage_check->checkState() == Qt::Checked){
        sage = "1";
    }
    auth_Key auth_key;
    QString api_name = "responses/post";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("t_id",now_topic_id);
    api_query.addQueryItem("text",post_text);
    api_query.addQueryItem("sage",sage);
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        qDebug()<<"error";
        qDebug()<<json.object().value("error").toString();
        state_log_data->add_log(QObject::tr("post failure.")+json.object().value("error").toString());
    }else{
        emit post_success();
        state_log_data->add_log(QObject::tr("post success!"));
    }
}
