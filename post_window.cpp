#include "post_window.h"
#include "ui_post_window.h"

post_window::post_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::post_window)
{
    ui->setupUi(this);
}

post_window::~post_window()
{
    delete ui;
}

void post_window::on_cancel_clicked()
{
    this->close();
}
