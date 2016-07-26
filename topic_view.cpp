#include "topic_view.h"

topic_view::topic_view(){
    this->set_topic_id("0");
}

topic_view::topic_view(QWidget *&)
{
    this->set_topic_id("0");
}

void topic_view::set_topic_id(QString arg){
    topic_id = arg;
}

QString topic_view::get_topic_id(){
    return topic_id;
}
