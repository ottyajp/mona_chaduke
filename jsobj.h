#ifndef JSOBJ_H
#define JSOBJ_H

#include <QObject>

class JsObj : public QObject
{
    Q_OBJECT
public:
    explicit JsObj(QObject *parent = 0);

signals:

public slots:
    void send_mona_to_res();
};

#endif // JSOBJ_H
