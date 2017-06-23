#include "topic_view.h"

topic_view::topic_view()
{
    webView = new QWebEngineView();

    QString source = read_file(":/template/template.html") +
            read_file("./style.css") +
            read_file(":/template/template2.html");
    webView->setHtml(source);
}

QWebEngineView* topic_view::view(){
    return this->webView;
}
