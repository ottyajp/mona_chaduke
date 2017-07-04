#ifndef AUTH_KEY_H
#define AUTH_KEY_H

#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QCryptographicHash>
#include <QSettings>

class auth_key
{
private:
    QString time;
    QByteArray nonce;
    QByteArray hash;
public:
    auth_key();
    QString read_time();
    QString read_nonce();
    QString read_auth_key();
};

#endif // AUTH_KEY_H
