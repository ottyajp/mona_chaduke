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

void post_window::setUserID(QString u_id){
    this->u_id = u_id;
}

void post_window::on_cancel_clicked()
{
    this->close();
}

void post_window::on_submit_clicked()
{
    QMessageBox box;
    box.setText(tr("ok?") + "<BR>" + ui->textEdit->toHtml());
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Ok);
    int ret = box.exec();
    QUrlQuery query;
    auth_key key;
    QJsonDocument json;
    switch(ret){
    case QMessageBox::Ok:
        query.addQueryItem("app_id", "2332");
        query.addQueryItem("u_id", this->u_id);
        query.addQueryItem("nonce", key.read_nonce());
        query.addQueryItem("time", key.read_time());
        query.addQueryItem("auth_key", key.read_auth_key());
        query.addQueryItem("t_id", this->t_id);
        query.addQueryItem("text", ui->textEdit->toPlainText());
        json = QJsonDocument::fromJson(
                    access_post("responses/post", query).toUtf8());
        if(json.object().value("status").toInt() == 0){
            qDebug()<<"failed";
            QErrorMessage *mes = new QErrorMessage(this);
            mes->showMessage(tr("posting messages was failed.") + "<BR>" + json.object().value("error").toString());
            break;
        }else{
            this->close();
            break;
        }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}
