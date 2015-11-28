#include "jsobj.h"
#include <QDebug>

JsObj::JsObj(QObject *parent) : QObject(parent)
{

}

void JsObj::send_mona_to_res(QString s){
    emit send_mona_to_res_signal(s);
}

void JsObj::open_image(QString s){
    emit open_image_window_signal(s);
}

void JsObj::open_profile_window(QString s){
    emit open_profile_window_signal(s);
}

void JsObj::anchor_click(QString s){
    emit anchor_click_signal(s);
}
