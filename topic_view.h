#ifndef TOPIC_VIEW_H
#define TOPIC_VIEW_H

#include <QWebEngineView>
#include <QWebChannel>
#include <QtCore/QThread>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QRegularExpression>

#include "func_var.h"
#include "image_window.h"

class topic_view : public QWebEngineView
{
    Q_OBJECT
private:
    QWebChannel channel;
    bool isInitialized;

public:
    explicit topic_view(QWidget *parent = 0);
    void setInitScreen();
    void setTitle(QString title);
    void loadTopic(QString t_id);

signals:

public slots:
    void jsComp();
    void put_log(QString str);
    void click_image(QString str);
};

#endif // TOPIC_VIEW_H
