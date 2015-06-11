#include "jsobj.h"
#include <QMessageBox>
#include <QDebug>

JsObj::JsObj(QObject *parent) : QObject(parent)
{

}

void JsObj::send_mona_to_res(QString s){
    emit send_mona_to_res_signal(s);
}

void JsObj::open_image(QString s){
    qDebug()<<"s";
    emit open_image_window_signal(s);
}
