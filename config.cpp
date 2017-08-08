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
    authenticated = false;
}

config::~config()
{
    delete ui;
}

void config::on_auth_button_clicked()
{
    if(authenticated) {
        ui->auth_button->setText(tr("Authenticate"));
        ui->address->setEnabled(true);
        ui->password->setEnabled(true);
        this->authenticated = false;
        return;
    }
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
    query.addQueryItem("app_secretkey", "AvoreZ4HUoNFuIEHZGomAjL8OYy3rMEMoUqfWkRXd6r0=");
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
        auth_key key;
        QUrlQuery query;
        query.addQueryItem("app_id", "2332");
        query.addQueryItem("u_id", uid);
        query.addQueryItem("nonce", key.read_nonce());
        query.addQueryItem("time", key.read_time());
        query.addQueryItem("auth_key", key.read_auth_key());
        QJsonDocument json = QJsonDocument::fromJson(
                    access_post("users/myprofile", query).toUtf8());
        if(json.object().value("status").toInt() == 0){
            ui->label_user->setText(tr("error occured."));
            qDebug()<<json.object().value("error").toString();
        }else{
            ui->label_user->setText(tr("authenticated"));
            QLabel *u_name = new QLabel();
            u_name->setText(json.object().value("u_name").toString() + " " +
                    json.object().value("u_dan").toString());
            QTextBrowser *profile = new QTextBrowser();
            QString str = json.object().value("profile").toString();
            str.replace(QRegularExpression("\\\\n"), "<BR>");
            profile->setHtml(str);
            QVBoxLayout *layout = new QVBoxLayout();
            layout->addWidget(u_name);
            layout->addWidget(profile);
            ui->userLayout->addLayout(layout);
            ui->address->setEnabled(false);
            ui->password->setEnabled(false);
            ui->auth_button->setText(tr("re-authenticate"));
            this->authenticated = true;
        }
    }
}
