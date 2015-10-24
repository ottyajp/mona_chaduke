#include "send_receive_history.h"
#include "ui_send_receive_history.h"

send_receive_history::send_receive_history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_receive_history)
{
    ui->setupUi(this);
}

send_receive_history::~send_receive_history()
{
    delete ui;
}
