#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QLineEdit>
#include <QErrorMessage>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

#include "network_access.h"

namespace Ui {
class config;
}

class config : public QDialog
{
    Q_OBJECT

public:
    explicit config(QWidget *parent = 0);
    ~config();

private slots:
    void on_auth_button_clicked();

private:
    Ui::config *ui;
};

#endif // CONFIG_H
