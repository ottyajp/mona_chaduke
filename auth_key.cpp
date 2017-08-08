#include "auth_key.h"

auth_key::auth_key()
{
    time = QString::number(QDateTime::currentDateTime().toTime_t());
    qsrand(QDateTime::currentDateTime().toTime_t());
    nonce.append(QString::number(qrand()));

    QSettings set("settings.ini", QSettings::IniFormat);
    hash = QCryptographicHash::hash("AvoreZ4HUoNFuIEHZGomAjL8OYy3rMEMoUqfWkRXd6r0=" +
                                    nonce.toBase64() +
                                    time.toUtf8() +
                                    set.value("secretkey").toString().toUtf8(),
                                    QCryptographicHash::Sha256);
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
