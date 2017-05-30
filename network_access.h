#ifndef NETWORK_ACCESS_H
#define NETWORK_ACCESS_H

#include <QString>
#include <QUrlQuery>
#include <QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

QString access_post(QString api_name, QUrlQuery api_query);
QString access_get(QString api_name, QUrlQuery api_query);


#endif // NETWORK_ACCESS_H
