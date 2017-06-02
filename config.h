#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QErrorMessage>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

#include "network_access.h"
#include "func_var.h"
#include "mainwindow.h"

namespace Ui {
class config;
}

class config : public QDialog
{
    Q_OBJECT

public:
    explicit config(QWidget *parent = 0);
    ~config();
    void set_uid(QString uid);

signals:
    void success_auth(QString key, QString id);

    void save();

private slots:
    void on_auth_button_clicked();

    void on_buttons_accepted();

    void on_buttons_rejected();

private:
    Ui::config *ui;
};

#endif // CONFIG_H
