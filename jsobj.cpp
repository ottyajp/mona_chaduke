#include "jsobj.h"
#include <QMessageBox>

JsObj::JsObj(QObject *parent) : QObject(parent)
{

}

void JsObj::hogeSlot(){
    QMessageBox::information(0,"JsObj::hogeSlot()","test");
}
