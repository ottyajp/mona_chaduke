#ifndef JSOBJ_H
#define JSOBJ_H

#include <QObject>
#include <QString>

class JsObj : public QObject
{
    Q_OBJECT
public:
    explicit JsObj(QObject *parent = 0);

signals:
    void send_mona_to_res_signal(QString);

    void open_image_window_signal(QString);

public slots:
    void send_mona_to_res(QString s);

    void open_image(QString s);
};

#endif // JSOBJ_H
