#include "post_window.h"
#include "ui_post_window.h"

post_window::post_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::post_window)
{
    ui->setupUi(this);
}

post_window::~post_window()
{
    delete ui;
}

void post_window::setTopicID(QString t_id){
    this->t_id = t_id;
    QUrlQuery query;
    query.addQueryItem("t_id", this->t_id);
    query.addQueryItem("topic_detail", "1");
    QJsonDocument json = QJsonDocument::fromJson(
                access_get("responses/list", query).toUtf8());
    if(json.object().value("status").toInt() == 0){
        qDebug()<<json.object().value("error").toString();
    }else{
        QJsonObject obj = json.object().value("topic").toObject();
        ui->t_id->setText(QString::number(obj.value("t_id").toInt()));
        ui->t_title->setText(obj.value("title").toString());
    }
}

void post_window::on_cancel_clicked()
{
    this->close();
}
