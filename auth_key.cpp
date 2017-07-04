#include "auth_key.h"

auth_key::auth_key()
{
    time = QString::number(QDateTime::currentDateTime().toTime_t());
    bool isValid = true;
    int pos;
    while(true){
        qsrand(QDateTime::currentDateTime().toTime_t());
        nonce.append(QString::number(qrand()));

        QSettings set("settings.ini", QSettings::IniFormat);
        hash = QCryptographicHash::hash("Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=" +
                                        nonce.toBase64() +
                                        time.toUtf8() +
                                        set.value("secretkey").toString().toUtf8(),
                                        QCryptographicHash::Sha256);
        pos = nonce.toBase64().toStdString().find("+", 0);
        if(pos != -1){
            isValid = false;
        }
        if(isValid){
            break;
        }
    }
}

QString auth_key::read_time(){
    return time;
}

QString auth_key::read_nonce(){
    return nonce.toBase64();
}

QString auth_key::read_auth_key(){
    return hash.toBase64();
}
