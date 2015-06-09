#include "user_profile_window.h"
#include "ui_user_profile_window.h"
#include "mainwindow.h"

User_Profile_window::User_Profile_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_Profile_window)
{
    ui->setupUi(this);
}

User_Profile_window::~User_Profile_window()
{
    delete ui;
}

void User_Profile_window::on_pushButton_clicked()
{
    QString api_name = "users/profile";
    QUrlQuery api_query;
    api_query.addQueryItem("u_id",ui->u_id->text());
    QString key = knock_api_get(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        ui->u_name->setText("error");
        ui->profile->setText(json.object().value("error").toString());
    }else{
        ui->u_name->setText(json.object().value("u_name").toString());
        ui->u_dan->setText(json.object().value("u_dan").toString());
        ui->profile->setText(json.object().value("profile").toString());
    }
}

void User_Profile_window::on_pushButton_2_clicked()
{
    close();
}
