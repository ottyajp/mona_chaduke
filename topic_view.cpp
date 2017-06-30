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
    this->setHtml(source);
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
