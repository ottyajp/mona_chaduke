#include "func_var.h"

QString from_unix_time(int unix_time){
    QDateTime from;
    from.setTime_t(unix_time);
    return from.toString("yyyy/MM/dd hh:mm:ss");
}

QString read_file(QString file_name){
    QFile *file = new QFile(file_name);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug()<<"can't open " + file_name;
        return "";
    }else{
        QTextStream *stream = new QTextStream(file);
        return stream->readAll();
    }
}

QString watanabe2mona(qlonglong watanabe){
    QString temp = QString::number(double(watanabe)/100000000, 'f', 8);
    temp.replace(QRegularExpression("[0]*$"),"");
    temp.replace(QRegularExpression("\\.$"),"");
    return temp;
}
