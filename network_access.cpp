#include "network_access.h"

QString access_post(QString api_name, QUrlQuery api_query){
    QEventLoop eventLoop;

    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QString address = "https://askmona.org/v1/" + api_name;
    QUrl url(address);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    data.append(api_query.toString());
    QNetworkReply *reply = mgr.post(req,data);
    eventLoop.exec();

    if(reply->error() == QNetworkReply::NoError){   //success
        QString r = reply->readAll();
        delete reply;
        return r;
    }else{  //error
        qDebug()<<"Failure:"<<reply->errorString();
        QString r="1";
        delete reply;
        return r;
    }
}

QString access_get(QString api_name, QUrlQuery api_query){
    QEventLoop eventLoop;

    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QString address = "https://askmona.org/v1/" + api_name;
    QUrl url(address);
    url.setQuery(api_query);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if(reply->error() == QNetworkReply::NoError){   //success
        QString r = reply->readAll();
        delete reply;
        return r;
    }else{  //error
        qDebug()<<"Failure:"<<reply->errorString();
        QString r="1";
        delete reply;
        return r;
    }
}
