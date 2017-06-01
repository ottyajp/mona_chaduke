#include "config.h"
#include "ui_config.h"

config::config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);
}

config::~config()
{
    delete ui;
}

void config::on_auth_button_clicked()
{
    if(ui->address->text() == ""){
        QErrorMessage *mes = new QErrorMessage(this);
        mes->showMessage(tr("you must fill Monacoin address."));
        return;
    }
    if(ui->password->text() == ""){
        QErrorMessage *mes = new QErrorMessage(this);
        mes->showMessage(tr("you must fill your Passowrd."));
        return;
    }
    QString api_name = "auth/secretkey";
    QUrlQuery query;
    query.addQueryItem("app_id", "2332");
    query.addQueryItem("app_secretkey", "Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=");
    query.addQueryItem("u_address", ui->address->text());
    query.addQueryItem("pass", ui->password->text());
    QJsonDocument json = QJsonDocument::fromJson(access_post(api_name, query).toUtf8());
    if(json.object().value("status").toInt() == 0){
        qDebug()<<"authentication failed";
        QErrorMessage *mes = new QErrorMessage(this);
        mes->showMessage(tr("authentication failed.")+"<BR>"+json.object().value("error").toString());
        return;
    }else{
        qDebug()<<json.object().value("u_id").toInt();
        qDebug()<<json.object().value("secretkey").toString();
    }
}
