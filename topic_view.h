#ifndef TOPIC_VIEW_H
#define TOPIC_VIEW_H

#include <QWebEngineView>

#include "func_var.h"

class topic_view
{
private:
    QWebEngineView *webView;

public:
    topic_view();
    QWebEngineView* view();

signals:

public slots:
};

#endif // TOPIC_VIEW_H
