#ifndef POST_WINDOW_H
#define POST_WINDOW_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "network_access.h"

namespace Ui {
class post_window;
}

class post_window : public QDialog
{
    Q_OBJECT

public:
    explicit post_window(QWidget *parent = 0);
    ~post_window();
    void setTopicID(QString t_id);

private slots:
    void on_cancel_clicked();

private:
    Ui::post_window *ui;
    QString t_id;
};

#endif // POST_WINDOW_H
