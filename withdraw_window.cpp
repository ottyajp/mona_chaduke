#include "withdraw_window.h"
#include "ui_withdraw_window.h"
#include "mainwindow.h"
#include <QDebug>

withdraw_window::withdraw_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::withdraw_window)
{
    ui->setupUi(this);
}

withdraw_window::~withdraw_window()
{
    delete ui;
}

void withdraw_window::on_cancel_button_clicked()
{
    delete this;
}

void withdraw_window::on_withdraw_button_clicked()
{
    QString amount = QString::number(ui->amount->text().toDouble()*100000000,'f',0);
    auth_Key auth_key;
    QString api_name = "account/withdraw";
    QUrlQuery api_query;
    api_query.addQueryItem("app_id","2332");
    api_query.addQueryItem("u_id",QString::number(user_id));
    api_query.addQueryItem("nonce",auth_key.read_nonce());
    api_query.addQueryItem("time",auth_key.read_time());
    api_query.addQueryItem("auth_key",auth_key.read_auth_key());
    api_query.addQueryItem("amount",amount);
    QString key = knock_api(api_name,api_query);
    QJsonDocument json = QJsonDocument::fromJson(key.toUtf8());
    if (json.object().value("status").toInt() == 0){
        status_bar->showMessage(tr("error. ") + json.object().value("error").toString());
    }else{
        status_bar->showMessage(tr("success.  balance:") +
                                json.object().value("balance").toString() + "watanabe");
    }
}
