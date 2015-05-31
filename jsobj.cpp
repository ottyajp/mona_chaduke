#include "jsobj.h"
#include <QMessageBox>

JsObj::JsObj(QObject *parent) : QObject(parent)
{

}

void JsObj::send_mona_to_res(){
    QMessageBox::information(0,"t","asd");
}
