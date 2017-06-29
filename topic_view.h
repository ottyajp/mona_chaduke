#ifndef TOPIC_VIEW_H
#define TOPIC_VIEW_H

#include <QWebEngineView>
#include <QWebChannel>
#include <QtCore/QThread>

#include "func_var.h"

class topic_view : public QWebEngineView
{
    Q_OBJECT
private:
    QWebChannel channel;
    bool isInitialized;

public:
    explicit topic_view(QWidget *parent = 0);
    QWebEngineView* view();
    void setInitScreen();
    void setTitle(QString title);

signals:

public slots:
    void jsComp();
    void put_log(QString str);
};

#endif // TOPIC_VIEW_H
