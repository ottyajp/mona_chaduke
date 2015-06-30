#include "func_var.h"

QString knock_api(QString api_name, QUrlQuery api_query){
status_bar->showMessage(QObject::tr("POST requesting..."));

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
status_bar->showMessage(QObject::tr("GET requesting..."));
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
status_bar->showMessage(QObject::tr("making auth_key..."));
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

void format_topic(QWebFrame* frame, QJsonDocument json, QString title_string){
status_bar->showMessage(QObject::tr("formatting topic..."));
        //load template file
    initial_contents_size = frame->contentsSize();
    QString source;
    QFile file("./template.html");
    if(!file.open(QIODevice::ReadOnly)){
        status_bar->showMessage(QObject::tr("can't open template file."));
    }
    QTextStream in(&file);
    source = in.readAll();
    frame->setHtml(source);
        //set title
    QWebElement title;
    title = frame->findFirstElement("div.title");
    title.appendInside(title_string);
        //set responses
    QJsonArray res = json.object().value("responses").toArray();
    QWebElement list;
    double receive;
    for (int i = 0; i < GET_RES_LIMIT; i++){
        if(res.at(i).toObject().value("u_name").toString() == ""){
            break;
        }
        QString received_mona;
        receive = res.at(i).toObject().value("receive").toString().toDouble() / 100000000;
        if(res.at(i).toObject().value("receive").toString()!="0"){
            received_mona = "<span class=\"mona_yay\">+" +
                    QString::number(receive,'f',8).replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"") +
                    "MONA</b> / " +
                    QString::number(res.at(i).toObject().value("rec_count").toInt()) + QObject::tr("man") + "</span>";
        }else{
            received_mona = "+" + QString::number(receive) + "MONA / " +
                    QString::number(res.at(i).toObject().value("rec_count").toInt()) + QObject::tr("man") + "</span>";
        }
        QString replace_text = res.at(i).toObject().value("response").toString();
        replace_text.replace(QRegularExpression("(http://i.imgur.com/.+)(\\..{3})"),
                              "<div class=\"image\" onclick=\"onClick_image('\\1\\2');\"><img src=\"\\1m\\2\" class=\"imgur\"></div>");
        QString created = from_unix_time(res.at(i).toObject().value("created").toInt());

        list = frame->findFirstElement("div.responses");
        QString response = QString::number(res.at(i).toObject().value("r_id").toInt()) + " : " +
                res.at(i).toObject().value("u_name").toString() +
                res.at(i).toObject().value("u_dan").toString() + " : " +
                created + " [" +
                res.at(i).toObject().value("u_times").toString() + "] " +
                received_mona +
                " <span onclick=\"onClick_send_mona_link(" + QString::number(res.at(i).toObject().value("r_id").toInt()) +
                ");\" class=\"send_mona\">" + QObject::tr("send mona") + "</span><BR>" +
                "<span class=\"level" + QString::number(res.at(i).toObject().value("res_lv").toInt()) + "\">" +
                replace_text +
                "</span>";
        list.appendInside("<div class=\"response\">"+response+"</div>");
    }
status_bar->showMessage(QObject::tr("formatting topic completed"));
}
