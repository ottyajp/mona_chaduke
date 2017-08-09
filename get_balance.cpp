#include "get_balance.h"
#include "ui_get_balance.h"

get_balance::get_balance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::get_balance)
{
    ui->setupUi(this);
}

get_balance::~get_balance()
{
    delete ui;
}

void get_balance::on_closeButton_clicked()
{
    this->close();
}

void get_balance::set_uid(QString str){
    this->u_id = str;
}

void get_balance::get(){
    auth_key key;
    QUrlQuery query;
    query.addQueryItem("app_id", "2332");
    query.addQueryItem("u_id", this->u_id);
    query.addQueryItem("nonce", key.read_nonce());
    query.addQueryItem("time", key.read_time());
    query.addQueryItem("auth_key", key.read_auth_key());
    query.addQueryItem("detail", "1");
    QJsonDocument json = QJsonDocument::fromJson(
                access_post("account/balance", query).toUtf8());
    if(json.object().value("status").toInt() == 0){
        qDebug()<<json.object().value("error").toString();
    }else{
        QJsonObject detail = json.object().value("accounts").toObject();
        ui->deposit->setText(detail.value("deposit").toString());
        ui->send->setText(detail.value("send").toString());
        ui->receive->setText(detail.value("receive").toString());
        ui->withdraw->setText(detail.value("withdraw").toString());
        ui->gift->setText(detail.value("gift").toString());
        ui->reserved->setText(detail.value("reserved").toString());
        ui->balance->setText(detail.value("balance").toString());
    }
}
