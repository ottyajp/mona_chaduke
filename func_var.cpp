#include "func_var.h"

QString knock_api(QString api_name, QUrlQuery api_query){

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QString address = "http://askmona.org/v1/" + api_name;
    QUrl url(address);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    data.append(api_query.toString());
    QNetworkReply *reply = mgr.post(req,data);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
//        qDebug() << "Success" <<reply->readAll();
        QString r = reply->readAll();
        delete reply;
        return r;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        QString r = "1";
        delete reply;
        return r;
    }
}

QString knock_api_get(QString api_name, QUrlQuery api_query){

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QString address = "http://askmona.org/v1/" + api_name;
    QUrl url(address);
    url.setQuery(api_query);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
//        qDebug() << "Success" <<reply->readAll();
        QString r = reply->readAll();
        delete reply;
        return r;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        QString r = "1";
        delete reply;
        return r;
    }
}

auth_Key::auth_Key(){
    time = QString::number(QDateTime::currentDateTime().toTime_t());
    int f = 0;
    int pos;
    while(1){
        qsrand(QDateTime::currentDateTime().toTime_t());
        for(int i=0; i<5; i++){
            nonce.append(QString(qrand()%256));
        }

        hash = QCryptographicHash::hash("Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=" +
                                        nonce.toBase64() +
                                        time.toUtf8() +
                                        secret_key.toUtf8(),QCryptographicHash::Sha256);
        pos = nonce.toBase64().toStdString().find("+",0);
        if(pos == -1){
            f = 1;
        }
        pos = hash.toBase64().toStdString().find("+",0);
        if(pos == -1){
            f = 1;
        }else{
            f = 0;
        }
        if(f==1){
            break;
        }
    }
}

QString auth_Key::read_time(){
    return time;
}

QString auth_Key::read_nonce(){
    return nonce.toBase64();
}

QString auth_Key::read_auth_key(){
    return hash.toBase64();
}

QString from_unix_time(int t){
    QDateTime from_time;
    from_time.setTime_t(t);
    return from_time.toString("yyyy/MM/dd hh:mm:ss");
}
