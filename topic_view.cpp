#include "topic_view.h"

topic_view::topic_view(QWidget *parent) : QWebEngineView(parent)
{
    this->isInitialized = false;
    QString source = read_file(":/template/template.html") +
            read_file("./style.css") +
            read_file(":/template/template2.html");
    this->page()->setWebChannel(&channel);
    channel.registerObject("widget", this);
    this->setHtml(source);
}

void topic_view::put_log(QString str){
    qDebug()<<str;
}

void topic_view::setInitScreen(){
    QString source = read_file(":/initScreen/initScreen.html");
    QString style = "<style type=\"text/css\">" +
            read_file("./style.css") +
            "</style>";
    this->setHtml(source + style);
}

void topic_view::jsComp(){
    this->isInitialized = true;
}

void topic_view::setTitle(QString title){
    for(;;){
        if(this->isInitialized){
            break;
        }
    }
    QString s = QString("PrintLog('%1');").arg(title);
    this->page()->runJavaScript(s);
}

void topic_view::loadTopic(QString t_id){
    QSettings set("settings.ini", QSettings::IniFormat);
    QUrlQuery query;
    query.addQueryItem("t_id", t_id);
    query.addQueryItem("to", set.value("responses_limit").toString());
    query.addQueryItem("topic_detail", "1");
    QJsonDocument json = QJsonDocument::fromJson(
                access_get("responses/list", query).toUtf8());
    QString data = json.toJson();
    QString s = QString("loadTopic(%1);").arg(data);
    this->page()->runJavaScript(s);
}
