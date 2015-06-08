#include "jsobj.h"
#include <QMessageBox>
#include <QDebug>

JsObj::JsObj(QObject *parent) : QObject(parent)
{

}

void JsObj::send_mona_to_res(QString s){
    emit send_mona_to_res_signal(s);
}
