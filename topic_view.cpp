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
    QString title = json.object().value("topic").toObject().value("title").toString();
    this->page()->runJavaScript(QString("setTitle('%1');").arg(title));
    QJsonArray resArray = json.object().value("responses").toArray();
    for(int i=0;;i++){
        QJsonObject res = resArray.at(i).toObject();
        if(res.value("u_name").toString() == ""){
            break;
        }
        QString created = from_unix_time(res.value("created").toInt());
        QString received_mona;
        double receive = res.value("receive").toString().toDouble() / 100000000;
        if(res.value("receive").toString() != "0"){
            received_mona = "<span class=\"mona_yay\">+" +
                    QString::number(receive,'f',8).replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"") +
                    "MONA</b> / " +
                    QString::number(res.value("res_count").toInt()) +
                    QObject::tr("people") + "</span>";
        }else{
            received_mona = "+" + QString::number(receive) +
                    "MONA / 0" + QObject::tr("people");
        }
        QString replace_text = res.value("response").toString();
        replace_text.replace(QRegularExpression("\n"),"<BR>");
        replace_text.replace(QRegularExpression("'"),"\\'");
        replace_text.replace(QRegularExpression("(>>)([0-9]{1,4})"),"<span class=\"anchor\">\\1\\2</span>");
        replace_text.replace(QRegularExpression("(http://i.imgur.com/.+)(\\..{3})"),
                             "<div class=\"image\"><img src=\"\\1m\\2\" class=\"imgur\" onclick=\"onClick_image(`\\1\\2`)\"></div>");
        QString response = QString::number(res.value("r_id").toInt()) + " : " +
                "<span class=\"u_name\">" + res.value("u_name").toString() + "</span>" +
                res.value("u_dan").toString() + " : " +
                created + " [" +
                res.value("u_times").toString() + "] " +
                received_mona +
                " <span class=\"send_mona\">" + QObject::tr("send mona") + "</span><BR>" +
                "<span class=\"level" + QString::number(res.value("res_lv").toInt()) + "\">" +
                replace_text +
                "</span>";
        QString s = QString("addRes('%1');").arg(response);
        this->page()->runJavaScript(s);
    }
}

void topic_view::click_image(QString str){
    image_window *window = new image_window(this);
    window->setUrl(str);
    window->show();
}
