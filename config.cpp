#include "config.h"
#include "ui_config.h"

config::config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);
    QSettings set("settings.ini", QSettings::IniFormat);
    ui->topics_limit->setText(set.value("topics_limit").toString());
    ui->responses_limit->setText(set.value("responses_limit").toString());
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
        QString u_id = QString::number(json.object().value("u_id").toInt());
        QString secretkey = json.object().value("secretkey").toString();
        emit success_auth(secretkey, u_id);
        this->set_uid(u_id);
    }
}

void config::on_buttons_accepted()
{
    QMap<QString, QString> data;
    data["topics_limit"] = ui->topics_limit->text();
    data["responses_limit"] = ui->responses_limit->text();
    emit save(data);
    this->close();
}

void config::on_buttons_rejected()
{
    this->close();
}

void config::set_uid(QString uid){
    if(uid != ""){
        QUrlQuery query;
        query.addQueryItem("u_id", uid);
        QJsonDocument json = QJsonDocument::fromJson(
                    access_get("users/profile", query).toUtf8());
        if(json.object().value("status").toInt() == 0){
            ui->label_user->setText(tr("invalid user"));
            qDebug()<<json.object().value("error").toString();
        }else{
            QString str = json.object().value("u_name").toString() +
                    " " +
                    json.object().value("u_dan").toString();
            ui->label_user->setText(str);
        }
    }
}
