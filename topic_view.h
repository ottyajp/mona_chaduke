#ifndef TOPIC_VIEW_H
#define TOPIC_VIEW_H

#include <QWidget>
#include <QWebView>

class topic_view : public QWebView
{
    Q_OBJECT
public:
    topic_view();
    topic_view(QWidget*&);
    void set_topic_id(QString arg);
    QString get_topic_id();
signals:
public slots:
private:
    QString topic_id;
};

#endif // TOPIC_VIEW_H
