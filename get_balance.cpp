#include "get_balance.h"
#include "ui_get_balance.h"

get_balance::get_balance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::get_balance)
{
    ui->setupUi(this);
}

get_balance::~get_balance()
{
    delete ui;
}
