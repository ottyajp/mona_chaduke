#ifndef GET_BALANCE_H
#define GET_BALANCE_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>

#include "func_var.h"
#include "network_access.h"
#include "auth_key.h"

namespace Ui {
class get_balance;
}

class get_balance : public QDialog
{
    Q_OBJECT
    QString u_id;

public:
    explicit get_balance(QWidget *parent = 0);
    ~get_balance();
    void set_uid(QString str);
    void get();

private slots:
    void on_closeButton_clicked();

private:
    Ui::get_balance *ui;
};

#endif // GET_BALANCE_H
