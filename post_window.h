#ifndef POST_WINDOW_H
#define POST_WINDOW_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QErrorMessage>

#include "network_access.h"
#include "auth_key.h"

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
    void setUserID(QString u_id);

private slots:
    void on_cancel_clicked();

    void on_submit_clicked();

private:
    Ui::post_window *ui;
    QString t_id;
    QString u_id;
};

#endif // POST_WINDOW_H
