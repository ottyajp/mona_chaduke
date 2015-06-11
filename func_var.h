#ifndef FUNC_VAR_H
#define FUNC_VAR_H

#include <QString>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

QString knock_api(QString, QUrlQuery);
QString knock_api_get(QString, QUrlQuery);
QString from_unix_time(int);
extern int get_topic_limit;
extern int get_res_limit;
extern int user_id;
extern QString now_topic_id;
extern QString secret_key;
extern QString post_text;
extern QString send_to;
extern QString send_mona_amount_1;
extern QString send_mona_amount_2;
extern QString send_mona_amount_3;
extern QString send_mona_amount_4;
extern bool sage_check;


#endif // FUNC_VAR_H

