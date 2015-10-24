#ifndef FUNC_VAR_H
#define FUNC_VAR_H

#include <QString>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QWebFrame>
#include <QWebElement>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QObject>
#include <QRegularExpression>
#include <QSize>
#include <QPoint>
#include <QStatusBar>

#define GET_RES_LIMIT 1000

QString knock_api(QString, QUrlQuery);
QString knock_api_get(QString, QUrlQuery);
QString from_unix_time(int);
QJsonDocument get_tx_history(QString, QString);
void format_topic(QWebFrame*, QJsonDocument, QString);
extern int get_topic_limit;
extern int user_id;
extern QString now_topic_id;
extern QString secret_key;
extern QString post_text;
extern QString send_to;
extern QString send_mona_amount_1;
extern QString send_mona_amount_2;
extern QString send_mona_amount_3;
extern QString send_mona_amount_4;
extern QString tx_send_receive_limit;
extern QString tx_with_depo_limit;
extern bool sage_check;
extern QSize initial_contents_size;
extern int position;
extern QStatusBar *status_bar;

class auth_Key{
private:
    QString time;
    QByteArray nonce;
    QByteArray hash;
public:
    auth_Key();
    QString read_time();
    QString read_nonce();
    QString read_auth_key();
};


#endif // FUNC_VAR_H

